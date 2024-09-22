#ifndef RAPID_JSON_TEST_H
#define RAPID_JSON_TEST_H

#pragma once

#include "TypeSafe.h"
#include "test.h"
#include <string>
#include <memory>
#include <vector>
#include "JsonifierCatalog.h"
#include "JsonifierCountry.h"
#include "JsonifierTwitter.h"

// __SSE2__ and __SSE4_2__ are recognized by gcc, clang, and the Intel compiler.
// We use -march=native with gmake to enable -msse2 and -msse4.2, if supported.
#if defined(__SSE4_2__)
#  define RAPIDJSON_SSE42      
#elif defined(__SSE2__)        
#  define RAPIDJSON_SSE2
#elif defined(_MSC_VER) // Turn on SSE4.2 for VC
#  define RAPIDJSON_SSE42
#endif

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace rapidjson;

template<typename value_type>
concept bool_t = std::is_same_v<std::remove_cvref_t<value_type>, bool> || std::same_as<std::remove_cvref_t<value_type>, std::vector<bool>::reference> ||
std::same_as<std::remove_cvref_t<value_type>, std::vector<bool>::const_reference>;

template<typename value_type>
concept floating_type = std::floating_point<std::remove_cvref_t<value_type>> && !bool_t<value_type>;

template<typename value_type>
concept unsigned_type = std::unsigned_integral<std::remove_cvref_t<value_type>> && !floating_type<value_type> && !bool_t<value_type>;

template<typename value_type>
concept signed_type = std::signed_integral<std::remove_cvref_t<value_type>> && !floating_type<value_type> && !bool_t<value_type>;

template<typename value_type>
concept range = requires(std::remove_cvref_t<value_type> value) {
    typename std::remove_cvref_t<value_type>::value_type;
    { value.begin() } -> std::same_as<typename std::remove_cvref_t<value_type>::const_iterator>;
    { value.end() } -> std::same_as<typename std::remove_cvref_t<value_type>::const_iterator>;
} || requires(std::remove_cvref_t<value_type> value) {
    typename std::remove_cvref_t<value_type>::value_type;
    { value.begin() } -> std::same_as<typename std::remove_cvref_t<value_type>::iterator>;
    { value.end() } -> std::same_as<typename std::remove_cvref_t<value_type>::iterator>;
};

template<typename value_type> concept string_t = std::convertible_to<std::remove_cvref_t<value_type>, std::string> || std::same_as<std::remove_cvref_t<value_type>, std::string> || std::same_as<std::remove_cvref_t<value_type>, std::string_view>;

template<typename value_type>
concept map_subscriptable = requires(std::remove_cvref_t<value_type> value) {
    { value[typename std::remove_cvref_t<value_type>::key_type{}] } -> std::same_as<const typename std::remove_cvref_t<value_type>::mapped_type&>;
} || requires(std::remove_cvref_t<value_type> value) {
    { value[typename std::remove_cvref_t<value_type>::key_type{}] } -> std::same_as<typename std::remove_cvref_t<value_type>::mapped_type&>;
};

template<typename value_type>
concept map_t = requires(std::remove_cvref_t<value_type> value) {
    typename std::remove_cvref_t<value_type>::mapped_type;
    typename std::remove_cvref_t<value_type>::key_type;
}&& range<value_type>&& map_subscriptable<value_type> && !std::is_integral_v<std::remove_cvref_t<value_type>>;

template <typename Encoding = UTF8<> >
class StatHandler : public BaseReaderHandler<Encoding, StatHandler<Encoding> > {
public:
    typedef typename Encoding::Ch Ch;

    StatHandler(Stat& stat) : stat_(stat) {}

    bool Null() { stat_.nullCount++; return true; }
    bool Bool(bool b) { if (b) stat_.trueCount++; else stat_.falseCount++; return true; }
    bool Int(int) { stat_.numberCount++; return true; }
    bool Uint(unsigned) { stat_.numberCount++; return true; }
    bool Int64(int64_t) { stat_.numberCount++; return true; }
    bool Uint64(uint64_t) { stat_.numberCount++; return true; }
    bool Double(double) { stat_.numberCount++; return true; }
    bool String(const Ch*, SizeType length, bool) { stat_.stringCount++; stat_.stringLength += length; return true; }
    bool StartObject() { return true; }
    bool EndObject(SizeType memberCount) { stat_.objectCount++; stat_.memberCount += memberCount; return true; }
    bool StartArray() { return true; }
    bool EndArray(SizeType elementCount) { stat_.arrayCount++; stat_.elementCount += elementCount; return true; }

private:
    StatHandler& operator=(const StatHandler&);

    Stat& stat_;
};

static void GenStat(Stat& stat, const Value& v) {
    switch (v.GetType()) {
    case kNullType:  stat.nullCount++; break;
    case kFalseType: stat.falseCount++; break;
    case kTrueType:  stat.trueCount++; break;

    case kObjectType:
        for (Value::ConstMemberIterator m = v.MemberBegin(); m != v.MemberEnd(); ++m) {
            stat.stringLength += m->name.GetStringLength();
            GenStat(stat, m->value);
        }
        stat.objectCount++;
        stat.memberCount += (v.MemberEnd() - v.MemberBegin());
        stat.stringCount += (v.MemberEnd() - v.MemberBegin());
        break;

    case kArrayType:
        for (Value::ConstValueIterator i = v.Begin(); i != v.End(); ++i)
            GenStat(stat, *i);
        stat.arrayCount++;
        stat.elementCount += v.Size();
        break;

    case kStringType:
        stat.stringCount++;
        stat.stringLength += v.GetStringLength();
        break;

    case kNumberType:
        stat.numberCount++;
        break;
    }
}

template<typename value_type> inline void getValue(rapidjson::Value jsonValue, std::optional<value_type>& data) {
    value_type newValue;
    getValue(jsonValue, newValue);
    data.emplace(newValue);
}

inline void getValue(rapidjson::Value& jsonCoordinates, std::string& coordinates) {
    if (jsonCoordinates.IsString()) {
        coordinates = jsonCoordinates.GetString();
    }
}

inline void getValue(rapidjson::Value& jsonCoordinates, std::nullptr_t&) {
    if (jsonCoordinates.IsNull()) {
    }
}

template<floating_type value_type> inline void getValue(rapidjson::Value& jsonCoordinates, value_type& coordinates) {
    if (jsonCoordinates.IsDouble()) {
        coordinates = jsonCoordinates.GetDouble();
    }
}

template<signed_type value_type> inline void getValue(rapidjson::Value& jsonCoordinates, value_type& coordinates) {
    if constexpr (sizeof(value_type) == 8) {
        if (jsonCoordinates.IsInt64()) {
            coordinates = jsonCoordinates.GetInt64();
        }
    }
    else {
        if (jsonCoordinates.IsInt()) {
            coordinates = jsonCoordinates.GetInt();
        }
    }
}

template<unsigned_type value_type> inline void getValue(rapidjson::Value& jsonCoordinates, value_type& coordinates) {
    if constexpr (sizeof(value_type) == 8) {
        if (jsonCoordinates.IsUint64()) {
            coordinates = jsonCoordinates.GetUint64();
        }
    }
    else {
        if (jsonCoordinates.IsUint()) {
            coordinates = jsonCoordinates.GetUint();
        }
    }
}


inline void getValue(rapidjson::Value& jsonCoordinates, int*& coordinates) {
    if (coordinates) {
        getValue(jsonCoordinates, *coordinates);
    }
}


inline void getValue(rapidjson::Value& jsonCoordinates, bool& coordinates) {
    if (jsonCoordinates.IsBool()) {
        coordinates = jsonCoordinates.GetBool();
    }
}

template<typename value_type>
inline void getValue(rapidjson::Value& jsonCoordinates, std::vector<value_type>& coordinates) {
    if (jsonCoordinates.IsArray()) {
        for (auto& val : jsonCoordinates.GetArray()) {
            getValue(val, coordinates.emplace_back());
        }
    }
}

inline void getValue(rapidjson::Value& jsonCoordinates, std::vector<bool>& coordinates) {
    if (jsonCoordinates.IsArray()) {
        for (auto& val : jsonCoordinates.GetArray()) {
            coordinates.emplace_back(val.GetBool());
        }
    }
}


inline void getValue(rapidjson::Value& jsonCoordinates, std::vector<int*>& coordinates) {
    if (jsonCoordinates.IsArray()) {
        for (auto& val : jsonCoordinates.GetArray()) {
            getValue(val, *coordinates.emplace_back(new int{}));
        }
    }
}

template<map_t value_type>
inline void getValue(rapidjson::Value& jsonCoordinates, value_type& coordinates) {
    for (auto& m : jsonCoordinates.GetObject()) {
        typename std::remove_cvref_t<value_type>::key_type key{};
        getValue(m.name, key);
        typename std::remove_cvref_t<value_type>::mapped_type value{};
        getValue(m.value, value);
        coordinates[key] = std::move(value);
    }
}

inline void getValue(rapidjson::Value& jsonCoordinates, std::vector<std::string>& coordinates) {
    for (auto& m : jsonCoordinates.GetArray()) {
        getValue(m, coordinates.emplace_back());
    }
}

template<floating_type value_type> inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, value_type& coordinates) {
    writer.Double(coordinates);
}

template<signed_type value_type> inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, value_type& coordinates) {
    writer.Int64(coordinates);
}

template<unsigned_type value_type> inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, value_type& coordinates) {
    writer.Uint64(coordinates);
}

inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const std::string& data) {
    writer.String(data.c_str());
}

inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const std::nullptr_t&) {
    writer.Null();
}

template<typename value_type>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const std::vector<value_type>& msg) {
    writer.StartArray();
    for (auto& str : msg) {
        putValue(writer, str);
    }
    writer.EndArray();
}

template<map_t value_type>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const value_type& msg) {
    writer.StartObject();
    for (auto& outerPair : msg) {
        putValue(writer, outerPair.first);
        putValue(writer, outerPair.second);
    }
    writer.EndObject();
}

inline void getValue(rapidjson::Value& jsonValue, std::vector<std::vector<std::vector<double>>>& data) {
    if (jsonValue.IsArray()) {
        for (auto& outerArray : jsonValue.GetArray()) {
            std::vector<std::vector<double>> middleVec;
            for (auto& middleArray : outerArray.GetArray()) {
                std::vector<double> innerVec;
                for (auto& value : middleArray.GetArray()) {
                    innerVec.push_back(value.GetDouble());
                }
                middleVec.push_back(innerVec);
            }
            data.push_back(middleVec);
        }
    }
}


inline void getValue(rapidjson::Value& jsonValue, geometry_data& data) {
    getValue(jsonValue["type"], data.type);
    getValue(jsonValue["coordinates"], data.coordinates);
}


inline void getValue(rapidjson::Value& jsonValue, properties_data& data) {
    getValue(jsonValue["name"], data.name);
}


inline void getValue(rapidjson::Value& jsonValue, feature& data) {
    getValue(jsonValue["type"], data.type);
    getValue(jsonValue["properties"], data.properties);
    getValue(jsonValue["geometry"], data.geometry);
}


inline void getValue(rapidjson::Value& jsonValue, canada_message& data) {
    getValue(jsonValue["type"], data.type);
    if (jsonValue.HasMember("features") && jsonValue["features"].IsArray()) {
        for (auto& featureVal : jsonValue["features"].GetArray()) {
            feature feat;
            getValue(featureVal, feat);
            data.features.push_back(feat);
        }
    }
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const std::vector<std::vector<std::vector<double>>>& data) {
    writer.StartArray();
    for (auto& outerVec : data) {
        writer.StartArray();
        for (auto& middleVec : outerVec) {
            writer.StartArray();
            for (double val : middleVec) {
                writer.Double(val);
            }
            writer.EndArray();
        }
        writer.EndArray();
    }
    writer.EndArray();
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const geometry_data& data) {
    writer.StartObject();
    writer.Key("type");
    putValue(writer, data.type);
    writer.Key("coordinates");
    putValue(writer, data.coordinates);
    writer.EndObject();
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const properties_data& data) {
    writer.StartObject();
    writer.Key("name");
    putValue(writer, data.name);
    writer.EndObject();
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const feature& data) {
    writer.StartObject();
    writer.Key("type");
    putValue(writer, data.type);
    writer.Key("properties");
    putValue(writer, data.properties);
    writer.Key("geometry");
    putValue(writer, data.geometry);
    writer.EndObject();
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const canada_message& data) {
    writer.StartObject();
    writer.Key("type");
    putValue(writer, data.type);
    writer.Key("features");
    writer.StartArray();
    for (auto& feat : data.features) {
        putValue(writer, feat);
    }
    writer.EndArray();
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, search_metadata_data& data) {
    getValue(jsonValue["completed_in"], data.completed_in);
    getValue(jsonValue["max_id"], data.max_id);
    getValue(jsonValue["max_id_str"], data.max_id_str);
    getValue(jsonValue["next_results"], data.next_results);
    getValue(jsonValue["query"], data.query);
    getValue(jsonValue["refresh_url"], data.refresh_url);
    getValue(jsonValue["count"], data.count);
    getValue(jsonValue["since_id"], data.since_id);
    getValue(jsonValue["since_id_str"], data.since_id_str);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const search_metadata_data& data) {
    writer.StartObject();
    putValue(writer, data.completed_in);
    putValue(writer, data.max_id);
    putValue(writer, data.max_id_str);
    putValue(writer, data.next_results);
    putValue(writer, data.query);
    putValue(writer, data.refresh_url);
    putValue(writer, data.count);
    putValue(writer, data.since_id);
    putValue(writer, data.since_id_str);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, hashtag_data& data) {
    getValue(jsonValue["text"], data.text);
    getValue(jsonValue["indices"], data.indices);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const hashtag_data& data) {
    writer.StartObject();
    putValue(writer, data.text);
    putValue(writer, data.indices);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, large_data& data) {
    getValue(jsonValue["w"], data.w);
    getValue(jsonValue["h"], data.h);
    getValue(jsonValue["resize"], data.resize);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const large_data& data) {
    writer.StartObject();
    putValue(writer, data.w);
    putValue(writer, data.h);
    putValue(writer, data.resize);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, sizes_data& data) {
    getValue(jsonValue["medium"], data.medium);
    getValue(jsonValue["small"], data.small);
    getValue(jsonValue["thumb"], data.thumb);
    getValue(jsonValue["large"], data.large);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const sizes_data& data) {
    writer.StartObject();
    putValue(writer, data.medium);
    putValue(writer, data.small);
    putValue(writer, data.thumb);
    putValue(writer, data.large);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, media_data& data) {
    getValue(jsonValue["id"], data.id);
    getValue(jsonValue["id_str"], data.id_str);
    getValue(jsonValue["indices"], data.indices);
    getValue(jsonValue["media_url"], data.media_url);
    getValue(jsonValue["media_url_https"], data.media_url_https);
    getValue(jsonValue["url"], data.url);
    getValue(jsonValue["display_url"], data.display_url);
    getValue(jsonValue["expanded_url"], data.expanded_url);
    getValue(jsonValue["type"], data.type);
    getValue(jsonValue["sizes"], data.sizes);
    if (jsonValue.HasMember("source_status_id"))
        getValue(jsonValue["source_status_id"], data.source_status_id.emplace());
    if (jsonValue.HasMember("source_status_id_str"))
        getValue(jsonValue["source_status_id_str"], data.source_status_id_str.emplace());
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const media_data& data) {
    writer.StartObject();
    putValue(writer, data.id);
    putValue(writer, data.id_str);
    putValue(writer, data.indices);
    putValue(writer, data.media_url);
    putValue(writer, data.media_url_https);
    putValue(writer, data.url);
    putValue(writer, data.display_url);
    putValue(writer, data.expanded_url);
    putValue(writer, data.type);
    putValue(writer, data.sizes);
    if (data.source_status_id.has_value())
        putValue(writer, *data.source_status_id);
    if (data.source_status_id_str.has_value())
        putValue(writer, *data.source_status_id_str);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, url_data& data) {
    getValue(jsonValue["url"], data.url);
    getValue(jsonValue["expanded_url"], data.expanded_url);
    getValue(jsonValue["display_url"], data.display_url);
    getValue(jsonValue["indices"], data.indices);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const url_data& data) {
    writer.StartObject();
    putValue(writer, data.url);
    putValue(writer, data.expanded_url);
    putValue(writer, data.display_url);
    putValue(writer, data.indices);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, user_mention& data) {
    getValue(jsonValue["screen_name"], data.screen_name);
    getValue(jsonValue["name"], data.name);
    getValue(jsonValue["id"], data.id);
    getValue(jsonValue["id_str"], data.id_str);
    getValue(jsonValue["indices"], data.indices);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const user_mention& data) {
    writer.StartObject();
    putValue(writer, data.screen_name);
    putValue(writer, data.name);
    putValue(writer, data.id);
    putValue(writer, data.id_str);
    putValue(writer, data.indices);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, status_entities& data) {
    getValue(jsonValue["hashtags"], data.hashtags);
    getValue(jsonValue["symbols"], data.symbols);
    getValue(jsonValue["urls"], data.urls);
    getValue(jsonValue["user_mentions"], data.user_mentions);
    if (jsonValue.HasMember("media") && !jsonValue["media"].IsNull()) {
        getValue(jsonValue["media"], data.media.emplace());
    }
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const status_entities& data) {
    writer.StartObject();
    putValue(writer, data.hashtags);
    putValue(writer, data.symbols);
    putValue(writer, data.urls);
    putValue(writer, data.user_mentions);
    if (data.media.has_value())
        putValue(writer, *data.media);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, metadata_data& data) {
    getValue(jsonValue["result_type"], data.result_type);
    getValue(jsonValue["iso_language_code"], data.iso_language_code);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const metadata_data& data) {
    writer.StartObject();
    putValue(writer, data.result_type);
    putValue(writer, data.iso_language_code);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, description_data& data) {
    getValue(jsonValue["urls"], data.urls);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const description_data& data) {
    writer.StartObject();
    putValue(writer, data.urls);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, user_entities& data) {
    getValue(jsonValue["description"], data.description);
    if (jsonValue.HasMember("url"))
        getValue(jsonValue["url"], data.url.emplace());
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const user_entities& data) {
    writer.StartObject();
    putValue(writer, data.description);
    if (data.url.has_value())
        putValue(writer, *data.url);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, twitter_user_data& data) {
    getValue(jsonValue["id"], data.id);
    getValue(jsonValue["id_str"], data.id_str);
    getValue(jsonValue["name"], data.name);
    getValue(jsonValue["screen_name"], data.screen_name);
    getValue(jsonValue["location"], data.location);
    getValue(jsonValue["description"], data.description);
    if (jsonValue.HasMember("url"))
        getValue(jsonValue["url"], data.url.emplace());
    getValue(jsonValue["entities"], data.entities);
    getValue(jsonValue["protected"], data.protectedVal);
    getValue(jsonValue["followers_count"], data.followers_count);
    getValue(jsonValue["friends_count"], data.friends_count);
    getValue(jsonValue["listed_count"], data.listed_count);
    getValue(jsonValue["created_at"], data.created_at);
    getValue(jsonValue["favourites_count"], data.favourites_count);
    if (jsonValue.HasMember("utc_offset"))
        getValue(jsonValue["utc_offset"], data.utc_offset.emplace());
    if (jsonValue.HasMember("time_zone"))
        getValue(jsonValue["time_zone"], data.time_zone.emplace());
    getValue(jsonValue["geo_enabled"], data.geo_enabled);
    getValue(jsonValue["verified"], data.verified);
    getValue(jsonValue["statuses_count"], data.statuses_count);
    getValue(jsonValue["lang"], data.lang);
    getValue(jsonValue["contributors_enabled"], data.contributors_enabled);
    getValue(jsonValue["is_translator"], data.is_translator);
    getValue(jsonValue["is_translation_enabled"], data.is_translation_enabled);
    getValue(jsonValue["profile_background_color"], data.profile_background_color);
    getValue(jsonValue["profile_background_image_url"], data.profile_background_image_url);
    getValue(jsonValue["profile_background_image_url_https"], data.profile_background_image_url_https);
    getValue(jsonValue["profile_background_tile"], data.profile_background_tile);
    getValue(jsonValue["profile_image_url"], data.profile_image_url);
    getValue(jsonValue["profile_image_url_https"], data.profile_image_url_https);
    if (jsonValue.HasMember("profile_banner_url"))
        getValue(jsonValue["profile_banner_url"], data.profile_banner_url.emplace());
    getValue(jsonValue["profile_link_color"], data.profile_link_color);
    getValue(jsonValue["profile_sidebar_border_color"], data.profile_sidebar_border_color);
    getValue(jsonValue["profile_sidebar_fill_color"], data.profile_sidebar_fill_color);
    getValue(jsonValue["profile_text_color"], data.profile_text_color);
    getValue(jsonValue["profile_use_background_image"], data.profile_use_background_image);
    getValue(jsonValue["default_profile"], data.default_profile);
    getValue(jsonValue["default_profile_image"], data.default_profile_image);
    getValue(jsonValue["following"], data.following);
    getValue(jsonValue["follow_request_sent"], data.follow_request_sent);
    getValue(jsonValue["notifications"], data.notifications);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const twitter_user_data& data) {
    writer.StartObject();
    putValue(writer, data.id);
    putValue(writer, data.id_str);
    putValue(writer, data.name);
    putValue(writer, data.screen_name);
    putValue(writer, data.location);
    putValue(writer, data.description);
    if (data.url.has_value())
        putValue(writer, *data.url);
    putValue(writer, data.entities);
    putValue(writer, data.protectedVal);
    putValue(writer, data.followers_count);
    putValue(writer, data.friends_count);
    putValue(writer, data.listed_count);
    putValue(writer, data.created_at);
    putValue(writer, data.favourites_count);
    if (data.utc_offset.has_value())
        putValue(writer, *data.utc_offset);
    if (data.time_zone.has_value())
        putValue(writer, *data.time_zone);
    putValue(writer, data.geo_enabled);
    putValue(writer, data.verified);
    putValue(writer, data.statuses_count);
    putValue(writer, data.lang);
    putValue(writer, data.contributors_enabled);
    putValue(writer, data.is_translator);
    putValue(writer, data.is_translation_enabled);
    putValue(writer, data.profile_background_color);
    putValue(writer, data.profile_background_image_url);
    putValue(writer, data.profile_background_image_url_https);
    putValue(writer, data.profile_background_tile);
    putValue(writer, data.profile_image_url);
    putValue(writer, data.profile_image_url_https);
    if (data.profile_banner_url.has_value())
        putValue(writer, *data.profile_banner_url);
    putValue(writer, data.profile_link_color);
    putValue(writer, data.profile_sidebar_border_color);
    putValue(writer, data.profile_sidebar_fill_color);
    putValue(writer, data.profile_text_color);
    putValue(writer, data.profile_use_background_image);
    putValue(writer, data.default_profile);
    putValue(writer, data.default_profile_image);
    putValue(writer, data.following);
    putValue(writer, data.follow_request_sent);
    putValue(writer, data.notifications);
    writer.EndObject();
}

inline void getValue(rapidjson::Value& jsonValue, status_data& data) {
    getValue(jsonValue["metadata"], data.metadata);
    getValue(jsonValue["created_at"], data.created_at);
    getValue(jsonValue["id"], data.id);
    getValue(jsonValue["id_str"], data.id_str);
    getValue(jsonValue["text"], data.text);
    getValue(jsonValue["source"], data.source);
    getValue(jsonValue["truncated"], data.truncated);
    if (jsonValue.HasMember("in_reply_to_status_id"))
        getValue(jsonValue["in_reply_to_status_id"], data.in_reply_to_status_id.emplace());
    if (jsonValue.HasMember("in_reply_to_status_id_str"))
        getValue(jsonValue["in_reply_to_status_id_str"], data.in_reply_to_status_id_str.emplace());
    if (jsonValue.HasMember("in_reply_to_user_id"))
        getValue(jsonValue["in_reply_to_user_id"], data.in_reply_to_user_id.emplace());
    if (jsonValue.HasMember("in_reply_to_user_id_str"))
        getValue(jsonValue["in_reply_to_user_id_str"], data.in_reply_to_user_id_str.emplace());
    if (jsonValue.HasMember("in_reply_to_screen_name"))
        getValue(jsonValue["in_reply_to_screen_name"], data.in_reply_to_screen_name.emplace());
    getValue(jsonValue["user"], data.user);
    getValue(jsonValue["geo"], data.geo);
    getValue(jsonValue["coordinates"], data.coordinates);
    getValue(jsonValue["place"], data.place);
    getValue(jsonValue["contributors"], data.contributors);
    getValue(jsonValue["retweet_count"], data.retweet_count);
    getValue(jsonValue["favorite_count"], data.favorite_count);
    getValue(jsonValue["entities"], data.entities);
    getValue(jsonValue["favorited"], data.favorited);
    getValue(jsonValue["retweeted"], data.retweeted);
    getValue(jsonValue["lang"], data.lang);
    if (jsonValue.HasMember("retweeted_status")) {
        auto newPtr = std::make_shared<status_data>();
        getValue(jsonValue["retweeted_status"], *newPtr);
        data.retweeted_status = newPtr;
    }
    if (jsonValue.HasMember("possibly_sensitive"))
        getValue(jsonValue["possibly_sensitive"], data.possibly_sensitive.emplace());
}

inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const status_data& data) {
    writer.StartObject();
    putValue(writer, data.metadata);
    putValue(writer, data.created_at);
    putValue(writer, data.id);
    putValue(writer, data.id_str);
    putValue(writer, data.text);
    putValue(writer, data.source);
    putValue(writer, data.truncated);
    if (data.in_reply_to_status_id.has_value())
        putValue(writer, *data.in_reply_to_status_id);
    if (data.in_reply_to_status_id_str.has_value())
        putValue(writer, *data.in_reply_to_status_id_str);
    if (data.in_reply_to_user_id.has_value())
        putValue(writer, *data.in_reply_to_user_id);
    if (data.in_reply_to_user_id_str.has_value())
        putValue(writer, *data.in_reply_to_user_id_str);
    if (data.in_reply_to_screen_name.has_value())
        putValue(writer, *data.in_reply_to_screen_name);
    putValue(writer, data.user);
    putValue(writer, data.geo);
    putValue(writer, data.coordinates);
    putValue(writer, data.place);
    putValue(writer, data.contributors);
    putValue(writer, data.retweet_count);
    putValue(writer, data.favorite_count);
    putValue(writer, data.entities);
    putValue(writer, data.favorited);
    putValue(writer, data.retweeted);
    putValue(writer, data.lang);
    if (data.retweeted_status)
        putValue(writer, *data.retweeted_status);
    if (data.possibly_sensitive.has_value())
        putValue(writer, *data.possibly_sensitive);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, twitter_message& data) {
    getValue(jsonValue["statuses"], data.statuses);
    getValue(jsonValue["search_metadata"], data.search_metadata);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const twitter_message& data) {
    writer.StartObject();
    putValue(writer, data.statuses);
    putValue(writer, data.search_metadata);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, audience_sub_category_names& data) {
    getValue(jsonValue["337100890"], data.the337100890);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const audience_sub_category_names& data) {
    writer.StartObject();
    putValue(writer, data.the337100890);
    writer.EndObject();
}


inline void getValue(rapidjson::Value&, names&) {
    // Implementation for names struct if any fields are added
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const names&) {
    writer.StartObject();
    // Implementation for names struct if any fields are added
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, event& data) {
    getValue(jsonValue["description"], data.description);
    getValue(jsonValue["id"], data.id);
    if (jsonValue.HasMember("logo"))
        getValue(jsonValue["logo"], data.logo.emplace());
    getValue(jsonValue["name"], data.name);
    getValue(jsonValue["subTopicIds"], data.subTopicIds);
    getValue(jsonValue["subjectCode"], data.subjectCode);
    getValue(jsonValue["subtitle"], data.subtitle);
    getValue(jsonValue["topicIds"], data.topicIds);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const event& data) {
    writer.StartObject();
    putValue(writer, data.description);
    putValue(writer, data.id);
    if (data.logo.has_value())
        putValue(writer, *data.logo);
    putValue(writer, data.name);
    putValue(writer, data.subTopicIds);
    putValue(writer, data.subjectCode);
    putValue(writer, data.subtitle);
    putValue(writer, data.topicIds);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, price& data) {
    getValue(jsonValue["amount"], data.amount);
    getValue(jsonValue["audienceSubCategoryId"], data.audienceSubCategoryId);
    getValue(jsonValue["seatCategoryId"], data.seatCategoryId);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const price& data) {
    writer.StartObject();
    putValue(writer, data.amount);
    putValue(writer, data.audienceSubCategoryId);
    putValue(writer, data.seatCategoryId);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, area& data) {
    getValue(jsonValue["areaId"], data.areaId);
    getValue(jsonValue["blockIds"], data.blockIds);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const area& data) {
    writer.StartObject();
    putValue(writer, data.areaId);
    putValue(writer, data.blockIds);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, seat_category& data) {
    getValue(jsonValue["areas"], data.areas);
    getValue(jsonValue["seatCategoryId"], data.seatCategoryId);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const seat_category& data) {
    writer.StartObject();
    putValue(writer, data.areas);
    putValue(writer, data.seatCategoryId);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, performance& data) {
    getValue(jsonValue["eventId"], data.eventId);
    getValue(jsonValue["id"], data.id);
    if (jsonValue.HasMember("logo"))
        getValue(jsonValue["logo"], data.logo.emplace());
    getValue(jsonValue["name"], data.name);
    getValue(jsonValue["prices"], data.prices);
    getValue(jsonValue["seatCategories"], data.seatCategories);
    getValue(jsonValue["seatMapImage"], data.seatMapImage);
    getValue(jsonValue["start"], data.start);
    getValue(jsonValue["venueCode"], data.venueCode);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const performance& data) {
    writer.StartObject();
    putValue(writer, data.eventId);
    putValue(writer, data.id);
    if (data.logo.has_value())
        putValue(writer, *data.logo);
    putValue(writer, data.name);
    putValue(writer, data.prices);
    putValue(writer, data.seatCategories);
    putValue(writer, data.seatMapImage);
    putValue(writer, data.start);
    putValue(writer, data.venueCode);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, venue_names& data) {
    getValue(jsonValue["PLEYEL_PLEYEL"], data.PLEYEL_PLEYEL);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const venue_names& data) {
    writer.StartObject();
    putValue(writer, data.PLEYEL_PLEYEL);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& jsonValue, citm_catalog_message& data) {
    getValue(jsonValue["areaNames"], data.areaNames);
    getValue(jsonValue["audienceSubCategoryNames"], data.audienceSubCategoryNames);
    getValue(jsonValue["blockNames"], data.blockNames);
    getValue(jsonValue["events"], data.events);
    getValue(jsonValue["performances"], data.performances);
    getValue(jsonValue["seatCategoryNames"], data.seatCategoryNames);
    getValue(jsonValue["subTopicNames"], data.subTopicNames);
    getValue(jsonValue["subjectNames"], data.subjectNames);
    getValue(jsonValue["topicNames"], data.topicNames);
    getValue(jsonValue["topicSubTopics"], data.topicSubTopics);
    getValue(jsonValue["venueNames"], data.venueNames);
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const citm_catalog_message& data) {
    writer.StartObject();
    putValue(writer, data.areaNames);
    putValue(writer, data.audienceSubCategoryNames);
    putValue(writer, data.blockNames);
    putValue(writer, data.events);
    putValue(writer, data.performances);
    putValue(writer, data.seatCategoryNames);
    putValue(writer, data.subTopicNames);
    putValue(writer, data.subjectNames);
    putValue(writer, data.topicNames);
    putValue(writer, data.topicSubTopics);
    putValue(writer, data.venueNames);
    writer.EndObject();
}


inline void getValue(rapidjson::Value& json, Obj3& geometry) {
    geometry.a = std::make_unique<int>(json["a"].GetInt());
    if (geometry.foo) {
        getValue(json["foo"], *geometry.foo);
    }
}


inline void getValue(rapidjson::Value& json, Obj2& value) {
    getValue(json["foo"], value.foo);
}


inline void getValue(rapidjson::Value&, Special&) {
}


inline void getValue(rapidjson::Value&, Empty&) {
}

using tuple_type = Pass01;
static constexpr auto tupleSize = std::tuple_size_v<tuple_type>;

template<std::size_t Index = 0, typename... Types>
inline typename std::enable_if<Index == sizeof...(Types), void>::type
parseTupleElements(rapidjson::Value&, std::tuple<Types...>&) {
}

template<std::size_t Index = 0, typename... Types>
inline typename std::enable_if < Index < sizeof...(Types), void>::type
    parseTupleElements(rapidjson::Value& jsonValue, std::tuple<Types...>& tuple) {
    getValue(jsonValue[Index], std::get<Index>(tuple));
    parseTupleElements<Index + 1>(jsonValue, tuple);
}

template<typename... Args>
inline void getValue(rapidjson::Value& jsonValue, tuple_type& tuple) {
    if (jsonValue.IsArray()) {
        parseTupleElements<>(jsonValue, tuple);
    }
    else {
        throw std::runtime_error("Expected JSON array for tuple parsing");
    }
}

template<typename value_type>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const std::unique_ptr<value_type>& data) {
    if (data) {
        putValue(writer, *data);
    }
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const std::string*& data) {
    if (data) {
        putValue(writer, *data);
    }
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const Obj3& obj) {
    writer.StartObject();
    writer.Key("foo");
    putValue(writer, obj.foo);
    writer.Key("a");
    putValue(writer, obj.a);
    writer.EndObject();
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const Obj2& obj) {

    writer.StartObject();
    writer.Key("foo");
    putValue(writer, obj.foo);
    writer.EndObject();

}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const std::vector<double>& values) {

    writer.StartArray();
    for (auto& val : values) {
        writer.Double(val);
    }
    writer.EndArray();

}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const std::vector<int>& values) {

    writer.StartArray();
    for (auto& val : values) {
        writer.Int(val);
    }
    writer.EndArray();

}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const std::vector<bool>& values) {

    writer.StartArray();
    for (const auto& val : values) {
        writer.Bool(val);
    }
    writer.EndArray();
}


inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const std::vector<int*>& values) {

    writer.StartArray();
    for (auto& ptr : values) {
        writer.Int(*ptr);
    }
    writer.EndArray();
}


template <size_t currentIndex = 0> bool putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const tuple_type& out_value) {
    if constexpr (currentIndex < tupleSize) {
        //putValue(writer, std::get<currentIndex>(out_value));
        return putValue<currentIndex + 1>(writer, out_value);
    }
    return true;
}

template<typename T>
class RapidjsonParseResult : public ParseResultBase {
public:
    RapidjsonParseResult(const char* json, size_t length) : buffer() {
        (void)json;
        (void)length;
#ifdef TEST_INSITU
        buffer = (char*)malloc(length + 1);
        memcpy(buffer, json, length + 1);
#endif
    }
    ~RapidjsonParseResult() { free(buffer); }
    Document document;
    char* buffer;
    T       data;
};

namespace rapidjsonTypes {

    class RapidjsonStringResult : public StringResultBase {
    public:
        virtual const char* c_str() const { return sb.data(); }

        std::string sb;
    };

    template<typename value_type>
    class GetValue : public TestAction {
    public:

        virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
        {
            (void)length;
            std::unique_ptr<RapidjsonParseResult<value_type>> pr = std::make_unique<RapidjsonParseResult<value_type>>(json, length);
#ifdef TEST_INSITU
            pr->document.template ParseInsitu<TEST_PARSE_FLAG>(pr->buffer);
#else
            pr->document.template Parse<TEST_PARSE_FLAG>(json);
#endif
            if (!pr->document.HasParseError()) {
                getValue(pr->document, pr->data);
                reply = std::move(pr);
            }
            return true;
        }

        virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
        {
            return Prettify(parseResult, reply);
        }

        virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
        {
            const RapidjsonParseResult<value_type>& pr = static_cast<const RapidjsonParseResult<value_type>&>(parseResult);
            std::unique_ptr<RapidjsonStringResult> sr = std::make_unique<RapidjsonStringResult>();
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer{ buffer };
            putValue(writer, pr.data);
            sr->sb = buffer.GetString();
            reply = std::move(sr);
            return true;
        }

        virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
        {
            const RapidjsonParseResult<value_type>& pr = static_cast<const RapidjsonParseResult<value_type>&>(parseResult);
            memset(&stat, 0, sizeof(Stat));
#if SLOWER_STAT
            StatHandler<> h(stat);
            doc->Accept(h);
#else
            GenStat(stat, pr.document);
#endif
            return true;
        }

        virtual bool SaxStatistics(const char* json, size_t length, Stat& stat) const override
        {
            (void)length;
            memset(&stat, 0, sizeof(Stat));
            Reader reader;

#ifdef TEST_INSITU
            RapidjsonParseResult<value_type> pr(json, length);
            InsituStringStream is(pr.buffer);
#else
            StringStream is(json);
#endif

            StatHandler<> handler(stat);
            return reader.Parse<TEST_PARSE_FLAG>(is, handler);
        }

        virtual bool SaxStatisticsUTF16(const char* json, size_t length, Stat& stat) const override
        {
            (void)length;
            memset(&stat, 0, sizeof(Stat));
            GenericReader<UTF8<>, UTF16<> > reader;

#ifdef TEST_INSITU
            RapidjsonParseResult<value_type> pr(json, length);
            InsituStringStream is(pr.buffer);
#else
            StringStream is(json);
#endif
            StatHandler<UTF16<> > handler(stat);
            return reader.Parse<TEST_PARSE_FLAG>(is, handler);
        }
    };

    class VectorDouble : public TestAction
    {
    public:
        virtual bool ParseDouble(const char* json, long double& reply) const
        {

            Document doc;
#ifdef TEST_INSITU
            RapidjsonParseResult<std::vector<double>> pr(json, strlen(json));
            doc.ParseInsitu<TEST_PARSE_FLAG>(pr.buffer);
#else
            doc.Parse<TEST_PARSE_FLAG>(json);
#endif 
            if (!doc.HasParseError() && doc.IsArray() && doc.Size() == 1 && doc[0].IsNumber()) {
                reply = doc[0].GetDouble();
            }
            return true;
        }
    };

    class VectorString : public TestAction
    {
    public:
        virtual bool ParseString(const char* json, std::string& reply) const
        {

            Document doc;
#ifdef TEST_INSITU
            RapidjsonParseResult<std::vector<std::string>> pr(json, strlen(json));
            doc.ParseInsitu<TEST_PARSE_FLAG>(pr.buffer);
#else
            doc.Parse<TEST_PARSE_FLAG>(json);
#endif
            if (!doc.HasParseError() && doc.IsArray() && doc.Size() == 1 && doc[0].IsString()) {
                reply = std::string(doc[0].GetString(), doc[0].GetStringLength());
            }
            return true;
        }
    };
}

// OK: The TypeSafeTest test is a template type that has a handler for each test case.
//     There are three default handlers that you need to provide.
//          class VectorDouble: public TestAction
//          class VectorString: public TestAction
//          template<typename T>
//          class GetValue: public TestAction
//
// See the file: JsonifierTypes.h for the framework.
//
// This should cover most cases.
// If you need to do some special handling of types you can override the default actions.
// See comments blow:

class TEST_CLASS : public TypeSafeTest<rapidjsonTypes::VectorDouble,      // Type for parsing array of single double.
    rapidjsonTypes::VectorString,      // Type ofr parsing array of single string
    rapidjsonTypes::GetValue>          // Template template class. This is templatezed with the type
    // That needs to be read from the json string.
{
    rapidjsonTypes::GetValue<canada_message>                    testGetValueCountry;
    rapidjsonTypes::GetValue<twitter_message>                   testGetValueTwitter;
    rapidjsonTypes::GetValue<citm_catalog_message>              testGetValueCatalog;
public:
    TEST_CLASS()
    {
        actionMap["performance/canada.json"] = &testGetValueCountry;
        actionMap["performance/twitter.json"] = &testGetValueTwitter;
        actionMap["performance/citm_catalog.json"] = &testGetValueCatalog;
    }

    virtual const char* GetName()     const override { return TEST_NAME; }
    virtual const char* Type()        const override { return "C++"; }
    virtual const char* GetFilename() const override { return __FILE__; }
};

#endif


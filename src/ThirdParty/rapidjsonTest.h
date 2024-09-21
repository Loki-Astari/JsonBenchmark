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

template<typename value_type> void getValue(rapidjson::Value& jsonCoordinates, value_type& coordinates);

template<>
inline void getValue(rapidjson::Value& jsonValue, std::string& value) {
    if (jsonValue.IsString()) {
        value = jsonValue.GetString();
    }
    else {
        throw std::runtime_error("Expected a JSON string");
    }
}

template<>
inline void getValue(rapidjson::Value& jsonCoordinates, std::vector<std::vector<std::vector<double>>>& coordinatesVal) {
    coordinatesVal.resize(jsonCoordinates.Size());
    for (size_t i = 0; i < jsonCoordinates.Size(); ++i) {
        coordinatesVal[i].resize(jsonCoordinates[i].Size());
        for (size_t j = 0; j < jsonCoordinates[i].Size(); ++j) {
            auto& innerArray = jsonCoordinates[i][j];
            coordinatesVal[i][j].resize(innerArray.Size());
            for (size_t k = 0; k < innerArray.Size(); ++k) {
                coordinatesVal[i][j][k] = innerArray[k].GetDouble();
            }
        }
    }
}

template<>
inline void getValue(rapidjson::Value& json, Obj3& geometry) {
    geometry.a = std::make_unique<int>(json["a"].GetInt());
    if (geometry.foo) {
        getValue(json["foo"], *geometry.foo);
    }
}

template<>
inline void getValue(rapidjson::Value& json, Obj2& value) {
    getValue(json["foo"], value.foo);
}

template<>
inline void getValue(rapidjson::Value&, Special&) {
}

template<>
inline void getValue(rapidjson::Value& , Empty& ) {
}

template<typename value_type>
inline void getValue(rapidjson::Value& jsonCoordinates, std::vector<value_type>& coordinates) {
    if (jsonCoordinates.IsArray()) {
        for (auto& val : jsonCoordinates.GetArray()) {
            value_type newValue;
            getValue(val, newValue);
            coordinates.emplace_back(newValue);
        }
    }
}

template<>
inline void getValue(rapidjson::Value& jsonCoordinates, double& coordinates) {
    if (jsonCoordinates.IsDouble()) {
        coordinates = jsonCoordinates.GetDouble();
    }
}

template<>
inline void getValue(rapidjson::Value& jsonCoordinates, int& coordinates) {
    if (jsonCoordinates.IsInt()) {
        coordinates = jsonCoordinates.GetInt();
    }
}

template<>
inline void getValue(rapidjson::Value& jsonCoordinates, int*& coordinates) {
    if (coordinates) {
        if (jsonCoordinates.IsInt()) {
            *coordinates = jsonCoordinates.GetInt();
        }
    }
}

template<>
inline void getValue(rapidjson::Value& jsonCoordinates, bool& coordinates) {
    if (jsonCoordinates.IsBool()) {
        coordinates = jsonCoordinates.GetBool();
    }
}

template<>
inline void getValue(rapidjson::Value& jsonCoordinates, std::vector<double>& coordinates) {
    if (jsonCoordinates.IsArray()) {
        for (auto& val : jsonCoordinates.GetArray()) {
            coordinates.push_back(val.GetDouble());
        }
    }
}

template<>
inline void getValue(rapidjson::Value& jsonCoordinates, std::vector<long int>& coordinates) {
    if (jsonCoordinates.IsArray()) {
        for (auto& val : jsonCoordinates.GetArray()) {
            coordinates.push_back(val.GetInt64());
        }
    }
}

template<>
inline void getValue(rapidjson::Value& jsonCoordinates, std::vector<int>& coordinates) {
    if (jsonCoordinates.IsArray()) {
        for (auto& val : jsonCoordinates.GetArray()) {
            coordinates.push_back(val.GetInt());
        }
    }
}

template<>
inline void getValue(rapidjson::Value& jsonCoordinates, std::vector<bool>& coordinates) {
    if (jsonCoordinates.IsArray()) {
        for (auto& val : jsonCoordinates.GetArray()) {
            coordinates.push_back(val.GetBool());
        }
    }
}

template<>
inline void getValue(rapidjson::Value& jsonCoordinates, std::vector<int*>& coordinates) {
    if (jsonCoordinates.IsArray()) {
        for (auto& val : jsonCoordinates.GetArray()) {
            int* ptr = new int(val.GetInt());
            coordinates.push_back(ptr);
        }
    }
}

template<>
inline void getValue(rapidjson::Value& jsonGeometry, geometry_data& geometry) {
    geometry.type = jsonGeometry["type"].GetString();
    getValue(jsonGeometry["coordinates"], geometry.coordinates);
}

template<>
inline void getValue(rapidjson::Value& jsonProperties, properties_data& properties) {
    properties.name = jsonProperties["name"].GetString();
}

template<>
inline void getValue(rapidjson::Value& jsonFeature, feature& feat) {
    feat.type = jsonFeature["type"].GetString();
    getValue(jsonFeature["properties"], feat.properties);
    getValue(jsonFeature["geometry"], feat.geometry);
}

template<>
inline void getValue(rapidjson::Value& document, canada_message& msg) {
    msg.type = document["type"].GetString();
    auto& jsonFeatures = document["features"];
    msg.features.resize(jsonFeatures.Size());

    for (size_t i = 0; i < jsonFeatures.Size(); ++i) {
        getValue(jsonFeatures[i], msg.features[i]);
    }
}

template<>
inline void getValue(rapidjson::Value& jsonEvent, event& evt) {
    evt.id = jsonEvent["id"].GetInt64();
    evt.name = jsonEvent["name"].GetString();

    if (jsonEvent.HasMember("logo") && jsonEvent["logo"].IsString()) {
        evt.logo = jsonEvent["logo"].GetString();
    }

    auto& subTopicIds = jsonEvent["subTopicIds"];
    evt.subTopicIds.resize(subTopicIds.Size());
    for (size_t i = 0; i < subTopicIds.Size(); ++i) {
        evt.subTopicIds[i] = subTopicIds[i].GetInt64();
    }

    auto& topicIds = jsonEvent["topicIds"];
    evt.topicIds.resize(topicIds.Size());
    for (size_t i = 0; i < topicIds.Size(); ++i) {
        evt.topicIds[i] = topicIds[i].GetInt64();
    }
}

template<>
inline void getValue(rapidjson::Value& jsonPrice, price& prc) {
    prc.amount = jsonPrice["amount"].GetInt64();
    prc.audienceSubCategoryId = jsonPrice["audienceSubCategoryId"].GetInt64();
    prc.seatCategoryId = jsonPrice["seatCategoryId"].GetInt64();
}

template<>
inline void getValue(rapidjson::Value& jsonArea, area& ar) {
    ar.areaId = jsonArea["areaId"].GetInt64();
    ar.blockIds.resize(jsonArea["blockIds"].Size());
}

template<>
inline void getValue(rapidjson::Value& jsonSeatCategory, seat_category& seatCat) {
    seatCat.seatCategoryId = jsonSeatCategory["seatCategoryId"].GetInt64();

    auto& areas = jsonSeatCategory["areas"];
    seatCat.areas.resize(areas.Size());
    for (size_t i = 0; i < areas.Size(); ++i) {
        getValue(areas[i], seatCat.areas[i]);
    }
}

template<>
inline void getValue(rapidjson::Value& jsonPerformance, performance& perf) {
    perf.eventId = jsonPerformance["eventId"].GetInt64();
    perf.id = jsonPerformance["id"].GetInt64();

    if (jsonPerformance.HasMember("logo") && jsonPerformance["logo"].IsString()) {
        perf.logo = jsonPerformance["logo"].GetString();
    }

    perf.start = jsonPerformance["start"].GetInt64();
    perf.venueCode = jsonPerformance["venueCode"].GetString();

    auto& prices = jsonPerformance["prices"];
    perf.prices.resize(prices.Size());
    for (size_t i = 0; i < prices.Size(); ++i) {
        getValue(prices[i], perf.prices[i]);
    }

    auto& seatCategories = jsonPerformance["seatCategories"];
    perf.seatCategories.resize(seatCategories.Size());
    for (size_t i = 0; i < seatCategories.Size(); ++i) {
        getValue(seatCategories[i], perf.seatCategories[i]);
    }
}

template<>
inline void getValue(rapidjson::Value& document, citm_catalog_message& coordinates) {
    auto& areaNames = document["areaNames"];
    for (auto itr = areaNames.MemberBegin(); itr != areaNames.MemberEnd(); ++itr) {
        coordinates.areaNames[itr->name.GetString()] = itr->value.GetString();
    }

    coordinates.audienceSubCategoryNames.the337100890 = document["audienceSubCategoryNames"]["337100890"].GetString();

    auto& events = document["events"];
    for (auto itr = events.MemberBegin(); itr != events.MemberEnd(); ++itr) {
        event evt;
        getValue(itr->value, evt);
        coordinates.events[itr->name.GetString()] = evt;
    }

    auto& performances = document["performances"];
    coordinates.performances.resize(performances.Size());
    for (size_t i = 0; i < performances.Size(); ++i) {
        getValue(performances[i], coordinates.performances[i]);
    }

    auto& seatCategoryNames = document["seatCategoryNames"];
    for (auto itr = seatCategoryNames.MemberBegin(); itr != seatCategoryNames.MemberEnd(); ++itr) {
        coordinates.seatCategoryNames[itr->name.GetString()] = itr->value.GetString();
    }

    auto& subTopicNames = document["subTopicNames"];
    for (auto itr = subTopicNames.MemberBegin(); itr != subTopicNames.MemberEnd(); ++itr) {
        coordinates.subTopicNames[itr->name.GetString()] = itr->value.GetString();
    }

    auto& topicNames = document["topicNames"];
    for (auto itr = topicNames.MemberBegin(); itr != topicNames.MemberEnd(); ++itr) {
        coordinates.topicNames[itr->name.GetString()] = itr->value.GetString();
    }

    auto& topicSubTopics = document["topicSubTopics"];
    for (auto itr = topicSubTopics.MemberBegin(); itr != topicSubTopics.MemberEnd(); ++itr) {
        std::vector<int64_t> subTopics;
        const auto& jsonSubTopics = itr->value.GetArray();
        for (const auto& subTopic : jsonSubTopics) {
            subTopics.push_back(subTopic.GetInt64());
        }
        coordinates.topicSubTopics[itr->name.GetString()] = std::move(subTopics);
    }

    coordinates.venueNames.PLEYEL_PLEYEL = document["venueNames"]["PLEYEL_PLEYEL"].GetString();
}

template<>
inline void getValue(rapidjson::Value& jsonValue, search_metadata_data& data) {
    data.completed_in = jsonValue["completed_in"].GetDouble();
    data.max_id = jsonValue["max_id"].GetInt64();
    data.max_id_str = jsonValue["max_id_str"].GetString();
    data.next_results = jsonValue["next_results"].GetString();
    data.query = jsonValue["query"].GetString();
    data.refresh_url = jsonValue["refresh_url"].GetString();
    data.count = jsonValue["count"].GetInt64();
    data.since_id = jsonValue["since_id"].GetInt64();
    data.since_id_str = jsonValue["since_id_str"].GetString();
}

template<>
inline void getValue(rapidjson::Value& jsonValue, hashtag_data& data) {
    data.text = jsonValue["text"].GetString();

    auto& indices = jsonValue["indices"];
    data.indices.resize(indices.Size());
    for (size_t i = 0; i < indices.Size(); ++i) {
        data.indices[i] = indices[i].GetInt64();
    }
}

template<>
inline void getValue(rapidjson::Value& jsonValue, large_data& data) {
    data.w = jsonValue["w"].GetInt64();
    data.h = jsonValue["h"].GetInt64();
    data.resize = jsonValue["resize"].GetString();
}

template<>
inline void getValue(rapidjson::Value& jsonValue, sizes_data& data) {
    getValue(jsonValue["medium"], data.medium);
    getValue(jsonValue["small"], data.small);
    getValue(jsonValue["thumb"], data.thumb);
    getValue(jsonValue["large"], data.large);
}

template<>
inline void getValue(rapidjson::Value& jsonValue, media_data& data) {
    data.id = jsonValue["id"].GetInt64();
    data.id_str = jsonValue["id_str"].GetString();

    auto& indices = jsonValue["indices"];
    data.indices.resize(indices.Size());
    for (size_t i = 0; i < indices.Size(); ++i) {
        data.indices[i] = indices[i].GetInt64();
    }

    data.media_url = jsonValue["media_url"].GetString();
    data.media_url_https = jsonValue["media_url_https"].GetString();
    data.url = jsonValue["url"].GetString();
    data.display_url = jsonValue["display_url"].GetString();
    data.expanded_url = jsonValue["expanded_url"].GetString();
    data.type = jsonValue["type"].GetString();
    getValue(jsonValue["sizes"], data.sizes);

    if (jsonValue.HasMember("source_status_id")) {
        data.source_status_id = jsonValue["source_status_id"].GetInt64();
    }

    if (jsonValue.HasMember("source_status_id_str")) {
        data.source_status_id_str = jsonValue["source_status_id_str"].GetString();
    }
}

template<>
inline void getValue(rapidjson::Value& jsonValue, url_data& data) {
    data.url = jsonValue["url"].GetString();
    data.expanded_url = jsonValue["expanded_url"].GetString();
    data.display_url = jsonValue["display_url"].GetString();

    auto& indices = jsonValue["indices"];
    data.indices.resize(indices.Size());
    for (size_t i = 0; i < indices.Size(); ++i) {
        data.indices[i] = indices[i].GetInt64();
    }
}

template<>
inline void getValue(rapidjson::Value& jsonValue, user_mention& data) {
    data.screen_name = jsonValue["screen_name"].GetString();
    data.name = jsonValue["name"].GetString();
    data.id = jsonValue["id"].GetInt64();
    data.id_str = jsonValue["id_str"].GetString();

    auto& indices = jsonValue["indices"];
    data.indices.resize(indices.Size());
    for (size_t i = 0; i < indices.Size(); ++i) {
        data.indices[i] = indices[i].GetInt64();
    }
}

template<>
inline void getValue(rapidjson::Value& jsonValue, status_entities& data) {
    auto& hashtags = jsonValue["hashtags"];
    data.hashtags.resize(hashtags.Size());
    for (size_t i = 0; i < hashtags.Size(); ++i) {
        getValue(hashtags[i], data.hashtags[i]);
    }

    auto& urls = jsonValue["urls"];
    data.urls.resize(urls.Size());
    for (size_t i = 0; i < urls.Size(); ++i) {
        getValue(urls[i], data.urls[i]);
    }

    auto& user_mentions = jsonValue["user_mentions"];
    data.user_mentions.resize(user_mentions.Size());
    for (size_t i = 0; i < user_mentions.Size(); ++i) {
        getValue(user_mentions[i], data.user_mentions[i]);
    }

    if (jsonValue.HasMember("media") && jsonValue["media"].IsArray()) {
        auto& mediaArray = jsonValue["media"];
        data.media = std::vector<media_data>();
        data.media->resize(mediaArray.Size());
        for (size_t i = 0; i < mediaArray.Size(); ++i) {
            getValue(mediaArray[i], (*data.media)[i]);
        }
    }
}

template<>
inline void getValue(rapidjson::Value& jsonValue, metadata_data& data) {
    data.result_type = jsonValue["result_type"].GetString();
    data.iso_language_code = jsonValue["iso_language_code"].GetString();
}

template<>
inline void getValue(rapidjson::Value& jsonValue, description_data& data) {
    auto& urls = jsonValue["urls"];
    data.urls.resize(urls.Size());
    for (size_t i = 0; i < urls.Size(); ++i) {
        getValue(urls[i], data.urls[i]);
    }
}

template<>
inline void getValue(rapidjson::Value& jsonValue, user_entities& data) {
    getValue(jsonValue["description"], data.description);
    if (jsonValue.HasMember("url")) {
        getValue(jsonValue["url"], *data.url);
    }
}

using tuple_type = Pass01;
static constexpr auto tupleSize = std::tuple_size_v<tuple_type>;

// For std::map<K, V>, recursively handle the key-value pairs
template<typename K, typename V>
void getValue(rapidjson::Value& jsonValue, std::map<K, V>& map) {
    if (jsonValue.IsObject()) {
        for (auto itr = jsonValue.MemberBegin(); itr != jsonValue.MemberEnd(); ++itr) {
            K key;
            V value;
            getValue(itr->name, key);     // Get the key
            getValue(itr->value, value);  // Get the value
            map.emplace(std::move(key), std::move(value));
        }
    }
}

inline void getValue(const rapidjson::Value& jsonValue, std::string& value) {
    if (jsonValue.IsString()) {
        value = jsonValue.GetString();
    } else {
        throw std::runtime_error("Expected JSON string");
    }
}

// Helper function to handle each element of the tuple recursively.
template<std::size_t Index = 0, typename... Types>
inline typename std::enable_if<Index == sizeof...(Types), void>::type
parseTupleElements(rapidjson::Value& , std::tuple<Types...>& ) {
    // Base case: If we've reached the end of the tuple, do nothing.
}

template<std::size_t Index = 0, typename... Types>
inline typename std::enable_if < Index < sizeof...(Types), void>::type
    parseTupleElements(rapidjson::Value& jsonValue, std::tuple<Types...>& tuple) {
    // Call getValue for each tuple element
    getValue(jsonValue[Index], std::get<Index>(tuple));
    // Recurse to handle the next tuple element
    parseTupleElements<Index + 1>(jsonValue, tuple);
}

// The main function to get value from a JSON array and parse it into a tuple.
template<typename... Args>
inline void getValue(rapidjson::Value& jsonValue, tuple_type& tuple) {
    if (jsonValue.IsArray()) {
        parseTupleElements<>(jsonValue, tuple);
    }
    else {
        // Handle the error case: the JSON value is not an array.
        throw std::runtime_error("Expected JSON array for tuple parsing");
    }
}


template<>
inline void getValue(rapidjson::Value& jsonValue, twitter_user_data& data) {
    data.id = jsonValue["id"].GetInt64();
    data.id_str = jsonValue["id_str"].GetString();
    data.name = jsonValue["name"].GetString();
    data.screen_name = jsonValue["screen_name"].GetString();
    data.location = jsonValue["location"].GetString();
    data.description = jsonValue["description"].GetString();

    if (jsonValue.HasMember("url")) {
        data.url = jsonValue["url"].GetString();
    }

    getValue(jsonValue["entities"], data.entities);
    data.protectedVal = jsonValue["protected"].GetBool();
    data.followers_count = jsonValue["followers_count"].GetInt64();
    data.friends_count = jsonValue["friends_count"].GetInt64();
    data.listed_count = jsonValue["listed_count"].GetInt64();
    data.created_at = jsonValue["created_at"].GetString();
    data.favourites_count = jsonValue["favourites_count"].GetInt64();

    if (jsonValue.HasMember("utc_offset")) {
        data.utc_offset = jsonValue["utc_offset"].GetInt64();
    }

    if (jsonValue.HasMember("time_zone")) {
        data.time_zone = jsonValue["time_zone"].GetString();
    }

    data.geo_enabled = jsonValue["geo_enabled"].GetBool();
    data.verified = jsonValue["verified"].GetBool();
    data.statuses_count = jsonValue["statuses_count"].GetInt64();
    data.lang = jsonValue["lang"].GetString();
    data.contributors_enabled = jsonValue["contributors_enabled"].GetBool();
    data.is_translator = jsonValue["is_translator"].GetBool();
    data.is_translation_enabled = jsonValue["is_translation_enabled"].GetBool();
    data.profile_background_color = jsonValue["profile_background_color"].GetString();
    data.profile_background_image_url = jsonValue["profile_background_image_url"].GetString();
    data.profile_background_image_url_https = jsonValue["profile_background_image_url_https"].GetString();
    data.profile_background_tile = jsonValue["profile_background_tile"].GetBool();
    data.profile_image_url = jsonValue["profile_image_url"].GetString();
    data.profile_image_url_https = jsonValue["profile_image_url_https"].GetString();

    if (jsonValue.HasMember("profile_banner_url")) {
        data.profile_banner_url = jsonValue["profile_banner_url"].GetString();
    }

    data.profile_link_color = jsonValue["profile_link_color"].GetString();
    data.profile_sidebar_border_color = jsonValue["profile_sidebar_border_color"].GetString();
    data.profile_sidebar_fill_color = jsonValue["profile_sidebar_fill_color"].GetString();
    data.profile_text_color = jsonValue["profile_text_color"].GetString();
    data.profile_use_background_image = jsonValue["profile_use_background_image"].GetBool();
    data.default_profile = jsonValue["default_profile"].GetBool();
    data.default_profile_image = jsonValue["default_profile_image"].GetBool();
    data.following = jsonValue["following"].GetBool();
    data.follow_request_sent = jsonValue["follow_request_sent"].GetBool();
    data.notifications = jsonValue["notifications"].GetBool();
}

template<>
inline void getValue(rapidjson::Value& jsonValue, status_data& data) {
    getValue(jsonValue["metadata"], data.metadata);
    data.created_at = jsonValue["created_at"].GetString();
    data.id = jsonValue["id"].GetInt64();
    data.id_str = jsonValue["id_str"].GetString();
    data.text = jsonValue["text"].GetString();
    data.source = jsonValue["source"].GetString();
    data.truncated = jsonValue["truncated"].GetBool();

    if (jsonValue.HasMember("in_reply_to_status_id")) {
        data.in_reply_to_status_id = jsonValue["in_reply_to_status_id"].GetInt64();
    }

    if (jsonValue.HasMember("in_reply_to_status_id_str")) {
        data.in_reply_to_status_id_str = jsonValue["in_reply_to_status_id_str"].GetString();
    }

    if (jsonValue.HasMember("in_reply_to_user_id")) {
        data.in_reply_to_user_id = jsonValue["in_reply_to_user_id"].GetInt64();
    }

    if (jsonValue.HasMember("in_reply_to_user_id_str")) {
        data.in_reply_to_user_id_str = jsonValue["in_reply_to_user_id_str"].GetString();
    }

    if (jsonValue.HasMember("in_reply_to_screen_name")) {
        data.in_reply_to_screen_name = jsonValue["in_reply_to_screen_name"].GetString();
    }

    getValue(jsonValue["user"], data.user);
    data.geo = nullptr;
    data.coordinates = nullptr;
    data.place = nullptr;
    data.contributors = nullptr;
    data.retweet_count = jsonValue["retweet_count"].GetInt64();
    data.favorite_count = jsonValue["favorite_count"].GetInt64();
    getValue(jsonValue["entities"], data.entities);
    data.favorited = jsonValue["favorited"].GetBool();
    data.retweeted = jsonValue["retweeted"].GetBool();
    data.lang = jsonValue["lang"].GetString();

    if (jsonValue.HasMember("retweeted_status")) {
        data.retweeted_status = std::make_shared<status_data>();
        getValue(jsonValue["retweeted_status"], *data.retweeted_status);
    }

    if (jsonValue.HasMember("possibly_sensitive")) {
        data.possibly_sensitive = jsonValue["possibly_sensitive"].GetBool();
    }
}

template<>
inline void getValue(rapidjson::Value& jsonValue, twitter_message& message) {
    auto& statuses = jsonValue["statuses"];
    message.statuses.resize(statuses.Size());
    for (size_t i = 0; i < statuses.Size(); ++i) {
        getValue(statuses[i], message.statuses[i]);
    }

    getValue(jsonValue["search_metadata"], message.search_metadata);
}

template<typename value_type> void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, value_type& coordinates);

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, search_metadata_data& data) {
    writer.Key("completed_in");
    writer.Double(data.completed_in);
    writer.Key("max_id");
    writer.Int64(data.max_id);
    writer.Key("max_id_str");
    writer.String(data.max_id_str.c_str());
    writer.Key("next_results");
    writer.String(data.next_results.c_str());
    writer.Key("query");
    writer.String(data.query.c_str());
    writer.Key("refresh_url");
    writer.String(data.refresh_url.c_str());
    writer.Key("count");
    writer.Int64(data.count);
    writer.Key("since_id");
    writer.Int64(data.since_id);
    writer.Key("since_id_str");
    writer.String(data.since_id_str.c_str());
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, geometry_data& geometry) {
    writer.StartObject();
    writer.Key("type");
    writer.String(geometry.type.c_str());
    writer.Key("coordinates");
    putValue(writer, geometry.coordinates);
    writer.EndObject();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, properties_data& properties) {
    writer.StartObject();
    writer.Key("name");
    writer.String(properties.name.c_str());
    writer.EndObject();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, feature& feat) {
    writer.StartObject();
    writer.Key("type");
    writer.String(feat.type.c_str());

    writer.Key("properties");
    putValue(writer, feat.properties);

    writer.Key("geometry");
    putValue(writer, feat.geometry);

    writer.EndObject();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, canada_message& msg) {

    writer.StartObject();
    writer.Key("type");
    writer.String(msg.type.c_str());

    writer.Key("features");
    writer.StartArray();
    for (auto& feat : msg.features) {
        putValue(writer, feat);
    }
    writer.EndArray();
    writer.EndObject();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, price& prc) {
    writer.StartObject();
    writer.Key("amount");
    writer.Int64(prc.amount);
    writer.Key("audienceSubCategoryId");
    writer.Int64(prc.audienceSubCategoryId);
    writer.Key("seatCategoryId");
    writer.Int64(prc.seatCategoryId);
    writer.EndObject();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, area& ar) {
    writer.StartObject();
    writer.Key("areaId");
    writer.Int64(ar.areaId);
    writer.Key("blockIds");
    writer.StartArray();
    for (auto& value : ar.blockIds) {
        (void)value;
        writer.Null();
    }
    writer.EndArray();
    writer.EndObject();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, seat_category& seatCat) {
    writer.StartObject();
    writer.Key("seatCategoryId");
    writer.Int64(seatCat.seatCategoryId);

    writer.Key("areas");
    writer.StartArray();
    for (auto& ar : seatCat.areas) {
        putValue(writer, ar);
    }
    writer.EndArray();
    writer.EndObject();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, performance& perf) {
    writer.StartObject();
    writer.Key("eventId");
    writer.Int64(perf.eventId);
    writer.Key("id");
    writer.Int64(perf.id);

    if (perf.logo.has_value()) {
        writer.Key("logo");
        writer.String(perf.logo.value().c_str());
    }

    writer.Key("start");
    writer.Int64(perf.start);
    writer.Key("venueCode");
    writer.String(perf.venueCode.c_str());

    writer.Key("prices");
    writer.StartArray();
    for (auto& prc : perf.prices) {
        putValue(writer, prc);
    }
    writer.EndArray();

    writer.Key("seatCategories");
    writer.StartArray();
    for (auto& seatCat : perf.seatCategories) {
        putValue(writer, seatCat);
    }
    writer.EndArray();

    writer.EndObject();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, citm_catalog_message& msg) {

    writer.StartObject();

    writer.Key("areaNames");
    writer.StartObject();
    for (auto& areaName : msg.areaNames) {
        writer.Key(areaName.first.c_str());
        writer.String(areaName.second.c_str());
    }
    writer.EndObject();

    writer.Key("audienceSubCategoryNames");
    writer.StartObject();
    writer.Key("337100890");
    writer.String(msg.audienceSubCategoryNames.the337100890.c_str());
    writer.EndObject();

    writer.Key("events");
    writer.StartObject();
    for (auto& event : msg.events) {
        writer.Key(event.first.c_str());
        writer.StartObject();
        writer.Key("id");
        writer.Int64(event.second.id);
        writer.Key("name");
        writer.String(event.second.name.c_str());
        writer.Key("subTopicIds");
        writer.StartArray();
        for (auto& subTopicId : event.second.subTopicIds) {
            writer.Int64(subTopicId);
        }
        writer.EndArray();
        writer.Key("topicIds");
        writer.StartArray();
        for (auto& topicId : event.second.topicIds) {
            writer.Int64(topicId);
        }
        writer.EndArray();
        writer.EndObject();
    }
    writer.EndObject();

    writer.Key("performances");
    writer.StartArray();
    for (auto& perf : msg.performances) {
        putValue(writer, perf);
    }
    writer.EndArray();

    writer.Key("seatCategoryNames");
    writer.StartObject();
    for (auto& seatCategoryName : msg.seatCategoryNames) {
        writer.Key(seatCategoryName.first.c_str());
        writer.String(seatCategoryName.second.c_str());
    }
    writer.EndObject();

    writer.Key("subTopicNames");
    writer.StartObject();
    for (auto& subTopicName : msg.subTopicNames) {
        writer.Key(subTopicName.first.c_str());
        writer.String(subTopicName.second.c_str());
    }
    writer.EndObject();

    writer.Key("topicNames");
    writer.StartObject();
    for (auto& topicName : msg.topicNames) {
        writer.Key(topicName.first.c_str());
        writer.String(topicName.second.c_str());
    }
    writer.EndObject();

    writer.Key("topicSubTopics");
    writer.StartObject();
    for (auto& topicSubTopic : msg.topicSubTopics) {
        writer.Key(topicSubTopic.first.c_str());
        writer.StartArray();
        for (auto& subTopicId : topicSubTopic.second) {
            writer.Int64(subTopicId);
        }
        writer.EndArray();
    }
    writer.EndObject();

    writer.Key("venueNames");
    writer.StartObject();
    writer.Key("PLEYEL_PLEYEL");
    writer.String(msg.venueNames.PLEYEL_PLEYEL.c_str());
    writer.EndObject();

    writer.EndObject();
}

template<typename value_type>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::unique_ptr<value_type>& data) {
    if (data) {
        putValue(writer, *data);
    }
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::string& data) {
    writer.String(data.data());
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::string*& data) {
    if (data) {
        putValue(writer, *data);
    }
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, int& data) {
    writer.Int(data);
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, double& data) {
    writer.Double(data);
}

template<typename value_type>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer> &writer, std::vector<value_type>& values) {
    writer.StartArray();
    for (auto& val : values) {
        putValue(writer, val);
    }
    writer.EndArray();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, twitter_message& msg) {
    writer.StartObject();
    writer.Key("statuses");
    putValue(writer, msg.statuses);
    writer.Key("search_metadata");
    putValue(writer, msg.search_metadata);
    writer.EndObject();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, Obj3& obj) {
    writer.StartObject();
    writer.Key("foo");
    putValue(writer, obj.foo);
    writer.Key("a");
    putValue(writer, obj.a);
    writer.EndObject();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, Obj2& obj) {

    writer.StartObject();
    writer.Key("foo");
    putValue(writer, obj.foo);
    writer.EndObject();

}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::vector<double>& values) {

    writer.StartArray();
    for (auto& val : values) {
        writer.Double(val);
    }
    writer.EndArray();

}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::vector<long int>& values) {

    writer.StartArray();
    for (auto& val : values) {
        writer.Int64(val);
    }
    writer.EndArray();

}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::vector<int>& values) {

    writer.StartArray();
    for (auto& val : values) {
        writer.Int(val);
    }
    writer.EndArray();

}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::vector<bool>& values) {

    writer.StartArray();
    for (const auto& val : values) {
        writer.Bool(val);
    }
    writer.EndArray();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::vector<int*>& values) {

    writer.StartArray();
    for (auto& ptr : values) {
        writer.Int(*ptr);
    }
    writer.EndArray();
}

template <size_t currentIndex = 0> bool putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, tuple_type& out_value) {
    if constexpr (currentIndex < tupleSize) {
        putValue(writer, std::get<currentIndex>(out_value));
        return putValue<currentIndex + 1>(writer, out_value);
    }
    return true;
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, hashtag_data& data) {
    writer.Key("text");
    writer.String(data.text.c_str());
    writer.Key("indices");
    writer.StartArray();
    for (auto& idx : data.indices) {
        writer.Int64(idx);
    }
    writer.EndArray();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, large_data& data) {
    writer.Key("w");
    writer.Int64(data.w);
    writer.Key("h");
    writer.Int64(data.h);
    writer.Key("resize");
    writer.String(data.resize.c_str());
}

inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, sizes_data& data) {
    writer.Key("medium");
    putValue(writer, data.medium);
    writer.Key("small");
    putValue(writer, data.small);
    writer.Key("thumb");
    putValue(writer, data.thumb);
    writer.Key("large");
    putValue(writer, data.large);
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, media_data& data) {
    writer.Key("id");
    writer.Int64(data.id);
    writer.Key("id_str");
    writer.String(data.id_str.c_str());
    writer.Key("indices");
    writer.StartArray();
    for (auto& idx : data.indices) {
        writer.Int64(idx);
    }
    writer.EndArray();
    writer.Key("media_url");
    writer.String(data.media_url.c_str());
    writer.Key("media_url_https");
    writer.String(data.media_url_https.c_str());
    writer.Key("url");
    writer.String(data.url.c_str());
    writer.Key("display_url");
    writer.String(data.display_url.c_str());
    writer.Key("expanded_url");
    writer.String(data.expanded_url.c_str());
    writer.Key("type");
    writer.String(data.type.c_str());
    writer.Key("sizes");
    putValue(writer, data.sizes);
    if (data.source_status_id.has_value()) {
        writer.Key("source_status_id");
        writer.Int64(data.source_status_id.value());
    }
    if (data.source_status_id_str.has_value()) {
        writer.Key("source_status_id_str");
        writer.String(data.source_status_id_str.value().c_str());
    }
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, url_data& data) {
    writer.Key("url");
    writer.String(data.url.c_str());
    writer.Key("expanded_url");
    writer.String(data.expanded_url.c_str());
    writer.Key("display_url");
    writer.String(data.display_url.c_str());
    writer.Key("indices");
    writer.StartArray();
    for (auto& idx : data.indices) {
        writer.Int64(idx);
    }
    writer.EndArray();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, user_mention& data) {
    writer.Key("screen_name");
    writer.String(data.screen_name.c_str());
    writer.Key("name");
    writer.String(data.name.c_str());
    writer.Key("id");
    writer.Int64(data.id);
    writer.Key("id_str");
    writer.String(data.id_str.c_str());
    writer.Key("indices");
    writer.StartArray();
    for (auto& idx : data.indices) {
        writer.Int64(idx);
    }
    writer.EndArray();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, status_entities& data) {
    writer.Key("hashtags");
    writer.StartArray();
    for (auto& hashtag : data.hashtags) {
        putValue(writer, hashtag);
    }
    writer.EndArray();

    writer.Key("symbols");
    writer.StartArray();
    writer.EndArray();

    writer.Key("urls");
    writer.StartArray();
    for (auto& url : data.urls) {
        putValue(writer, url);
    }
    writer.EndArray();

    writer.Key("user_mentions");
    writer.StartArray();
    for (auto& mention : data.user_mentions) {
        putValue(writer, mention);
    }
    writer.EndArray();

    if (data.media.has_value()) {
        writer.Key("media");
        writer.StartArray();
        for (auto& media : data.media.value()) {
            putValue(writer, media);
        }
        writer.EndArray();
    }
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, metadata_data& data) {
    writer.Key("result_type");
    writer.String(data.result_type.c_str());
    writer.Key("iso_language_code");
    writer.String(data.iso_language_code.c_str());
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, description_data& data) {
    writer.Key("urls");
    writer.StartArray();
    for (auto& url : data.urls) {
        putValue(writer, url);
    }
    writer.EndArray();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, user_entities& data) {
    writer.Key("description");
    putValue(writer, data.description);
    if (data.url.has_value()) {
        writer.Key("url");
        putValue(writer, data.url.value());
    }
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, twitter_user_data& data) {
    writer.Key("id");
    writer.Int64(data.id);
    writer.Key("id_str");
    writer.String(data.id_str.c_str());
    writer.Key("name");
    writer.String(data.name.c_str());
    writer.Key("screen_name");
    writer.String(data.screen_name.c_str());
    writer.Key("location");
    writer.String(data.location.c_str());
    writer.Key("description");
    writer.String(data.description.c_str());
    if (data.url.has_value()) {
        writer.Key("url");
        writer.String(data.url.value().c_str());
    }
    writer.Key("entities");
    putValue(writer, data.entities);
    writer.Key("protected");
    writer.Bool(data.protectedVal);
    writer.Key("followers_count");
    writer.Int64(data.followers_count);
    writer.Key("friends_count");
    writer.Int64(data.friends_count);
    writer.Key("listed_count");
    writer.Int64(data.listed_count);
    writer.Key("created_at");
    writer.String(data.created_at.c_str());
    writer.Key("favourites_count");
    writer.Int64(data.favourites_count);
    if (data.utc_offset.has_value()) {
        writer.Key("utc_offset");
        writer.Int64(data.utc_offset.value());
    }
    if (data.time_zone.has_value()) {
        writer.Key("time_zone");
        writer.String(data.time_zone.value().c_str());
    }
    writer.Key("geo_enabled");
    writer.Bool(data.geo_enabled);
    writer.Key("verified");
    writer.Bool(data.verified);
    writer.Key("statuses_count");
    writer.Int64(data.statuses_count);
    writer.Key("lang");
    writer.String(data.lang.c_str());
    writer.Key("contributors_enabled");
    writer.Bool(data.contributors_enabled);
    writer.Key("is_translator");
    writer.Bool(data.is_translator);
    writer.Key("is_translation_enabled");
    writer.Bool(data.is_translation_enabled);
    writer.Key("profile_background_color");
    writer.String(data.profile_background_color.c_str());
    writer.Key("profile_background_image_url");
    writer.String(data.profile_background_image_url.c_str());
    writer.Key("profile_background_image_url_https");
    writer.String(data.profile_background_image_url_https.c_str());
    writer.Key("profile_background_tile");
    writer.Bool(data.profile_background_tile);
    writer.Key("profile_image_url");
    writer.String(data.profile_image_url.c_str());
    writer.Key("profile_image_url_https");
    writer.String(data.profile_image_url_https.c_str());
    if (data.profile_banner_url.has_value()) {
        writer.Key("profile_banner_url");
        writer.String(data.profile_banner_url.value().c_str());
    }
    writer.Key("profile_link_color");
    writer.String(data.profile_link_color.c_str());
    writer.Key("profile_sidebar_border_color");
    writer.String(data.profile_sidebar_border_color.c_str());
    writer.Key("profile_sidebar_fill_color");
    writer.String(data.profile_sidebar_fill_color.c_str());
    writer.Key("profile_text_color");
    writer.String(data.profile_text_color.c_str());
    writer.Key("profile_use_background_image");
    writer.Bool(data.profile_use_background_image);
    writer.Key("default_profile");
    writer.Bool(data.default_profile);
    writer.Key("default_profile_image");
    writer.Bool(data.default_profile_image);
    writer.Key("following");
    writer.Bool(data.following);
    writer.Key("follow_request_sent");
    writer.Bool(data.follow_request_sent);
    writer.Key("notifications");
    writer.Bool(data.notifications);
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, status_data& data) {
    writer.Key("metadata");
    putValue(writer, data.metadata);
    writer.Key("created_at");
    writer.String(data.created_at.c_str());
    writer.Key("id");
    writer.Int64(data.id);
    writer.Key("id_str");
    writer.String(data.id_str.c_str());
    writer.Key("text");
    writer.String(data.text.c_str());
    writer.Key("source");
    writer.String(data.source.c_str());
    writer.Key("truncated");
    writer.Bool(data.truncated);
    if (data.in_reply_to_status_id.has_value()) {
        writer.Key("in_reply_to_status_id");
        writer.Int64(data.in_reply_to_status_id.value());
    }
    if (data.in_reply_to_status_id_str.has_value()) {
        writer.Key("in_reply_to_status_id_str");
        writer.String(data.in_reply_to_status_id_str.value().c_str());
    }
    if (data.in_reply_to_user_id.has_value()) {
        writer.Key("in_reply_to_user_id");
        writer.Int64(data.in_reply_to_user_id.value());
    }
    if (data.in_reply_to_user_id_str.has_value()) {
        writer.Key("in_reply_to_user_id_str");
        writer.String(data.in_reply_to_user_id_str.value().c_str());
    }
    if (data.in_reply_to_screen_name.has_value()) {
        writer.Key("in_reply_to_screen_name");
        writer.String(data.in_reply_to_screen_name.value().c_str());
    }
    writer.Key("user");
    putValue(writer, data.user);
    writer.Key("geo");
    writer.Null();
    writer.Key("coordinates");
    writer.Null();
    writer.Key("place");
    writer.Null();
    writer.Key("contributors");
    writer.Null();
    writer.Key("retweet_count");
    writer.Int64(data.retweet_count);
    writer.Key("favorite_count");
    writer.Int64(data.favorite_count);
    writer.Key("entities");
    putValue(writer, data.entities);
    writer.Key("favorited");
    writer.Bool(data.favorited);
    writer.Key("retweeted");
    writer.Bool(data.retweeted);
    writer.Key("lang");
    writer.String(data.lang.c_str());
    if (data.retweeted_status) {
        writer.Key("retweeted_status");
        putValue(writer, *data.retweeted_status);
    }
    if (data.possibly_sensitive.has_value()) {
        writer.Key("possibly_sensitive");
        writer.Bool(data.possibly_sensitive.value());
    }
}

template<typename value_type>
inline void getValue(rapidjson::Value& jsonCoordinates, value_type& coordinates);

template<>
inline void getValue<std::unordered_map<std::string, std::string>>(rapidjson::Value& jsonCoordinates, std::unordered_map<std::string, std::string>& coordinates) {
    for (auto& m : jsonCoordinates.GetObject()) {
        coordinates[m.name.GetString()] = m.value.GetString();
    }
}

template<>
inline void getValue<std::unordered_map<std::string, bool>>(rapidjson::Value& jsonCoordinates, std::unordered_map<std::string, bool>& coordinates) {
    for (auto& m : jsonCoordinates.GetObject()) {
        coordinates[m.name.GetString()] = m.value.GetBool();
    }
}

template<>
inline void getValue<std::unordered_map<std::string, int>>(rapidjson::Value& jsonCoordinates, std::unordered_map<std::string, int>& coordinates) {
    for (auto& m : jsonCoordinates.GetObject()) {
        coordinates[m.name.GetString()] = m.value.GetInt();
    }
}

template<>
inline void getValue<std::unordered_map<std::string, int*>>(rapidjson::Value& jsonCoordinates, std::unordered_map<std::string, int*>& coordinates) {
    for (auto& m : jsonCoordinates.GetObject()) {
        int* value = new int(m.value.GetInt());
        coordinates[m.name.GetString()] = value;
    }
}

template<>
inline void getValue<std::unordered_map<std::string, std::map<std::string, std::string>>>(rapidjson::Value& jsonCoordinates, std::unordered_map<std::string, std::map<std::string, std::string>>& coordinates) {
    for (auto& m : jsonCoordinates.GetObject()) {
        std::map<std::string, std::string> subMap;
        for (auto& subM : m.value.GetObject()) {
            subMap[subM.name.GetString()] = subM.value.GetString();
        }
        coordinates[m.name.GetString()] = subMap;
    }
}

template<>
inline void getValue<std::vector<std::string>>(rapidjson::Value& jsonCoordinates, std::vector<std::string>& coordinates) {
    for (auto& m : jsonCoordinates.GetArray()) {
        coordinates.push_back(m.GetString());
    }
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::unordered_map<std::string, std::string>& msg) {
    writer.StartObject();
    for (auto& pair : msg) {
        writer.Key(pair.first.c_str());
        writer.String(pair.second.c_str());
    }
    writer.EndObject();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::unordered_map<std::string, bool>& msg) {
    writer.StartObject();
    for (auto& pair : msg) {
        writer.Key(pair.first.c_str());
        writer.Bool(pair.second);
    }
    writer.EndObject();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::unordered_map<std::string, int>& msg) {
    writer.StartObject();
    for (auto& pair : msg) {
        writer.Key(pair.first.c_str());
        writer.Int(pair.second);
    }
    writer.EndObject();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::vector<std::string>& msg) {
    writer.StartArray();
    for (auto& str : msg) {
        writer.String(str.c_str());
    }
    writer.EndArray();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::unordered_map<std::string, std::map<std::string, std::string>>& msg) {
    writer.StartObject();
    for (auto& outerPair : msg) {
        writer.Key(outerPair.first.c_str());
        writer.StartObject();
        for (auto& innerPair : outerPair.second) {
            writer.Key(innerPair.first.c_str());
            writer.String(innerPair.second.c_str());
        }
        writer.EndObject();
    }
    writer.EndObject();
}

template<>
inline void putValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, std::unordered_map<std::string, int*>& msg) {
    writer.StartObject();
    for (auto& pair : msg) {
        writer.Key(pair.first.c_str());
        writer.Int(*pair.second);
    }
    writer.EndObject();
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

    virtual const char* GetName()     const override { return "Jsonifier"; }
    virtual const char* Type()        const override { return "C++"; }
    virtual const char* GetFilename() const override { return __FILE__; }
};

#endif


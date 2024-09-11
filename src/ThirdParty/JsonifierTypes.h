#ifndef JSONIFIERTYPES_H
#define JSONIFIERTYPES_H

#include "TypeSafe.h"
#include "jsonifier/Index.hpp"

template<> struct jsonifier::core<geometry_data> {
	using value_type = geometry_data;
	static constexpr auto parseValue = createValue("coordinates", &value_type::coordinates, "type", &value_type::type);
};

template<> struct jsonifier::core<properties_data> {
	using value_type = properties_data;
	static constexpr auto parseValue = createValue<&value_type::name>();
};

template<> struct jsonifier::core<feature> {
	using value_type = feature;
	static constexpr auto parseValue = createValue<&value_type::properties, &value_type::geometry, &value_type::type>();
};

template<> struct jsonifier::core<canada_message> {
	using value_type = canada_message;
	static constexpr auto parseValue = createValue<&value_type::features, &value_type::type>();
};

template<> struct jsonifier::core<Properties> {
    using value_type = Properties;
    static constexpr auto parseValue = createValue<&value_type::name>();
};

template<> struct jsonifier::core<Cord> {
    using value_type = Cord;
    static constexpr auto parseValue = createValue<&value_type::value>();
};

template<> struct jsonifier::core<Geometry> {
    using value_type = Geometry;
    static constexpr auto parseValue = createValue<&value_type::type, &value_type::coordinates>();
};

template<> struct jsonifier::core<Feature> {
    using value_type = Feature;
    static constexpr auto parseValue = createValue<&value_type::type, &value_type::properties, &value_type::geometry>();
};

template<> struct jsonifier::core<Country> {
    using value_type = Country;
    static constexpr auto parseValue = createValue<&value_type::type, &value_type::features>();
};

// Specialization for Event
template<>
struct jsonifier::core<Event> {
	using value_type = Event;
	static constexpr auto parseValue = createValue<
		&value_type::description,
		&value_type::id,
		&value_type::logo,
		&value_type::name,
		&value_type::subTopicIds,
		&value_type::subjectCode,
		&value_type::subtitle,
		&value_type::topicIds
	>();
};

// Specialization for Price
template<>
struct jsonifier::core<Price> {
	using value_type = Price;
	static constexpr auto parseValue = createValue<
		&value_type::amount,
		&value_type::audienceSubCategoryId,
		&value_type::seatCategoryId
	>();
};

// Specialization for Area
template<>
struct jsonifier::core<Area> {
	using value_type = Area;
	static constexpr auto parseValue = createValue<
		&value_type::areaId,
		&value_type::blockIds
	>();
};

// Specialization for SeatCategorie
template<>
struct jsonifier::core<SeatCategorie> {
	using value_type = SeatCategorie;
	static constexpr auto parseValue = createValue<
		&value_type::areas,
		&value_type::seatCategoryId
	>();
};

// Specialization for Performance
template<>
struct jsonifier::core<Performance> {
	using value_type = Performance;
	static constexpr auto parseValue = createValue<
		&value_type::eventId,
		&value_type::id,
		&value_type::logo,
		&value_type::name,
		&value_type::prices,
		&value_type::seatCategories,
		&value_type::seatMapImage,
		&value_type::start,
		&value_type::venueCode
	>();
};

// Specialization for VenueNames
template<>
struct jsonifier::core<VenueNames> {
	using value_type = VenueNames;
	static constexpr auto parseValue = createValue<
		&value_type::PLEYEL_PLEYEL
	>();
};

// Specialization for Perform
template<>
struct jsonifier::core<Perform> {
	using value_type = Perform;
	static constexpr auto parseValue = createValue<
		&value_type::areaNames,
		&value_type::audienceSubCategoryNames,
		&value_type::blockNames,
		&value_type::events,
		&value_type::performances,
		&value_type::seatCategoryNames,
		&value_type::subTopicNames,
		&value_type::subjectNames,
		&value_type::topicNames,
		&value_type::topicSubTopics,
		&value_type::venueNames
	>();
};


template<> struct jsonifier::core<Metadata> {
	using value_type = Metadata;
	static constexpr auto parseValue = createValue<
		&value_type::result_type,
		&value_type::iso_language_code
	>();
};

template<> struct jsonifier::core<URL> {
	using value_type = URL;
	static constexpr auto parseValue = createValue<
		&value_type::url,
		&value_type::expanded_url,
		&value_type::display_url,
		&value_type::indices
	>();
};

template<> struct jsonifier::core<URLObject> {
	using value_type = URLObject;
	static constexpr auto parseValue = createValue<
		&value_type::urls
	>();
};

template<> struct jsonifier::core<Entities> {
	using value_type = Entities;
	static constexpr auto parseValue = createValue<
		&value_type::url,
		&value_type::description
	>();
};

template<> struct jsonifier::core<User> {
	using value_type = User;
	static constexpr auto parseValue = createValue <
		&value_type::id,
		&value_type::id_str,
		&value_type::name,
		&value_type::screen_name,
		&value_type::location,
		&value_type::description,
		&value_type::url,
		&value_type::entities,
		&value_type::followers_count,
		&value_type::friends_count,
		&value_type::listed_count,
		&value_type::created_at,
		&value_type::favourites_count,
		&value_type::utc_offset,
		&value_type::time_zone,
		&value_type::geo_enabled,
		&value_type::verified,
		&value_type::statuses_count,
		&value_type::lang,
		&value_type::contributors_enabled,
		&value_type::is_translator,
		&value_type::is_translation_enabled,
		&value_type::profile_background_color,
		&value_type::profile_background_image_url,
		&value_type::profile_background_image_url_https,
		&value_type::profile_background_tile,
		&value_type::profile_image_url,
		&value_type::profile_image_url_https,
		&value_type::profile_banner_url,
		&value_type::profile_link_color,
		&value_type::profile_sidebar_border_color,
		&value_type::profile_sidebar_fill_color,
		&value_type::profile_text_color,
		&value_type::profile_use_background_image,
		&value_type::default_profile,
		&value_type::default_profile_image,
		&value_type::following,
		&value_type::follow_request_sent,
		&value_type::notifications
	> ();
};

template<> struct jsonifier::core<Hashtag> {
	using value_type = Hashtag;
	static constexpr auto parseValue = createValue<
		&value_type::text,
		&value_type::indices
	>();
};

template<> struct jsonifier::core<UserMention> {
	using value_type = UserMention;
	static constexpr auto parseValue = createValue<
		&value_type::screen_name,
		&value_type::name,
		&value_type::id,
		&value_type::id_str,
		&value_type::indices
	>();
};

template<> struct jsonifier::core<Size> {
	using value_type = Size;
	static constexpr auto parseValue = createValue<
		&value_type::w,
		&value_type::h,
		&value_type::resize
	>();
};

template<> struct jsonifier::core<Sizes> {
	using value_type = Sizes;
	static constexpr auto parseValue = createValue<
		&value_type::medium,
		&value_type::small,
		&value_type::thumb,
		&value_type::large
	>();
};

template<> struct jsonifier::core<Media> {
	using value_type = Media;
	static constexpr auto parseValue = createValue<
		&value_type::id,
		&value_type::id_str,
		&value_type::indices,
		&value_type::media_url,
		&value_type::media_url_https,
		&value_type::url,
		&value_type::display_url,
		&value_type::expanded_url,
		&value_type::type,
		&value_type::sizes,
		&value_type::source_status_id,
		&value_type::source_status_id_str
	>();
};

template<> struct jsonifier::core<TwitEntities> {
	using value_type = TwitEntities;
	static constexpr auto parseValue = createValue<
		&value_type::hashtags,
		&value_type::symbols,
		&value_type::urls,
		&value_type::user_mentions,
		&value_type::media
	>();
};

template<> struct jsonifier::core<BaseStatus> {
	using value_type = BaseStatus;
	static constexpr auto parseValue = createValue <
		&value_type::metadata,
		&value_type::created_at,
		&value_type::id,
		&value_type::id_str,
		&value_type::text,
		&value_type::source,
		&value_type::truncated,
		&value_type::in_reply_to_status_id,
		&value_type::in_reply_to_status_id_str,
		&value_type::in_reply_to_user_id,
		&value_type::in_reply_to_user_id_str,
		&value_type::in_reply_to_screen_name,
		&value_type::user,
		&value_type::geo,
		&value_type::coordinates,
		&value_type::place,
		&value_type::contributors,
		&value_type::retweet_count,
		&value_type::favorite_count,
		&value_type::entities,
		&value_type::favorited,
		&value_type::retweeted,
		&value_type::possibly_sensitive,
		&value_type::lang
	> ();
};

template<> struct jsonifier::core<MainStatus> {
	using value_type = MainStatus;
	static constexpr auto parseValue = createValue <
		&value_type::metadata,
		&value_type::created_at,
		&value_type::id,
		&value_type::id_str,
		&value_type::text,
		&value_type::source,
		&value_type::truncated,
		&value_type::in_reply_to_status_id,
		&value_type::in_reply_to_status_id_str,
		&value_type::in_reply_to_user_id,
		&value_type::in_reply_to_user_id_str,
		&value_type::in_reply_to_screen_name,
		&value_type::user,
		&value_type::geo,
		&value_type::coordinates,
		&value_type::place,
		&value_type::contributors,
		&value_type::retweet_count,
		&value_type::favorite_count,
		&value_type::entities,
		&value_type::favorited,
		&value_type::retweeted,
		&value_type::possibly_sensitive,
		&value_type::lang,
		&value_type::retweeted_status
	> ();
};

template<> struct jsonifier::core<SearchMetadata> {
	using value_type = SearchMetadata;
	static constexpr auto parseValue = createValue<
		&value_type::completed_in,
		&value_type::max_id,
		&value_type::max_id_str,
		&value_type::next_results,
		&value_type::query,
		&value_type::refresh_url,
		&value_type::count,
		&value_type::since_id,
		&value_type::since_id_str
	>();
};

template<> struct jsonifier::core<Twitter> {
	using value_type = Twitter;
	static constexpr auto parseValue = createValue<
		&value_type::statuses,
		&value_type::search_metadata
	>();
};

template<> struct jsonifier::core<Obj3> {
	using value_type = Obj3;
	static constexpr auto parseValue = createValue<
		&value_type::a,
		&value_type::foo
	>();
};

template<> struct jsonifier::core<Obj2> {
	using value_type = Obj2;
	static constexpr auto parseValue = createValue<
		&value_type::foo
	>();
};

template<> struct jsonifier::core<names> {
	using value_type = names;
	static constexpr auto parseValue = createValue();
};

template<> struct jsonifier::core<event> {
	using value_type = event;
	static constexpr auto parseValue = createValue<&value_type::description, &value_type::subTopicIds, &value_type::logo, &value_type::topicIds, &value_type::subjectCode,
		&value_type::subtitle, &value_type::name, &value_type::id>();
};

template<> struct jsonifier::core<price> {
	using value_type = price;
	static constexpr auto parseValue = createValue<&value_type::audienceSubCategoryId, &value_type::seatCategoryId, &value_type::amount>();
};

template<> struct jsonifier::core<area> {
	using value_type = area;
	static constexpr auto parseValue = createValue<&value_type::blockIds, &value_type::areaId>();
};

template<> struct jsonifier::core<seat_category> {
	using value_type = seat_category;
	static constexpr auto parseValue = createValue<&value_type::areas, &value_type::seatCategoryId>();
};

template<> struct jsonifier::core<performance> {
	using value_type = performance;
	static constexpr auto parseValue = createValue<&value_type::seatCategories, &value_type::logo, &value_type::seatMapImage, &value_type::prices, &value_type::venueCode,
		&value_type::name, &value_type::eventId, &value_type::start, &value_type::id>();
};

template<> struct jsonifier::core<venue_names> {
	using value_type = venue_names;
	static constexpr auto parseValue = createValue<&value_type::PLEYEL_PLEYEL>();
};

template<> struct jsonifier::core<citm_catalog_message> {
	using value_type = citm_catalog_message;
	static constexpr auto parseValue =
		createValue<&value_type::audienceSubCategoryNames, &value_type::topicSubTopics, &value_type::seatCategoryNames, &value_type::subTopicNames, &value_type::areaNames,
		&value_type::topicNames, &value_type::performances, &value_type::events, &value_type::venueNames, &value_type::subjectNames, &value_type::blockNames>();
};

template<> struct jsonifier::core<Special> {
	using value_type = Special;
	static constexpr auto parseValue = createValue("integer", &value_type::integer, "real", &value_type::real, "E", &value_type::E, "zero", &value_type::zero, "one",
		&value_type::one, "space", &value_type::space, "quote", &value_type::quote, "backslash", &value_type::backslash, "controls", &value_type::controls, "slash",
		&value_type::slash, "alpha", &value_type::alpha, "ALPHA", &value_type::ALPHA, "0123456789", &value_type::digit, "number", &value_type::number, "special",
		&value_type::special, "hex", &value_type::hex, "null", &value_type::null, "array", &value_type::array);
};

template<> struct jsonifier::core<search_metadata_data> {
	using value_type = search_metadata_data;
	static constexpr auto parseValue = createValue<&value_type::since_id_str, &value_type::next_results, &value_type::refresh_url, &value_type::max_id_str,
		&value_type::completed_in, &value_type::query, &value_type::since_id, &value_type::count, &value_type::max_id>();
};

template<> struct jsonifier::core<hashtag> {
	using value_type = hashtag;
	static constexpr auto parseValue = createValue<&value_type::indices, &value_type::text>();
};

template<> struct jsonifier::core<large_data> {
	using value_type = large_data;
	static constexpr auto parseValue = createValue<&value_type::resize, &value_type::w, &value_type::h>();
};

template<> struct jsonifier::core<sizes_data> {
	using value_type = sizes_data;
	static constexpr auto parseValue = createValue<&value_type::medium, &value_type::small, &value_type::thumb, &value_type::large>();
};

template<> struct jsonifier::core<media_data> {
	using value_type = media_data;
	static constexpr auto parseValue =
		createValue<&value_type::source_status_id_str, &value_type::source_status_id, &value_type::indices, &value_type::media_url_https, &value_type::expanded_url,
		&value_type::display_url, &value_type::media_url, &value_type::id_str, &value_type::type, &value_type::sizes, &value_type::url, &value_type::id>();
};

template<> struct jsonifier::core<url_data> {
	using value_type = url_data;
	static constexpr auto parseValue = createValue<&value_type::indices, &value_type::expanded_url, &value_type::display_url, &value_type::url>();
};

template<> struct jsonifier::core<user_mention> {
	using value_type = user_mention;
	static constexpr auto parseValue = createValue<&value_type::indices, &value_type::screen_name, &value_type::id_str, &value_type::name, &value_type::id>();
};

template<> struct jsonifier::core<status_entities> {
	using value_type = status_entities;
	static constexpr auto parseValue = createValue<&value_type::media, &value_type::user_mentions, &value_type::symbols, &value_type::hashtags, &value_type::urls>();
};

template<> struct jsonifier::core<metadata_data> {
	using value_type = metadata_data;
	static constexpr auto parseValue = createValue<&value_type::iso_language_code, &value_type::result_type>();
};

template<> struct jsonifier::core<description_data> {
	using value_type = description_data;
	static constexpr auto parseValue = createValue<&value_type::urls>();
};

template<> struct jsonifier::core<user_entities> {
	using value_type = user_entities;
	static constexpr auto parseValue = createValue<&value_type::url, &value_type::description>();
};

template<> struct jsonifier::core<twitter_user> {
	using value_type = twitter_user;
	static constexpr auto parseValue = createValue(
		"profile_background_image_url_https", &value_type::profile_background_image_url_https,
		"profile_banner_url", &value_type::profile_banner_url,
		"profile_background_image_url", &value_type::profile_background_image_url,
		"profile_sidebar_border_color", &value_type::profile_sidebar_border_color,
		"profile_sidebar_fill_color", &value_type::profile_sidebar_fill_color,
		"time_zone", &value_type::time_zone,
		"profile_background_color", &value_type::profile_background_color,
		"profile_image_url_https", &value_type::profile_image_url_https,
		"utc_offset", &value_type::utc_offset,
		"profile_use_background_image", &value_type::profile_use_background_image,
		"url", &value_type::url,
		"profile_text_color", &value_type::profile_text_color,
		"profile_link_color", &value_type::profile_link_color,
		"profile_image_url", &value_type::profile_image_url,
		"profile_background_tile", &value_type::profile_background_tile,
		"is_translation_enabled", &value_type::is_translation_enabled,
		"default_profile_image", &value_type::default_profile_image,
		"contributors_enabled", &value_type::contributors_enabled,
		"follow_request_sent", &value_type::follow_request_sent,
		"favourites_count", &value_type::favourites_count,
		"description", &value_type::description,
		"screen_name", &value_type::screen_name,
		"followers_count", &value_type::followers_count,
		"statuses_count", &value_type::statuses_count,
		"created_at", &value_type::created_at,
		"entities", &value_type::entities,
		"friends_count", &value_type::friends_count,
		"default_profile", &value_type::default_profile,
		"listed_count", &value_type::listed_count,
		"location", &value_type::location,
		"user_protected", &value_type::user_protected,
		"is_translator", &value_type::is_translator,
		"id_str", &value_type::id_str,
		"notifications", &value_type::notifications,
		"string", &value_type::string,
		"name", &value_type::name,
		"geo_enabled", &value_type::geo_enabled,
		"lang", &value_type::lang,
		"protected", &value_type::protectedVal, // Note: 'protected' is a reserved keyword, consider renaming if needed
		"following", &value_type::following,
		"verified", &value_type::verified,
		"id", &value_type::id
	);
};


template<> struct jsonifier::core<retweeted_status_data> {
	using value_type = retweeted_status_data;
	static constexpr auto parseValue = createValue<&value_type::in_reply_to_status_id_str, &value_type::in_reply_to_user_id_str, &value_type::in_reply_to_screen_name,
		&value_type::in_reply_to_status_id, &value_type::in_reply_to_user_id, &value_type::possibly_sensitive, &value_type::contributors, &value_type::coordinates,
		&value_type::place, &value_type::geo, &value_type::entities, &value_type::favorite_count, &value_type::metadata, &value_type::created_at, &value_type::retweet_count,
		&value_type::source, &value_type::id_str, &value_type::user, &value_type::lang, &value_type::text, &value_type::truncated, &value_type::favorited, &value_type::retweeted,
		&value_type::id>();
};

template<> struct jsonifier::core<status_data> {
	using value_type = status_data;
	static constexpr auto parseValue = createValue<&value_type::in_reply_to_status_id_str, &value_type::in_reply_to_user_id_str, &value_type::in_reply_to_screen_name,
		&value_type::in_reply_to_status_id, &value_type::in_reply_to_user_id, &value_type::possibly_sensitive, &value_type::contributors, &value_type::coordinates,
		&value_type::retweeted_status, &value_type::place, &value_type::geo, &value_type::entities, &value_type::favorite_count, &value_type::metadata, &value_type::created_at,
		&value_type::retweet_count, &value_type::source, &value_type::id_str, &value_type::user, &value_type::lang, &value_type::text, &value_type::truncated,
		&value_type::favorited, &value_type::retweeted, &value_type::id>();
};

template<> struct jsonifier::core<twitter_message> {
	using value_type = twitter_message;
	static constexpr auto parseValue = createValue<&value_type::search_metadata, &value_type::statuses>();
};

template<> struct jsonifier::core<Empty> {
	using value_type = Empty;
	static constexpr auto parseValue = createValue();
};

namespace JsonifierTypes
{
	jsonifier::jsonifier_core<true> parser{};
	class VectorDouble : public TestAction
	{
	public:
		virtual bool ParseDouble(const char* json, long double& reply) const
		{
			std::vector<double> data{};
			if (parser.parseJson < jsonifier::parse_options{} > (data, jsonifier::string_view{ json })) {
				if (data.size() == 1) {
					reply = data[0];
				}
			}
			return true;
		}
	};

	class VectorString : public TestAction
	{
	public:
		virtual bool ParseString(const char* json, std::string& reply) const
		{
			std::vector<std::string> data{};
			if (parser.parseJson < jsonifier::parse_options{} > (data, jsonifier::string_view{ json })) {
				if (data.size() == 1) {
					reply = data[0];
				}
			}
			return true;
		}
	};
}

template<typename T>
struct GetValueResult: public ParseResultBase
{
    T       data;
};

template<typename T> struct jsonifier::core<GetValueResult<T>> {
	using value_type = GetValueResult<T>;
	static constexpr auto parseValue =
		createValue(&value_type::data);
};

namespace JsonifierTypes {

template<typename T>
class GetValue: public TestAction
{

  public:
	virtual bool Parse(const char* json, size_t, std::unique_ptr<ParseResultBase>& reply) const
	{
		std::unique_ptr<GetValueResult<T>>    parsedData = std::make_unique<GetValueResult<T>>();
		auto error = parser.parseJson(parsedData->data, json);
		if (error) [[likely]] {
			reply = std::move(parsedData);
			return true;
		}
        auto errors = parser.getErrors();
        for (const auto& error : errors) {
            std::cout << "Jsonifier Error: " << error.reportError() << "\n";
        }
		return true;
	}
	virtual bool Stringify(const ParseResultBase& parsedData, std::unique_ptr<StringResultBase>& reply)  const
	{
		return Prettify(parsedData, reply);
	}
	virtual bool Prettify(const ParseResultBase& parsedData, std::unique_ptr<StringResultBase>& reply) const
	{
		GetValueResult<T>const& parsedDataInput = dynamic_cast<GetValueResult<T> const&>(parsedData);
		std::unique_ptr<StringResultUsingString>    output = std::make_unique<StringResultUsingString>();
		auto error = parser.serializeJson < jsonifier::serialize_options{ .prettify = true } > (parsedDataInput.data, output->result);
		if (error) [[likely]] {
			reply = std::move(output);
		}
		return true;
	}
};

}

#endif

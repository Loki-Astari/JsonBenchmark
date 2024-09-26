#ifndef GLAZE_COMMON_H
#define GLAZE_COMMON_H

#include "test.h"
#include "TypeSafe.h"
#include "JsonifierCatalog.h"
#include "JsonifierCountry.h"
#include "JsonifierTwitter.h"
#include "glaze/glaze.hpp"

template<> struct glz::meta<geometry_data> {
	using value_type = geometry_data;
	static constexpr auto value = object("coordinates", &value_type::coordinates, "type", &value_type::type);
};

template<> struct glz::meta<properties_data> {
	using value_type = properties_data;
	static constexpr auto value = object("name", &value_type::name);
};

template<> struct glz::meta<feature> {
	using value_type = feature;
	static constexpr auto value = object("properties", &value_type::properties, "geometry", &value_type::geometry, "type", &value_type::type);
};

template<> struct glz::meta<canada_message> {
	using value_type = canada_message;
	static constexpr auto value = object("features", &value_type::features, "type", &value_type::type);
};

template<> struct glz::meta<search_metadata_data> {
	using value_type = search_metadata_data;
	static constexpr auto value =
		object("since_id_str", &value_type::since_id_str, "next_results", &value_type::next_results, "refresh_url", &value_type::refresh_url, "max_id_str", &value_type::max_id_str,
			"completed_in", &value_type::completed_in, "query", &value_type::query, "since_id", &value_type::since_id, "count", &value_type::count, "max_id", &value_type::max_id);
};

template<> struct glz::meta<hashtag_data> {
	using value_type = hashtag_data;
	static constexpr auto value = object("indices", &value_type::indices, "text", &value_type::text);
};

template<> struct glz::meta<large_data> {
	using value_type = large_data;
	static constexpr auto value = object("resize", &value_type::resize, "w", &value_type::w, "h", &value_type::h);
};

template<> struct glz::meta<sizes_data> {
	using value_type = sizes_data;
	static constexpr auto value = object("medium", &value_type::medium, "small", &value_type::small, "thumb", &value_type::thumb, "large", &value_type::large);
};

template<> struct glz::meta<media_data> {
	using value_type = media_data;
	static constexpr auto value = object("source_status_id_str", &value_type::source_status_id_str, "source_status_id", &value_type::source_status_id, "indices",
		&value_type::indices, "media_url_https", &value_type::media_url_https, "expanded_url", &value_type::expanded_url, "display_url", &value_type::display_url, "media_url",
		&value_type::media_url, "id_str", &value_type::id_str, "type", &value_type::type, "sizes", &value_type::sizes, "url", &value_type::url, "id", &value_type::id);
};

template<> struct glz::meta<url_data> {
	using value_type = url_data;
	static constexpr auto value =
		object("indices", &value_type::indices, "expanded_url", &value_type::expanded_url, "display_url", &value_type::display_url, "url", &value_type::url);
};

template<> struct glz::meta<user_mention> {
	using value_type = user_mention;
	static constexpr auto value =
		object("indices", &value_type::indices, "screen_name", &value_type::screen_name, "id_str", &value_type::id_str, "name", &value_type::name, "id", &value_type::id);
};

template<> struct glz::meta<status_entities> {
	using value_type = status_entities;
	static constexpr auto value = object("media", &value_type::media, "user_mentions", &value_type::user_mentions, "symbols", &value_type::symbols, "hashtags",
		&value_type::hashtags, "urls", &value_type::urls);
};

template<> struct glz::meta<metadata_data> {
	using value_type = metadata_data;
	static constexpr auto value = object("iso_language_code", &value_type::iso_language_code, "result_type", &value_type::result_type);
};

template<> struct glz::meta<description_data> {
	using value_type = description_data;
	static constexpr auto value = object("urls", &value_type::urls);
};

template<> struct glz::meta<user_entities> {
	using value_type = user_entities;
	static constexpr auto value = object("url", &value_type::url, "description", &value_type::description);
};

template<> struct glz::meta<twitter_user_data> {
	using value_type = twitter_user_data;
	static constexpr auto value = object("profile_background_image_url_https", &value_type::profile_background_image_url_https, "profile_banner_url",
		&value_type::profile_banner_url, "profile_background_image_url", &value_type::profile_background_image_url, "profile_sidebar_border_color",
		&value_type::profile_sidebar_border_color, "profile_sidebar_fill_color", &value_type::profile_sidebar_fill_color, "time_zone", &value_type::time_zone,
		"profile_background_color", &value_type::profile_background_color, "profile_image_url_https", &value_type::profile_image_url_https, "utc_offset", &value_type::utc_offset,
		"profile_use_background_image", &value_type::profile_use_background_image, "url", &value_type::url, "profile_text_color", &value_type::profile_text_color,
		"profile_link_color", &value_type::profile_link_color, "profile_image_url", &value_type::profile_image_url, "profile_background_tile", &value_type::profile_background_tile,
		"is_translation_enabled", &value_type::is_translation_enabled, "default_profile_image", &value_type::default_profile_image, "contributors_enabled",
		&value_type::contributors_enabled, "follow_request_sent", &value_type::follow_request_sent, "favourites_count", &value_type::favourites_count, "description",
		&value_type::description, "screen_name", &value_type::screen_name, "followers_count", &value_type::followers_count, "statuses_count", &value_type::statuses_count,
		"created_at", &value_type::created_at, "entities", &value_type::entities, "friends_count", &value_type::friends_count, "default_profile", &value_type::default_profile,
		"listed_count", &value_type::listed_count, "location", &value_type::location, "protected", &value_type::protectedVal, "is_translator", &value_type::is_translator, "id_str",
		&value_type::id_str, "notifications", &value_type::notifications, "name", &value_type::name, "geo_enabled", &value_type::geo_enabled, "lang", &value_type::lang,
		"following", &value_type::following, "verified", &value_type::verified, "id", &value_type::id);
};

template<> struct glz::meta<status_data> {
	using value_type = status_data;
	static constexpr auto value = object("in_reply_to_status_id_str", &value_type::in_reply_to_status_id_str, "in_reply_to_user_id_str", &value_type::in_reply_to_user_id_str,
		"in_reply_to_screen_name", &value_type::in_reply_to_screen_name, "in_reply_to_status_id", &value_type::in_reply_to_status_id, "in_reply_to_user_id",
		&value_type::in_reply_to_user_id, "possibly_sensitive", &value_type::possibly_sensitive, "contributors", &value_type::contributors, "coordinates", &value_type::coordinates,
		"place", &value_type::place, "geo", &value_type::geo, "entities", &value_type::entities, "favorite_count", &value_type::favorite_count, "metadata", &value_type::metadata,
		"created_at", &value_type::created_at, "retweeted_status", &value_type::retweeted_status, "retweet_count", &value_type::retweet_count, "source", &value_type::source,
		"id_str", &value_type::id_str, "user", &value_type::user, "lang", &value_type::lang, "text", &value_type::text, "truncated", &value_type::truncated, "favorited",
		&value_type::favorited, "retweeted", &value_type::retweeted, "id", &value_type::id);
};

template<> struct glz::meta<twitter_message> {
	using value_type = twitter_message;
	static constexpr auto value = object("search_metadata", &value_type::search_metadata, "statuses", &value_type::statuses);
};


template<> struct glz::meta<audience_sub_category_names> {
	using value_type = audience_sub_category_names;
	static constexpr auto value = object("337100890", &value_type::the337100890);
};

template<> struct glz::meta<names> {
	using value_type = names;
	static constexpr auto value = object();
};

template<> struct glz::meta<event> {
	using value_type = event;
	static constexpr auto value = object("description", &value_type::description, "subTopicIds", &value_type::subTopicIds, "logo", &value_type::logo, "topicIds",
		&value_type::topicIds, "subjectCode", &value_type::subjectCode, "subtitle", &value_type::subtitle, "name", &value_type::name, "id", &value_type::id);
};

template<> struct glz::meta<price> {
	using value_type = price;
	static constexpr auto value = object("audienceSubCategoryId", &value_type::audienceSubCategoryId, "seatCategoryId", &value_type::seatCategoryId, "amount", &value_type::amount);
};

template<> struct glz::meta<area> {
	using value_type = area;
	static constexpr auto value = object("blockIds", &value_type::blockIds, "areaId", &value_type::areaId);
};

template<> struct glz::meta<seat_category> {
	using value_type = seat_category;
	static constexpr auto value = object("areas", &value_type::areas, "seatCategoryId", &value_type::seatCategoryId);
};

template<> struct glz::meta<performance> {
	using value_type = performance;
	static constexpr auto value = object("seatCategories", &value_type::seatCategories, "logo", &value_type::logo, "seatMapImage", &value_type::seatMapImage, "prices",
		&value_type::prices, "venueCode", &value_type::venueCode, "name", &value_type::name, "eventId", &value_type::eventId, "start", &value_type::start, "id", &value_type::id);
};

template<> struct glz::meta<venue_names> {
	using value_type = venue_names;
	static constexpr auto value = object("PLEYEL_PLEYEL", &value_type::PLEYEL_PLEYEL);
};

template<> struct glz::meta<citm_catalog_message> {
	using value_type = citm_catalog_message;
	static constexpr auto value = object("audienceSubCategoryNames", &value_type::audienceSubCategoryNames, "topicSubTopics", &value_type::topicSubTopics, "seatCategoryNames",
		&value_type::seatCategoryNames, "subTopicNames", &value_type::subTopicNames, "areaNames", &value_type::areaNames, "topicNames", &value_type::topicNames, "performances",
		&value_type::performances, "events", &value_type::events, "venueNames", &value_type::venueNames, "subjectNames", &value_type::subjectNames, "blockNames",
		&value_type::blockNames);
};

template<> struct glz::meta<Obj3> {
	using value_type = Obj3;
	static constexpr auto value = object(
		"a", &value_type::a,
		"foo", &value_type::foo
	);
};

template<> struct glz::meta<Obj2> {
	using value_type = Obj2;
	static constexpr auto value = object(
		"foo", &value_type::foo
	);
};

namespace Glaze
{

	class VectorDouble : public TestAction
	{
	public:
		virtual bool ParseDouble(const char* json, long double& reply) const
		{
			auto result = glz::read_json<std::vector<double>>(json);
			if (result) {
				std::vector<double> data;
				data = std::move(result.value());
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
			auto result = glz::read_json<std::vector<std::string>>(json);
			if (result) {
				std::vector<std::string> data;
				data = std::move(result.value());
				if (data.size() == 1) {
					reply = data[0];
				}
			}
			return true;
		}
	};


	template<typename T>
	struct GetValueResult : public ParseResultBase
	{
		T       data;
	};
}


template<typename T> struct glz::meta<Glaze::GetValueResult<T>> {
	using value_type = Glaze::GetValueResult<T>;
	static constexpr auto value =
		object(&value_type::data);
};


namespace Glaze
{

	template<typename T>
	class GetValue : public TestAction
	{

	public:
		virtual bool Parse(const char* json, size_t, std::unique_ptr<ParseResultBase>& reply) const
		{
			std::unique_ptr<GetValueResult<T>>    parsedData = std::make_unique<GetValueResult<T>>();
			auto error = glz::read < glz::opts{ .validate_trailing_whitespace = true } > (parsedData->data, json);
			if (!error) {
				reply = std::move(parsedData);
			}
			return true;
		}
		virtual bool Stringify(const ParseResultBase& parsedData, std::unique_ptr<StringResultBase>& reply)  const
		{
			return Prettify(parsedData, reply);
		}
		virtual bool Prettify(const ParseResultBase& parsedData, std::unique_ptr<StringResultBase>& reply) const
		{
			GetValueResult<T> const& parsedDataInput = dynamic_cast<GetValueResult<T> const&>(parsedData);
			std::unique_ptr<StringResultUsingString>    output = std::make_unique<StringResultUsingString>();

			output->result = glz::write_json(parsedDataInput.data).value_or("error");
			reply = std::move(output);
			return true;
		}
	};

}
#endif

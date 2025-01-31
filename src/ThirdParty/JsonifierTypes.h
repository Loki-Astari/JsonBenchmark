#ifndef JSONIFIER_TYPES_H
#define JSONIFIER_TYPES_H

#pragma once

#include "jsonifier/Index.hpp"
#include "TypeSafe.h"
#include "JsonifierCatalog.h"
#include "JsonifierCountry.h"
#include "JsonifierTwitter.h"

template<typename value_type> concept performance_data = std::same_as<std::remove_cvref_t<value_type>, twitter_message> 
|| std::same_as<std::remove_cvref_t<value_type>, citm_catalog_message> 
|| std::same_as<std::remove_cvref_t<value_type>, canada_message>;

template<> struct jsonifier::core<geometry_data> {
	using value_type = geometry_data;
	static constexpr auto parseValue = createValue<&value_type::type, &value_type::coordinates>();
};

template<> struct jsonifier::core<properties_data> {
	using value_type = properties_data;
	static constexpr auto parseValue = createValue<&value_type::name>();
};

template<> struct jsonifier::core<feature> {
	using value_type = feature;
	static constexpr auto parseValue = createValue<&value_type::properties, &value_type::type, &value_type::geometry>();
};

template<> struct jsonifier::core<canada_message> {
	using value_type = canada_message;
	static constexpr auto parseValue = createValue<&value_type::type, &value_type::features>();
};

template<> struct jsonifier::core<search_metadata_data> {
	using value_type = search_metadata_data;
	static constexpr auto parseValue = createValue<&value_type::completed_in, &value_type::max_id, &value_type::max_id_str, &value_type::next_results, &value_type::query,
		&value_type::refresh_url, &value_type::count, &value_type::since_id, &value_type::since_id_str>();
};

template<> struct jsonifier::core<hashtag_data> {
	using value_type = hashtag_data;
	static constexpr auto parseValue = createValue<&value_type::text, &value_type::indices>();
};

template<> struct jsonifier::core<large_data> {
	using value_type = large_data;
	static constexpr auto parseValue = createValue<&value_type::w, &value_type::h, &value_type::resize>();
};

template<> struct jsonifier::core<sizes_data> {
	using value_type = sizes_data;
	static constexpr auto parseValue = createValue<&value_type::medium, &value_type::small, &value_type::thumb, &value_type::large>();
};

template<> struct jsonifier::core<media_data> {
	using value_type = media_data;
	static constexpr auto parseValue =
		createValue<&value_type::id, &value_type::id_str, &value_type::indices, &value_type::media_url, &value_type::media_url_https, &value_type::url, &value_type::display_url,
		&value_type::expanded_url, &value_type::type, &value_type::sizes, &value_type::source_status_id, &value_type::source_status_id_str>();
};

template<> struct jsonifier::core<url_data> {
	using value_type = url_data;
	static constexpr auto parseValue = createValue<&value_type::url, &value_type::expanded_url, &value_type::display_url, &value_type::indices>();
};

template<> struct jsonifier::core<user_mention> {
	using value_type = user_mention;
	static constexpr auto parseValue = createValue<&value_type::screen_name, &value_type::name, &value_type::id, &value_type::id_str, &value_type::indices>();
};

template<> struct jsonifier::core<status_entities> {
	using value_type = status_entities;
	static constexpr auto parseValue = createValue<&value_type::hashtags, &value_type::symbols, &value_type::urls, &value_type::user_mentions, &value_type::media>();
};

template<> struct jsonifier::core<metadata_data> {
	using value_type = metadata_data;
	static constexpr auto parseValue = createValue<&value_type::result_type, &value_type::iso_language_code>();
};

template<> struct jsonifier::core<description_data> {
	using value_type = description_data;
	static constexpr auto parseValue = createValue<&value_type::urls>();
};

template<> struct jsonifier::core<user_entities> {
	using value_type = user_entities;
	static constexpr auto parseValue = createValue<&value_type::description, &value_type::url>();
};

template<> struct jsonifier::core<twitter_user_data> {
	using value_type = twitter_user_data;
	static constexpr auto parseValue =
		createValue<&value_type::id, &value_type::id_str, &value_type::name, &value_type::screen_name, &value_type::location, &value_type::description, &value_type::url,
		&value_type::entities, makeJsonEntity<&value_type::protectedVal, "protected">(), &value_type::followers_count, &value_type::friends_count, &value_type::listed_count,
		&value_type::created_at, &value_type::favourites_count, &value_type::utc_offset, &value_type::time_zone, &value_type::geo_enabled, &value_type::verified,
		&value_type::statuses_count, &value_type::lang, &value_type::contributors_enabled, &value_type::is_translator, &value_type::is_translation_enabled,
		&value_type::profile_background_color, &value_type::profile_background_image_url, &value_type::profile_background_image_url_https, &value_type::profile_background_tile,
		&value_type::profile_image_url, &value_type::profile_image_url_https, &value_type::profile_banner_url, &value_type::profile_link_color,
		&value_type::profile_sidebar_border_color, &value_type::profile_sidebar_fill_color, &value_type::profile_text_color, &value_type::profile_use_background_image,
		&value_type::default_profile, &value_type::default_profile_image, &value_type::following, &value_type::follow_request_sent, &value_type::notifications>();
};

template<> struct jsonifier::core<status_data> {
	using value_type = status_data;
	static constexpr auto parseValue = createValue<&value_type::metadata, &value_type::created_at, &value_type::id, &value_type::id_str, &value_type::text, &value_type::source,
		&value_type::truncated, &value_type::in_reply_to_status_id, &value_type::in_reply_to_status_id_str, &value_type::in_reply_to_user_id, &value_type::in_reply_to_user_id_str,
		&value_type::in_reply_to_screen_name, &value_type::user, &value_type::geo, &value_type::coordinates, &value_type::place, &value_type::contributors,
		&value_type::retweet_count, &value_type::favorite_count, &value_type::entities, &value_type::favorited, &value_type::retweeted, &value_type::lang,
		&value_type::retweeted_status, &value_type::possibly_sensitive>();
};

template<> struct jsonifier::core<twitter_message> {
	using value_type = twitter_message;
	static constexpr auto parseValue = createValue<&value_type::statuses, &value_type::search_metadata>();
};

template<> struct jsonifier::core<audience_sub_category_names> {
	using value_type = audience_sub_category_names;
	static constexpr auto parseValue = createValue<makeJsonEntity<&value_type::the337100890, "337100890">()>();
};

template<> struct jsonifier::core<names> {
	using value_type = names;
	static constexpr auto parseValue = createValue();
};

template<> struct jsonifier::core<event> {
	using value_type = event;
	static constexpr auto parseValue = createValue<&value_type::description, &value_type::id, &value_type::logo, &value_type::name, &value_type::subTopicIds,
		&value_type::subjectCode, &value_type::subtitle, &value_type::topicIds>();
};

template<> struct jsonifier::core<price> {
	using value_type = price;
	static constexpr auto parseValue = createValue<&value_type::amount, &value_type::audienceSubCategoryId, &value_type::seatCategoryId>();
};

template<> struct jsonifier::core<area> {
	using value_type = area;
	static constexpr auto parseValue = createValue<&value_type::areaId, &value_type::blockIds>();
};

template<> struct jsonifier::core<seat_category> {
	using value_type = seat_category;
	static constexpr auto parseValue = createValue<&value_type::areas, &value_type::seatCategoryId>();
};

template<> struct jsonifier::core<performance> {
	using value_type = performance;
	static constexpr auto parseValue = createValue<&value_type::eventId, &value_type::id, &value_type::logo, &value_type::name, &value_type::prices, &value_type::seatCategories,
		&value_type::seatMapImage, &value_type::start, &value_type::venueCode>();
};

template<> struct jsonifier::core<venue_names> {
	using value_type = venue_names;
	static constexpr auto parseValue = createValue<&value_type::PLEYEL_PLEYEL>();
};

template<> struct jsonifier::core<citm_catalog_message> {
	using value_type = citm_catalog_message;
	static constexpr auto parseValue =
		createValue<&value_type::areaNames, &value_type::audienceSubCategoryNames, &value_type::blockNames, &value_type::events, &value_type::performances,
		&value_type::seatCategoryNames, &value_type::subTopicNames, &value_type::subjectNames, &value_type::topicNames, &value_type::topicSubTopics, &value_type::venueNames>();
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

template<> struct jsonifier::core<Special> {
	using value_type = Special;
	static constexpr auto parseValue = createValue<&value_type::integer, &value_type::real, &value_type::E, &value_type::zero,
		&value_type::one, &value_type::space, &value_type::quote, &value_type::backslash, &value_type::controls,
		&value_type::slash, &value_type::alpha, &value_type::ALPHA, &value_type::digit, &value_type::number,
		&value_type::special, &value_type::hex, &value_type::null, &value_type::array>();
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
		bool ParseDouble(const char* json, long double& reply) const
		{
			std::vector<double> data;
			if (parser.parseJson(data, jsonifier::string_view{ json })) {
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
		bool ParseString(const char* json, std::string& reply) const
		{
			std::vector<std::string> data;
			if (parser.parseJson(data, jsonifier::string_view{ json })) {
				if (data.size() == 1) {
					reply = data[0];
				}
			}
			return true;
		}
	};
}

template<typename T>
struct GetValueResult : public ParseResultBase
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
	class GetValue : public TestAction
	{

	public:
		JSONIFIER_INLINE bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const
		{
			std::unique_ptr<GetValueResult<T>>    parsedData = std::make_unique<GetValueResult<T>>();
			if (parser.parseJson < jsonifier::parse_options{ .knownOrder = true } > (parsedData->data, jsonifier::string_view{ json, length })) {
				reply = std::move(parsedData);
			}
			return true;
		}
		JSONIFIER_INLINE bool Stringify(const ParseResultBase& parsedData, std::unique_ptr<StringResultBase>& reply)  const
		{
			GetValueResult<T>const& parsedDataInput = static_cast<GetValueResult<T> const&>(parsedData);
			reply.reset(new StringResultUsingString{});
			parser.serializeJson(parsedDataInput.data, static_cast<StringResultUsingString*>(reply.get())->result);
			return true;
		}
		JSONIFIER_INLINE bool Prettify(const ParseResultBase& parsedData, std::unique_ptr<StringResultBase>& reply) const
		{
			GetValueResult<T>const& parsedDataInput = static_cast<GetValueResult<T> const&>(parsedData);
			reply.reset(new StringResultUsingString{});
			parser.serializeJson < jsonifier::serialize_options{ .prettify = true } > (parsedDataInput.data, static_cast<StringResultUsingString*>(reply.get())->result);
			return true;
		}
	};

}

#endif

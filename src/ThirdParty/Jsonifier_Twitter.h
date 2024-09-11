#ifndef JSONIFIER_TWITTER_H
#define JSONIFIER_TWITTER_H

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <optional>

// Used by performance/twitter.json
#include "TypeSafeTwitter.h"
#include "jsonifier/Index.hpp"

struct search_metadata_data {
	std::string since_id_str{};
	std::string next_results{};
	std::string refresh_url{};
	std::string max_id_str{};
	double completed_in{};
	std::string query{};
	int64_t since_id{};
	int64_t count{};
	double max_id{};
};

struct hashtag {
	std::vector<int64_t> indices{};
	std::string text{};
};

struct large_data {
	std::string resize{};
	int64_t w{};
	int64_t h{};
};

struct sizes_data {
	large_data medium{};
	large_data small{};
	large_data thumb{};
	large_data large{};
};

struct media_data {
	std::optional<std::string> source_status_id_str{};
	std::optional<double> source_status_id{};
	std::vector<int64_t> indices{};
	std::string media_url_https{};
	std::string expanded_url{};
	std::string display_url{};
	std::string media_url{};
	std::string id_str{};
	std::string type{};
	sizes_data sizes{};
	std::string url{};
	double id{};
};

struct url_data {
	std::vector<int64_t> indices{};
	std::string expanded_url{};
	std::string display_url{};
	std::string url{};
};

struct user_mention {
	std::vector<int64_t> indices{};
	std::string screen_name{};
	std::string id_str{};
	std::string name{};
	int64_t id{};
};

struct status_entities {
	std::optional<std::vector<media_data>> media{};
	std::vector<user_mention> user_mentions{};
	std::vector<std::nullptr_t> symbols{};
	std::vector<hashtag> hashtags{};
	std::vector<url_data> urls{};
};

struct metadata_data {
	std::string iso_language_code{};
	std::string result_type{};
};

struct description_data {
	std::vector<url_data> urls{};
};

struct user_entities {
	std::optional<description_data> url{};
	description_data description{};
};

struct twitter_user {
	std::string profile_background_image_url_https{};
	std::optional<std::string> profile_banner_url{};
	std::string profile_sidebar_border_color{};
	std::string profile_background_image_url{};
	std::string profile_sidebar_fill_color{};
	std::optional<std::string> time_zone{};
	std::string profile_background_color{};
	std::string profile_image_url_https{};
	std::optional<int64_t> utc_offset{};
	bool profile_use_background_image{};
	std::optional<std::string> url{};
	std::string profile_text_color{};
	std::string profile_link_color{};
	std::string profile_image_url{};
	bool profile_background_tile{};
	bool is_translation_enabled{};
	bool default_profile_image{};
	bool contributors_enabled{};
	bool follow_request_sent{};
	int64_t favourites_count{};
	std::string description{};
	std::string screen_name{};
	int64_t followers_count{};
	int64_t statuses_count{};
	std::string created_at{};
	user_entities entities{};
	int64_t friends_count{};
	bool default_profile{};
	int64_t listed_count{};
	std::string location{};
	bool user_protected{};
	bool is_translator{};
	std::string id_str{};
	bool notifications{};
	std::string string{};
	bool protectedVal{};
	std::string name{};
	bool geo_enabled{};
	std::string lang{};
	bool following{};
	bool verified{};
	int64_t id{};
};

struct retweeted_status_data {
	std::optional<std::string> in_reply_to_status_id_str{};
	std::optional<std::string> in_reply_to_user_id_str{};
	std::optional<std::string> in_reply_to_screen_name{};
	std::optional<double> in_reply_to_status_id{};
	std::optional<int64_t> in_reply_to_user_id{};
	std::optional<bool> possibly_sensitive{};
	std::nullptr_t contributors{ nullptr };
	std::nullptr_t coordinates{ nullptr };
	std::nullptr_t place{ nullptr };
	std::nullptr_t geo{ nullptr };
	status_entities entities{};
	int64_t favorite_count{};
	metadata_data metadata{};
	std::string created_at{};
	int64_t retweet_count{};
	std::string source{};
	std::string id_str{};
	twitter_user user{};
	std::string lang{};
	std::string text{};
	bool truncated{};
	bool favorited{};
	bool retweeted{};
	double id{};
};

struct status_data {
	std::optional<retweeted_status_data> retweeted_status{};
	std::optional<std::string> in_reply_to_status_id_str{};
	std::optional<std::string> in_reply_to_user_id_str{};
	std::optional<std::string> in_reply_to_screen_name{};
	std::optional<double> in_reply_to_status_id{};
	std::optional<int64_t> in_reply_to_user_id{};
	std::optional<bool> possibly_sensitive{};
	std::nullptr_t contributors{ nullptr };
	std::nullptr_t coordinates{ nullptr };
	std::nullptr_t place{ nullptr };
	std::nullptr_t geo{ nullptr };
	status_entities entities{};
	int64_t favorite_count{};
	metadata_data metadata{};
	std::string created_at{};
	int64_t retweet_count{};
	std::string source{};
	std::string id_str{};
	twitter_user user{};
	std::string lang{};
	std::string text{};
	bool truncated{};
	bool favorited{};
	bool retweeted{};
	double id{};
};

struct twitter_message {
	search_metadata_data search_metadata{};
	std::vector<status_data> statuses{};
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

#if 0
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

template<> struct jsonifier::core<Status> {
	using value_type = Status;
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
#endif


#endif

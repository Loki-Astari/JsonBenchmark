#ifndef GLAZE_COMMON_H
#define GLAZE_COMMON_H

#include "JsonifierCatalog.h"
#include "JsonifierCountry.h"
#include "JsonifierTwitter.h"
#include "TypeSafe.h"
#include "glaze/glaze.hpp"
#include "test.h"

template <> struct glz::meta<geometry_data> {
  using T = geometry_data;
  static constexpr auto value = object(&T::coordinates, &T::type);
};

template <> struct glz::meta<properties_data> {
  using T = properties_data;
  static constexpr auto value = object(&T::name);
};

template <> struct glz::meta<feature> {
  using T = feature;
  static constexpr auto value = object(&T::properties, &T::geometry, &T::type);
};

template <> struct glz::meta<canada_message> {
  using T = canada_message;
  static constexpr auto value = object(&T::features, &T::type);
};

template <> struct glz::meta<search_metadata_data> {
  using T = search_metadata_data;
  static constexpr auto value = object(
      &T::since_id_str, &T::next_results, &T::refresh_url, &T::max_id_str,
      &T::completed_in, &T::query, &T::since_id, &T::count, &T::max_id);
};

template <> struct glz::meta<hashtag_data> {
  using T = hashtag_data;
  static constexpr auto value = object(&T::indices, &T::text);
};

template <> struct glz::meta<large_data> {
  using T = large_data;
  static constexpr auto value = object(&T::resize, &T::w, &T::h);
};

template <> struct glz::meta<sizes_data> {
  using T = sizes_data;
  static constexpr auto value =
      object(&T::medium, &T::small, &T::thumb, &T::large);
};

template <> struct glz::meta<media_data> {
  using T = media_data;
  static constexpr auto value =
      object(&T::source_status_id_str, &T::source_status_id, &T::indices,
             &T::media_url_https, &T::expanded_url, &T::display_url,
             &T::media_url, &T::id_str, &T::type, &T::sizes, &T::url, &T::id);
};

template <> struct glz::meta<url_data> {
  using T = url_data;
  static constexpr auto value =
      object(&T::indices, &T::expanded_url, &T::display_url, &T::url);
};

template <> struct glz::meta<user_mention> {
  using T = user_mention;
  static constexpr auto value =
      object(&T::indices, &T::screen_name, &T::id_str, &T::name, &T::id);
};

template <> struct glz::meta<status_entities> {
  using T = status_entities;
  static constexpr auto value =
      object(&T::media, &T::user_mentions, &T::symbols, &T::hashtags, &T::urls);
};

template <> struct glz::meta<metadata_data> {
  using T = metadata_data;
  static constexpr auto value = object(&T::iso_language_code, &T::result_type);
};

template <> struct glz::meta<description_data> {
  using T = description_data;
  static constexpr auto value = object(&T::urls);
};

template <> struct glz::meta<user_entities> {
  using T = user_entities;
  static constexpr auto value = object(&T::url, &T::description);
};

template <> struct glz::meta<twitter_user_data> {
  using T = twitter_user_data;
  static constexpr auto value = object(
      &T::profile_background_image_url_https, &T::profile_banner_url,
      &T::profile_background_image_url, &T::profile_sidebar_border_color,
      &T::profile_sidebar_fill_color, &T::time_zone,
      &T::profile_background_color, &T::profile_image_url_https, &T::utc_offset,
      &T::profile_use_background_image, &T::url, &T::profile_text_color,
      &T::profile_link_color, &T::profile_image_url,
      &T::profile_background_tile, &T::is_translation_enabled,
      &T::default_profile_image, &T::contributors_enabled,
      &T::follow_request_sent, &T::favourites_count, &T::description,
      &T::screen_name, &T::followers_count, &T::statuses_count, &T::created_at,
      &T::entities, &T::friends_count, &T::default_profile, &T::listed_count,
      &T::location, "protected", &T::protectedVal, &T::is_translator,
      &T::id_str, &T::notifications, &T::name, &T::geo_enabled, &T::lang,
      &T::following, &T::verified, &T::id);
};

template <> struct glz::meta<status_data> {
  using T = status_data;
  static constexpr auto value = object(
      &T::in_reply_to_status_id_str, &T::in_reply_to_user_id_str,
      &T::in_reply_to_screen_name, &T::in_reply_to_status_id,
      &T::in_reply_to_user_id, &T::possibly_sensitive, &T::contributors,
      &T::coordinates, &T::place, &T::geo, &T::entities, &T::favorite_count,
      &T::metadata, &T::created_at, &T::retweeted_status, &T::retweet_count,
      &T::source, &T::id_str, &T::user, &T::lang, &T::text, &T::truncated,
      &T::favorited, &T::retweeted, &T::id);
};

template <> struct glz::meta<twitter_message> {
  using T = twitter_message;
  static constexpr auto value = object(&T::search_metadata, &T::statuses);
};

template <> struct glz::meta<audience_sub_category_names> {
  using T = audience_sub_category_names;
  static constexpr auto value = object("337100890", &T::the337100890);
};

template <> struct glz::meta<names> {
  using T = names;
  static constexpr auto value = object();
};

template <> struct glz::meta<event> {
  using T = event;
  static constexpr auto value =
      object(&T::description, &T::subTopicIds, &T::logo, &T::topicIds,
             &T::subjectCode, &T::subtitle, &T::name, &T::id);
};

template <> struct glz::meta<price> {
  using T = price;
  static constexpr auto value =
      object(&T::audienceSubCategoryId, &T::seatCategoryId, &T::amount);
};

template <> struct glz::meta<area> {
  using T = area;
  static constexpr auto value = object(&T::blockIds, &T::areaId);
};

template <> struct glz::meta<seat_category> {
  using T = seat_category;
  static constexpr auto value = object(&T::areas, &T::seatCategoryId);
};

template <> struct glz::meta<performance> {
  using T = performance;
  static constexpr auto value =
      object(&T::seatCategories, &T::logo, &T::seatMapImage, &T::prices,
             &T::venueCode, &T::name, &T::eventId, &T::start, &T::id);
};

template <> struct glz::meta<venue_names> {
  using T = venue_names;
  static constexpr auto value = object("PLEYEL_PLEYEL", &T::PLEYEL_PLEYEL);
};

template <> struct glz::meta<citm_catalog_message> {
  using T = citm_catalog_message;
  static constexpr auto value = object(
      &T::audienceSubCategoryNames, &T::topicSubTopics, &T::seatCategoryNames,
      &T::subTopicNames, &T::areaNames, &T::topicNames, &T::performances,
      &T::events, &T::venueNames, &T::subjectNames, &T::blockNames);
};

template <> struct glz::meta<Obj3> {
  using T = Obj3;
  static constexpr auto value = object(&T::a, &T::foo);
};

template <> struct glz::meta<Obj2> {
  using T = Obj2;
  static constexpr auto value = object(&T::foo);
};

namespace Glaze {

class VectorDouble : public TestAction {
public:
  virtual bool ParseDouble(const char *json, long double &reply) const {
    std::vector<double> data;
    auto ec = glz::read_json(data, json);
    if (not ec) {
      if (data.size() == 1) {
        reply = data[0];
      }
    }
    return true;
  }
};

class VectorString : public TestAction {
public:
  virtual bool ParseString(const char *json, std::string &reply) const {
    std::vector<std::string> data;
    auto ec = glz::read_json(data, json);
    if (not ec) {
      if (data.size() == 1) {
        reply = data[0];
      }
    }
    return true;
  }
};

template <typename T> struct GetValueResult : public ParseResultBase {
  T data;
};
} // namespace Glaze

template <typename T> struct glz::meta<Glaze::GetValueResult<T>> {
  using T = Glaze::GetValueResult<T>;
  static constexpr auto value = object(&T::data);
};

namespace Glaze {

template <typename T> class GetValue : public TestAction {

public:
  virtual bool Parse(const char *json, size_t,
                     std::unique_ptr<ParseResultBase> &reply) const {
    std::unique_ptr<GetValueResult<T>> parsedData =
        std::make_unique<GetValueResult<T>>();
    auto error = glz::read<glz::opts{.validate_trailing_whitespace = true}>(
        parsedData->data, json);
    if (!error) {
      reply = std::move(parsedData);
    }
    return true;
  }
  virtual bool Stringify(const ParseResultBase &parsedData,
                         std::unique_ptr<StringResultBase> &reply) const {
    GetValueResult<T> const &parsedDataInput =
        dynamic_cast<GetValueResult<T> const &>(parsedData);
    std::unique_ptr<StringResultUsingString> output =
        std::make_unique<StringResultUsingString>();

    auto ec = glz::write_json(parsedDataInput.data, output->result);
    if (not ec) {
      reply = std::move(output);
    }
    return true;
  }
  virtual bool Prettify(const ParseResultBase &parsedData,
                        std::unique_ptr<StringResultBase> &reply) const {
    GetValueResult<T> const &parsedDataInput =
        dynamic_cast<GetValueResult<T> const &>(parsedData);
    std::unique_ptr<StringResultUsingString> output =
        std::make_unique<StringResultUsingString>();

    auto ec = glz::write<glz::opts{.prettify = true}>(parsedDataInput.data, output->result)
    if (not ec) {
      reply = std::move(output);
    }
    return true;
  }
};

} // namespace Glaze
#endif

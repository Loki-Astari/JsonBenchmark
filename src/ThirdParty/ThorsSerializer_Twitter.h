#ifndef THORS_SERIALIZER_TWITTER_H
#define THORS_SERIALIZER_TWITTER_H

#include "test.h"
#include "TypeSafeTwitter.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"


ThorsAnvil_MakeTrait(Metadata,      result_type, iso_language_code);
ThorsAnvil_MakeTrait(URL,           url, expanded_url, display_url, indices);
ThorsAnvil_MakeTrait(URLObject,     urls);
ThorsAnvil_MakeTrait(Entities,      url, description);
ThorsAnvil_MakeOverride(User, {"protectedField", "protected"});
ThorsAnvil_MakeTrait(User,          id, id_str, name, screen_name, location, description, url, entities, followers_count,
                                    friends_count, listed_count, created_at, favourites_count, utc_offset, time_zone,
                                    geo_enabled, verified, statuses_count, lang, contributors_enabled, is_translator,
                                    is_translation_enabled, profile_background_color, profile_background_image_url,
                                    profile_background_image_url_https, profile_background_tile, profile_image_url,
                                    profile_image_url_https, profile_banner_url, profile_link_color, profile_sidebar_border_color,
                                    profile_sidebar_fill_color, profile_text_color, profile_use_background_image, default_profile,
                                    default_profile_image, following, follow_request_sent, notifications, protectedField);
ThorsAnvil_MakeTrait(Hashtag,       text, indices);
ThorsAnvil_MakeTrait(UserMention,   screen_name, name, id, id_str, indices);
ThorsAnvil_MakeTrait(Size,          w, h, resize);
ThorsAnvil_MakeTrait(Sizes,         medium, small, thumb, large);
ThorsAnvil_MakeTrait(Media,         id, id_str, indices, media_url, media_url_https, url, display_url, expanded_url,
                                    type, sizes, source_status_id, source_status_id_str);
ThorsAnvil_MakeTrait(TwitEntities,  hashtags, symbols, urls, user_mentions, media);
ThorsAnvil_MakeTrait(Status,        metadata, created_at, id, id_str, text, source, truncated, in_reply_to_status_id,
                                    in_reply_to_status_id_str, in_reply_to_user_id, in_reply_to_user_id_str, in_reply_to_screen_name,
                                    user, geo, coordinates, place, contributors, retweet_count, favorite_count, entities, favorited,
                                    retweeted, possibly_sensitive, lang, retweeted_status);
ThorsAnvil_MakeTrait(SearchMetadata, completed_in, max_id, max_id_str, next_results, query, refresh_url, count, since_id, since_id_str);
ThorsAnvil_MakeTrait(Twitter,       statuses, search_metadata);
ThorsAnvil_MakeTrait(Unknown);

#endif

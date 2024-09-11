#ifndef JSONBENCHMARK_TYPE_SAFE_TWITTER_H
#define JSONBENCHMARK_TYPE_SAFE_TWITTER_H

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <optional>

// Used by performance/twitter.json

using IntVec = std::vector<std::uint32_t>;

template<typename T>
using Up        = std::unique_ptr<T>;
using Symbols   = std::vector<std::int32_t>;
template<typename T>
using Opt      = std::optional<T>;

struct Metadata
{
    std::string         result_type;
    std::string         iso_language_code;
};

struct URL
{
    std::string         url;
    std::string         expanded_url;
    std::string         display_url;
    IntVec              indices;
};

using URLS = std::vector<URL>;

struct URLObject
{
    URLS                urls;
};

struct Entities
{
    Opt<URLObject>      url;
    URLObject           description;
};

struct User
{
    std::uint64_t       id;
    std::string         id_str;
    std::string         name;
    std::string         screen_name;
    std::string         location;
    std::string         description;
    Up<std::string>     url;
    Entities            entities;
    std::uint32_t       followers_count;
    std::uint32_t       friends_count;
    std::uint32_t       listed_count;
    std::string         created_at;
    std::uint32_t       favourites_count;
    Up<std::int32_t>    utc_offset;
    Up<std::string>     time_zone;
    bool                geo_enabled;
    bool                verified;
    std::uint32_t       statuses_count;
    std::string         lang;
    bool                contributors_enabled;
    bool                is_translator;
    bool                is_translation_enabled;
    std::string         profile_background_color;
    std::string         profile_background_image_url;
    std::string         profile_background_image_url_https;
    bool                profile_background_tile;
    std::string         profile_image_url;
    std::string         profile_image_url_https;
    Opt<std::string>    profile_banner_url;
    std::string         profile_link_color;
    std::string         profile_sidebar_border_color;
    std::string         profile_sidebar_fill_color;
    std::string         profile_text_color;
    bool                profile_use_background_image;
    bool                default_profile;
    bool                default_profile_image;
    bool                following;
    bool                follow_request_sent;
    bool                notifications;
    bool                protectedField;                         // In the JSON this field is "proteted"
                                                                // But that can not be used as a member name in
                                                                // C++ so implementations need to compensate.
};

using Users = std::vector<User>;

struct Hashtag
{
    std::string         text;
    IntVec              indices;
};

using Hashtags = std::vector<Hashtag>;

struct UserMention
{
    std::string         screen_name;
    std::string         name;
    std::uint64_t       id;
    std::string         id_str;
    IntVec              indices;
};

using UserMentions = std::vector<UserMention>;

struct Size
{
    std::uint32_t       w;
    std::uint32_t       h;
    std::string         resize;
};

struct Sizes
{
    Size                small;
    Size                medium;
    Size                thumb;
    Size                large;
};

struct Media
{
    std::uint64_t       id;
    std::string         id_str;
    IntVec              indices;
    std::string         media_url;
    std::string         media_url_https;
    std::string         url;
    std::string         display_url;
    std::string         expanded_url;
    std::string         type;
    Sizes               sizes;
    Opt<std::uint64_t>  source_status_id;
    Opt<std::string>    source_status_id_str;
};

using Medias = std::vector<Media>;

struct TwitEntities
{
    Hashtags            hashtags;
    Symbols             symbols;
    URLS                urls;
    UserMentions        user_mentions;
    Opt<Medias>         media;
};

struct Unknown
{};

struct Status
{
    Metadata            metadata;
    std::string         created_at;
    std::uint64_t       id;
    std::string         id_str;
    std::string         text;
    std::string         source;
    bool                truncated;
    Up<std::uint64_t>   in_reply_to_status_id;
    Up<std::string>     in_reply_to_status_id_str;
    Up<std::uint64_t>   in_reply_to_user_id;
    Up<std::string>     in_reply_to_user_id_str;
    Up<std::string>     in_reply_to_screen_name;
    User                user;
    Up<Unknown>         geo;
    Up<Unknown>         coordinates;
    Up<Unknown>         place;
    Up<Unknown>         contributors;
    std::uint64_t       retweet_count;
    std::uint64_t       favorite_count;
    TwitEntities        entities;
    bool                favorited;
    bool                retweeted;
    Opt<Up<Status>>     retweeted_status;
    Opt<bool>           possibly_sensitive;
    std::string         lang;
};

using Statuses = std::vector<Status>;

struct SearchMetadata
{
    double              completed_in;
    std::uint64_t       max_id;
    std::string         max_id_str;
    std::string         next_results;
    std::string         query;
    std::string         refresh_url;
    std::uint32_t       count;
    std::uint64_t       since_id;
    std::string         since_id_str;
};

struct Twitter
{
    Statuses            statuses;
    SearchMetadata      search_metadata;
};

#endif

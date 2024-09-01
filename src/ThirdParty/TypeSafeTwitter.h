#ifndef JSONBENCHMARK_TYPE_SAFE_TWITTER_H
#define JSONBENCHMARK_TYPE_SAFE_TWITTER_H

#include <vector>
#include <map>
#include <string>
#include <memory>

// Used by performance/twitter.json

using IntVec = std::vector<int>;

template<typename T>
using Opt       = std::unique_ptr<T>;
using Symbol    = std::map<std::string, std::string>;
using Symbols   = std::vector<Symbol>;

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
    long                id;
    std::string         id_str;
    std::string         name;
    std::string         screen_name;
    std::string         location;
    std::string         description;
    Opt<std::string>    url;
    Entities            entities;
    int                 followers_count;
    int                 friends_count;
    int                 listed_count;
    std::string         created_at;
    int                 favourites_count;
    Opt<int>            utc_offset;
    Opt<std::string>    time_zone;
    bool                geo_enabled;
    bool                verified;
    int                 statuses_count;
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
    long                id;
    std::string         id_str;
    IntVec              indices;
};

using UserMentions = std::vector<UserMention>;

struct Size
{
    int                 w;
    int                 h;
    std::string         resize;
};

struct Sizes
{
    Size                medium;
    Size                small;
    Size                thumb;
    Size                large;
};

struct Media
{
    long                id;
    std::string         id_str;
    IntVec              indices;
    std::string         media_url;
    std::string         media_url_https;
    std::string         url;
    std::string         display_url;
    std::string         expanded_url;
    std::string         type;
    Sizes               sizes;
    Opt<long>           source_status_id;
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

struct BaseStatus
{
    Metadata            metadata;
    std::string         created_at;
    long                id;
    std::string         id_str;
    std::string         text;
    std::string         source;
    bool                truncated;
    Opt<long>           in_reply_to_status_id;
    Opt<std::string>    in_reply_to_status_id_str;
    Opt<long>           in_reply_to_user_id;
    Opt<std::string>    in_reply_to_user_id_str;
    Opt<std::string>    in_reply_to_screen_name;
    User                user;
    Opt<int>            geo;
    Opt<int>            coordinates;
    Opt<int>            place;
    Opt<int>            contributors;
    int                 retweet_count;
    int                 favorite_count;
    TwitEntities        entities;
    bool                favorited;
    bool                retweeted;
    Opt<bool>           possibly_sensitive;
    std::string         lang;
};

struct MainStatus: public BaseStatus
{
    Opt<BaseStatus>     retweeted_status;
};

using Statuses = std::vector<MainStatus>;

struct SearchMetadata
{
    double              completed_in;
    long                max_id;
    std::string         max_id_str;
    std::string         next_results;
    std::string         query;
    std::string         refresh_url;
    int                 count;
    long                since_id;
    std::string         since_id_str;
};

struct Twitter
{
    Statuses            statuses;
    SearchMetadata      search_metadata;
};

#endif

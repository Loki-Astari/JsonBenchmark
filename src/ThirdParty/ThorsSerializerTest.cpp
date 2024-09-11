#include "test.h"
#include "TypeSafe.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorsSerializer_Common.h"


// For: data/jsonchecker_pass/pass01.json
ThorsAnvil_MakeTrait(Empty);
ThorsAnvil_MakeOverride(Special,    {"emptyKey", ""},
                                    {"number", "0123456789"},
                                    {"aTrue", "true"},
                                    {"aFalse", "false"},
                                    {"commentKey", "# -- --> */"},
                                    {"spaced", " s p a c e d "},
                                    {"key", "/\\\"\uCAFE\uBABE\uAB98\uFCDE\ubcda\uef4A\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?"}
                       );
ThorsAnvil_MakeTrait(Special,       integer, real, e, E, emptyKey, zero, one, space, quote, backslash,
                                    controls, slash, alpha, ALPHA, digit, number, special, hex,
                                    aTrue, aFalse, null, array, object, address, url, comment,
                                    commentKey, spaced, compact, jsontext, quotes, key);

// For: data/roundtrip/roundtrip09.json
ThorsAnvil_MakeTrait(Obj2,          foo);
// For: data/roundtrip/roundtrip10.json
ThorsAnvil_MakeTrait(Obj3,          a, foo);
// For: data/performance/citm_catalog.json
ThorsAnvil_MakeTrait(Event,         description, id, logo, name, subTopicIds, subjectCode, subtitle, topicIds);
ThorsAnvil_MakeTrait(Price,         amount, audienceSubCategoryId, seatCategoryId);
ThorsAnvil_MakeTrait(Area,          areaId, blockIds);
ThorsAnvil_MakeTrait(SeatCategorie, areas, seatCategoryId);
ThorsAnvil_MakeTrait(Performance,   eventId, id, logo, name, prices, seatCategories, seatMapImage, start, venueCode);
ThorsAnvil_MakeTrait(VenueNames,    PLEYEL_PLEYEL);
ThorsAnvil_MakeTrait(Perform,       areaNames, audienceSubCategoryNames, blockNames, events, performances, seatCategoryNames,
                                    subTopicNames, subjectNames, topicNames, topicSubTopics, venueNames);
// For: data/performance/canada.json
#include "ThorsSerializer_Country.h"
// For: data/performance/twitter.json
ThorsAnvil_MakeTrait(Metadata,      result_type, iso_language_code);
ThorsAnvil_MakeTrait(URL,           url, expanded_url, display_url, indices);
ThorsAnvil_MakeTrait(URLObject,     urls);
ThorsAnvil_MakeTrait(Entities,      url, description);
ThorsAnvil_MakeTrait(User,          id, id_str, name, screen_name, location, description, url, entities, followers_count,
                                    friends_count, listed_count, created_at, favourites_count, utc_offset, time_zone,
                                    geo_enabled, verified, statuses_count, lang, contributors_enabled, is_translator,
                                    is_translation_enabled, profile_background_color, profile_background_image_url,
                                    profile_background_image_url_https, profile_background_tile, profile_image_url,
                                    profile_image_url_https, profile_banner_url, profile_link_color, profile_sidebar_border_color,
                                    profile_sidebar_fill_color, profile_text_color, profile_use_background_image, default_profile,
                                    default_profile_image, following, follow_request_sent, notifications);
ThorsAnvil_MakeTrait(Hashtag,       text, indices);
ThorsAnvil_MakeTrait(UserMention,   screen_name, name, id, id_str, indices);
ThorsAnvil_MakeTrait(Size,          w, h, resize);
ThorsAnvil_MakeTrait(Sizes,         medium, small, thumb, large);
ThorsAnvil_MakeTrait(Media,         id, id_str, indices, media_url, media_url_https, url, display_url, expanded_url,
                                    type, sizes, source_status_id, source_status_id_str);
ThorsAnvil_MakeTrait(TwitEntities,  hashtags, symbols, urls, user_mentions, media);
ThorsAnvil_MakeTrait(BaseStatus,    metadata, created_at, id, id_str, text, source, truncated, in_reply_to_status_id,
                                    in_reply_to_status_id_str, in_reply_to_user_id, in_reply_to_user_id_str, in_reply_to_screen_name,
                                    user, geo, coordinates, place, contributors, retweet_count, favorite_count, entities, favorited,
                                    retweeted, possibly_sensitive, lang);
ThorsAnvil_ExpandTrait(BaseStatus, MainStatus, retweeted_status);
ThorsAnvil_MakeTrait(SearchMetadata, completed_in, max_id, max_id_str, next_results, query, refresh_url, count, since_id, since_id_str);
ThorsAnvil_MakeTrait(Twitter,       statuses, search_metadata);


class ThorsSerializerTest: public TypeSafeTest<ThorsSerializer::VectorDouble,
                                               ThorsSerializer::VectorString,
                                               ThorsSerializer::GetValue>
{
    ThorsSerializer::GetValue<std::vector<ThorsSerializer::StringNumber>>          testGetValueVec2StringNumber;
    ThorsSerializer::GetValue<Country>                                             testGetValueCountry;
    ThorsSerializer::GetValue<Twitter>                                             testGetValueTwitter;
    ThorsSerializer::GetValue<Perform>                                             testGetValueCatalog;

    public:
        ThorsSerializerTest()
        {
            actionMap["performance/canada.json"]         = &testGetValueCountry;
            actionMap["performance/twitter.json"]        = &testGetValueTwitter;
            actionMap["performance/citm_catalog.json"]   = &testGetValueCatalog;

            actionMap["roundtrip/roundtrip21.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip22.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip23.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip24.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip25.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip26.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip27.json"] = &testGetValueVec2StringNumber;
        }

        virtual const char* GetName()     const override    { return "ThorsSerializer"; }
        virtual const char* Type()        const override    { return "C++20";}
        virtual const char* GetFilename() const override    { return __FILE__; }
};

REGISTER_TEST(ThorsSerializerTest);


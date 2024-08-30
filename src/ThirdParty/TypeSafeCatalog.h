#ifndef JSONBENCHMARK_TYPE_SAFE_CATALOG_H
#define JSONBENCHMARK_TYPE_SAFE_CATALOG_H

#include <memory>
#include <vector>
#include <map>

//used by performance/citm_catalog.json
template<typename T>
using Opt = std::unique_ptr<T>;

using IntVec   = std::vector<int>;

struct Event
{
    Opt<int>            description;
    long                id;
    Opt<std::string>    logo;
    std::string         name;
    IntVec              subTopicIds;
    Opt<int>            subjectCode;
    Opt<int>            subtitle;
    IntVec              topicIds;
};

struct Price
{
    int                 amount;
    int                 audienceSubCategoryId;
    int                 seatCategoryId;
};

using Prices = std::vector<Price>;

struct Area
{
    int                 areaId;
    IntVec              blockIds;
};

using Areas = std::vector<Area>;

struct SeatCategorie
{
    Areas               areas;
    int                 seatCategoryId;
};

using SeatCategories = std::vector<SeatCategorie>;

struct Performance
{
    int                 eventId;
    int                 id;
    Opt<std::string>    logo;
    Opt<std::string>    name;
    Prices              prices;
    SeatCategories      seatCategories;
    Opt<int>            seatMapImage;
    long                start;
    std::string         venueCode;
};

using Performances = std::vector<Performance>;

struct VenueNames
{
    std::string     PLEYEL_PLEYEL;
};

struct Perform
{
    std::map<std::string, std::string>  areaNames;
    std::map<std::string, std::string>  audienceSubCategoryNames;
    std::map<std::string, std::string>  blockNames;
    std::map<std::string, Event>        events;
    Performances                        performances;
    std::map<std::string, std::string>  seatCategoryNames;
    std::map<std::string, std::string>  subTopicNames;
    std::map<std::string, std::string>  subjectNames;
    std::map<std::string, std::string>  topicNames;
    std::map<std::string, IntVec>       topicSubTopics;
    VenueNames                          venueNames;
};

#endif

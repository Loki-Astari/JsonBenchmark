#ifndef JSONBENCHMARK_TYPE_SAFE_CATALOG_H
#define JSONBENCHMARK_TYPE_SAFE_CATALOG_H

#include <memory>
#include <vector>
#include <map>

//used by performance/citm_catalog.json
template<typename T>
using Up = std::unique_ptr<T>;

using IntVec   = std::vector<std::uint32_t>;

struct Event
{
    Up<std::uint32_t>   description;
    long                id;
    Up<std::string>     logo;
    std::string         name;
    IntVec              subTopicIds;
    Up<std::uint32_t>   subjectCode;
    Up<std::uint32_t>   subtitle;
    IntVec              topicIds;
};

struct Price
{
    std::uint32_t       amount;
    std::uint32_t       audienceSubCategoryId;
    std::uint32_t       seatCategoryId;
};

using Prices = std::vector<Price>;

struct Area
{
    std::uint32_t       areaId;
    IntVec              blockIds;
};

using Areas = std::vector<Area>;

struct SeatCategorie
{
    Areas               areas;
    std::uint32_t       seatCategoryId;
};

using SeatCategories = std::vector<SeatCategorie>;

struct Performance
{
    std::uint64_t       eventId;
    std::uint64_t       id;
    Up<std::string>     logo;
    Up<std::string>     name;
    Prices              prices;
    SeatCategories      seatCategories;
    Up<std::uint64_t>   seatMapImage;
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

#ifndef JSONIFIER_CATALOG_H
#define JSONIFIER_CATALOG_H

#include <memory>
#include <vector>
#include <map>
#include <optional>
//used by performance/citm_catalog.json
#include "TypeSafeCatalog.h"
#include "jsonifier/Index.hpp"

struct names {};

struct event {
	std::optional<std::string> description{};
	std::vector<int64_t> subTopicIds{};
	std::optional<std::string> logo{};
	std::vector<int64_t> topicIds{};
	std::nullptr_t subjectCode{};
	std::nullptr_t subtitle{};
	std::string name{};
	int64_t id{};
};

struct price {
	int64_t audienceSubCategoryId{};
	int64_t seatCategoryId{};
	int64_t amount{};
};

struct area {
	std::vector<std::nullptr_t> blockIds{};
	int64_t areaId{};
};

struct seat_category {
	std::vector<area> areas{};
	int64_t seatCategoryId{};
};

struct performance {
	std::vector<seat_category> seatCategories{};
	std::optional<std::string> logo{};
	std::nullptr_t seatMapImage{};
	std::vector<price> prices{};
	std::string venueCode{};
	std::nullptr_t name{};
	uint64_t eventId{};
	int64_t start{};
	uint64_t id{};
};

struct venue_names {
	std::string PLEYEL_PLEYEL{};
};

struct citm_catalog_message {
	std::map<std::string, std::string> audienceSubCategoryNames{};
	std::map<std::string, std::vector<uint64_t>> topicSubTopics{};
	std::map<std::string, std::string> seatCategoryNames{};
	std::map<std::string, std::string> subTopicNames{};
	std::map<std::string, std::string> areaNames{};
	std::map<std::string, std::string> topicNames{};
	std::vector<performance> performances{};
	std::map<std::string, event> events{};
	venue_names venueNames{};
	names subjectNames{};
	names blockNames{};
};

template<> struct jsonifier::core<names> {
	using value_type = names;
	static constexpr auto parseValue = createValue();
};

template<> struct jsonifier::core<event> {
	using value_type = event;
	static constexpr auto parseValue = createValue<&value_type::description, &value_type::subTopicIds, &value_type::logo, &value_type::topicIds, &value_type::subjectCode,
		&value_type::subtitle, &value_type::name, &value_type::id>();
};

template<> struct jsonifier::core<price> {
	using value_type = price;
	static constexpr auto parseValue = createValue<&value_type::audienceSubCategoryId, &value_type::seatCategoryId, &value_type::amount>();
};

template<> struct jsonifier::core<area> {
	using value_type = area;
	static constexpr auto parseValue = createValue<&value_type::blockIds, &value_type::areaId>();
};

template<> struct jsonifier::core<seat_category> {
	using value_type = seat_category;
	static constexpr auto parseValue = createValue<&value_type::areas, &value_type::seatCategoryId>();
};

template<> struct jsonifier::core<performance> {
	using value_type = performance;
	static constexpr auto parseValue = createValue<&value_type::seatCategories, &value_type::logo, &value_type::seatMapImage, &value_type::prices, &value_type::venueCode,
		&value_type::name, &value_type::eventId, &value_type::start, &value_type::id>();
};

template<> struct jsonifier::core<venue_names> {
	using value_type = venue_names;
	static constexpr auto parseValue = createValue<&value_type::PLEYEL_PLEYEL>();
};

template<> struct jsonifier::core<citm_catalog_message> {
	using value_type = citm_catalog_message;
	static constexpr auto parseValue =
		createValue<&value_type::audienceSubCategoryNames, &value_type::topicSubTopics, &value_type::seatCategoryNames, &value_type::subTopicNames, &value_type::areaNames,
		&value_type::topicNames, &value_type::performances, &value_type::events, &value_type::venueNames, &value_type::subjectNames, &value_type::blockNames>();
};

#if 0
// Specialization for Event
template<>
struct jsonifier::core<Event> {
	using value_type = Event;
	static constexpr auto parseValue = createValue<
		&value_type::description,
		&value_type::id,
		&value_type::logo,
		&value_type::name,
		&value_type::subTopicIds,
		&value_type::subjectCode,
		&value_type::subtitle,
		&value_type::topicIds
	>();
};

// Specialization for Price
template<>
struct jsonifier::core<Price> {
	using value_type = Price;
	static constexpr auto parseValue = createValue<
		&value_type::amount,
		&value_type::audienceSubCategoryId,
		&value_type::seatCategoryId
	>();
};

// Specialization for Area
template<>
struct jsonifier::core<Area> {
	using value_type = Area;
	static constexpr auto parseValue = createValue<
		&value_type::areaId,
		&value_type::blockIds
	>();
};

// Specialization for SeatCategorie
template<>
struct jsonifier::core<SeatCategorie> {
	using value_type = SeatCategorie;
	static constexpr auto parseValue = createValue<
		&value_type::areas,
		&value_type::seatCategoryId
	>();
};

// Specialization for Performance
template<>
struct jsonifier::core<Performance> {
	using value_type = Performance;
	static constexpr auto parseValue = createValue<
		&value_type::eventId,
		&value_type::id,
		&value_type::logo,
		&value_type::name,
		&value_type::prices,
		&value_type::seatCategories,
		&value_type::seatMapImage,
		&value_type::start,
		&value_type::venueCode
	>();
};

// Specialization for VenueNames
template<>
struct jsonifier::core<VenueNames> {
	using value_type = VenueNames;
	static constexpr auto parseValue = createValue<
		&value_type::PLEYEL_PLEYEL
	>();
};

// Specialization for Perform
template<>
struct jsonifier::core<Perform> {
	using value_type = Perform;
	static constexpr auto parseValue = createValue<
		&value_type::areaNames,
		&value_type::audienceSubCategoryNames,
		&value_type::blockNames,
		&value_type::events,
		&value_type::performances,
		&value_type::seatCategoryNames,
		&value_type::subTopicNames,
		&value_type::subjectNames,
		&value_type::topicNames,
		&value_type::topicSubTopics,
		&value_type::venueNames
	>();
};
#endif


#endif

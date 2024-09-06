#ifndef JSONIFIER_CATALOG_H
#define JSONIFIER_CATALOG_H

#include <memory>
#include <vector>
#include <map>
#include <optional>
//used by performance/citm_catalog.json
#include "TypeSafeCatalog.h"

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

#endif

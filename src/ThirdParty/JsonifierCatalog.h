#ifndef JSONIFIER_CATALOG_H
#define JSONIFIER_CATALOG_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <optional>
#include <unordered_map>
//used by performance/citm_catalog.json
#include "TypeSafeCatalog.h"

struct audience_sub_category_names {
	std::string the337100890;
};

struct names {};

struct event {
	std::nullptr_t description;
	int64_t id;
	std::optional<std::string> logo;
	std::string name;
	std::vector<int64_t> subTopicIds;
	std::nullptr_t subjectCode;
	std::nullptr_t subtitle;
	std::vector<int64_t> topicIds;
};

struct price {
	int64_t amount;
	int64_t audienceSubCategoryId;
	int64_t seatCategoryId;
};

struct area {
	int64_t areaId;
	std::vector<std::nullptr_t> blockIds;
};

struct seat_category {
	std::vector<area> areas;
	int64_t seatCategoryId;
};

struct performance {
	int64_t eventId;
	int64_t id;
	std::optional<std::string> logo;
	std::nullptr_t name;
	std::vector<price> prices;
	std::vector<seat_category> seatCategories;
	std::nullptr_t seatMapImage;
	int64_t start;
	std::string venueCode;
};

struct venue_names {
	std::string PLEYEL_PLEYEL;
};

struct citm_catalog_message {
	std::unordered_map<std::string, std::string> areaNames;
	audience_sub_category_names audienceSubCategoryNames;
	names blockNames;
	std::unordered_map<std::string, event> events;
	std::vector<performance> performances;
	std::unordered_map<std::string, std::string> seatCategoryNames;
	std::unordered_map<std::string, std::string> subTopicNames;
	names subjectNames;
	std::unordered_map<std::string, std::string> topicNames;
	std::unordered_map<std::string, std::vector<int64_t>> topicSubTopics;
	venue_names venueNames;
};

#endif
#ifndef JSONIFIER_COUNTRY_H
#define JSONIFIER_COUNTRY_H

#include <string>
#include <vector>

// Used by performance/canada.json
#include "TypeSafeCountry.h"

struct geometry_data {
	std::string type;
	std::vector<std::vector<std::vector<double>>> coordinates;
};

struct properties_data {
	std::string name;
};

struct feature {
	std::string type;
	properties_data properties;
	geometry_data geometry;
};

struct canada_message {
	std::string type;
	std::vector<feature> features;
};

#endif

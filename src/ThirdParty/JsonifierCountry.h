#ifndef JSONIFIER_COUNTRY_H
#define JSONIFIER_COUNTRY_H

#include <string>
#include <vector>

// Used by performance/canada.json
#include "TypeSafeCountry.h"

struct geometry_data {
    std::vector<std::vector<std::vector<double>>> coordinates{};
    std::string type{};
};

struct properties_data {
    std::string name{};
};

struct feature {
    properties_data properties{};
    geometry_data geometry{};
    std::string type{};
};

struct canada_message {
    std::vector<feature> features{};
    std::string type{};
};

#endif

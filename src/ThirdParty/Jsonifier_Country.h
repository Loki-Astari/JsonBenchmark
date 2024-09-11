#ifndef JSONIFIER_COUNTRY_H
#define JSONIFIER_COUNTRY_H

#include <string>
#include <vector>

// Used by performance/canada.json
#include "TypeSafeCountry.h"
#include "jsonifier/Index.hpp"

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

template<> struct jsonifier::core<geometry_data> {
	using value_type = geometry_data;
	static constexpr auto parseValue = createValue("coordinates", &value_type::coordinates, "type", &value_type::type);
};

template<> struct jsonifier::core<properties_data> {
	using value_type = properties_data;
	static constexpr auto parseValue = createValue<&value_type::name>();
};

template<> struct jsonifier::core<feature> {
	using value_type = feature;
	static constexpr auto parseValue = createValue<&value_type::properties, &value_type::geometry, &value_type::type>();
};

template<> struct jsonifier::core<canada_message> {
	using value_type = canada_message;
	static constexpr auto parseValue = createValue<&value_type::features, &value_type::type>();
};

#if 0
template<> struct jsonifier::core<Properties> {
    using value_type = Properties;
    static constexpr auto parseValue = createValue<&value_type::name>();
};

template<> struct jsonifier::core<Geometry> {
    using value_type = Geometry;
    static constexpr auto parseValue = createValue<&value_type::type, &value_type::coordinates>();
};

template<> struct jsonifier::core<Feature> {
    using value_type = Feature;
    static constexpr auto parseValue = createValue<&value_type::type, &value_type::properties, &value_type::geometry>();
};

template<> struct jsonifier::core<Country> {
    using value_type = Country;
    static constexpr auto parseValue = createValue<&value_type::type, &value_type::features>();
};
#endif



#endif

#ifndef JSONBENCHMARK_TYPE_SAFE_COUNTRY_H
#define JSONBENCHMARK_TYPE_SAFE_COUNTRY_H

#include <string>
#include <vector>

// Used by performance/canada.json
struct Properties
{
    std::string     name;
};

struct Cord
{
    using value_type = double;
    std::vector<double> value;
    Cord(): value(2) {}
    std::size_t size() const {return value.size();}
};

using   CordVec = std::vector<Cord>;
using   CordMat = std::vector<CordVec>;

struct Geometry
{
    std::string     type;
    CordMat         coordinates;
};

struct Feature
{
    std::string     type;
    Properties      properties;
    Geometry        geometry;
};

using Features  = std::vector<Feature>;

struct Country
{
    std::string     type;
    Features        features;
};

#endif

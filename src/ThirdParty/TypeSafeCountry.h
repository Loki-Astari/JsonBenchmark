#ifndef JSONBENCHMARK_TYPE_SAFE_COUNTRY_H
#define JSONBENCHMARK_TYPE_SAFE_COUNTRY_H

#include <string>
#include <vector>

// Used by performance/canada.json
struct Properties
{
    std::string     name;
};

using   Cord    = std::array<double, 2>; // vector double
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

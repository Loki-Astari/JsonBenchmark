#ifndef THORS_SERIALIZER_CANADA_H
#define THORS_SERIALIZER_CANADA_H

#include "test.h"
#include "TypeSafeCountry.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"

ThorsAnvil_MakeTrait(Properties, name);
ThorsAnvil_MakeTrait(Geometry, type, coordinates);
ThorsAnvil_MakeTrait(Feature, type, properties, geometry);
ThorsAnvil_MakeTrait(Country, type, features);

#endif

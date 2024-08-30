#ifndef THORS_SERIALIZER_ROUND_10_H
#define THORS_SERIALIZER_ROUND_10_H

#include "test.h"
#include "TypeSafeRound10.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"

ThorsAnvil_MakeTrait(Obj3, a, foo);
inline void getStats(Stat* stat, Obj3 const& value)
{
    stat->objectCount++;
    stat->elementCount += 2;
    ((value.a) ? stat->numberCount : stat->nullCount)++;
    stat->stringCount += 3; /*Key + Key + Value*/
    stat->stringLength += (3 /*foo*/ + 1 /*a*/ + (value.foo == nullptr ? 0 : value.foo->size()));
}

#endif


#ifndef THORS_SERIALIZER_ROUND_09_H
#define THORS_SERIALIZER_ROUND_09_H

#include "test.h"
#include "TypeSafeRound09.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"

ThorsAnvil_MakeTrait(Obj2, foo);
inline void getStats(Stat* stat, Obj2 const& value)
{
    stat->objectCount++;
    stat->elementCount++;
    stat->stringCount += 2; /* Key + Value */
    stat->stringLength += (3 /*foo*/ + value.foo.size());
}

#endif

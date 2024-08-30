#ifndef THORS_SERIALIZER_PASS_01_H
#define THORS_SERIALIZER_PASS_01_H

#include "test.h"
#include "TypeSafePass01.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"


ThorsAnvil_MakeTrait(Empty);
ThorsAnvil_MakeOverride(Special,    {"emptyKey", ""},
                                    {"number", "0123456789"},
                                    {"aTrue", "true"},
                                    {"aFalse", "false"},
                                    {"commentKey", "# -- --> */"},
                                    {"spaced", " s p a c e d "},
                                    {"key", "/\\\"\uCAFE\uBABE\uAB98\uFCDE\ubcda\uef4A\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?"}
                       );
ThorsAnvil_MakeTrait(Special,       integer, real, e, E, emptyKey, zero, one, space, quote, backslash,
                                    controls, slash, alpha, ALPHA, digit, number, special, hex,
                                    aTrue, aFalse, null, array, object, address, url, comment,
                                    commentKey, spaced, compact, jsontext, quotes, key);


inline void getStats(Stat* stat, Special const& value)
{
    stat->objectCount   += 1;   // Empty
    stat->arrayCount    += 3;   // std::vector<int>
    stat->numberCount   += 7;
    stat->stringCount   += 18;
    stat->trueCount     += 1;
    stat->falseCount    += 1;
    stat->nullCount     += 1;

    stat->memberCount   += 0;
    stat->elementCount  += value.array.size() + value.spaced.size() + value.compact.size();
    stat->stringLength  += value.space.size() + value.quote.size() + value.backslash.size() +
                           value.controls.size() + value.slash.size() + value.alpha.size() +
                           value.ALPHA.size() + value.digit.size() + value.number.size() +
                           value.special.size() + value.hex.size() + value.address.size() +
                           value.url.size() + value.comment.size() + value.commentKey.size() +
                           value.spaced.size() + value.compact.size() + value.jsontext.size() +
                           value.quotes.size() + value.key.size();
}

inline void getStats(Stat* stat, Pass01 const& value)
{
    stat->objectCount   += 3;   // std::map<std::string, std::vector<std::string>> + Empty + Special
    stat->arrayCount    += 1;   // std::vector<int>
    stat->numberCount   += 11;
    stat->stringCount   += 2;
    stat->trueCount     += 1;
    stat->falseCount    += 1;
    stat->nullCount     += 1;

    stat->memberCount   += std::get<1>(value).size() + 0 + /*Special*/32;   // Number of members in all objects
    stat->elementCount  += std::get<3>(value).size();  // Number of elements in all arrays
    stat->stringLength  += std::get<0>(value).size() + std::get<19>(value).size();  // Number of code units in all strings

    getStats(stat, std::get<8>(value));
}

#endif

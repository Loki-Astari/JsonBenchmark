#ifndef THORS_SERIALIZER_CANADA_H
#define THORS_SERIALIZER_CANADA_H

#include "test.h"
#include "TypeSafeCountry.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"

ThorsAnvil_MakeTrait(Properties, name);
inline void getStats(Stat* stat, Properties const& value)
{
    stat->objectCount++;
    stat->memberCount++;
    stat->stringCount += 2;
    stat->stringLength += (4 /*name*/ + value.name.size());
}

inline void getStats(Stat* stat, Cord const& value)
{
    getStats(stat, value.value);
}
namespace ThorsAnvil
{
    namespace Serialize
    {
template<>
class ContainerMemberExtractorEmplacer<Cord>
{
    public:
        constexpr ContainerMemberExtractorEmplacer() {}
        constexpr std::size_t getHash(std::size_t start) const
        {
            return 0x4567 | start;
        }
        void operator()(PrinterInterface& printer, Cord const& object) const
        {
            PutValueType<double>     valuePutter(printer);
            for (auto const& loop: object.value)
            {
                valuePutter.putValue(loop);
            }
        }
        void operator()(ParserInterface& parser, std::size_t const& index, Cord& object) const
        {
            GetValueType<double>   valueGetter(parser, object.value[index]);
        }
};
template<>
class Traits<Cord>
{
    public:
        static constexpr TraitType type = TraitType::Array;
        typedef ContainerMemberExtractorEmplacer<Cord>    MemberExtractor;
        static MemberExtractor const& getMembers()
        {
            static constexpr MemberExtractor    memberExtractor;
            return memberExtractor;
        }
        static std::size_t getPrintSize(PrinterInterface& /*printer*/, Cord const& /*object*/, bool /*poly*/)
        {
            // Only used by BSON.
            // So not needed for JSON tests.
            return 0;
        }
};
    }
}

ThorsAnvil_MakeTrait(Geometry, type, coordinates);
inline void getStats(Stat* stat, Geometry const& value)
{
    stat->objectCount++;
    stat->memberCount += 2;
    stat->stringCount += 3; /* Key + Key + Value*/
    stat->stringLength += (4 /*type*/ + 11 /*coordinates*/ + value.type.size());
    getStats(stat, value.coordinates);
}

ThorsAnvil_MakeTrait(Feature, type, properties, geometry);
inline void getStats(Stat* stat, Feature const& value)
{
    stat->objectCount++;
    stat->memberCount   += 3;
    stat->stringCount   += 4; /* Key + Key + Key + Value */
    stat->stringLength  += (4 /*type*/ + 10 /*properties*/ + 8 /*geometry*/ + value.type.size());
    getStats(stat, value.properties);
    getStats(stat, value.geometry);
}

ThorsAnvil_MakeTrait(Country, type, features);
inline void getStats(Stat* stat, Country const& value)
{
    stat->objectCount++;
    stat->memberCount+=2;
    stat->stringCount += 3; /*Key + Key + Value */
    stat->stringLength += (4 /*types*/ + 8 /*features*/ + value.type.size());
    getStats(stat, value.features);
}

#endif

#ifndef THORS_SERIALIZER_CANADA_H
#define THORS_SERIALIZER_CANADA_H

#include "test.h"
#include "TypeSafeCountry.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"

ThorsAnvil_MakeTrait(Properties, name);

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
ThorsAnvil_MakeTrait(Feature, type, properties, geometry);
ThorsAnvil_MakeTrait(Country, type, features);

#endif

#include "test.h"
#include "TypeSafe.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorsSerializer_Common.h"


// For: data/jsonchecker_pass/pass01.json
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

// For: data/roundtrip/roundtrip09.json
ThorsAnvil_MakeTrait(Obj2,          foo);
// For: data/roundtrip/roundtrip10.json
ThorsAnvil_MakeTrait(Obj3,          a, foo);
// For: data/performance/citm_catalog.json
#include "ThorsSerializer_Catalog.h"
// For: data/performance/canada.json
#include "ThorsSerializer_Country.h"
// For: data/performance/twitter.json
#include "ThorsSerializer_Twitter.h"


class ThorsSerializerTest: public TypeSafeTest<ThorsSerializer::VectorDouble,
                                               ThorsSerializer::VectorString,
                                               ThorsSerializer::GetValue>
{
    ThorsSerializer::GetValue<std::vector<ThorsSerializer::StringNumber>>          testGetValueVec2StringNumber;
    ThorsSerializer::GetValuePerformance<Country>                                  testGetValueCountry;
    ThorsSerializer::GetValuePerformance<Twitter>                                  testGetValueTwitter;
    ThorsSerializer::GetValuePerformance<Perform>                                  testGetValueCatalog;

    public:
        ThorsSerializerTest()
        {
            actionMap["performance/canada.json"]         = &testGetValueCountry;
            actionMap["performance/twitter.json"]        = &testGetValueTwitter;
            actionMap["performance/citm_catalog.json"]   = &testGetValueCatalog;

            actionMap["roundtrip/roundtrip21.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip22.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip23.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip24.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip25.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip26.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip27.json"] = &testGetValueVec2StringNumber;
        }

        virtual const char* GetName()     const override    { return "ThorsSerializer"; }
        virtual const char* Type()        const override    { return "C++20";}
        virtual const char* GetFilename() const override    { return __FILE__; }
};

REGISTER_TEST(ThorsSerializerTest);


#include "test.h"
#include "TypeSafe.h"

#define HEADER_ONLY 1

#include "ThorsSerializer_Pass01.h"
#include "ThorsSerializer_Round09.h"
#include "ThorsSerializer_Round10.h"
#include "ThorsSerializer_Catalog.h"
#include "ThorsSerializer_Country.h"
#include "ThorsSerializer_Twitter.h"



#include "ThorsSerializer_StringNumber.h"
#include "ThorsSerializer_GetStats.h"
#include "ThorsSerializer_Common.h"


class ThorsSerializerTest: public TypeSafeTest<ThorsSerializer::VectorDouble,
                                               ThorsSerializer::VectorString,
                                               ThorsSerializer::GetValue>
{
    ThorsSerializer::GetValue<std::vector<ThorsSerializer::StringNumber>>          testGetValueVec2StringNumber;
    ThorsSerializer::GetValue<Country>                                             testGetValueCountry;
    ThorsSerializer::GetValue<Twitter>                                             testGetValueTwitter;

    public:
        ThorsSerializerTest()
        {
            actionMap["performance/canada.json"]         = &testGetValueCountry;
            actionMap["performance/twitter.json"]        = &testGetValueTwitter;

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


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
   
    public:
        ThorsSerializerTest()
        {
            actionMap["roundtrip/roundtrip21.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip22.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip23.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip24.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip25.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip26.json"] = &testGetValueVec2StringNumber;
            actionMap["roundtrip/roundtrip27.json"] = &testGetValueVec2StringNumber;
        }
};

REGISTER_TEST(ThorsSerializerTest);


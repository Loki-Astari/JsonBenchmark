#include "test.h"

#define HEADER_ONLY 1

#include "ThorsSerializer_StringNumber.h"
#include "ThorsSerializer_GetStats.h"
#include "ThorsSerializer_Twitter.h"
#include "ThorsSerializer_Canada.h"
#include "ThorsSerializer_citm_catalog.h"

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/SerUtil.h"
#include <iostream>
#include <sstream>
#include <map>
#include <memory>
#include "ThorsSerializer_Common.h"

class ThorsSerializerTest: public TestBase
{
    TestAction                                   testNotImplemented;
    VectorDouble                                 testVectorDouble;
    VectorString                                 testVectorString;
    GetValue<std::map<std::string, std::string>> testGetValueMap2String;
    GetValue<std::map<std::string, bool>>        testGetValueMap2Bool;
    GetValue<std::map<std::string, int>>         testGetValueMap2Int;
    GetValue<std::map<std::string, int*>>        testGetValueMap2IntPointer;
    GetValue<std::map<std::string, M01>>         testGetValueMap2M01;
    GetValue<std::vector<std::string>>           testGetValueVec2String;
    //GetValue<std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<std::string>>>>>>>>>>>>>>>>>>>> testGetValueVec2L18;
    GetValue<std::vector<int*>>                  testGetValueVec2IntPointer;
    GetValue<std::vector<bool>>                  testGetValueVec2Bool;
    GetValue<std::vector<int>>                   testGetValueVec2Int;
    GetValue<std::vector<long>>                  testGetValueVec2Long;
    GetValue<std::vector<double>>                testGetValueVec2Double;
    GetValue<std::vector<StringNumber>>          testGetValueVec2StringNumber;
    GetValue<Obj2>                               testGetValueObj2;
    GetValue<Obj3>                               testGetValueObj3;
    GetValue<Country>                            testGetValueCountry;
    GetValue<Perform>                            testGetValuePerform;
    GetValue<Twitter>                            testGetValueTwitter;

    ActionMap                   actionMap;
    mutable TestAction const*   currentRunner;

    public:
    ThorsSerializerTest()
    {
        actionMap["vector-double"]                   = &testVectorDouble;
        actionMap["vector-string"]                   = &testVectorString;

        actionMap["performance/canada.json"]         = &testGetValueCountry;
        actionMap["performance/citm_catalog.json"]   = &testGetValuePerform;
        actionMap["performance/twitter.json"]        = &testGetValueTwitter;

        actionMap["jsonchecker_fail/fail02.json"]    = &testGetValueVec2String;
        actionMap["jsonchecker_fail/fail03.json"]    = &testGetValueMap2String;
        actionMap["jsonchecker_fail/fail04.json"]    = &testGetValueVec2String;
        actionMap["jsonchecker_fail/fail05.json"]    = &testGetValueVec2String;
        actionMap["jsonchecker_fail/fail06.json"]    = &testGetValueVec2String;
        actionMap["jsonchecker_fail/fail07.json"]    = &testGetValueVec2String;
        actionMap["jsonchecker_fail/fail08.json"]    = &testGetValueVec2String;
        actionMap["jsonchecker_fail/fail09.json"]    = &testGetValueMap2Bool;
        actionMap["jsonchecker_fail/fail10.json"]    = &testGetValueMap2Bool;
        actionMap["jsonchecker_fail/fail11.json"]    = &testGetValueMap2Int;
        actionMap["jsonchecker_fail/fail12.json"]    = &testGetValueMap2String;
        actionMap["jsonchecker_fail/fail13.json"]    = &testGetValueMap2Int;
        actionMap["jsonchecker_fail/fail14.json"]    = &testGetValueMap2Int;
        actionMap["jsonchecker_fail/fail15.json"]    = &testGetValueVec2String;
        actionMap["jsonchecker_fail/fail16.json"]    = &testGetValueVec2String;
        actionMap["jsonchecker_fail/fail17.json"]    = &testGetValueVec2String;

        actionMap["jsonchecker_fail/fail19.json"]    = &testGetValueMap2IntPointer;
        actionMap["jsonchecker_fail/fail20.json"]    = &testGetValueMap2IntPointer;
        actionMap["jsonchecker_fail/fail21.json"]    = &testGetValueMap2IntPointer;
        actionMap["jsonchecker_fail/fail22.json"]    = &testGetValueVec2String;
        actionMap["jsonchecker_fail/fail23.json"]    = &testGetValueVec2String;
        actionMap["jsonchecker_fail/fail24.json"]    = &testGetValueVec2String;
        actionMap["jsonchecker_fail/fail25.json"]    = &testGetValueVec2String;
        actionMap["jsonchecker_fail/fail26.json"]    = &testGetValueVec2String;
        actionMap["jsonchecker_fail/fail27.json"]    = &testGetValueVec2String;
        actionMap["jsonchecker_fail/fail28.json"]    = &testGetValueVec2String;
        actionMap["jsonchecker_fail/fail29.json"]    = &testGetValueVec2Double;
        actionMap["jsonchecker_fail/fail30.json"]    = &testGetValueVec2Double;
        actionMap["jsonchecker_fail/fail31.json"]    = &testGetValueVec2Double;
        actionMap["jsonchecker_fail/fail32.json"]    = &testGetValueMap2Bool;
        actionMap["jsonchecker_fail/fail33.json"]    = &testGetValueVec2String;

        //  pass01.json     ThorsSerializer does not support polymorphic arrays
        // actionMap["jsonchecker_pass/pass02.json"]    = &testGetValueVec2L18;
        actionMap["jsonchecker_pass/pass03.json"]    = &testGetValueMap2M01;


        actionMap["roundtrip/roundtrip01.json"] = &testGetValueVec2IntPointer;
        actionMap["roundtrip/roundtrip02.json"] = &testGetValueVec2Bool;
        actionMap["roundtrip/roundtrip03.json"] = &testGetValueVec2Bool;
        actionMap["roundtrip/roundtrip04.json"] = &testGetValueVec2Int;
        actionMap["roundtrip/roundtrip05.json"] = &testGetValueVec2String;
        actionMap["roundtrip/roundtrip06.json"] = &testGetValueVec2Int;
        actionMap["roundtrip/roundtrip07.json"] = &testGetValueMap2String;
        actionMap["roundtrip/roundtrip08.json"] = &testGetValueVec2Int;
        actionMap["roundtrip/roundtrip09.json"] = &testGetValueObj2;
        actionMap["roundtrip/roundtrip10.json"] = &testGetValueObj3;
        actionMap["roundtrip/roundtrip11.json"] = &testGetValueVec2Int;
        actionMap["roundtrip/roundtrip12.json"] = &testGetValueVec2Int;
        actionMap["roundtrip/roundtrip13.json"] = &testGetValueVec2Long;
        actionMap["roundtrip/roundtrip14.json"] = &testGetValueVec2Long;
        actionMap["roundtrip/roundtrip15.json"] = &testGetValueVec2Int;
        actionMap["roundtrip/roundtrip16.json"] = &testGetValueVec2Int;
        actionMap["roundtrip/roundtrip17.json"] = &testGetValueVec2Long;
        actionMap["roundtrip/roundtrip18.json"] = &testGetValueVec2Long;
        actionMap["roundtrip/roundtrip19.json"] = &testGetValueVec2Long;
        actionMap["roundtrip/roundtrip20.json"] = &testGetValueVec2Double;
        actionMap["roundtrip/roundtrip21.json"] = &testGetValueVec2StringNumber;
        actionMap["roundtrip/roundtrip22.json"] = &testGetValueVec2StringNumber;
        actionMap["roundtrip/roundtrip23.json"] = &testGetValueVec2StringNumber;
        actionMap["roundtrip/roundtrip24.json"] = &testGetValueVec2StringNumber;
        actionMap["roundtrip/roundtrip25.json"] = &testGetValueVec2StringNumber;
        actionMap["roundtrip/roundtrip26.json"] = &testGetValueVec2StringNumber;
        actionMap["roundtrip/roundtrip27.json"] = &testGetValueVec2StringNumber;

        currentRunner      = &testNotImplemented;
    }
    virtual void SetUp(char const* fullPath) const
    {
        // std::cout << "Test: " << fullPath << " : " << fileName << "\n";

        auto find = actionMap.find(fullPath);
        if (find != actionMap.end())
        {
            currentRunner   = find->second;
        }
    }
    virtual void TearDown(char const*) const
    {
        currentRunner = &testNotImplemented;
    }

    virtual const char* GetName() const /*override*/        { return "ThorsSerializer"; }
    virtual const char* Type()    const /*override*/        { return "C++20";}
    virtual const char* GetFilename() const /*override*/    { return __FILE__; }

    virtual ParseResultBase* Parse(const char* json, size_t length) const {
        return currentRunner->Parse(json, length);
    }

    virtual bool ParseDouble(const char* json, double* d) const {
        return currentRunner->ParseDouble(json, d);
    }

    virtual bool ParseString(const char* json, std::string& s) const {
        return currentRunner->ParseString(json, s);
    }

    virtual StringResultBase* SaxRoundtrip(const char* json, size_t length) const {
        return currentRunner->SaxRoundtrip(json, length);
    }

    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const {
        return currentRunner->Stringify(parseResult);
    }

    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const {
        return currentRunner->Prettify(parseResult);
    }

    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const {
        stat->objectCount   = 0;
        stat->arrayCount    = 0;
        stat->numberCount   = 0;
        stat->stringCount   = 0;
        stat->trueCount     = 0;
        stat->falseCount    = 0;
        stat->nullCount     = 0;

        stat->memberCount   = 0;
        stat->elementCount  = 0;
        stat->stringLength  = 0;

        return currentRunner->Statistics(parseResult, stat);
    }

    virtual bool SaxStatistics(const char* json, size_t length, Stat* stat) const {
        stat->objectCount   = 0;
        stat->arrayCount    = 0;
        stat->numberCount   = 0;
        stat->stringCount   = 0;
        stat->trueCount     = 0;
        stat->falseCount    = 0;
        stat->nullCount     = 0;

        stat->memberCount   = 0;
        stat->elementCount  = 0;
        stat->stringLength  = 0;

        return currentRunner->SaxStatistics(json, length, stat);
    }

    virtual bool SaxStatisticsUTF16(const char*, size_t, Stat*) const {
        return false;
    }
};

REGISTER_TEST(ThorsSerializerTest);


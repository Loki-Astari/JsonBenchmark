#ifndef JSONBENCHMARK_TYPE_SAFE_H
#define JSONBENCHMARK_TYPE_SAFE_H

#include "test.h"
#include <map>
#include <string>
#include <vector>

#include "TypeSafePass01.h"
#include "TypeSafePass02.h"
#include "TypeSafePass03.h"
#include "TypeSafeRound09.h"
#include "TypeSafeRound10.h"
#include "TypeSafeCountry.h"
#include "TypeSafeCatalog.h"
#include "TypeSafeTwitter.h"


class TestAction
{
    public:
    virtual bool ParseValidate(const char* json, size_t size, bool& reply) const
    {
        std::unique_ptr<ParseResultBase> result;
        bool implemented = Parse(json, size, result);
        if (!implemented) {
            return false;
        }
        reply = result.get() != nullptr;
        return true;
    }
    virtual bool Parse(const char*, size_t, std::unique_ptr<ParseResultBase>&)          const {return false;}
    virtual bool ParseDouble(const char*, long double&)                                 const {return false;}
    virtual bool ParseString(const char*, std::string&)                                 const {return false;}
    virtual bool SaxRoundtrip(const char*, size_t, std::unique_ptr<StringResultBase>&)  const {return false;}
    virtual bool Stringify(const ParseResultBase&, std::unique_ptr<StringResultBase>&)  const {return false;}
    virtual bool Prettify(const ParseResultBase&, std::unique_ptr<StringResultBase>&)   const {return false;}
    virtual bool Statistics(const ParseResultBase&, Stat&)                              const {return false;}
    virtual bool SaxStatistics(const char*, size_t, Stat&)                              const {return false;}
    virtual bool SaxStatisticsUTF16(const char*, size_t, Stat&)                         const {return false;}
};

/*
 * Instructions:
 *      VectorDouble:   Inherit from TestAction and implement: ParseDouble(const char* json, double* result)
 *                      Return true: Indicates you have implemented the method.
 *                      Action: json contains a vector with a single "double value".
 *                              You should read the double from the vector and assign it to result.
 *                      Note:   Json support a lot of different double formats this will test the conformance against the standard.
 *
 *      VectorString:   Inherit from TestAction and implement: ParseString(const char* json, std::string& result)
 *                      Return true: Indicates you have implemented the method.
 *                      Action: json contains a vector with a single "string"
 *                              You should read the string from the vector and assign it to result.
 *                      Note:   Json support a lot of special characters. These should be decoded into a UTF-8 string.
 *
 *      GetValue:       This is a template type. The type parameter is what you will need to be read from the stream.
 *                      Arrays  => std::vector
 *                      Objects => C++ structure
 *                      Inherit from TestAction and implement: ParseValidate(const char* json, size_t size, bool& result)
 *                      Action: The parse valid should validate the at the json is correctly formatted for the type given.
 *                      Return true: Indicates you have implemented the method.
 *                      Action: Set the result to true if the JSON is valid and matches the input type false otherwise.
 *                      Note:   Trailing data on the json string should indicate a fail.
 *
 *                      Optionally: For Performance tests:
 *                      Implement:
 *                          Parse(const char*, size_t):         Parse a json string. The result is passed to Stringify() and Prettify()
 *                          Stringify(const ParseResultBase*)
 *                          Prettify(const ParseResultBase*)
 *      --
 *                          SaxRoundtrip(const char*, size_t)
 *                      Optionally: Implement the statistics gathering functions:
 *                          Statistics(const ParseResultBase*, Stat*)
 *                          SaxStatistics(const char*, size_t, Stat*)
 */

template<typename VectorDouble, typename VectorString, template<typename> typename GetValue>
class TypeSafeTest: public TestBase
{
    using ActionMap = std::map<std::string, TestAction*>;

    TestAction                                   testNotImplemented;

    VectorDouble                                 testVectorDouble;
    VectorString                                 testVectorString;
    GetValue<std::map<std::string, std::string>> testGetValueMap2String;
    GetValue<std::map<std::string, bool>>        testGetValueMap2Bool;
    GetValue<std::map<std::string, int>>         testGetValueMap2Int;
    GetValue<std::map<std::string, int*>>        testGetValueMap2IntPointer;
    GetValue<std::map<std::string, M01>>         testGetValueMap2M01;
    GetValue<std::vector<std::string>>           testGetValueVec2String;
    GetValue<Pass01>                             testGetValuePass1;
    GetValue<VVVVVVVVVVVVVVVVVVVS>               testGetValueVec2L18;
    GetValue<std::vector<int*>>                  testGetValueVec2IntPointer;
    GetValue<std::vector<bool>>                  testGetValueVec2Bool;
    GetValue<std::vector<int>>                   testGetValueVec2Int;
    GetValue<std::vector<long>>                  testGetValueVec2Long;
    GetValue<std::vector<double>>                testGetValueVec2Double;
    GetValue<Obj2>                               testGetValueObj2;
    GetValue<Obj3>                               testGetValueObj3;
    GetValue<Country>                            testGetValueCountry;
    GetValue<Perform>                            testGetValuePerform;
    GetValue<Twitter>                            testGetValueTwitter;

    mutable TestAction const*   currentRunner;

    protected:
    ActionMap                   actionMap;

    public:
    TypeSafeTest()
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

        actionMap["jsonchecker_pass/pass01.json"]    = &testGetValuePass1;
        actionMap["jsonchecker_pass/pass02.json"]    = &testGetValueVec2L18;
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
        actionMap["roundtrip/roundtrip21.json"] = &testGetValueVec2Double;
        actionMap["roundtrip/roundtrip22.json"] = &testGetValueVec2Double;
        actionMap["roundtrip/roundtrip23.json"] = &testGetValueVec2Double;
        actionMap["roundtrip/roundtrip24.json"] = &testGetValueVec2Double;
        actionMap["roundtrip/roundtrip25.json"] = &testGetValueVec2Double;
        actionMap["roundtrip/roundtrip26.json"] = &testGetValueVec2Double;
        actionMap["roundtrip/roundtrip27.json"] = &testGetValueVec2Double;

        currentRunner      = &testNotImplemented;
    }
    virtual void SetUp(char const* fullPath) const override
    {
        // std::cout << "Test: " << fullPath << " : " << fileName << "\n";

        auto find = actionMap.find(fullPath);
        if (find != actionMap.end())
        {
            currentRunner   = find->second;
        }
    }
    virtual void TearDown(char const*) const override
    {
        currentRunner = &testNotImplemented;
    }

    virtual const char* GetName()     const override    { return "ThorsSerializer"; }
    virtual const char* Type()        const override    { return "C++20";}
    virtual const char* GetFilename() const override    { return __FILE__; }

    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override {
        return currentRunner->Parse(json, length, reply);
    }

    virtual bool ParseDouble(const char* json, long double& d) const override {
        return currentRunner->ParseDouble(json, d);
    }

    virtual bool ParseString(const char* json, std::string& s) const override {
        return currentRunner->ParseString(json, s);
    }

    virtual bool SaxRoundtrip(const char* json, size_t length, std::unique_ptr<StringResultBase>& reply) const override {
        return currentRunner->SaxRoundtrip(json, length, reply);
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override {
        return currentRunner->Stringify(parseResult, reply);
    }

    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override {
        return currentRunner->Prettify(parseResult, reply);
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override {
        stat.objectCount   = 0;
        stat.arrayCount    = 0;
        stat.numberCount   = 0;
        stat.stringCount   = 0;
        stat.trueCount     = 0;
        stat.falseCount    = 0;
        stat.nullCount     = 0;

        stat.memberCount   = 0;
        stat.elementCount  = 0;
        stat.stringLength  = 0;

        return currentRunner->Statistics(parseResult, stat);
    }

    virtual bool SaxStatistics(const char* json, size_t length, Stat& stat) const override {
        stat.objectCount   = 0;
        stat.arrayCount    = 0;
        stat.numberCount   = 0;
        stat.stringCount   = 0;
        stat.trueCount     = 0;
        stat.falseCount    = 0;
        stat.nullCount     = 0;

        stat.memberCount   = 0;
        stat.elementCount  = 0;
        stat.stringLength  = 0;

        return currentRunner->SaxStatistics(json, length, stat);
    }

    virtual bool SaxStatisticsUTF16(const char* json, size_t length, Stat& stat) const override {
        stat.objectCount   = 0;
        stat.arrayCount    = 0;
        stat.numberCount   = 0;
        stat.stringCount   = 0;
        stat.trueCount     = 0;
        stat.falseCount    = 0;
        stat.nullCount     = 0;

        stat.memberCount   = 0;
        stat.elementCount  = 0;
        stat.stringLength  = 0;

        return currentRunner->SaxStatisticsUTF16(json, length, stat);
    }
};

#endif

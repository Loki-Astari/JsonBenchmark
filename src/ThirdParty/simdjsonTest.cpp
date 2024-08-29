#include "test.h"

#include "simdjson.h"
using namespace simdjson;

struct SimdResult: public ParseResultBase
{
    ondemand::document doc;
};

struct SimdStringResult: public StringResultBase
{
    std::string value;
    virtual const char* c_str() const
    {
        return value.c_str();
    }
};

class SimdJsonTest: public TestBase
{
    public:
    SimdJsonTest()
    {
    }
    virtual void SetUp(char const* /*fullPath*/) const
    {
    }
    virtual void TearDown(char const*) const
    {
    }

    virtual const char* GetName() const /*override*/        { return "simdjson"; }
    virtual const char* Type()    const /*override*/        { return "C++";}
    virtual const char* GetFilename() const /*override*/    { return __FILE__; }

    virtual ParseResultBase* Parse(const char* json, size_t /*length*/) const
    {
        SimdResult*         result = new SimdResult;
        ondemand::parser    parser;
        if (parser.iterate(json).get(result->doc) != SUCCESS) {
            delete result;
            return nullptr;
        }
        return result;
    }

    virtual bool ParseDouble(const char* json, double* d) const
    {
        ondemand::parser    parser;
        ondemand::document  doc;
        if (parser.iterate(json).get(doc) != SUCCESS) {
            return false;
        }
        double value = doc.get_double();
        return value == *d;
    }

    virtual bool ParseString(const char* json, std::string& s) const
    {
        ondemand::parser    parser;
        ondemand::document  doc;
        if (parser.iterate(json).get(doc) != SUCCESS) {
            return false;
        }
        std::string_view    value = doc;
        return value == s;
    }

    virtual StringResultBase* SaxRoundtrip(const char* json, size_t /*length*/) const
    {
        ondemand::parser    parser;
        ondemand::document  doc;
        if (parser.iterate(json).get(doc) != SUCCESS) {
            return nullptr;
        }
        std::stringstream output;
        output << doc.current_location();

        SimdStringResult*   result = new SimdStringResult;
        result->value = output.str();
        return result;
    }

    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const
    {
        SimdResult const*     simdParseResult = dynamic_cast<SimdResult const*>(parseResult);
        std::stringstream output;
        output << simdParseResult->doc.current_location();

        SimdStringResult*   result = new SimdStringResult;
        result->value = output.str();
        return result;
    }

    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const
    {
        SimdResult const*     simdParseResult = dynamic_cast<SimdResult const*>(parseResult);
        std::stringstream output;
        output << simdParseResult->doc.current_location();

        SimdStringResult*   result = new SimdStringResult;
        result->value = output.str();
        return result;
    }

    virtual bool Statistics(const ParseResultBase* /*parseResult*/, Stat* /*stat*/) const
    {
        return false;
    }

    virtual bool SaxStatistics(const char* /*json*/, size_t /*length*/, Stat* /*stat*/) const
    {
        return false;
    }

    virtual bool SaxStatisticsUTF16(const char* /*json*/, size_t /*length*/, Stat* /*stat*/) const
    {
        return false;
    }
};

REGISTER_TEST(SimdJsonTest);


#include "test.h"
#include <sstream>

#include "simdjson.h"
using namespace simdjson;

struct SimdDOMResult: public ParseResultBase
{
    simdjson::padded_string     json;
    dom::parser                 parser;
    dom::element                doc;
    SimdDOMResult(char const* j, size_t length)
        : json(j, length)
    {}
};

struct SimdDomStringResult: public StringResultBase
{
    std::string value;
    virtual const char* c_str() const
    {
        return value.c_str();
    }
};

class SimdJsonDomTest: public TestBase
{
    public:
    SimdJsonDomTest()
    {}
    virtual void SetUp(char const* /*fullPath*/) const override
    {}
    virtual void TearDown(char const* /*fullPath*/) const override
    {}

    virtual const char* GetName() const override        { return "simdjsonDom"; }
    virtual const char* Type()    const override        { return "C++";}
    virtual const char* GetFilename() const override    { return __FILE__; }

    // virtual bool ParseValidate(const char* json, std::size_t length) const override
    virtual ParseResultBase* Parse(const char* json, size_t length) const override
    {
        SimdDOMResult*             result = new SimdDOMResult(json, length);
        try
        {
            result->doc = result->parser.parse(result->json);
        }
        catch(...)
        {
            delete result;
            return nullptr;
        }
        return result;
    }

    virtual bool ParseDouble(const char* json, double* d) const override
    {
        ondemand::parser        parser;
        ondemand::document      doc;
        simdjson::padded_string jsonStr(json, strlen(json));
        if (parser.iterate(jsonStr).get(doc) != SUCCESS) {
            return false;
        }
        auto array = doc.get_array();
        for (auto val: array) {
            *d = val.get_double();
        }
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        ondemand::parser        parser;
        ondemand::document      doc;
        simdjson::padded_string jsonStr(json, strlen(json));
        if (parser.iterate(jsonStr).get(doc) != SUCCESS) {
            return false;
        }
        auto array = doc.get_array();
        for (auto val: array) {
            std::string_view view = val.get_string();
            s = std::string(std::begin(view), std::end(view));
        }
        return true;
    }

    virtual StringResultBase* SaxRoundtrip(const char* json, size_t length) const override
    {
        ondemand::parser        parser;
        ondemand::document      doc;
        simdjson::padded_string jsonStr(json, length);
        if (parser.iterate(jsonStr).get(doc) != SUCCESS) {
            return nullptr;
        }

        std::stringstream   output;
        output << doc;
        SimdDomStringResult*   result = new SimdDomStringResult;
        result->value = output.str();

        return result;
    }

    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const override
    {
        SimdDOMResult const*     simdParseResult = dynamic_cast<SimdDOMResult const*>(parseResult);
        std::stringstream output;

        output << simdParseResult->doc;
        SimdDomStringResult*   result = new SimdDomStringResult;
        result->value = output.str();

        return result;
    }

    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const override
    {
        SimdDOMResult const*     simdParseResult = dynamic_cast<SimdDOMResult const*>(parseResult);
        std::stringstream output;

        output << prettify(simdParseResult->doc);

        SimdDomStringResult*   result = new SimdDomStringResult;
        result->value = output.str();
        return result;
    }

    // virtual bool Statistics(const ParseResultBase* /*parseResult*/, Stat* /*stat*/) const override
    // virtual bool SaxStatistics(const char* /*json*/, size_t /*length*/, Stat* /*stat*/) const override
    // virtual bool SaxStatisticsUTF16(const char* /*json*/, size_t /*length*/, Stat* /*stat*/) const override
};

REGISTER_TEST(SimdJsonDomTest);


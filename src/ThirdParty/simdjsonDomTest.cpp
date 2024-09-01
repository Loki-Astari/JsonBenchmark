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
    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        std::unique_ptr<SimdDOMResult> result = std::make_unique<SimdDOMResult>(json, length);
        try
        {
            result->doc = result->parser.parse(result->json);
            reply = std::move(result);
        }
        catch(...) {}
        return true;
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        ondemand::parser        parser;
        ondemand::document      doc;
        simdjson::padded_string jsonStr(json, strlen(json));
        if (parser.iterate(jsonStr).get(doc) != SUCCESS) {
            return true;
        }
        auto array = doc.get_array();
        for (auto val: array) {
            d = val.get_double();
        }
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        ondemand::parser        parser;
        ondemand::document      doc;
        simdjson::padded_string jsonStr(json, strlen(json));
        if (parser.iterate(jsonStr).get(doc) != SUCCESS) {
            return true;
        }
        auto array = doc.get_array();
        for (auto val: array) {
            std::string_view view = val.get_string();
            s = std::string(std::begin(view), std::end(view));
        }
        return true;
    }

    virtual bool SaxRoundtrip(const char* json, size_t length, std::unique_ptr<StringResultBase>& reply) const override
    {
        ondemand::parser        parser;
        ondemand::document      doc;
        simdjson::padded_string jsonStr(json, length);
        if (parser.iterate(jsonStr).get(doc) != SUCCESS) {
            return true;
        }

        std::unique_ptr<StringResultUsingStream>   result = std::make_unique<StringResultUsingStream>();
        result->stream << doc;
        reply = std::move(result);

        return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        SimdDOMResult const&     simdParseResult = dynamic_cast<SimdDOMResult const&>(parseResult);

        std::unique_ptr<StringResultUsingStream>   result = std::make_unique<StringResultUsingStream>();
        result->stream << simdParseResult.doc;
        reply = std::move(result);

        return true;
    }

    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        SimdDOMResult const&     simdParseResult = dynamic_cast<SimdDOMResult const&>(parseResult);

        std::unique_ptr<StringResultUsingStream>   result = std::make_unique<StringResultUsingStream>();
        result->stream << prettify(simdParseResult.doc);

        reply = std::move(result);
        return true;
    }

    // virtual bool Statistics(const ParseResultBase* /*parseResult*/, Stat* /*stat*/) const override
    // virtual bool SaxStatistics(const char* /*json*/, size_t /*length*/, Stat* /*stat*/) const override
    // virtual bool SaxStatisticsUTF16(const char* /*json*/, size_t /*length*/, Stat* /*stat*/) const override
};

REGISTER_TEST(SimdJsonDomTest);


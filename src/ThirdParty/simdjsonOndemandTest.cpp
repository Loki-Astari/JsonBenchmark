#include "test.h"

#include "simdjson.h"
using namespace simdjson;

struct SimdOndemandResult: public ParseResultBase
{
    simdjson::padded_string     json;
    ondemand::parser            parser;
    ondemand::document          doc;
    SimdOndemandResult(char const* j, size_t length)
        : json(j, length)
    {}
};

class SimdJsonOndemandTest: public TestBase
{
    public:
    SimdJsonOndemandTest()
    {}
    virtual void SetUp(char const* /*fullPath*/) const override
    {}
    virtual void TearDown(char const* /*fullPath*/) const override
    {}

    virtual const char* GetName() const override        { return "simdjsonOndemand"; }
    virtual const char* Type()    const override        { return "C++";}
    virtual const char* GetFilename() const override    { return __FILE__; }

    bool validateDoc(ondemand::value element) const
    {
        switch (element.type())
        {
            case ondemand::json_type::array:
            {
                for (auto child : element.get_array()) {
                    if (!validateDoc(child.value())) {
                        return false;
                    }
                }
                break;
            }
            case ondemand::json_type::object:
            {
                for (auto field: element.get_object()) {
                    if (!validateDoc(field.value())) {
                        return false;
                    }
                }
                break;
            }
            case ondemand::json_type::number:
            {
                double value = element.get_double();
                ((void)value);
                break;
            }
            case ondemand::json_type::string:
            {
                std::string_view value = element.get_string();
                ((void)value);
                break;
            }
            case ondemand::json_type::boolean:
            {
                bool    value = element.get_bool();
                ((void)value);
                break;
            }
            case ondemand::json_type::null:
            {
                bool value = element.is_null();
                ((void)value);
                break;
            }
            default:
                return false;
        }
        return true;
    }
    virtual bool ParseValidate(const char* json, std::size_t length) const override
    {
        std::unique_ptr<SimdOndemandResult>  result{dynamic_cast<SimdOndemandResult*>(Parse(json, length))};
        if (result.get() == nullptr) {
            return false;
        }
        try
        {
            ondemand::value val = result->doc;
            validateDoc(val);
            if (!result->doc.at_end()) {
                return false;
            }
        }
        catch(...) {
            return false;
        }
        return true;
    }
    virtual ParseResultBase* Parse(const char* json, size_t length) const override
    {
        SimdOndemandResult*             result = new SimdOndemandResult(json, length);
        auto error = result->parser.iterate(result->json).get(result->doc);
        if (error) {
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

    // virtual StringResultBase* SaxRoundtrip(const char* json, size_t length) const override
    // virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const override
    // virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const override

    // virtual bool Statistics(const ParseResultBase* /*parseResult*/, Stat* /*stat*/) const override
    // virtual bool SaxStatistics(const char* /*json*/, size_t /*length*/, Stat* /*stat*/) const override
    // virtual bool SaxStatisticsUTF16(const char* /*json*/, size_t /*length*/, Stat* /*stat*/) const override
};

REGISTER_TEST(SimdJsonOndemandTest);


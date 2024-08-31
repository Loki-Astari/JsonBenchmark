#include "test.h"

#include "simdjson.h"
#include <iostream>

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

struct SimdOndemandStringResult: public StringResultBase
{
    std::string value;
    virtual const char* c_str() const
    {
        return value.c_str();
    }
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

    void streamDoc(ondemand::value element, std::ostream& s) const
    {
        switch (element.type())
        {
            case ondemand::json_type::array:
            {
                char sep =' ';
                s << '[';
                for (auto child : element.get_array()) {
                    s << sep;
                    streamDoc(child.value(), s);
                    sep = ',';
                }
                s << ']';
                break;
            }
            case ondemand::json_type::object:
            {
                char sep =' ';
                s << '{';
                for (auto field: element.get_object()) {
                    s << sep << '"' << field.key() << '"' << ':';
                    streamDoc(field.value(), s);
                    sep = ',';
                }
                s << '}';
                break;
            }
            case ondemand::json_type::number:
            {
                double value = element.get_double();
                s << std::setprecision(17) << value;
                break;
            }
            case ondemand::json_type::string:
            {
                std::string_view value = element.get_string();
                s << '"' << value << '"';
                break;
            }
            case ondemand::json_type::boolean:
            {
                bool    value = element.get_bool();
                s << (value ? "true" : "false");
                break;
            }
            case ondemand::json_type::null:
            {
                s << "null";
                break;
            }
            default: {
                throw std::runtime_error("Fail");
            }
        }
    }
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
    virtual bool ParseValidate(const char* json, std::size_t length, bool& reply) const override
    {
        reply = false;
        std::unique_ptr<ParseResultBase>  dom;
        Parse(json, length, dom);
        if (dom.get() != nullptr)
        {
            try
            {
                SimdOndemandResult& result = dynamic_cast<SimdOndemandResult&>(*dom);
                ondemand::value val = result.doc;
                if (validateDoc(val) && result.doc.at_end()) {
                    reply = true;
                }
            }
            catch(...) {
            }
        }
        return true;
    }
    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        std::unique_ptr<SimdOndemandResult> result = std::make_unique<SimdOndemandResult>(json, length);
        auto error = result->parser.iterate(result->json).get(result->doc);
        if (!error) {
            reply = std::move(result);
        }
        return true;
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        ondemand::parser        parser;
        ondemand::document      doc;
        simdjson::padded_string jsonStr(json, strlen(json));
        if (parser.iterate(jsonStr).get(doc) != SUCCESS) {
            return false;
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
            return false;
        }
        auto array = doc.get_array();
        for (auto val: array) {
            std::string_view view = val.get_string();
            s = std::string(std::begin(view), std::end(view));
        }
        return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        //SimdOndemandResult const& input = dynamic_cast<SimdOndemandResult const&>(parseResult);
        SimdOndemandResult& input = const_cast<SimdOndemandResult&>(dynamic_cast<SimdOndemandResult const&>(parseResult));
        ondemand::value const val = input.doc;
        std::stringstream ss;
        streamDoc(val, ss);
        std::unique_ptr<SimdOndemandStringResult> result = std::make_unique<SimdOndemandStringResult>();
        result->value = ss.str();
        reply = std::move(result);
        return true;
    }
    // virtual bool SaxRoundtrip(const char* json, size_t length, std::unique_ptr<StringResultBase>& reply) const override
    // virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override

    // virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    // virtual bool SaxStatistics(const char* json, size_t length, Stat& stat) const override
    // virtual bool SaxStatisticsUTF16(const char* json, size_t length, Stat& stat) const override
};

REGISTER_TEST(SimdJsonOndemandTest);


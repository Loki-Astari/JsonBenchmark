#ifndef __linux__

#include "test.h"
#include <sstream>
#include <boost/json.hpp>
#include <iostream>
#include <memory>

struct BoostJsonResult: public ParseResultBase
{
    boost::json::value value;
};

class BoostJsonTest: public TestBase
{
    public:
    BoostJsonTest()
    {}
    virtual void SetUp(char const* /*fullPath*/) const override
    {}
    virtual void TearDown(char const* /*fullPath*/) const override
    {}

    virtual const char* GetName() const override        { return "boostJson"; }
    virtual const char* Type()    const override        { return "C++";}
    virtual const char* GetFilename() const override    { return __FILE__; }

    // virtual bool ParseValidate(const char* json, std::size_t length) const override
    virtual bool Parse(const char* json, size_t /*length*/, std::unique_ptr<ParseResultBase>& reply) const override
    {
        std::unique_ptr<BoostJsonResult>  result = std::make_unique<BoostJsonResult>();

        boost::system::error_code ec;
        result->value = boost::json::parse(json, ec);
        if (!ec) {
            reply = std::move(result);
        }
        return true;
    }
    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        boost::system::error_code   ec;
        boost::json::value          value;

        value = boost::json::parse(json, ec);
        if (!ec) {
            boost::json::array&  array = value.get_array();
            boost::json::value   number= array[0];
            d = number.get_double();
        }
        return true;
    }
    virtual bool ParseString(const char* json, std::string& s) const override
    {
        boost::system::error_code   ec;
        boost::json::value          value;

        value = boost::json::parse(json, ec);
        if (!ec) {
            boost::json::array&  array = value.get_array();
            boost::json::value   number= array[0];
            s = number.get_string();
        }
        return true;
    }
    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        BoostJsonResult const& boostParserResult = dynamic_cast<BoostJsonResult const&>(parseResult);
        std::unique_ptr<StringResultUsingString> result = std::make_unique<StringResultUsingString>();

        result->result = boost::json::serialize(boostParserResult.value);
        reply = std::move(result);

        return true;
    }
    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        BoostJsonResult const& boostParserResult = dynamic_cast<BoostJsonResult const&>(parseResult);
        std::unique_ptr<StringResultUsingStream> result = std::make_unique<StringResultUsingStream>();

        pretty_print(result->stream, boostParserResult.value);
        reply = std::move(result);

        return true;
    }

    void pretty_print(std::ostream& os, boost::json::value const& jv, std::string* indent = nullptr ) const
    {
        std::string indent_;
        if(! indent) {
            indent = &indent_;
        }
        switch(jv.kind())
        {
            case boost::json::kind::object:
                {
                    os << "{\n";
                    indent->append(4, ' ');
                    auto const& obj = jv.get_object();
                    if(! obj.empty())
                    {
                        auto it = obj.begin();
                        for(;;)
                        {
                            os << *indent << boost::json::serialize(it->key()) << " : ";
                            pretty_print(os, it->value(), indent);
                            if(++it == obj.end())
                                break;
                            os << ",\n";
                        }
                    }
                    os << "\n";
                    indent->resize(indent->size() - 4);
                    os << *indent << "}";
                    break;
                }

            case boost::json::kind::array:
                {
                    os << "[\n";
                    indent->append(4, ' ');
                    auto const& arr = jv.get_array();
                    if(! arr.empty())
                    {
                        auto it = arr.begin();
                        for(;;)
                        {
                            os << *indent;
                            pretty_print( os, *it, indent);
                            if(++it == arr.end())
                                break;
                            os << ",\n";
                        }
                    }
                    os << "\n";
                    indent->resize(indent->size() - 4);
                    os << *indent << "]";
                    break;
                }

            case boost::json::kind::string:
                {
                    os << boost::json::serialize(jv.get_string());
                    break;
                }

            case boost::json::kind::uint64:
            case boost::json::kind::int64:
            case boost::json::kind::double_:
                os << jv;
                break;

            case boost::json::kind::bool_:
                os << (jv.get_bool() ? "true" : "false");
                break;

            case boost::json::kind::null:
                os << "null";
                break;
        }

        if(indent->empty())
            os << "\n";
    }

    // virtual bool Statistics(const ParseResultBase* /*parseResult*/, Stat* /*stat*/) const override
    // virtual bool SaxStatistics(const char* /*json*/, size_t /*length*/, Stat* /*stat*/) const override
    // virtual bool SaxStatisticsUTF16(const char* /*json*/, size_t /*length*/, Stat* /*stat*/) const override
};

REGISTER_TEST(BoostJsonTest);

#endif

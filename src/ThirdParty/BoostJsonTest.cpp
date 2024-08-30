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

struct BoosJsonStringResult: public StringResultBase
{
    std::string value;
    virtual const char* c_str() const
    {
        return value.c_str();
    }
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
    virtual ParseResultBase* Parse(const char* json, size_t /*length*/) const override
    {
        BoostJsonResult*  result = new BoostJsonResult;

        boost::system::error_code ec;
        result->value = boost::json::parse(json, ec);
        if (ec) {
            delete result;
            return nullptr;
        }
        return result;
    }
    virtual bool ParseDouble(const char* json, double* d) const override
    {
        boost::system::error_code   ec;
        boost::json::value          value;

        value = boost::json::parse(json, ec);
        if (ec) {
            return false;
        }
        boost::json::array&  array = value.get_array();
        boost::json::value   number= array[0];
        *d = number.get_double();
        return true;
    }
    virtual bool ParseString(const char* json, std::string& s) const override
    {
        boost::system::error_code   ec;
        boost::json::value          value;

        value = boost::json::parse(json, ec);
        if (ec) {
            return false;
        }
        boost::json::array&  array = value.get_array();
        boost::json::value   number= array[0];
        s = number.get_string();
        return true;
    }
    virtual StringResultBase* SaxRoundtrip(const char* json, size_t length) const override
    {
        std::unique_ptr<ParseResultBase> boostParserResult{this->Parse(json, length)};
        return Stringify(boostParserResult.get());
    }
    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const override
    {
        BoostJsonResult const* boostParserResult = dynamic_cast<BoostJsonResult const*>(parseResult);
        BoosJsonStringResult* result = new BoosJsonStringResult;

        result->value = boost::json::serialize(boostParserResult->value);
        return result;
    }
    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const override
    {
        BoostJsonResult const* boostParserResult = dynamic_cast<BoostJsonResult const*>(parseResult);
        BoosJsonStringResult* result = new BoosJsonStringResult;

        std::stringstream ss;
        pretty_print(ss, boostParserResult->value);

        result->value = ss.str();
        return result;
    }

    void pretty_print(std::ostream& os, boost::json::value const& jv, std::string* indent = nullptr ) const
    {
        std::string indent_;
        if(! indent)
            indent = &indent_;
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
            if(jv.get_bool())
                os << "true";
            else
                os << "false";
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

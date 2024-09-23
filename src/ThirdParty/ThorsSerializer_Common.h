#ifndef THORS_SERIALIZER_COMMON_H
#define THORS_SERIALIZER_COMMON_H

#include "TypeSafe.h"

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/SerUtil.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <chrono>
#include <string_view>
using namespace std::chrono;


namespace ThorsSerializer
{

using ThorsAnvil::Serialize::jsonExporter;
using ThorsAnvil::Serialize::jsonImporter;
using ThorsAnvil::Serialize::ParserConfig;
using ThorsAnvil::Serialize::ParseType;
using ThorsAnvil::Serialize::OutputType;

class StringNumber
{
    std::string     data;
    friend std::ostream& operator<<(std::ostream& str, StringNumber const& val)
    {
        return str << val.data;
    }
    friend std::istream& operator>>(std::istream& str, StringNumber& val)
    {
        return std::getline(str, val.data);
    }
};

class VectorDouble: public TestAction
{
    public:
    virtual bool ParseDouble(const char* json, long double& d) const override {
        std::stringstream stream(json);
        std::vector<double> result;
        stream >> jsonImporter(result);
        if (stream && result.size() == 1) {
            d = result[0];
        }
        return true;
    }
};
class VectorString: public TestAction
{
    public:
    virtual bool ParseString(const char* json, std::string& output) const override {
        std::stringstream stream(json);
        std::vector<std::string> result;
        stream >> jsonImporter(result);
        if (stream && result.size() == 1) {
            output = result[0];
        }
        return true;
    }
};


template<typename Value>
struct GetValueResult: public ParseResultBase
{
    Value       data;
    GetValueResult()
        : data{}
    {}
};

template<typename Value>
class GetValue: public TestAction
{
    ParserConfig    config;

    public:
    GetValue(ParserConfig config = ParserConfig{}.setValidateNoTrailingData()): config(config) {}
    virtual bool Parse(const char* json, size_t size, std::unique_ptr<ParseResultBase>& reply) const override
    {
        //std::cerr << "GetValue::Parse\n\n\n";
        std::unique_ptr<GetValueResult<Value>> result = std::make_unique<GetValueResult<Value>>();
        bool ok = (std::string_view(json, size) >> jsonImporter(result->data, config));
        if (ok) {
            reply = std::move(result);
        }
        return true;
    }
    virtual bool Stringify(ParseResultBase const& value, std::unique_ptr<StringResultBase>& reply) const override
    {
        std::unique_ptr<StringResultUsingString> result = std::make_unique<StringResultUsingString>();
        GetValueResult<Value> const& inputValue = dynamic_cast<GetValueResult<Value> const&>(value);
        bool ok = result->result << jsonExporter(inputValue.data, OutputType::Stream);
        if (ok) {
            reply = std::move(result);
        }
        return true;
    }
    virtual bool Prettify(const ParseResultBase& value, std::unique_ptr<StringResultBase>& reply) const override
    {
        GetValueResult<Value> const& inputValue = dynamic_cast<GetValueResult<Value> const&>(value);
        std::unique_ptr<StringResultUsingString> result = std::make_unique<StringResultUsingString>();
        bool ok = result->result << jsonExporter(inputValue.data, OutputType::Config);
        if (ok) {
            reply = std::move(result);
        }
        return true;
    }
};

template<typename Value>
class GetValuePerf: public GetValue<Value>
{
    public:
    GetValuePerf()
        : GetValue<Value>(ParserConfig{}.setValidateNoTrailingData().setNoBackslashConversion())
    {}
};

}

ThorsAnvil_MakeTraitCustom(ThorsSerializer::StringNumber);

#endif

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
using namespace std::chrono;


namespace ThorsSerializer
{

using ThorsAnvil::Serialize::jsonExporter;
using ThorsAnvil::Serialize::jsonImporter;
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
            return true;
        }
        return false;
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
            return true;
        }
        return false;
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
    public:
    virtual bool Parse(const char* json, size_t, std::unique_ptr<ParseResultBase>& reply) const override
    {
        std::unique_ptr<GetValueResult<Value>> result = std::make_unique<GetValueResult<Value>>();
        std::stringstream stream(json);
        stream >> jsonImporter(result->data);
        char bad;
        if (!stream || stream >> bad) {
            result.release();
        }
        reply = std::move(result);
        return true;
    }
    virtual bool Stringify(ParseResultBase const& value, std::unique_ptr<StringResultBase>& reply) const override
    {
        std::unique_ptr<StringResultUsingStream> result = std::make_unique<StringResultUsingStream>();
        GetValueResult<Value> const& inputValue = dynamic_cast<GetValueResult<Value> const&>(value);
        result->stream << jsonExporter(inputValue.data, OutputType::Stream);
        reply = std::move(result);
        return true;
    }
    virtual bool Prettify(const ParseResultBase& value, std::unique_ptr<StringResultBase>& reply) const override
    {
        GetValueResult<Value> const& inputValue = dynamic_cast<GetValueResult<Value> const&>(value);
        std::unique_ptr<StringResultUsingStream> result = std::make_unique<StringResultUsingStream>();
        result->stream << jsonExporter(inputValue.data, OutputType::Config);
        reply = std::move(result);
        return true;
    }
};

}

ThorsAnvil_MakeTraitCustom(ThorsSerializer::StringNumber);

#endif

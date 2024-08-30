#ifndef THORS_SERIALIZER_COMMON_H
#define THORS_SERIALIZER_COMMON_H

#include "TypeSafe.h"

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/SerUtil.h"

#include <sstream>

using ThorsAnvil::Serialize::jsonExporter;
using ThorsAnvil::Serialize::jsonImporter;
using ThorsAnvil::Serialize::ParseType;
using ThorsAnvil::Serialize::OutputType;

namespace ThorsSerializer
{

class VectorDouble: public TestAction
{
    public:
    virtual bool ParseDouble(const char* json, double* d) const {
        std::stringstream stream(json);
        std::vector<double> result;
        stream >> jsonImporter(result);
        if (stream && result.size() == 1) {
            *d = result[0];
            return true;
        }
        return false;
    }
};
class VectorString: public TestAction
{
    public:
    virtual bool ParseString(const char* json, std::string& output) const {
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

struct GetValueStream: public StringResultBase {
    std::string         value;
    virtual const char* c_str() const   {return value.c_str();}
};

template<typename Value>
class GetValue: public TestAction
{
    public:
    virtual ParseResultBase* Parse(const char* json, size_t) const {
        GetValueResult<Value>* result = new GetValueResult<Value>();
        std::stringstream stream(json);
        stream >> jsonImporter(result->data);
        char bad;
        if (!stream || stream >> bad) {
            delete result;
            result = nullptr;
        }
        return result;
    }
    virtual StringResultBase* Stringify(ParseResultBase const* value) const {
        GetValueStream* result = new GetValueStream;
        GetValueResult<Value> const* inputValue = dynamic_cast<GetValueResult<Value> const*>(value);
        std::stringstream ss;
        ss << jsonExporter(inputValue->data, OutputType::Stream);
        result->value = ss.str();
        return result;
    }
    virtual StringResultBase* Prettify(const ParseResultBase* value) const {
        GetValueStream* result = new GetValueStream;
        GetValueResult<Value> const* inputValue = dynamic_cast<GetValueResult<Value> const*>(value);
        std::stringstream ss;
        ss << jsonExporter(inputValue->data, OutputType::Config);
        result->value = ss.str();
        return result;
    }
    virtual bool Statistics(const ParseResultBase* value, Stat* stat) const {
        GetValueResult<Value> const* inputValue = dynamic_cast<GetValueResult<Value> const*>(value);
        getStats(stat, inputValue->data);
        return true;
    }
    virtual StringResultBase* SaxRoundtrip(const char* json, size_t length) const {
        std::unique_ptr<ParseResultBase> dom(Parse(json, length));
        if (!dom)
        {   return nullptr;
        }
        return Stringify(dom.get());
    }
    virtual bool SaxStatistics(const char* json, size_t length, Stat* stat) const {
        std::unique_ptr<ParseResultBase> dom(Parse(json, length));
        if (!dom)
        {   return false;
        }
        return Statistics(dom.get(), stat);
    }
};

inline void getStats(Stat* stat, std::map<std::string, M01> const& value)
{
    stat->objectCount++;
    stat->memberCount += value.size();
    for(auto const& da: value) {
        stat->stringLength += da.first.size();
        getStats(stat, da.second);
    }
}

}

#endif

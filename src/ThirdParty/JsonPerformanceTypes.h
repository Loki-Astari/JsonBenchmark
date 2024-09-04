#ifndef JSONPERFORMANCETYPES_H
#define JSONPERFORMANCETYPES_H

#include "TypeSafe.h"

namespace JsonPerformanceTypes
{

class VectorDouble: public TestAction
{
    public:
    virtual bool ParseDouble(const char* json, long double& reply) const
    {
        // Add Implementation of Parsing an array with a single double value.
        // Assign the value of the double to the parameter "reply"

        // Note: Always return true to indicate you have implemented function.
        return true;
    }
};

class VectorString: public TestAction
{
    public:
    virtual bool ParseString(const char* json, std::string& reply) const
    {
        // Add Implementation of Parsing an array with a single string value.
        // Assign the value of the double to the parameter "reply"

        // Note: Always return true to indicate you have implemented function.
        return true;
    }
};

template<typename T>
struct GetValueResult: public ParseResultBase
{
    T       data;
};

template<typename T>
class GetValue: public TestAction
{

    public:
    virtual bool Parse(const char* json, size_t, std::unique_ptr<ParseResultBase>& reply) const
    {
        auto result = std::make_unique<GetValueResult>();
        // Read the object from JSON into an object of type T
        // in the object result.
        // ie convert JSON  => result->data



        // If your parse succeeds. Then move the result to reply.
        if (success) {
            reply = std::move(result);
        }

        // Note: Always return true to indicate you have implemented function.
        return true;
    }
    virtual bool Stringify(const ParseResultBase& parsedData, std::unique_ptr<StringResultBase>& reply)  const
    {
        // paredData is the value returned by parse.
        // It has a value of type T. Convert this to JSON.
        auto result = std::make_unique<StringResultUsingString>();  // Note there is a also a stream version. StringResultUsingStream

        // convert parsedData => result->result;


        if (success) {
            reply = std::move(result);
        }

        // Note: Always return true to indicate you have implemented function.
        retrurn true;
    }
    virtual bool Prettify(const ParseResultBase& parsedData, std::unique_ptr<StringResultBase>& reply) const
    {
        // paredData is the value returned by parse.
        // It has a value of type T. Convert this to JSON.
        auto result = std::make_unique<StringResultUsingString>();  // Note there is a also a stream version. StringResultUsingStream

        // convert parsedData => result->result;


        if (success) {
            reply = std::move(result);
        }

        // Note: Always return true to indicate you have implemented function.
        retrurn true;
        return true;
    }
};

}

#endif

#ifndef GLAZE_COMMON_H
#define GLAZE_COMMON_H

#include "TypeSafe.h"
#include "glaze/glaze.hpp"
#include <iostream>


namespace Glaze
{

class VectorDouble: public TestAction
{
    public:
    virtual bool ParseDouble(const char* json, long double& reply) const
    {
        auto result = glz::read_json<std::vector<double>>(json);
        if (result) {
            std::vector<double> data;
            data = std::move(result.value());
            if (data.size() == 1) {
                reply = data[0];
            }
        }
        return true;
    }
};

class VectorString: public TestAction
{
    public:
    virtual bool ParseString(const char* json, std::string& reply) const
    {
        auto result = glz::read_json<std::vector<std::string>>(json);
        if (result) {
            std::vector<std::string> data;
            data = std::move(result.value());
            if (data.size() == 1) {
                reply = data[0];
            }
        }
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
        std::unique_ptr<GetValueResult<T>>    parsedData = std::make_unique<GetValueResult<T>>();
        auto error = glz::read<glz::opts{.validate_trailing_whitespace = true, .error_on_unknown_keys = false}>(parsedData->data, json);
        if (error)
        {
            std::string descriptive_error = glz::format_error(error, json);
            std::cerr << "Error: >" << descriptive_error << "\n";
        }
        else
        {
            reply = std::move(parsedData);
        }
        return true;
    }
    virtual bool Stringify(const ParseResultBase& parsedData, std::unique_ptr<StringResultBase>& reply)  const
    {
        return Prettify(parsedData, reply);
    }
    virtual bool Prettify(const ParseResultBase& parsedData, std::unique_ptr<StringResultBase>& reply) const
    {
        GetValueResult<T> const& parsedDataInput = dynamic_cast<GetValueResult<T> const&>(parsedData);
        std::unique_ptr<StringResultUsingString>    output = std::make_unique<StringResultUsingString>();

        output->result = glz::write_json(parsedDataInput.data).value_or("error");
        reply = std::move(output);
        return true;
    }
};

}

#endif

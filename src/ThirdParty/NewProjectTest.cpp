#include "test.h"


class NewProjectResult : public ParseResultBase
{
    public:
};

class NewProjectTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "NewProject"; }
    virtual const char* Type()        const override { return "C++";}
    virtual const char* GetFilename() const override { return __FILE__; }

#if 0
    virtual bool ParseValidate(const char* j, size_t length, bool& reply) const override
    {
        // This function is optional.
        // If you don't provide it then it uses Parse().
        // If Parse() returns a value in its `reply` then it will set the above reply to true otherwise false.


        // Has to return true to indicate that it implemented the function.
        return true;
    }
    virtual bool SaxRoundtrip(const char* json, std::size_t length, std::unique_ptr<StringResultBase>& result)  const
    {
        // This function is optional
        // If you don't provide it then us uses Parse() followed by Stringify().

        return true;
    }
#endif
    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        // Parse the JSON string in "json" with length "length"

        // Your intermediate result is stored in "reply".
        // If you fail to parse (or there is an error in the JSON) then don't set reply (just leave it null).

        // Note every JSON parser is different so you should define your own reply structure.
        // See the "NewProjectResult" above, configure this to hold your intermediate data.

        // The value returned in "reply" will be passed as the first value to Stringify() and Prettify() (if it is not null).
        return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        // Convert the structure you create in "Parse" into a JSON string.
        // Put the string "reply"
        // Note: See test.h for two common implementations of StringResultBase: => (StringResultUsingString / StringResultUsingStream)
        //       You can use these types or you can define your own.
        return true;
    }

    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        // Convert the structure you create in "Parse" into a JSON string.
        //      The expectation is that this is "Human readable Nice version"
        // Put the string "reply"
        // Note: See test.h for two common implementations of StringResultBase: => (StringResultUsingString / StringResultUsingStream)
        //       You can use these types or you can define your own.
        return true;
    }

    virtual bool ParseDouble(const char* j, long double& d) const override
    {
        // The input JSON is an array with a single double value.
        // Read the double from the array and assign to d.
        //
        // Note: There are lots of valid double formats you are expected to be able to decode them.

        return true;
    }

    virtual bool ParseString(const char* j, std::string& s) const override
    {
        // The input JSON is an array with a single string value.
        // Read the string from the array and assign to s.
        //
        // Note: There are lots of escape sequence in JSON you are expected to decode these into the C++ representation of the string.
        //       e.g.: the '\t' sequence is expected to be decoded into the "Tab" character. etc.
        return true;
    }
};

REGISTER_TEST(NewProjectTest);

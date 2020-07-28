#include "test.h"
#include "singleheader/simdjson.h"

class SimdJsonResult : public ParseResultBase
{
};

class SimdJsonTest : public TestBase
{
public:
    virtual const char* GetName() const { return "simdjson"; }
    virtual const char* Type()    const { return "C++";}
    virtual const char* GetFilename() const { return __FILE__; }
	
    virtual ParseResultBase* Parse(const char* /*json*/, size_t /*length*/) const
    {
        return nullptr;
    }

    virtual bool Statistics(const ParseResultBase* /*parseResult*/, Stat* /*stat*/) const
    {
        return false;
    }

    virtual bool ParseDouble(const char* /*json*/, double* /*d*/) const
    {
        return false;
    }

    virtual bool ParseString(const char* /*json*/, std::string& /*s*/) const
    {
        return false;
    }
};

REGISTER_TEST(SimdJsonTest);

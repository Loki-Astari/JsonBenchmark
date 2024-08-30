#include "test.h"
#include <sstream>

#include "glaze/glaze.hpp"

struct GlazeResult: public ParseResultBase
{
};

struct GlazeStringResult: public StringResultBase
{
    //virtual const char* c_str() const
};

class GlazeTest: public TestBase
{
    public:
    GlazeTest()
    {}
    virtual void SetUp(char const* /*fullPath*/) const override
    {}
    virtual void TearDown(char const* /*fullPath*/) const override
    {}

    virtual const char* GetName() const override        { return "Glaze"; }
    virtual const char* Type()    const override        { return "C++";}
    virtual const char* GetFilename() const override    { return __FILE__; }

    // virtual bool ParseValidate(const char* json, std::size_t length) const override
    // virtual ParseResultBase* Parse(const char* json, size_t length) const override
    // virtual bool ParseDouble(const char* json, double* d) const override
    // virtual bool ParseString(const char* json, std::string& s) const override
    // virtual StringResultBase* SaxRoundtrip(const char* json, size_t length) const override
    // virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const override
    // virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const override

    // virtual bool Statistics(const ParseResultBase* /*parseResult*/, Stat* /*stat*/) const override
    // virtual bool SaxStatistics(const char* /*json*/, size_t /*length*/, Stat* /*stat*/) const override
    // virtual bool SaxStatisticsUTF16(const char* /*json*/, size_t /*length*/, Stat* /*stat*/) const override
};

REGISTER_TEST(GlazeTest);


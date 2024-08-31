#ifndef __linux__

#include "test.h"
#include <sstream>

#include "glaze/glaze.hpp"


// data/jsonchecker_pass/pass01.json 
//  Not Supported by Glaze

// data/jsonchecker_pass/pass02.json
using S                             = std::string;
using VS                            = std::vector<S>;
using VVS                           = std::vector<VS>;
using VVVS                          = std::vector<VVS>;
using VVVVS                         = std::vector<VVVS>;
using VVVVVS                        = std::vector<VVVVS>;
using VVVVVVS                       = std::vector<VVVVVS>;
using VVVVVVVS                      = std::vector<VVVVVVS>;
using VVVVVVVVS                     = std::vector<VVVVVVVS>;
using VVVVVVVVVS                    = std::vector<VVVVVVVVS>;
using VVVVVVVVVVS                   = std::vector<VVVVVVVVVS>;
using VVVVVVVVVVVS                  = std::vector<VVVVVVVVVVS>;
using VVVVVVVVVVVVS                 = std::vector<VVVVVVVVVVVS>;
using VVVVVVVVVVVVVS                = std::vector<VVVVVVVVVVVVS>;
using VVVVVVVVVVVVVVS               = std::vector<VVVVVVVVVVVVVS>;
using VVVVVVVVVVVVVVVS              = std::vector<VVVVVVVVVVVVVVS>;
using VVVVVVVVVVVVVVVVS             = std::vector<VVVVVVVVVVVVVVVS>;
using VVVVVVVVVVVVVVVVVS            = std::vector<VVVVVVVVVVVVVVVVS>;
using VVVVVVVVVVVVVVVVVVS           = std::vector<VVVVVVVVVVVVVVVVVS>;
using VVVVVVVVVVVVVVVVVVVS          = std::vector<VVVVVVVVVVVVVVVVVVS>;

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
    // virtual bool ParseDouble(const char* json, long double* d) const override
    // virtual bool ParseString(const char* json, std::string& s) const override
    // virtual StringResultBase* SaxRoundtrip(const char* json, size_t length) const override
    // virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const override
    // virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const override

    // virtual bool Statistics(const ParseResultBase* /*parseResult*/, Stat* /*stat*/) const override
    // virtual bool SaxStatistics(const char* /*json*/, size_t /*length*/, Stat* /*stat*/) const override
    // virtual bool SaxStatisticsUTF16(const char* /*json*/, size_t /*length*/, Stat* /*stat*/) const override
};

REGISTER_TEST(GlazeTest);

#endif

#ifndef THORS_ANVIL_BENCHMARK_PASS_CHECKER_H
#define THORS_ANVIL_BENCHMARK_PASS_CHECKER_H

#include "CommonReader.h"
#include <memory>

namespace ThorsAnvil
{
    namespace Benchmark
    {

class PassChecker: public CommonReader
{
    public:
        using CommonReader::CommonReader;
        virtual std::string getDir() const override
        {
            return "jsonchecker_pass";
        }
        virtual State executeTest(TestBase const& parser, Test const& test) override
        {
            std::cerr << "Pass Checker\n";
            bool result(parser.ParseValidate(test.input.c_str(), test.input.size()));
            return result == true ? Pass : Fail;
        }
        virtual void generateConPerData(TestBase const& parser, Test const& test, State state) override
        {
            std::size_t begin   = test.path.str().rfind('/') + 1;
            std::size_t end     = test.path.str().rfind('.');
            std::string fileName = test.path.str().substr(begin, (end - begin));
            options.conformance << "1. Parse Validation,"
                                << parser.GetName() << ","
                                << fileName << ","
                                << (state == Pass ? "true" : "false")
                                << "\n";
        }
};

    }
}

#endif

#ifndef THORS_ANVIL_BENCHMARK_FAIL_CHECKER_H
#define THORS_ANVIL_BENCHMARK_FAIL_CHECKER_H

#include "CommonReader.h"
#include <memory>

namespace ThorsAnvil
{
    namespace Benchmark
    {

class FailChecker: public CommonReader
{
    public:
        using CommonReader::CommonReader;
        virtual std::string getDir() const override
        {
            return "jsonchecker_fail";
        }
        virtual State executeTest(TestBase const& parser, Test const& test) override
        {
            std::unique_ptr<ParseResultBase> result(parser.Parse(test.input.c_str(), test.input.size()));
            return result == nullptr ? Pass : Fail;
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

#ifndef THORS_ANVIL_BENCHMARK_VALIDATE_STRING_H
#define THORS_ANVIL_BENCHMARK_VALIDATE_STRING_H

#include "benchmark.h"
#include <fstream>
#include <string>

namespace ThorsAnvil
{
    namespace Benchmark
    {

class ValidateString: public Benchmark
{
    public:
        using Benchmark::Benchmark;
        virtual std::string setupName(Test const&) override
        {
            return "vector-string";
        }
        virtual void preloadData(Test& test) override
        {
            std::ifstream   input(test.path.str());

            std::getline(input, test.input, '<');
            input.ignore(std::numeric_limits<std::streamsize>::max(), '>');
            std::getline(input, test.output, '<');
        }
        virtual State executeTest(TestBase const& parser, Test const& test) override
        {
            std::string output;
            bool result = parser.ParseString(test.input.c_str(), output);
            return !result ? NotImplemented : output == test.output ? Pass: Fail;
        }
        virtual void generateConPerData(TestBase const& parser, Test const& test, State state) override
        {
            std::size_t begin   = test.path.str().rfind('/') + 1;
            std::size_t end     = test.path.str().rfind('.');
            std::string fileName = test.path.str().substr(begin, (end - begin));
            options.conformance << "3. Parse String,"
                                << parser.GetName() << ","
                                << fileName << ","
                                << (state == Pass ? "true" : "false")
                                << "\n";
        }
};

    }
}

#endif

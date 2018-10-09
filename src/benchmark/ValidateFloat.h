#ifndef THORS_ANVIL_BENCHMARK_VALIDATE_FLOAT_H
#define THORS_ANVIL_BENCHMARK_VALIDATE_FLOAT_H

#include "TestSuite.h"
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>

namespace ThorsAnvil
{
    namespace Benchmark
    {

class ValidateFloat: public TestSuite
{
    public:
        using TestSuite::TestSuite;
        virtual std::string setupName(Test const&) override
        {
            return "vector-double";
        }
        virtual std::string getDir() const override
        {
            return "validate_float";
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
            double output;
            char *end;
            double expected = std::strtod(test.output.c_str(), &end);
            bool result = parser.ParseDouble(test.input.c_str(), &output);
            return !result ? NotImplemented : output == expected ? Pass: Fail;
        }
        virtual void generateConPerData(TestBase const& parser, Test const& test, State state) override
        {
            std::size_t begin   = test.path.str().rfind('/') + 1;
            std::size_t end     = test.path.str().rfind('.');
            std::string fileName = test.path.str().substr(begin, (end - begin));
            options.conformance << "2. Parse Double,"
                                << parser.GetName() << ","
                                << fileName << ","
                                << (state == Pass ? "true" : "false")
                                << "\n";
        }
};

    }
}

#endif

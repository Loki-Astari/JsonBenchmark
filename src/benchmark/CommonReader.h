#ifndef THORS_ANVIL_BENCHMARK_COMMON_READER_H
#define THORS_ANVIL_BENCHMARK_COMMON_READER_H

#include "benchmark.h"
#include <fstream>
#include <string>

namespace ThorsAnvil
{
    namespace Benchmark
    {

class CommonReader: public TestSuite
{
    public:
        using TestSuite::TestSuite;
        virtual std::string setupName(Test const& test) override
        {
            std::string const& path = test.path.str();
            std::size_t slash2      = path.rfind('/');
            std::size_t slash1      = path.rfind('/', slash2 - 1);

            return path.substr(slash1 + 1);
        }
        virtual void preloadData(Test& test) override
        {
            std::ifstream   input(test.path.str());
            input.seekg(0, std::ios_base::end);
            std::streampos  size = input.tellg();
            input.seekg(0, std::ios_base::beg);

            test.input.resize(size);
            input.read(&test.input[0], size);
        }
};

    }
}

#endif

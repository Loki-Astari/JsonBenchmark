#ifndef THORS_ANVIL_BENCHMARK_ROUNDTRIP_CHECKER_H
#define THORS_ANVIL_BENCHMARK_ROUNDTRIP_CHECKER_H

#include "CommonReader.h"
#include <string>
#include <memory>
#include <cctype>

namespace ThorsAnvil
{
    namespace Benchmark
    {

class RoundTripChecker: public CommonReader
{
    public:
        using CommonReader::CommonReader;
        virtual void preloadData(Test& test) override
        {
            CommonReader::preloadData(test);
            test.output = stripSpace(test.input);
        }
        virtual State executeTest(TestBase const& parser, Test const& test) override
        {
            std::unique_ptr<ParseResultBase> result(parser.Parse(test.input.c_str(), test.input.size()));
            if (!result)
            {
                return NotImplemented;
            }
            std::unique_ptr<StringResultBase> stringify(parser.Stringify(result.get()));
            std::string                       output;
            if (stringify)
            {
                output = stringify->c_str();
            }
            return test.output == stripSpace(output) ? Pass : Fail;
        }
    private:
        std::string stripSpace(std::string const from)
        {
            std::string result;
            std::copy_if(std::begin(from), std::end(from),
                         std::back_inserter(result),
                         [inString = false, lastChar = ' '](char next) mutable
                         {
                            if (next == '"' && lastChar != '\\')
                            {
                                inString    = !inString;
                            }
                            lastChar = next;
                            return !inString && std::isspace(next);
                         }
                        );
            return result;
        }
        virtual void generateConPerData(TestBase const& parser, Test const& test, State state) override
        {
            std::size_t begin   = test.path.str().rfind('/') + 1;
            std::size_t end     = test.path.str().rfind('.');
            std::string fileName = test.path.str().substr(begin, (end - begin));
            options.conformance << "4. Roundtrip,"
                                << parser.GetName() << ","
                                << fileName << ","
                                << (state == Pass ? "true" : "false")
                                << "\n";
        }
};

    }
}

#endif

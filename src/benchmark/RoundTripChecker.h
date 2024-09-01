#ifndef THORS_ANVIL_BENCHMARK_ROUNDTRIP_CHECKER_H
#define THORS_ANVIL_BENCHMARK_ROUNDTRIP_CHECKER_H

#include "CommonReader.h"
#include <string>
#include <memory>
#include <cctype>

extern bool debugJSONBenchmark;

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
            test.output = standardize(test.input);
        }
        virtual State executeTest(TestBase const& parser, Test const& test) override
        {
            std::unique_ptr<ParseResultBase> result;
            bool implemented = parser.Parse(test.input.c_str(), test.input.size(), result);
            if (!implemented) {
                return NotImplemented;
            }

            std::unique_ptr<StringResultBase> stringify;
            implemented = parser.Stringify(*result, stringify);
            if (!implemented) {
                return NotImplemented;
            }
            std::string                       output;
            if (stringify) {
                output = stringify->c_str();
            }
            if (debugJSONBenchmark) {
                std::cerr << "Expected >" << test.output << "<\n";
                std::cerr << "Stripped:>" << standardize(output) << "<\n";
            }
            return test.output == standardize(output) ? Pass : Fail;
        }
        virtual std::string getDir() const override
        {
            return "roundtrip";
        }
    private:
        std::string standardize(std::string const from)
        {
            // Strip meaningless white space.
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
                            return !(!inString && std::isspace(next));
                         }
                        );
            // The e for exponent. Don't care on the case.
            std::replace(std::begin(result), std::end(result), 'E', 'e');
            // Its just decorative and not a fatal to have a "+" in front of exponent.
            auto find = result.find("e+");
            if (find != std::string::npos) {
                result.replace(find, 2, "e");
            }
            // e0 is just decrative
            find = result.find("e0");
            if (find != std::string::npos) {
                if (result.size() <= find+2 || !isdigit(result[find+2])) {
                    result.replace(find, 2, "");
                }
            }
            // A value of .0 is decorative so remove.
            find = result.find(".0");
            if (find != std::string::npos) {
                if (result.size() <= find+2 || !isdigit(result[find+2])) {
                    result.replace(find, 2, "");
                }
            }

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

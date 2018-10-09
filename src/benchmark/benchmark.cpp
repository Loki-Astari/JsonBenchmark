#include "benchmarkConfig.h"
#include "TestSuite.h"
#include "ValidateString.h"
#include "ValidateFloat.h"
#include "PassChecker.h"
#include "FailChecker.h"
#include "RoundTripChecker.h"
#include "PerformanceChecker.h"

#include <map>
#include <regex>

namespace BM = ThorsAnvil::Benchmark;

using DirIter   = ThorsAnvil::FileSystem::DirectoryIterator;
using BenchList = std::map<std::string, BM::TestSuite*>;

BM::Options getOptions(int argc, char* argv[]);
void        displayOptions();
void        getBenchList(std::string const& testFilter, BenchList& benchList);
ParsrList   getParsrList(std::string const& parserFilter);

int main(int argc, char* argv[])
{
    BM::Options  options = getOptions(argc, argv);
    ParsrList    parsrList = getParsrList(options.parserFilter);

    BM::PassChecker         jsoncheckerPass(options, "jsonchecker_pass");
    BM::FailChecker         jsoncheckerFail(options, "jsonchecker_fail");
    BM::PerformanceChecker  performance(options, "performance");
    BM::RoundTripChecker    roundtrip(options, "roundtrip");
    BM::ValidateFloat       validate_float(options, "validate_float");
    BM::ValidateString      validate_string(options, "validate_string");

    BenchList   benchList   = { {"jsonchecker_pass",&jsoncheckerPass},
                                {"jsonchecker_fail",&jsoncheckerFail},
                                {"performance",     &performance},
                                {"roundtrip",       &roundtrip},
                                {"validate_float",  &validate_float},
                                {"validate_string", &validate_string}
                              };
    getBenchList(options.testFilter, benchList);
    options.conformance << "Type,Library,Test,Result\n";
    options.performance << "Type,Library,Filename,Time (ms),Memory (byte),MemoryPeak (byte),AllocCount,LeakedBytes,LeakCount,FileSize (byte)\n";


    jsoncheckerPass.executeTestOnAllParsers(parsrList);
    jsoncheckerFail.executeTestOnAllParsers(parsrList);
    validate_float.executeTestOnAllParsers(parsrList);
    validate_string.executeTestOnAllParsers(parsrList);
    roundtrip.executeTestOnAllParsers(parsrList);

    performance.executeTestOnAllParsers(parsrList);
}

BM::Options getOptions(int argc, char* argv[])
{
    BM::Options result;
    int loop = 1;
    for (; loop < argc; ++loop)
    {
        if (strncmp(argv[loop], "--filter=", 9) == 0)
        {
            result.testFilter = argv[loop] + 9;
        }
        else if (strncmp(argv[loop], "--parser=", 9) == 0)
        {
            result.parserFilter = argv[loop] + 9;
        }
        else if (strcmp(argv[loop], "--help") == 0)
        {
            displayOptions();
            exit(0);
        }
        else if (strcmp(argv[loop], "--") == 0)
        {
            break;
        }
        else
        {
            if (argv[loop][0] != '-')
            {
                break;
            }
            std::cerr << "Invalid option: " << argv[loop] << "\n";
            displayOptions();
            exit(1);
        }
    }
    if (loop + 2 != argc)
    {
        std::cerr << "Invalid Options: Need two output file names\n";
        displayOptions();
        exit(1);
    }
    result.conformance.open(argv[loop]);
    if (!result.conformance)
    {
        std::cerr << "Failed to open 'conformance file'\n";
        exit(1);
    }
    result.performance.open(argv[loop + 1]);
    if (!result.performance)
    {
        std::cerr << "Failed to open 'performance file'\n";
        exit(1);
    }
    return result;
}

void displayOptions()
{
#pragma vera-pushoff
    std::cout << R"(
benchmark [--filter=<filter>] [--parser=<parser>] [--help] <conformance file> <performance file>

    filter: Default value [^/]*/[^/]*
        This option is used as a regular expression to decide what tests to perform.
        The first part decides what family of tests jsonchecker/performance/roundtrip.
        The second part specifies a test name.

    parser:
        If unused this will will run all the parsers.
        If specified it will run the specified parser only. It choose the parser
        by comparing the value given against the result of GetName() method of each parser.

    conformance file:
        File conformance data is written to.

    performance file:
        File performance data is written to.
)";
#pragma vera-pop
}

void getBenchList(std::string const& testFilter, BenchList& benchList)
{
    for (auto const& dir: DirIter(QUOTE(DATA_DIR)))
    {
        if (!dir.isNormalDir())
        {   continue;
        }
        for (auto const& file: DirIter(dir.path()))
        {
            if (!file.isNormalFile())
            {   continue;
            }

            std::string testName = dir.name() + "/" + file.name();
            if (testName.find("EXCLUDE") != std::string::npos)
            {   continue;
            }
            std::regex  expression(testFilter + R"(\.json$)");
            if (!regex_search(testName, expression))
            {   continue;
            }

            auto find = benchList.find(dir.name());
            if (find != benchList.end())
            {
                find->second->emplace_back(file.path());
            }
        }
    }
}

ParsrList getParsrList(std::string const& parserFilter)
{
    ParsrList    result = TestManager::instance().getTests();
    if (parserFilter != "")
    {
        result.erase(std::remove_if(std::begin(result),
                                    std::end(result),
                                    [&parserFilter](TestBase const* parser){return parser->GetName() != parserFilter;}
                                   ),
                     std::end(result)
                    );
    }

    return result;
}

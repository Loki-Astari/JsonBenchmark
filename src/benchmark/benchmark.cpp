#include "benchmarkConfig.h"
#include "TestSuite.h"
#include "ValidateString.h"
#include "ValidateFloat.h"
#include "PassChecker.h"
#include "FailChecker.h"
#include "RoundTripChecker.h"
#include "PerformanceChecker.h"

#include <list>
#include <regex>

namespace BM = ThorsAnvil::Benchmark;

using DirIter   = ThorsAnvil::FileSystem::DirectoryIterator;
using TestSuiteList = std::list<BM::TestSuite*>;

BM::Options getOptions(int argc, char* argv[]);
void        displayOptions();
void        getTestSuiteList(std::string const& testFilter, TestSuiteList& tSuiteList);
ParsrList   getParsrList(std::string const& parserFilter);

int main(int argc, char* argv[])
{
    BM::Options  options = getOptions(argc, argv);
    if (!options.append)
    {
        options.conformance << "Type,Library,Test,Result\n";
        options.performance << "Type,Library,Filename,Time (ms),Memory (byte),MemoryPeak (byte),AllocCount,LeakedBytes,LeakCount,FileSize (byte)\n";
    }
    if (options.titleOnly)
    {
        // We are simply adding the title to the two files.
        return 0;
    }

    ParsrList    parsrList = getParsrList(options.parserFilter);

    BM::PassChecker         jsoncheckerPass(options);
    BM::FailChecker         jsoncheckerFail(options);
    BM::PerformanceChecker  performance(options);
    BM::RoundTripChecker    roundtrip(options);
    BM::ValidateFloat       validate_float(options);
    BM::ValidateString      validate_string(options);

    TestSuiteList tSuiteList = {&jsoncheckerPass,
                                &jsoncheckerFail,
                                &performance,
                                &roundtrip,
                                &validate_float,
                                &validate_string
                               };

    getTestSuiteList(options.testFilter, tSuiteList);

    if (options.displayOptionsOnly)
    {
        displayOptions();
        return 0;
    }
    if (options.listTest)
    {
        std::string  dataDir = QUOTE(DATA_DIR);
        std::size_t  dataDirSize = dataDir.size();
        if (dataDir[dataDirSize - 1] != '/')
        {
            ++dataDirSize;
        }

        for (auto const& test: tSuiteList)
        {
            for (auto const& item: *test)
            {
                std::string fileName = item.path.str();
                std::string testName = fileName.substr(dataDirSize, (fileName.size() - dataDirSize - 5 /*.json*/));
                std::cout << testName << " ";
            }
        }
        std::cout << "\n";
        return 0;
    }
    if (options.listParser)
    {
        for (auto const& parse: parsrList)
        {
            char const* name = parse->GetName();
            //std::size_t len  = strlen(name);
            std::cout << name << " ";
        }
        return 0;
    }

    for (auto const& test: tSuiteList)
    {
        test->executeTestOnAllParsers(parsrList, options);
    }
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
            result.displayOptionsOnly = true;
        }
        else if (strcmp(argv[loop], "--titleOnly") == 0)
        {
            result.titleOnly = true;
        }
        else if (strcmp(argv[loop], "--append") == 0)
        {
            result.append = true;
        }
        else if (strcmp(argv[loop], "--listTests") == 0)
        {
            result.listTest = true;
            result.useFiles = false;
        }
        else if (strcmp(argv[loop], "--listParser") == 0)
        {
            result.listParser = true;
            result.useFiles = false;
        }
        else if (strcmp(argv[loop], "--markFailed") == 0)
        {
            result.markFailed = true;
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
            result.displayOptionsOnly = true;
        }
    }
    if (result.useFiles)
    {
        if (loop + 2 != argc)
        {
            std::cerr << "Invalid Options: Need two output file names\n";
            result.displayOptionsOnly = true;
        }
        result.conformance.open(argv[loop], result.append ? std::ios::app : std::ios::out);
        if (!result.conformance)
        {
            std::cerr << "Failed to open 'conformance file'\n";
            result.displayOptionsOnly = true;
        }
        result.performance.open(argv[loop + 1], result.append ? std::ios::app : std::ios::out);
        if (!result.performance)
        {
            std::cerr << "Failed to open 'performance file'\n";
            result.displayOptionsOnly = true;
        }
    }
    return result;
}

void displayOptions()
{
#pragma vera-pushoff
    std::cout << R"(
benchmark [--filter=<filter>] [--parser=<parser>] [--help] [--titleOnly] [--append] [--listTests] [--listParser] <conformance file> <performance file>

    --markFailed:   Marks a test as failed.
                    This is called by the test harness after a crash to record the result.

    --titleOnly:    Clears the "<conformance file>" and "<performance file>" and adds the
                    the title line only to these files.
                    The application then exits.

    --append:       Will run the specified tests and parsers appending the reusult to the
                    "<conformance file>" and <performancce file>. Not with this options
                    the title line is not added to these file. You should run the application
                    onece with only `--titleOnly' to initiate the file then run as required
                    with this flag to get the results you need.

    --listTests:    Lists the set of tests that can be run.
                    These values can be used with the --filter= flag.

    -- filter=<type>/<test>
                    Default value all types and tests.
                    If this flag is used only the test specified by filter will be run.

    --listParser:   Lists the set of parsers that can be tested.
                    These values can be used with the --parser= flag.

    --parser=<parser>
                    Default all parsers.
                    If this flag is used only the specified parser will be run.

    conformance file:
        File conformance data is written to.

    performance file:
        File performance data is written to.
)";
#pragma vera-pop
}

void getTestSuiteList(std::string const& testFilter, TestSuiteList& suiteList)
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

            auto find = std::find_if(std::begin(suiteList), std::end(suiteList),
                                     [&dir](BM::TestSuite const* test){return test->getDir() == dir.name();}
                                    );
            if (find != std::end(suiteList))
            {
                (*find)->emplace_back(file.path());
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

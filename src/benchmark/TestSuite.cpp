#include "TestSuite.h"

using namespace ThorsAnvil::Benchmark;

extern bool debugJSONBenchmark;

Test::Test(FileSystem::Path const& path)
    : path(path)
{}

void Test::clear()
{
    std::string().swap(output);  // Shrink to default size
    std::string().swap(input);   // Shrink to default size
}

namespace ThorsAnvil
{
    namespace Benchmark
    {

std::ostream& operator<<(std::ostream& str, Test const& test)
{
    return str << test.path.str();
}
    }
}

TestSetUp::TestSetUp(TestBase const& parser, std::string const& name, bool callFuncs)
    : parser(parser)
    , name(name)
    , callFuncs(callFuncs)
{
    if (callFuncs)
    {   parser.SetUp(name.c_str());
    }
}

TestSetUp::~TestSetUp()
{
    if (callFuncs)
    {   parser.TearDown(name.c_str());
    }
}

TestSuite::TestSuite(Options& options)
    : options(options)
{}

void TestSuite::executeTestOnAllParsers(ParsrList const& parsrList, Options const& options)
{
    if (!tests.empty())
    {
        DataLoader  loadData(*this);

        for (auto const& parser: parsrList)
        {
            executeTest(*parser, options);
        }
    }
}

void TestSuite::executeTest(TestBase const& parser, Options const& options)
{
    if (options.debug) {
        printTestSuitName();
    }

    int count[3] = {0, 0, 0};
    std::vector<Test const*>  failed;
    std::vector<Test const*>  passed;

    for (auto const& test: tests)
    {
        State state = Fail;
        if (options.supported)
        {
            if (!options.markFailed )
            {
                TestSetUp   testSetUp(parser, setupName(test), useSetUp());
                state = executeTest(parser, options, test);
            }
        }
        else
        {
            state = NotImplemented;
        }
        generateConPerData(parser, test, state);
        ++count[static_cast<int>(state)];
        if (state == Fail)
        {
            failed.push_back(&test);
        }
        else
        {
            passed.push_back(&test);
        }
    }
    printResults(parser, count, passed, failed);
}

void TestSuite::printResults(TestBase const& parser, int (&count)[3], std::vector<Test const*>& passed, std::vector<Test const*>& failed)
{
    std::cerr << "\tParser: " << parser.GetName();
    if (count[0] == 0 && count[2] == 0)
    {
        if (passed.size() == 1)
        {
            std::cerr << "  Passed: " << *passed[0] << "\n";
        }
        else
        {
            std::cerr << "  Perfect\n";
        }
    }
    else
    {
        std::cerr << "\n";
        if (count[0] != 0)
        {
            std::cerr << "\t\tNot Implemented: " << count[0] << "\n";
        }
        std::cerr << "\t\tPass:            " << count[1] << "\n";
        std::cerr << "\t\tFail:            " << count[2] << "\n";
        for (auto const& fail: failed)
        {
            std::cerr << "\t\t\tFailed: " << *fail << "\n";
        }
    }
}

void TestSuite::preload()
{
    for (auto& test: tests)
    {
        preloadData(test);
    }
}

void TestSuite::clear()
{
    for (auto& test: tests)
    {
        test.clear();
    }
}

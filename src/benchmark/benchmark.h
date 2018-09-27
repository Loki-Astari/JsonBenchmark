#ifndef THORS_ANVIL_BENCHMARK_BENCHMARK_H
#define THORS_ANVIL_BENCHMARK_BENCHMARK_H

#include "filesystem.h"
#include "ThirdParty/test.h"
#include <vector>
#include <string>
#include <fstream>

namespace ThorsAnvil
{
    namespace Benchmark
    {

struct Options
{
    std::string     testFilter      =R"([^/]*/[^/]*)";
    std::string     parserFilter    = "";
    std::ofstream   conformance;
    std::ofstream   performance;
};

class Test
{
    public:
        FileSystem::Path    path;
        std::string         input;
        std::string         output;

        Test(FileSystem::Path const& path)
            : path(path)
        {}
        void clear()
        {
            std::string().swap(output);  // Shrink to default size
            std::string().swap(input);   // Shrink to default size
        }

        friend std::ostream& operator<<(std::ostream& str, Test const& test)
        {
            return str << test.path.str();
        }
};

enum State {NotImplemented, Pass, Fail};
class TestSetUp
{
    TestBase const& parser;
    std::string     name;
    bool            callFuncs;
    public:
        TestSetUp(TestBase const& parser, std::string const& name, bool callFuncs)
            : parser(parser)
            , name(name)
            , callFuncs(callFuncs)
        {
            if (callFuncs)
            {   parser.SetUp(name.c_str());
            }
        }
        ~TestSetUp()
        {
            if (callFuncs)
            {   parser.TearDown(name.c_str());
            }
        }
};
class TestSuite
{
    using Cont = std::vector<Test>;
    std::string bName;
    protected:
        Options&    options;
        Cont        tests;
    private:
        virtual std::string setupName(Test const&)                              = 0;
        virtual void preloadData(Test&)                                         = 0;
        virtual State executeTest(TestBase const&, Test const&)                 = 0;
        virtual void generateConPerData(TestBase const&, Test const&, State)    = 0;
        virtual std::string benchmarkName()                                     const   {return bName;}
        virtual bool useSetUp()                                                 const   {return true;}
        void preload()
        {
            for (auto& test: tests)
            {
                preloadData(test);
            }
        }
        void clear()
        {
            for (auto& test: tests)
            {
                test.clear();
            }
        }
        virtual void printResults(TestBase const& parser, int (&count)[3], std::vector<Test const*>& failed)
        {
            std::cerr << "\tParser: " << parser.GetName();
            if (count[0] == 0 && count[2] == 0)
            {
                std::cerr << "  Perfect\n";
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
        virtual void executeTest(TestBase const& parser)
        {
            int count[3] = {0, 0, 0};
            std::vector<Test const*>  failed;

            for (auto const& test: tests)
            {
                TestSetUp   testSetUp(parser, setupName(test), useSetUp());

                State state = executeTest(parser, test);
                generateConPerData(parser, test, state);
                ++count[static_cast<int>(state)];
                if (state == Fail)
                {
                    failed.push_back(&test);
                }
            }
            printResults(parser, count, failed);
        }

        struct DataLoader
        {
            TestSuite& benchmark;
            DataLoader(TestSuite& benchmark) : benchmark(benchmark) {benchmark.preload();}
           ~DataLoader()                                            {benchmark.clear();}
        };
    public:
        void executeTestOnAllParsers(ParsrList const& parsrList)
        {
            std::cerr << "BenchMark: " << benchmarkName() << "\n";
            if (!tests.empty())
            {
                DataLoader  loadData(*this);

                for (auto const& parser: parsrList)
                {
                    executeTest(*parser);
                }
            }
        }
        TestSuite(Options& options, std::string const& name)
            : bName(name)
            , options(options)
        {}

        using iterator = Cont::iterator;
        iterator begin() {return tests.begin();}
        iterator end()   {return tests.end();}

        void emplace_back(FileSystem::Path const& path)
        {
            tests.emplace_back(path);
        }
};

    }
}

#endif

#ifndef THORS_ANVIL_BENCHMARK_BENCHMARK_H
#define THORS_ANVIL_BENCHMARK_BENCHMARK_H

#include "filesystem.h"
#include "ThirdParty/test.h"
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

        Test(FileSystem::Path const& path);
        void clear();
        friend std::ostream& operator<<(std::ostream& str, Test const& test);
};

enum State {NotImplemented, Pass, Fail};
class TestSetUp
{
    TestBase const& parser;
    std::string     name;
    bool            callFuncs;
    public:
        TestSetUp(TestBase const& parser, std::string const& name, bool callFuncs);
        ~TestSetUp();
};

class TestSuite
{
    using Cont = std::vector<Test>;
    protected:
        Options&    options;
        Cont        tests;
    public:
        TestSuite(Options& options);
        void executeTestOnAllParsers(ParsrList const& parsrList, std::string const& dirName);
        virtual void executeTest(TestBase const& parser);
        virtual State executeTest(TestBase const& parser, Test const& test) = 0;

        /* Interface for the range based for() */
        using iterator = Cont::iterator;
        iterator begin()                                            {return tests.begin();}
        iterator end()                                              {return tests.end();}
        void     emplace_back(FileSystem::Path const& path)         {tests.emplace_back(path);}
    private:
        struct DataLoader
        {
            TestSuite& benchmark;
            DataLoader(TestSuite& benchmark):benchmark(benchmark)   {benchmark.preload();}
           ~DataLoader()                                            {benchmark.clear();}
        };

        void preload();
        void clear();

        /* Used by preload() */
        virtual void preloadData(Test&) = 0;

        /* used in executeTest() to init TestSetUp() */
        virtual std::string setupName(Test const&)                  {return "";}
        virtual bool useSetUp() const                               {return true;}

        /* used executeTest() */
        virtual void generateConPerData(TestBase const& parser, Test const& test, State state) = 0;
        virtual void printResults(TestBase const& parser, int (&count)[3], std::vector<Test const*>& failed);

};

    }
}

#endif

#ifndef THORS_ANVIL_BENCHMARK_BENCHMARK_H
#define THORS_ANVIL_BENCHMARK_BENCHMARK_H

#include "filesystem.h"
#include "../ThirdParty/test.h"
#include "../ThirdParty/TestManager.h"
#include <string>
#include <vector>
#include <fstream>

namespace ThorsAnvil
{
    namespace Benchmark
    {

struct Options
{
    std::string     testFilter          =R"([^/]*/[^/]*)";
    std::string     parserFilter        = "";
    std::ofstream   conformance;
    std::ofstream   performance;
    bool            displayOptionsOnly  = false;
    bool            titleOnly           = false;
    bool            append              = false;
    bool            listTest            = false;
    bool            listParser          = false;
    bool            markFailed          = false;
    bool            parserType          = false;
    bool            useFiles            = true;
    bool            debug               = false;
    mutable bool    supported           = true;
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
        void executeTestOnAllParsers(ParsrList const& parsrList, Options const& options);
        virtual void executeTest(TestBase const& parser, Options const& options);
        virtual State executeTest(TestBase const& parser, Options const& options, Test const& test) = 0;

        /* Interface for the range based for() */
        using iterator          = Cont::iterator;
        using const_iterator    = Cont::const_iterator;
        iterator begin()                                            {return tests.begin();}
        iterator end()                                              {return tests.end();}
        const_iterator begin()  const                               {return tests.cbegin();}
        const_iterator end()    const                               {return tests.cend();}
        void     emplace_back(FileSystem::Path const& path)         {tests.emplace_back(path);}

        virtual std::string getDir() const = 0;
    private:
        struct DataLoader
        {
            TestSuite& benchmark;
            DataLoader(TestSuite& benchmark):benchmark(benchmark)   {benchmark.preload();}
           ~DataLoader()                                            {benchmark.clear();}
        };

        void preload();
        void clear();
        virtual void printTestSuitName() = 0;

        /* Used by preload() */
        virtual void preloadData(Test&) = 0;

        /* used in executeTest() to init TestSetUp() */
        virtual std::string setupName(Test const&) = 0;
        virtual bool useSetUp() const                               {return true;}

        /* used executeTest() */
        virtual void generateConPerData(TestBase const& parser, Test const& test, State state) = 0;
        virtual void printResults(TestBase const& parser, int (&count)[3], std::vector<Test const*>& passed, std::vector<Test const*>& failed);

};

    }
}

#endif

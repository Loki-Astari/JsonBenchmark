#ifndef THORS_ANVIL_BENCHMARK_PERFORMANCE_CHECKER_H
#define THORS_ANVIL_BENCHMARK_PERFORMANCE_CHECKER_H

#include "CommonReader.h"
#include <map>
#include <chrono>

namespace ThorsAnvil
{
    namespace Benchmark
    {

class PerformanceChecker: public CommonReader
{
    static std::map<std::string, Stat const*> const validator;
    static const int loopCount = 10;
    public:
        using CommonReader::CommonReader;
        virtual void executeTest(TestBase const& parser, Options const& options) override;
        virtual State executeTest(TestBase const& parser, Test const& test) override;

        virtual std::string getDir() const override
        {
            return "performance";
        }
    private:
        virtual bool useSetUp() const override {return false;}

        virtual void generateConPerData(TestBase const& parser, Test const& test, State state) override;
        virtual void printResults(TestBase const&, int (&)[3], std::vector<Test const*>&, std::vector<Test const*>&) override{}
    private:
        void getCodeSize(TestBase const& parser);
        void validatePerformance(TestBase const& parser);

        class Output
        {
            Options&            options;
            TestBase const&     parser;
            Test const&         test;
            std::string const   name;
            std::string const   action;
            double&             minDuration;
            bool                fail;
            public:
                Output(Options& options, TestBase const& parser_, Test const& test_, std::string const& name_, std::string const& action_, double& minDuration_);
                ~Output();
                void setPass();
                void validateMemory();
                void readoutput(int fd, std::ostream& output);
        };


        template<typename F>
        double timeExecution(F&& action);
        void executeParse(TestBase const& parser, Test const& test);
        void executeStringify(TestBase const& parser, Test const& test);
        void executePrettify(TestBase const& parser, Test const& test);
        void executeStatistics(TestBase const& parser, Test const& test);
        void executeSaxRoundtrip(TestBase const& parser, Test const& test);
        void executeSaxStatistics(TestBase const& parser, Test const& test);
};

template<typename F>
double PerformanceChecker::timeExecution(F&& action)
{
    auto start = std::chrono::high_resolution_clock::now();
    action();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end-start;
    return duration.count();
}

    }
}

#endif

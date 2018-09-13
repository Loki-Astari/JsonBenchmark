#ifndef THORS_ANVIL_BENCHMARK_PERFORMANCE_CHECKER_H
#define THORS_ANVIL_BENCHMARK_PERFORMANCE_CHECKER_H

#include "CommonReader.h"
#include <memory>
#include <map>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

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
        virtual void printResults(TestBase const&, int (&)[3], std::vector<Test const*>&) override{}
        virtual bool useSetUp() const override {return false;}
        virtual void executeTest(TestBase const& parser) override
        {
            std::cerr << "\t" << parser.GetName() << "\n";
            validatePerformance(parser);
            CommonReader::executeTest(parser);
            getCodeSize(parser);
        }
        virtual State executeTest(TestBase const& parser, Test const& test) override
        {
            executeParse(parser, test);
            executeStringify(parser, test);
            executePrettify(parser, test);
            executeStatistics(parser, test);
            executeSaxRoundtrip(parser, test);
            executeSaxStatistics(parser, test);
            return Pass;
        }
        virtual void generateConPerData(TestBase const& /*parser*/, Test const& /*test*/, State /*state*/) override
        {}
    private:
        void getCodeSize(TestBase const& parser)
        {
            std::string  path       = QUOTE(THORSANVIL_ROOT) "/build/lib/";
            std::string  fileName   = std::string(parser.GetName());
            std::size_t  endOfName  = fileName.find(' ');

            fileName = fileName.substr(0, endOfName);
            std::string  fullPath = path + "lib" + fileName + "Test" + QUOTE(LIBEXT);

            std::ifstream   obj(fullPath);
            obj.seekg(0, std::ios_base::end);
            std::size_t     size = obj.tellg();

            options.performance << "7. Code size," << parser.GetName() << ",XXX,0,0,0,0,0,0," << size << "\n";
        }
        void validatePerformance(TestBase const& parser)
        {
            for (auto const& test: tests)
            {
                std::string fileName = test.path.str().substr(test.path.str().rfind('/') + 1);
                TestSetUp   testSetUp(parser, setupName(test), true);
                std::unique_ptr<ParseResultBase> dom(parser.Parse(test.input.c_str(), test.input.size()));

                Stat    stats {};
                bool    result = parser.Statistics(dom.get(), &stats);
                std::cerr << "\t\t" << std::setw(15) << std::left << "Validate:"
                          << std::setw(20) << std::left << fileName
                          << std::right << std::flush;
                if (!result)
                {
                    std::cerr << " Not Implemented\n";
                }
                else
                {
                    auto find = validator.find(fileName);
                    if (find == validator.end())
                    {
                        std::cerr << " New Test! No comparison\n";
                        continue;
                    }
                    bool ok = stats == *find->second;
                    std::cerr << (ok ? " Pass" : " Fail") << "\n";
                    if (!ok)
                    {
                        std::cerr << "\t\t\tobjectCount  " << std::setw(8) << find->second->objectCount  << " : " << std::setw(8) << stats.objectCount  << (find->second->objectCount  == stats.objectCount ? "" : "  BAD") << "\n"
                                  << "\t\t\tarrayCount   " << std::setw(8) << find->second->arrayCount   << " : " << std::setw(8) << stats.arrayCount   << (find->second->arrayCount   == stats.arrayCount  ? "" : "  BAD") << "\n"
                                  << "\t\t\tnumberCount  " << std::setw(8) << find->second->numberCount  << " : " << std::setw(8) << stats.numberCount  << (find->second->numberCount  == stats.numberCount ? "" : "  BAD") << "\n"
                                  << "\t\t\tstringCount  " << std::setw(8) << find->second->stringCount  << " : " << std::setw(8) << stats.stringCount  << (find->second->stringCount  == stats.stringCount ? "" : "  BAD") << "\n"
                                  << "\t\t\ttrueCount    " << std::setw(8) << find->second->trueCount    << " : " << std::setw(8) << stats.trueCount    << (find->second->trueCount    == stats.trueCount   ? "" : "  BAD") << "\n"
                                  << "\t\t\tfalseCount   " << std::setw(8) << find->second->falseCount   << " : " << std::setw(8) << stats.falseCount   << (find->second->falseCount   == stats.falseCount  ? "" : "  BAD") << "\n"
                                  << "\t\t\tnullCount    " << std::setw(8) << find->second->nullCount    << " : " << std::setw(8) << stats.nullCount    << (find->second->nullCount    == stats.nullCount   ? "" : "  BAD") << "\n"
                                  << "\t\t\tmemberCount  " << std::setw(8) << find->second->memberCount  << " : " << std::setw(8) << stats.memberCount  << (find->second->memberCount  == stats.memberCount ? "" : "  BAD") << "\n"
                                  << "\t\t\telementCount " << std::setw(8) << find->second->elementCount << " : " << std::setw(8) << stats.elementCount << (find->second->elementCount == stats.elementCount? "" : "  BAD") << "\n"
                                  << "\t\t\tstringLength " << std::setw(8) << find->second->stringLength << " : " << std::setw(8) << stats.stringLength << (find->second->stringLength == stats.stringLength? "" : "  BAD") << "\n";
                    }
                }
            }
        }
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
                Output(Options& options, TestBase const& parser_, Test const& test_, std::string const& name_, std::string const& action_, double& minDuration_)
                    : options(options)
                    , parser(parser_)
                    , test(test_)
                    , name(name_)
                    , action(action_)
                    , minDuration(minDuration_)
                    , fail(true)
                {
                    ((void)parser);
                    std::string fileName = test.path.str().substr(test.path.str().rfind('/') + 1);
                    std::cerr << "\t\t" << std::setw(15) << std::left << name
                              << std::setw(20) << std::left << fileName << " ... "
                              << std::right << std::flush;
                }
                ~Output()
                {
                    if (!fail)
                    {
                        double throughput = test.input.size() / (1024.0 * 1024.0) / minDuration;
                        std::cerr << std::setw(6) << std::setprecision(3) << (minDuration * 1000) << " ms  "
                                  << std::setw(3) << std::setprecision(3) << throughput << " MB/s\n"
                                  << std::flush;
                        options.performance << action << ". "
                                            << name << ","
                                            << parser.GetName() << ","
                                            << test.path.str().substr(test.path.str().rfind('/') + 1) << ","
                                            << (minDuration * 1000) << ",";
                        validateMemory();
                        options.performance << "0\n";
                    }
                    else
                    {
                        std::cerr << "\t\tNot support\n" << std::flush;
                    }
                }
                void setPass() {fail = false;}
                void validateMemory()
                {
                    int stdoutPipe[2];
                    int stderrPipe[2];

                    if (pipe(stdoutPipe) < 0)
                    {
                        std::cerr << "Failed to Create Pipe: Stdout";
                        exit(1);
                    }
                    if (pipe(stderrPipe) < 0)
                    {
                        std::cerr << "Failed to Create Pipe: Stderr";
                        exit(1);
                    }

                    pid_t pid = fork();
                    if (pid < 0)
                    {
                        std::cerr << "Failed to Fork Memory App";
                        exit(1);
                    }
                    if (pid == 0)
                    {
                        dup2(stdoutPipe[1], fileno(stdout));
                        dup2(stderrPipe[1], fileno(stderr));
                        close(stdoutPipe[0]);
                        close(stdoutPipe[1]);
                        close(stderrPipe[0]);
                        close(stderrPipe[1]);
                        std::string fileName = test.path.str().substr(test.path.str().rfind('/') + 1);
                        std::string pName    = parser.GetName();
                        std::string memoryApp= QUOTE(THORSANVIL_ROOT) "/build/bin/memory17";

                        setenv("DYLD_LIBRARY_PATH", QUOTE(THORSANVIL_ROOT) "/build/lib/", 1);
                        setenv("DYLD_INSERT_LIBRARIES", QUOTE(THORSANVIL_ROOT) "/build/lib/libMemory17.dylib", 1);
                        setenv("DYLD_FORCE_FLAT_NAMESPACE", "1", 1);

                        execl(memoryApp.c_str(), memoryApp.c_str(), pName.c_str(), fileName.c_str(), action.c_str(), nullptr);//, envp);
                        exit(0);
                    }
                    close(stdoutPipe[1]);
                    close(stderrPipe[1]);

                    readoutput(stdoutPipe[0], std::cout);
                    readoutput(stderrPipe[0], options.performance);
                    int stat_loc;
                    pid_t child = wait4(pid, &stat_loc, 0, nullptr);
                    if (child != pid)
                    {
                        std::cerr << "Processes wait error\n";
                        exit(1);
                    }
                    close(stdoutPipe[0]);
                    close(stderrPipe[0]);
                }
                void readoutput(int fd, std::ostream& output)
                {
                    char buffer[4096];
                    int nbytes;

                    while ((nbytes = ::read(fd, buffer, sizeof(buffer))) != 0)
                    {
                        if (nbytes == -1)
                        {
                            switch (errno)
                            {
                                case EAGAIN:    continue;
                                case EINTR:     continue;

                                case EBADF:     std::cerr << "EBADF\n";     break;
                                case EFAULT:    std::cerr << "EFAULT\n";    break;
                                case EINVAL:    std::cerr << "EINVAL\n";    break;
                                case EIO:       std::cerr << "EIO\n";       break;
                                case EISDIR:    std::cerr << "EISDIR\n";    break;
                                case ENOBUFS:   std::cerr << "ENOBUFS\n";   break;
                                case ENOMEM:    std::cerr << "ENOMEM\n";    break;
                                case ENXIO:     std::cerr << "ENXIO\n";     break;
                                case ESPIPE:    std::cerr << "ESPIPE\n";    break;
                                case ECONNRESET:std::cerr << "ECONNRESET\n";break;
                                case ENOTCONN:  std::cerr << "ENOTCONN\n";  break;
                                case ETIMEDOUT: std::cerr << "ETIMEDOUT\n"; break;
                                default:
                                    std::cerr << "Unknown: " << errno << "\n";
                                    break;
                            }
                            std::cerr << "Read from socket failed\n";
                            exit(1);
                        }
                        output.write(buffer, nbytes);
                    }
                }
        };
        template<typename F>
        double timeExecution(F&& action)
        {
            auto start = std::chrono::high_resolution_clock::now();
            action();
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end-start;
            return duration.count();
        }
        void executeParse(TestBase const& parser, Test const& test)
        {
            double minDuration = std::chrono::duration<double>::max().count();
            Output generator(options, parser, test, "Parse", "1", minDuration);

            for (int loop = 0; loop < loopCount; ++loop)
            {
                TestSetUp   testSetUp(parser, setupName(test), true);
                std::unique_ptr<ParseResultBase> result;
                double duration = timeExecution([&parser, &test, &result]()
                    { result.reset(parser.Parse(test.input.c_str(), test.input.size()));});
                if (!result)
                {
                    return;
                }
                minDuration = std::min(minDuration, duration);
            }
            generator.setPass();
        }
        void executeStringify(TestBase const& parser, Test const& test)
        {
            double minDuration = std::chrono::duration<double>::max().count();
            Output generator(options, parser, test, "Stringify", "2", minDuration);

            TestSetUp   testSetUp(parser, setupName(test), true);
            std::unique_ptr<ParseResultBase> dom(parser.Parse(test.input.c_str(), test.input.size()));
            for (int loop = 0; loop < loopCount; ++loop)
            {
                std::unique_ptr<StringResultBase> result;
                double duration = timeExecution([&parser, &result, &dom]()
                    { result.reset(parser.Stringify(dom.get()));});
                if (!result)
                {
                    return;
                }
                minDuration = std::min(minDuration, duration);
            }
            generator.setPass();
        }
        void executePrettify(TestBase const& parser, Test const& test)
        {
            double minDuration = std::chrono::duration<double>::max().count();
            Output generator(options, parser, test, "Prettify", "3", minDuration);

            TestSetUp   testSetUp(parser, setupName(test), true);
            std::unique_ptr<ParseResultBase> dom(parser.Parse(test.input.c_str(), test.input.size()));
            for (int loop = 0; loop < loopCount; ++loop)
            {
                std::unique_ptr<StringResultBase> result;
                double duration = timeExecution([&parser, &result, &dom]()
                    { result.reset(parser.Prettify(dom.get()));});
                if (!result)
                {
                    return;
                }
                minDuration = std::min(minDuration, duration);
            }
            generator.setPass();
        }
        void executeStatistics(TestBase const& parser, Test const& test)
        {
            double minDuration = std::chrono::duration<double>::max().count();
            Output generator(options, parser, test, "Statistics", "4", minDuration);

            TestSetUp   testSetUp(parser, setupName(test), true);
            std::unique_ptr<ParseResultBase> dom(parser.Parse(test.input.c_str(), test.input.size()));
            for (int loop = 0; loop < loopCount; ++loop)
            {
                Stat    stats {};
                bool    result;
                double duration = timeExecution([&parser, &result, &stats, &dom]()
                    { result = parser.Statistics(dom.get(), &stats);});
                if (!result)
                {
                    return;
                }
                minDuration = std::min(minDuration, duration);
            }
            generator.setPass();
        }
        void executeSaxRoundtrip(TestBase const& parser, Test const& test)
        {
            double minDuration = std::chrono::duration<double>::max().count();
            Output generator(options, parser, test, "SaxRoundtrip", "5", minDuration);

            for (int loop = 0; loop < loopCount; ++loop)
            {
                TestSetUp   testSetUp(parser, setupName(test), true);
                std::unique_ptr<StringResultBase> result;
                double duration = timeExecution([&parser, &test, &result]()
                    { result.reset(parser.SaxRoundtrip(test.input.c_str(), test.input.size()));});
                if (!result)
                {
                    return;
                }
                minDuration = std::min(minDuration, duration);
            }
            generator.setPass();
        }
        void executeSaxStatistics(TestBase const& parser, Test const& test)
        {
            double minDuration = std::chrono::duration<double>::max().count();
            Output generator(options, parser, test, "SaxStatistics", "6", minDuration);

            for (int loop = 0; loop < loopCount; ++loop)
            {
                TestSetUp   testSetUp(parser, setupName(test), true);
                Stat        stats {};
                bool        result;
                double duration = timeExecution([&parser, &test, &result, &stats]()
                    { result = parser.SaxStatistics(test.input.c_str(), test.input.size(), &stats);});
                if (!result)
                {
                    return;
                }
                minDuration = std::min(minDuration, duration);
            }
            generator.setPass();
        }
};

    }
}

#endif

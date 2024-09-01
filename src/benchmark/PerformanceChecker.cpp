#include "PerformanceChecker.h"

#include <memory>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

using namespace ThorsAnvil::Benchmark;

#pragma vera-pushoff
using namespace std::string_literals;
#pragma vera-pop
static Stat constexpr canada =
{
    /*objectCount:*/           4,
    /*arrayCount:*/        56045,
    /*numberCount:*/      111126,
    /*stringCount:*/          12,
    /*trueCount:*/             0,
    /*falseCount:*/            0,
    /*nullCount:*/             0,
    /*memberCount:*/           8,
    /*elementCount:*/     167170,
    /*stringLength:*/         90
};
static Stat constexpr citm_catalog
{
    /*objectCount:*/       10937,
    /*arrayCount:*/        10451,
    /*numberCount:*/       14392,
    /*stringCount:*/       26604,
    /*trueCount:*/             0,
    /*falseCount:*/            0,
    /*nullCount:*/          1263,
    /*memberCou:nt*/       25869,
    /*elementCount:*/      11908,
    /*stringLength:*/     221379
};
static Stat constexpr twitter =
{
    /*objectCount:*/        1264,
    /*arrayCount:*/         1050,
    /*numberCount:*/        2109,
    /*stringCount:*/       18099,
    /*trueCount:*/           345,
    /*falseCount:*/         2446,
    /*nullCount:*/          1946,
    /*memberCou:nt*/       13345,
    /*elementCount:*/        568,
    /*stringLength:*/     367917
};
std::map<std::string, Stat const*> const PerformanceChecker::validator
{
    {"canada.json"s,         &canada},
    {"citm_catalog.json"s,   &citm_catalog},
    {"twitter.json"s,        &twitter}
};

void PerformanceChecker::executeTest(TestBase const& parser, Options const& options)
{
    if (!options.markFailed)
    {
        validatePerformance(parser);
    }
    CommonReader::executeTest(parser, options);
}

State PerformanceChecker::executeTest(TestBase const& parser, Test const& test)
{
    if (test.path.str().find("size.json") == std::string::npos)
    {
        executeParse(parser, test);
        executeStringify(parser, test);
        executePrettify(parser, test);
        executeStatistics(parser, test);
        executeSaxRoundtrip(parser, test);
        executeSaxStatistics(parser, test);
    }
    else
    {
        getCodeSize(parser);
    }
    return Pass;
}

void PerformanceChecker::generateConPerData(TestBase const& /*parser*/, Test const& /*test*/, State /*state*/)
{}

void PerformanceChecker::getCodeSize(TestBase const& parser)
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

void PerformanceChecker::validatePerformance(TestBase const& parser)
{
    for (auto const& test: tests)
    {
        std::string fileName = test.path.str().substr(test.path.str().rfind('/') + 1);
        TestSetUp   testSetUp(parser, setupName(test), true);
        std::unique_ptr<ParseResultBase> dom;

        std::cerr << "\t\t" << std::setw(15) << std::left << "Validate:"
                  << std::setw(20) << std::left << fileName
                  << std::right << std::flush;

        bool implemented = parser.Parse(test.input.c_str(), test.input.size(), dom);

        if (!implemented) {
            std::cerr << " Not Implemented\n";
            continue;
        }

        Stat    stats {};

        implemented = parser.Statistics(*dom, stats);
        if (implemented) {
            std::cerr << " Not Implemented\n";
            continue;
        }

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

void PerformanceChecker::executeParse(TestBase const& parser, Test const& test)
{
    double minDuration = std::chrono::duration<double>::max().count();
    Output generator(options, parser, test, "Parse", "1", minDuration);

    for (int loop = 0; loop < loopCount; ++loop)
    {
        TestSetUp   testSetUp(parser, setupName(test), true);
        std::unique_ptr<ParseResultBase> result;
        bool                             implemented;
        double duration = timeExecution([&parser, &test, &result, &implemented]()
            {
                implemented = parser.Parse(test.input.c_str(), test.input.size(), result);
            }
        );
        if (!implemented) {
            return;
        }
        minDuration = std::min(minDuration, duration);
    }
    generator.setPass();
}

void PerformanceChecker::executeStringify(TestBase const& parser, Test const& test)
{
    double minDuration = std::chrono::duration<double>::max().count();
    Output generator(options, parser, test, "Stringify", "2", minDuration);

    TestSetUp   testSetUp(parser, setupName(test), true);
    std::unique_ptr<ParseResultBase> dom;
    bool implemented = parser.Parse(test.input.c_str(), test.input.size(), dom);
    if (!implemented) {
        return;
    }
    for (int loop = 0; loop < loopCount; ++loop)
    {
        std::unique_ptr<StringResultBase> result;
        bool                              implemented;
        double duration = timeExecution([&parser, &result, &implemented, &dom]()
            {
                implemented = parser.Stringify(*dom, result);
            }
        );
        if (!implemented) {
            return;
        }
        minDuration = std::min(minDuration, duration);
    }
    generator.setPass();
}

void PerformanceChecker::executePrettify(TestBase const& parser, Test const& test)
{
    double minDuration = std::chrono::duration<double>::max().count();
    Output generator(options, parser, test, "Prettify", "3", minDuration);

    TestSetUp   testSetUp(parser, setupName(test), true);
    std::unique_ptr<ParseResultBase> dom;
    bool implemented = parser.Parse(test.input.c_str(), test.input.size(), dom);
    if (!implemented) {
        return;
    }
    for (int loop = 0; loop < loopCount; ++loop)
    {
        std::unique_ptr<StringResultBase> result;
        bool                              implemented;
        double duration = timeExecution([&parser, &result, &implemented, &dom]()
            {
                implemented = parser.Prettify(*dom, result);
            }
        );
        if (!implemented) {
            return;
        }
        minDuration = std::min(minDuration, duration);
    }
    generator.setPass();
}

void PerformanceChecker::executeStatistics(TestBase const& parser, Test const& test)
{
    double minDuration = std::chrono::duration<double>::max().count();
    Output generator(options, parser, test, "Statistics", "4", minDuration);

    TestSetUp   testSetUp(parser, setupName(test), true);
    std::unique_ptr<ParseResultBase> dom;
    bool    implemented = parser.Parse(test.input.c_str(), test.input.size(), dom);
    if (!implemented) {
        return;
    }
    for (int loop = 0; loop < loopCount; ++loop)
    {
        Stat    stats {};
        bool    implemented;
        double duration = timeExecution([&parser, &implemented, &stats, &dom]()
            {
                implemented = parser.Statistics(*dom, stats);
            }
        );
        if (!implemented) {
            return;
        }
        minDuration = std::min(minDuration, duration);
    }
    generator.setPass();
}

void PerformanceChecker::executeSaxRoundtrip(TestBase const& parser, Test const& test)
{
    double minDuration = std::chrono::duration<double>::max().count();
    Output generator(options, parser, test, "SaxRoundtrip", "5", minDuration);

    for (int loop = 0; loop < loopCount; ++loop)
    {
        TestSetUp   testSetUp(parser, setupName(test), true);
        std::unique_ptr<StringResultBase> result;
        bool                              implemented;
        double duration = timeExecution([&parser, &test, &result, &implemented]()
            {
                implemented = parser.SaxRoundtrip(test.input.c_str(), test.input.size(), result);
            }
        );
        if (!implemented) {
            return;
        }
        minDuration = std::min(minDuration, duration);
    }
    generator.setPass();
}

void PerformanceChecker::executeSaxStatistics(TestBase const& parser, Test const& test)
{
    double minDuration = std::chrono::duration<double>::max().count();
    Output generator(options, parser, test, "SaxStatistics", "6", minDuration);

    for (int loop = 0; loop < loopCount; ++loop)
    {
        TestSetUp   testSetUp(parser, setupName(test), true);
        Stat        stats {};
        bool        implemented;
        double duration = timeExecution([&parser, &test, &implemented, &stats]()
            {
                implemented = parser.SaxStatistics(test.input.c_str(), test.input.size(), stats);
            }
        );
        if (!implemented) {
            return;
        }
        minDuration = std::min(minDuration, duration);
    }
    generator.setPass();
}

PerformanceChecker::Output::Output(Options& options, TestBase const& parser_, Test const& test_, std::string const& name_, std::string const& action_, double& minDuration_)
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

PerformanceChecker::Output::~Output()
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

void PerformanceChecker::Output::setPass() {fail = false;}

void PerformanceChecker::Output::validateMemory()
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
        std::string memoryApp= QUOTE(THORSANVIL_ROOT) "/build/bin/memory";

        setenv("DYLD_LIBRARY_PATH", QUOTE(THORSANVIL_ROOT) "/build/lib/:/opt/lib:/opt/homebrew/lib", 1);
        setenv("DYLD_INSERT_LIBRARIES", QUOTE(THORSANVIL_ROOT) "/build/lib/libMemory.dylib", 1);
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

void PerformanceChecker::Output::readoutput(int fd, std::ostream& output)
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

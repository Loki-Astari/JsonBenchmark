#include "PerformanceChecker.h"

#include <memory>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

using namespace ThorsAnvil::Benchmark;

void PerformanceChecker::executeTest(TestBase const& parser, Options const& options)
{
    if (!options.markFailed) {
        options.supported = validatePerformance(parser);
    }
    CommonReader::executeTest(parser, options);
}

State PerformanceChecker::executeTest(TestBase const& parser, Options const&, Test const& test)
{
    if (options.markFailed) {
        return Fail;
    }
    if (options.validate)
    {
        std::cerr << std::setprecision(17);
        executeStringify(parser, test);
        return Pass;
    }
    if (test.path.str().find("size.json") == std::string::npos)
    {
        executeParse(parser, test);
        executeStringify(parser, test);
        executePrettify(parser, test);
        executeSaxRoundtrip(parser, test);
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

bool PerformanceChecker::validatePerformance(TestBase const& parser)
{
    auto const& test = *(std::begin(tests));

    std::string fileName = test.path.str().substr(test.path.str().rfind('/') + 1);
    TestSetUp   testSetUp(parser, setupName(test), true);

    std::unique_ptr<ParseResultBase>    dom;
    std::unique_ptr<StringResultBase>   result;
    bool implemented =  true;

    implemented = implemented && parser.Parse(test.input.c_str(), test.input.size(), dom);
    implemented = implemented && (dom.get() != nullptr);
    implemented = implemented && parser.Stringify(*dom, result);
    implemented = implemented && parser.Prettify(*dom, result);
    implemented = implemented && parser.SaxRoundtrip(test.input.c_str(), test.input.size(), result);

    return implemented;
}

void PerformanceChecker::executeParse(TestBase const& parser, Test const& test)
{
    double minDuration = 0;
    Output generator(options, parser, test, "Parse", "1", minDuration);

    TestSetUp   testSetUp(parser, setupName(test), true);
    for (int loop = 0; loop < loopCount; ++loop)
    {
        bool    result;
        double duration = timeExecution([&parser, &test, &result]()
            {
                parser.ParseValidate(test.input.c_str(), test.input.size(), result);
            }
        );
        minDuration += duration;
    }
    minDuration /= loopCount;
    generator.setPass();
}

void PerformanceChecker::executeStringify(TestBase const& parser, Test const& test)
{
    double minDuration = 0;
    Output generator(options, parser, test, "Stringify", "2", minDuration);

    TestSetUp   testSetUp(parser, setupName(test), true);
    std::unique_ptr<ParseResultBase> dom;
    parser.Parse(test.input.c_str(), test.input.size(), dom);
    if (dom.get() == nullptr) {
        return;
    }
    for (int loop = 0; loop < loopCount; ++loop)
    {
        std::unique_ptr<StringResultBase> result;
        double duration = timeExecution([&parser, &result, &dom]()
            {
                parser.Stringify(*dom, result);
            }
        );
        minDuration += duration;
        if (options.debug) {
            std::cerr << "Got >" << result->c_str() << "<\n";
        }
        if (options.validate) {
            std::cerr << result->c_str();
            break;
        }
    }
    minDuration /= loopCount;
    generator.setPass();
}

void PerformanceChecker::executePrettify(TestBase const& parser, Test const& test)
{
    double minDuration = 0;
    Output generator(options, parser, test, "Prettify", "3", minDuration);

    TestSetUp   testSetUp(parser, setupName(test), true);
    std::unique_ptr<ParseResultBase> dom;
    parser.Parse(test.input.c_str(), test.input.size(), dom);
    if (dom.get() == nullptr) {
        return;
    }
    for (int loop = 0; loop < loopCount; ++loop)
    {
        std::unique_ptr<StringResultBase> result;
        double duration = timeExecution([&parser, &result, &dom]()
            {
                parser.Prettify(*dom, result);
            }
        );
        minDuration += duration;
        if (options.debug) {
            std::cerr << "Got >" << result->c_str() << "<\n";
        }
    }
    minDuration /= loopCount;
    generator.setPass();
}

void PerformanceChecker::executeSaxRoundtrip(TestBase const& parser, Test const& test)
{
    double minDuration = 0;
    Output generator(options, parser, test, "SaxRoundtrip", "5", minDuration);

    TestSetUp   testSetUp(parser, setupName(test), true);
    for (int loop = 0; loop < loopCount; ++loop)
    {
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
        minDuration += duration;
        if (options.debug) {
            std::cerr << "Got >" << result->c_str() << "<\n";
        }
    }
    minDuration /= loopCount;
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
    if (options.validate) {
        return;
    }
    ((void)parser);
    std::string fileName = test.path.str().substr(test.path.str().rfind('/') + 1);
    std::cerr << "\t\t" << std::setw(15) << std::left << name
              << std::setw(20) << std::left << fileName << " ... "
              << std::right << std::flush;
}

PerformanceChecker::Output::~Output()
{
    if (options.validate) {
        return;
    }
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

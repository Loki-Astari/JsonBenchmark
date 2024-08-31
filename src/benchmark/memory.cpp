#include "ThirdParty/test.h"
#include "TestSuite.h"
#include "filesystem.h"
#include "Memory/Memory.h"
#include <fstream>
#include <memory>

TestBase const& findParser(std::string const& name)
{
    ParsrList    parsrList = TestManager::instance().getTests();
    for (auto const& parser: parsrList)
    {
        if (parser->GetName() == name)
        {
            return *parser;
        }
    }
    std::cerr << "Could not find Parser: " << name << "\n";
    exit(1);
}

void preloadData(ThorsAnvil::Benchmark::Test& test)
{
    std::ifstream   input(test.path.str());
    input.seekg(0, std::ios_base::end);
    std::streampos  size = input.tellg();
    input.seekg(0, std::ios_base::beg);

    test.input.resize(size);
    input.read(&test.input[0], size);
}

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cerr << "Not Enough Arguments\n";
        exit(1);
    }

    using ThorsAnvil::FileSystem::Path;
    using ThorsAnvil::Benchmark::Test;
    using ThorsAnvil::Benchmark::TestSetUp;

    TestBase const& parser = findParser(argv[1]);

    Path    dirPath(QUOTE(DATA_DIR) "/performance/");
    Path    filePath(dirPath, argv[2]);
    Test    test(filePath);
    std::string testName("performance/");
    testName.append(argv[2]);

    int     testType=std::stoi(argv[3]);
    if (testType < 1 || testType > 6)
    {
        std::cerr << "Unknown Test\n";
        exit(1);
    }

    preloadData(test);
    TestSetUp   testSetUp(parser, testName, true);
    {
        std::unique_ptr<ParseResultBase> dom;
        /*bool implementedParse =*/  parser.Parse(test.input.c_str(), test.input.size(), dom);
        std::unique_ptr<StringResultBase> json;
        /*bool implementedPretty =*/ parser.Prettify(*dom, json);
    }

    std::size_t currentSize = 0;
    switch (testType)
    {
        case 1:
        {
            MemoryInfo::active = true;
            {
                std::unique_ptr<ParseResultBase> dom;
                /*bool implemented =*/ parser.Parse(test.input.c_str(), test.input.size(), dom);
                currentSize = MemoryInfo::getInstance().currentSize;
            }
            MemoryInfo::active = false;
            break;
        }
        case 2:
        {
            std::unique_ptr<ParseResultBase> dom;
            /* bool implemented =*/ parser.Parse(test.input.c_str(), test.input.size(), dom);
            MemoryInfo::active = true;
            {
                std::unique_ptr<StringResultBase> json;
                /*bool implemented =*/ parser.Stringify(*dom, json);
                currentSize = MemoryInfo::getInstance().currentSize;
            }
            MemoryInfo::active = false;
            break;
        }
        case 3:
        {
            std::unique_ptr<ParseResultBase> dom;
            /* bool implemented =*/ parser.Parse(test.input.c_str(), test.input.size(), dom);
            MemoryInfo::active = true;
            {
                std::unique_ptr<StringResultBase> json;
                /* bool implemented =*/ parser.Prettify(*dom, json);
                currentSize = MemoryInfo::getInstance().currentSize;
            }
            MemoryInfo::active = false;
            break;
        }
        case 4:
        {
            std::unique_ptr<ParseResultBase> dom;
            /*bool implemented =*/ parser.Parse(test.input.c_str(), test.input.size(), dom);
            Stat stats;
            MemoryInfo::active = true;
            {
                parser.Statistics(*dom, stats);
                currentSize = MemoryInfo::getInstance().currentSize;
            }
            MemoryInfo::active = false;
            break;
        }
        case 5:
        {
            MemoryInfo::active = true;
            {
                std::unique_ptr<StringResultBase> json;
                /* bool implemented =*/ parser.SaxRoundtrip(test.input.c_str(), test.input.size(), json);
                currentSize = MemoryInfo::getInstance().currentSize;
            }
            MemoryInfo::active = false;
            break;
        }
        case 6:
        {
            Stat stats;
            MemoryInfo::active = true;
            {
                parser.SaxStatistics(test.input.c_str(), test.input.size(), stats);
                currentSize = MemoryInfo::getInstance().currentSize;
            }
            MemoryInfo::active = false;
            break;
        }
    }

    std::size_t     allocationLeak  = 0;
    std::size_t     allocationCount = 0;
    MemoryInfo& info = MemoryInfo::getInstance();
    for (auto const& item: info.countData)
    {
        if (item.second.first > item.second.second && item.first)
        {
            allocationLeak  += (item.second.first - item.second.second) * item.first;
            allocationCount++;
        }
    }
    if (allocationLeak != 0)
    {
        std::cout << "                    Warning: potential memory leak: (>" << allocationCount << "< allocations for " << allocationLeak << " bytes)\n"
                  << "                        Memory stats: " << argv[1] << " Test: " << argv[2] << "\n"
                  << "                           mallocCount = " << info.mallocCount << "\n"
                  << "                          raallocCount = " << info.reallocCount << "\n"
                  << "                             freeCount = " << info.freeCount << "\n"
                  << "                           currentSize = " << currentSize << "\n"         // See above
                  << "                              peakSize = " << info.peakSize << "\n"
                  << "                        Leaked Blocks\n";
        for (auto const& item: info.countData)
        {
            if (item.second.first > item.second.second && item.first)
            {
                std::cout << "                           Block Size: " << item.first << " Leaked: " << (item.second.first - item.second.second) << "\n";
            }
        }
    }
    std::cerr << currentSize << "," << info.peakSize << "," << (info.mallocCount + info.reallocCount) << "," << allocationLeak << "," << allocationCount << ",";
}

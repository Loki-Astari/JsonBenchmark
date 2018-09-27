#ifndef THORS_ANVIL_BENCHMARK_TEST_BASE_H
#define THORS_ANVIL_BENCHMARK_TEST_BASE_H

#include <cstdlib>
#include <string>

class ParseResultBase
{
    public:
        virtual ~ParseResultBase() {}
};

class StringResultBase
{
    public:
        virtual ~StringResultBase() {}
        virtual const char* c_str() const = 0;
};

struct Stat
{
    size_t objectCount;
    size_t arrayCount;
    size_t numberCount;
    size_t stringCount;
    size_t trueCount;
    size_t falseCount;
    size_t nullCount;

    size_t memberCount;   // Number of members in all objects
    size_t elementCount;  // Number of elements in all arrays
    size_t stringLength;  // Number of code units in all strings

    bool operator==(Stat const& rhs) const;
    bool operator!=(Stat const& rhs) const;
};

class TestBase
{
    public:
        bool operator<(const TestBase& rhs) const;

        virtual const char* GetName() const = 0;
        virtual const char* GetFilename() const = 0;

        // For each operation, call SetUp() before and TearDown() after.
        // It is mainly for libraries require huge initialize time (e.g. Creating Isolate in V8).
        virtual void SetUp()                                                                    const {}
        virtual void TearDown()                                                                 const {}
        virtual void SetUp(char const*)                                                         const {SetUp();}
        virtual void TearDown(char const*)                                                      const {TearDown();}

        virtual bool ParseDouble(const char* /*json*/, double* /*d*/)                            const { return false; }
        virtual bool ParseString(const char* /*json*/, std::string& /*s*/)                       const { return false; }
        virtual ParseResultBase* Parse(const char* /*json*/, size_t /*length*/)                  const { return nullptr; }
        virtual StringResultBase* Stringify(const ParseResultBase* /*parseResult*/)              const { return nullptr; }
        virtual StringResultBase* Prettify(const ParseResultBase* /*parseResult*/)               const { return nullptr; }
        virtual StringResultBase* SaxRoundtrip(const char* /*json*/, size_t /*length*/)          const { return nullptr; }
        virtual bool Statistics(const ParseResultBase* /*parseResult*/, Stat* /*stat*/)          const { return false; }
        virtual bool SaxStatistics(const char* /*json*/, size_t /*length*/, Stat* /*stat*/)      const { return false; }
        virtual bool SaxStatisticsUTF16(const char* /*json*/, size_t /*length*/, Stat* /*stat*/) const { return false; }
};

#endif

#ifndef THORS_ANVIL_BENCHMARK_TEST_H
#define THORS_ANVIL_BENCHMARK_TEST_H

#include <cstddef>
#include <string>
#include <sstream>
#include <memory>

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

class StringResultUsingString: public StringResultBase
{
    public:
        std::string result;
        virtual const char* c_str() const
        {
            return result.c_str();
        }
};

class StringResultUsingStream: public StringResultBase
{
    public:
        std::stringstream stream;
        mutable std::string       result;
        virtual const char* c_str() const
        {
            result = stream.str();
            return result.c_str();
        }
};

struct Stat
{
    std::size_t objectCount;
    std::size_t arrayCount;
    std::size_t numberCount;
    std::size_t stringCount;
    std::size_t trueCount;
    std::size_t falseCount;
    std::size_t nullCount;

    std::size_t memberCount;   // Number of members in all objects
    std::size_t elementCount;  // Number of elements in all arrays
    std::size_t stringLength;  // Number of code units in all strings

    bool operator==(Stat const& rhs) const;
    bool operator!=(Stat const& rhs) const;
};

class TestBase
{
    public:
        virtual ~TestBase() {}
        bool operator<(const TestBase& rhs) const;

        virtual const char* GetName() const = 0;
        virtual const char* Type()    const = 0;
        virtual const char* GetFilename() const = 0;

        // For each operation, call SetUp() before and TearDown() after.
        // It is mainly for libraries require huge initialize time (e.g. Creating Isolate in V8).
        virtual void SetUp()                                                                          const {}
        virtual void TearDown()                                                                       const {}
        virtual void SetUp(char const*)                                                               const {SetUp();}
        virtual void TearDown(char const*)                                                            const {TearDown();}

        virtual bool ParseDouble(const char* /*json*/, long double& /*d*/)                            const { return false; }
        virtual bool ParseString(const char* /*json*/, std::string& /*s*/)                            const { return false; }
        virtual bool Parse(const char* /*json*/, std::size_t /*length*/, std::unique_ptr<ParseResultBase>& /*result*/)          const { return false; }
        virtual bool Stringify(const ParseResultBase& /*parseResult*/, std::unique_ptr<StringResultBase>& /*result*/)           const { return false; }
        virtual bool Prettify(const ParseResultBase& /*parseResult*/, std::unique_ptr<StringResultBase>& /*result*/)            const { return false; }
        virtual bool Statistics(const ParseResultBase& /*parseResult*/, Stat& /*stat*/)               const { return false; }
        virtual bool SaxStatistics(const char* /*json*/, std::size_t /*length*/, Stat& /*stat*/)      const { return false; }
        virtual bool SaxStatisticsUTF16(const char* /*json*/, std::size_t /*length*/, Stat& /*stat*/) const { return false; }
        virtual bool ParseValidate(const char* json, std::size_t length, bool& result)                const
        {
            std::unique_ptr<ParseResultBase>    reply;
            bool implemented = Parse(json, length, reply);
            if (implemented) {
                result = reply.get() != nullptr;
            }
            return implemented;
        }
        virtual bool SaxRoundtrip(const char* json, std::size_t length, std::unique_ptr<StringResultBase>& result)  const
        {
            std::unique_ptr<ParseResultBase>    parse;
            bool implemented = this->Parse(json, length, parse);
            if (implemented && parse.get())
            {
                implemented = this->Stringify(*parse, result);
            }
            return implemented;
        }

        // --
};

#define REGISTER_TEST(cls)  std::unique_ptr<TestBase> get ## cls() { return std::make_unique<cls>(); }static_assert(true, "Should Work")

#endif

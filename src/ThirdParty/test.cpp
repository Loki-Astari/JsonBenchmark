#include "test.h"
#include "TestManager.h"
#include <memory>

bool Stat::operator!=(Stat const& rhs) const
{
    return !operator==(rhs);
}

bool TestBase::operator<(const TestBase& rhs) const
{
    return strcmp(GetName(), rhs.GetName()) < 0;
}

bool Stat::operator==(Stat const& rhs) const
{
    return std::forward_as_tuple(objectCount, arrayCount, numberCount,
                                 stringCount, trueCount, falseCount,
                                 nullCount, memberCount, elementCount,
                                 stringLength)
        ==
           std::forward_as_tuple(rhs.objectCount, rhs.arrayCount, rhs.numberCount,
                                 rhs.stringCount, rhs.trueCount, rhs.falseCount,
                                 rhs.nullCount, rhs.memberCount, rhs.elementCount,
                                 rhs.stringLength);
}

class TestRunner: public TestBase
{
    std::unique_ptr<TestBase>    pimpl;
    public:
        TestRunner(std::unique_ptr<TestBase>&& src)
            : pimpl(std::move(src))
        {
            TestManager::instance().addTest(this);
        }
        virtual const char* GetName()                                                                           const override {return pimpl->GetName();}
        virtual const char* Type()                                                                              const override {return pimpl->Type();}
        virtual const char* GetFilename()                                                                       const override {return pimpl->GetFilename();}

        virtual void SetUp(char const* test)                                                                    const override {return pimpl->SetUp(test);}
        virtual void TearDown(char const* test)                                                                 const override {return pimpl->TearDown(test);}

        virtual bool ParseValidate(const char* json, std::size_t length, bool& reply)                           const override {return pimpl->ParseValidate(json, length, reply);}
        virtual bool ParseDouble(const char* json, long double& d)                                              const override {return pimpl->ParseDouble(json, d);}
        virtual bool ParseString(const char* json, std::string& s)                                              const override {return pimpl->ParseString(json, s);}
        virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply)            const override {return pimpl->Parse(json, length, reply);}
        virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply)    const override {return pimpl->Stringify(parseResult, reply);}
        virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply)     const override {return pimpl->Prettify(parseResult, reply);}
        virtual bool SaxRoundtrip(const char* json, size_t length, std::unique_ptr<StringResultBase>& reply)    const override {return pimpl->SaxRoundtrip(json, length, reply);}
        virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat)                                 const override {return pimpl->Statistics(parseResult, stat);}
        virtual bool SaxStatistics(const char* json, size_t length, Stat& stat)                                 const override {return pimpl->SaxStatistics(json, length, stat);}
        virtual bool SaxStatisticsUTF16(const char* json, size_t length, Stat& stat)                            const override {return pimpl->SaxStatisticsUTF16(json, length, stat);}
};

#define REGISTER_TEST_OBJECT(cls)       std::unique_ptr<TestBase> get ## cls();TestRunner gRegister ## cls(get ## cls())

REGISTER_TEST_OBJECT(JsonPerformanceTest);


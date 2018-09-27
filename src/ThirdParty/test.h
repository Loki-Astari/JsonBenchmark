#ifndef THORS_ANVIL_BENCHMARK_TEST_H
#define THORS_ANVIL_BENCHMARK_TEST_H

#include "TestBase.h"
#include <vector>
#include <string.h>
#include <tuple>
#include <algorithm>

using ParsrList = std::vector<const TestBase*>;

class TestManager
{
    public:
        static TestManager& instance();
        void addTest(const TestBase* test);
        const ParsrList& getTests() const;

    private:
        ParsrList mTests;
};

#define REGISTER_TEST(cls)  std::unique_ptr<TestBase> get ## cls() { return std::make_unique<cls>(); }static_assert(true)

#endif

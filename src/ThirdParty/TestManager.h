#ifndef THORS_ANVIL_BENCHMARK_TEST_MANAGER_H
#define THORS_ANVIL_BENCHMARK_TEST_MANAGER_H

#include <vector>
#include <string.h>
#include <tuple>
#include <algorithm>

class TestBase;
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

#endif

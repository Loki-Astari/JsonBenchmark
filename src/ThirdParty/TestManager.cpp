#include "TestManager.h"
#include <memory>

TestManager& TestManager::instance()
{
    static TestManager singleton;
    return singleton;
}

void TestManager::addTest(const TestBase* test)
{
    mTests.push_back(test);
}

const ParsrList& TestManager::getTests() const
{
    return mTests;
}



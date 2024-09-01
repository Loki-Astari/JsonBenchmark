#ifndef JSONBENCHMARK_TYPE_SAFE_ROUND_10_H
#define JSONBENCHMARK_TYPE_SAFE_ROUND_10_H

#include <string>
#include <memory>

struct Obj3
{
    std::unique_ptr<int>    a;
    std::string*            foo;
};

#endif



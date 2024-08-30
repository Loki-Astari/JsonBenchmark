#ifndef JSONBENCHMARK_TYPE_SAFE_PASS_01H
#define JSONBENCHMARK_TYPE_SAFE_PASS_01H

#include <string>
#include <vector>
#include <map>

// Used by jsonchecker_pass/pass01.json
struct Empty {};
struct Special
{
    int                 integer;
    double              real;
    double              e;
    double              E;
    double              emptyKey;      // "":  23456789012E66,
    int                 zero;
    int                 one;
    std::string         space;
    std::string         quote;
    std::string         backslash;
    std::string         controls;
    std::string         slash;
    std::string         alpha;
    std::string         ALPHA;
    std::string         digit;
    std::string         number;     // "0123456789": "digit",
    std::string         special;
    std::string         hex;
    bool                aTrue;      // "true": true,
    bool                aFalse;     // "false": false,
    int*                null;
    std::vector<int>    array;
    Empty               object;
    std::string         address;
    std::string         url;
    std::string         comment;
    std::string         commentKey; // "# -- --> */": " ",
    std::vector<int>    spaced;     // " s p a c e d " :[1,2 , 3
    std::vector<int>    compact;
    std::string         jsontext;
    std::string         quotes;
    std::string         key;        // "\/\\\"\uCAFE\uBABE\uAB98\uFCDE\ubcda\uef4A\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?" : "A key can be any string"
};
using Pass01 = std::tuple<
    std::string,
    std::map<std::string, std::vector<std::string>>,
    Empty,
    std::vector<int>,
    int,
    bool,
    bool,
    int*,
    Special,
    double, double,
    double,
    int,
    double,
    double,
    double,
    double,double,double,
    std::string
>;

#endif

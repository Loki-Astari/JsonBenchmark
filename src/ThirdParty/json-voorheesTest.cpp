#include "test.h"


#include "jsonv/all.hpp"
#include <sstream>
#include <cctype>

using namespace jsonv;

static void GenStat(Stat& stat, const value& v) {
    switch (v.kind()) {
    case kind::array:
        {
            for (value::const_array_iterator itr = v.begin_array(); itr != v.end_array(); ++itr)
                GenStat(stat, *itr);
            stat.arrayCount++;
            stat.elementCount += v.size();
        }
        break;

    case kind::object:
        {
            for (value::const_object_iterator itr = v.begin_object(); itr != v.end_object(); ++itr) {
                GenStat(stat, itr->second);
                stat.stringLength += itr->first.size();
            }
            stat.objectCount++;
            stat.memberCount += v.size();
            stat.stringCount += v.size();
        }
        break;

    case kind::string: 
        stat.stringCount++;
        stat.stringLength += v.size();
        break;

    case kind::integer:
    case kind::decimal:
        stat.numberCount++;
        break;

    case kind::boolean:
        if (v.as_boolean())
            stat.trueCount++;
        else
            stat.falseCount++;
        break;

    case kind::null:
        stat.nullCount++;
        break;
    }
}

class VoorheesParseResult : public ParseResultBase {
public:
    value root;
};

class VoorheesStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return s.c_str(); }

    std::string s;
};
class VoorheesTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "json-voorhees"; }
    virtual const char* Type()        const override { return "C++";}
    virtual const char* GetFilename() const override { return __FILE__; }

    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        std::unique_ptr<VoorheesParseResult> pr = std::make_unique<VoorheesParseResult>();
        try {
            pr->root = parse(string_view(json, length));
            reply = std::move(pr);
        }
        catch (...) {}
        return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const VoorheesParseResult& pr = static_cast<const VoorheesParseResult&>(parseResult);
        std::unique_ptr<VoorheesStringResult> sr = std::make_unique<VoorheesStringResult>();
        sr->s = to_string(pr.root);
        reply = std::move(sr);
        return true;
    }

    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const VoorheesParseResult& pr = static_cast<const VoorheesParseResult&>(parseResult);
        std::unique_ptr<VoorheesStringResult> sr = std::make_unique<VoorheesStringResult>();
        std::ostringstream os;
        ostream_pretty_encoder e(os, 4);
        e.encode(pr.root);
        sr->s = os.str();
        reply = std::move(sr);
        return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const VoorheesParseResult& pr = static_cast<const VoorheesParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
        GenStat(stat, pr.root);
        return true;
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        try {
            value v = parse(json);
            d = v[0].as_decimal();
        }
        catch (...) {}
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        try {
            value v = parse(json);
            s = v[0].as_string();
        }
        catch (...) {}
        return true;
    }
};

REGISTER_TEST(VoorheesTest);

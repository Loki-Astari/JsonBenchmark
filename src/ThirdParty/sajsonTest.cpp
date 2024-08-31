#include "test.h"

#include "include/sajson.h"
#include <iostream>
#include <memory>

using namespace sajson;

static void GenStat(Stat& stat, const value& v) {
    switch (v.get_type()) {
    case TYPE_ARRAY:
        {
            size_t size = v.get_length();
            for (size_t i = 0; i < size; i++)
                GenStat(stat, v.get_array_element(i));
            stat.arrayCount++;
            stat.elementCount += size;
        }
        break;

    case TYPE_OBJECT:
        {
            size_t size = v.get_length();
            for (size_t i = 0; i < size; i++) {
                GenStat(stat, v.get_object_value(i));
                stat.stringLength += v.get_object_key(i).length();
            }
            stat.objectCount++;
            stat.memberCount += size;
            stat.stringCount += size;
        }
        break;

    case TYPE_STRING: 
        stat.stringCount++;
        stat.stringLength += v.get_string_length();
        break;

    case TYPE_INTEGER:
    case TYPE_DOUBLE:
        stat.numberCount++;
        break;

    case TYPE_TRUE:
        stat.trueCount++;
        break;

    case TYPE_FALSE:
        stat.falseCount++;
        break;

    case TYPE_NULL:
        stat.nullCount++;
        break;
    }
}

class SajsonParseResult : public ParseResultBase {
public:
    SajsonParseResult(document&& d_) : d(std::move(d_)) {}
    document d;
};

class SajsonStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return s.c_str(); }

    std::string s;
};
class SajsonTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "sajson"; }
    virtual const char* Type()        const override { return "C++";}
    virtual const char* GetFilename() const override { return __FILE__; }

    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        std::unique_ptr<SajsonParseResult> pr = std::make_unique<SajsonParseResult>(parse(dynamic_allocation(), string(json, length)));
        if (pr->d.is_valid()) {
            reply = std::move(pr);
        }
    	return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const SajsonParseResult& pr = static_cast<const SajsonParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
        GenStat(stat, pr.d.get_root());
        return true;
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        document doc = parse(dynamic_allocation(), string(json, strlen(json)));
        if (doc.is_valid() &&
            doc.get_root().get_type() == TYPE_ARRAY &&
            doc.get_root().get_length() == 1 &&
            doc.get_root().get_array_element(0).get_type() == TYPE_DOUBLE)
        {
            d = doc.get_root().get_array_element(0).get_double_value();
        }
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        document doc = parse(dynamic_allocation(), string(json, strlen(json)));
        if (doc.is_valid() &&
            doc.get_root().get_type() == TYPE_ARRAY &&
            doc.get_root().get_length() == 1 &&
            doc.get_root().get_array_element(0).get_type() == TYPE_STRING)
        {
            s = doc.get_root().get_array_element(0).as_string();
        }
        return true;
    }
};

REGISTER_TEST(SajsonTest);

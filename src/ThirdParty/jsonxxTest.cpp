#include "test.h"
#include "jsonxx.h"
#include <memory>
#include <string.h>

using namespace jsonxx;

static void GenStat(Stat& s, const Value& v) {
    switch (v.type_) {
    case Value::OBJECT_:
        {
            const Object& o = v.get<Object>();
            Object::container::const_iterator itr = o.kv_map().begin();
            Object::container::const_iterator end = o.kv_map().end();
            for (; itr != end; ++itr) {
                s.stringLength += itr->first.size(); // key
                GenStat(s, *itr->second);
            }
            s.objectCount++;
            s.memberCount += o.size();
            s.stringCount += o.size();  // Key
        }
        break;

    case Value::ARRAY_:
        {
            const Array& a = v.get<Array>();
            Array::container::const_iterator itr = a.values().begin();
            Array::container::const_iterator end = a.values().end();
            for (; itr != end; ++itr)
                GenStat(s, **itr);
            s.arrayCount++;
            s.elementCount += a.size();
        }
        break;

    case Value::STRING_:
        s.stringCount++;
        s.stringLength += v.get<String>().size();
        break;

    case Value::NUMBER_:
        s.numberCount++;
        break;

    case Value::BOOL_:
        if (v.get<bool>())
            s.trueCount++;
        else
            s.falseCount++;
        break;

    case Value::NULL_:
        s.nullCount++;
        break;

    case Value::INVALID_:
        assert(false);
    }
}

class JsonxxParseResult : public ParseResultBase {
public:
    Value v;
};

class JsonxxTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "jsonxx"; }
    virtual const char* Type()        const override { return "C++";}
    virtual const char* GetFilename() const override { return __FILE__; }

    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        (void)length;
        std::unique_ptr<JsonxxParseResult> pr = std::make_unique<JsonxxParseResult>();
        if (pr->v.parse(json)) {
            reply = std::move(pr);
        }
        return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const JsonxxParseResult& pr = static_cast<const JsonxxParseResult&>(parseResult);
        std::unique_ptr<StringResultUsingString> sr = std::make_unique<StringResultUsingString>();
        sr->result = pr.v.is<Object>() ? pr.v.get<Object>().json() : pr.v.get<Array>().json();
        reply = std::move(sr);
        return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const JsonxxParseResult& pr = static_cast<const JsonxxParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
        GenStat(stat, pr.v);
        return true;
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        Value v;
        if (v.parse(json) && v.is<Array>() && v.get<Array>().size() == 1) {
            d = (long double)v.get<Array>().get<Number>(0);
        }
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        Value v;
        if (v.parse(json) && v.is<Array>() && v.get<Array>().size() == 1) {
            s = v.get<Array>().get<String>(0);
        }
        return true;
    }
};

REGISTER_TEST(JsonxxTest);

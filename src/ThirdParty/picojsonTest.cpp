#include "test.h"
#include "picojson.h"
#include <memory>

using namespace picojson;

static void GenStat(Stat& s, const value& v) {
    // Note: No public API to get value::_type
    if (v.is<object>()) {
        const object& o = v.get<object>();
        for (object::const_iterator itr = o.begin(); itr != o.end(); ++itr) {
            s.stringLength += itr->first.size(); // key
            GenStat(s, itr->second);
        }
        s.objectCount++;
        s.memberCount += o.size();
        s.stringCount += o.size();  // Key
    }
    else if (v.is<array>()) {
        const array& a = v.get<array>();
        for (array::const_iterator itr = a.begin(); itr != a.end(); ++itr)
            GenStat(s, *itr);
        s.arrayCount++;
        s.elementCount += a.size();
    }
    else if (v.is<std::string>()) {
        s.stringCount++;
        s.stringLength += v.get<std::string>().size();
    }
    else if (v.is<double>())
        s.numberCount++;
    else if (v.is<bool>()) {
        if (v.get<bool>())
            s.trueCount++;
        else
            s.falseCount++;
    }
    else if (v.is<null>())
        s.nullCount++;
}

class PicojsonParseResult : public ParseResultBase {
public:
    value v;
};

class PicojsonStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return s.c_str(); }

    std::string s;
};

class PicojsonTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "picojson"; }
    virtual const char* Type()        const override { return "C++";}
    virtual const char* GetFilename() const override { return __FILE__; }
	
    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        std::unique_ptr<PicojsonParseResult> pr = std::make_unique<PicojsonParseResult>();
        std::string err;
        parse(pr->v, json, json + length, &err);
    	if (err.empty()) {
            reply = std::move(pr);
    	}
    	return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const PicojsonParseResult& pr = static_cast<const PicojsonParseResult&>(parseResult);
        std::unique_ptr<PicojsonStringResult> sr = std::make_unique<PicojsonStringResult>();
        sr->s = pr.v.serialize();
        reply = std::move(sr);
        return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const PicojsonParseResult& pr = static_cast<const PicojsonParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
        GenStat(stat, pr.v);
        return true;
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        value v;
        std::string err;
        parse(v, json, json + strlen(json), &err);
        if (err.empty() &&
            v.is<array>() &&
            v.get<array>().size() == 1 &&
            v.get<array>()[0].is<double>())
        {
            d = v.get<array>()[0].get<double>();
        }
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        value v;
        std::string err;
        parse(v, json, json + strlen(json), &err);
        if (err.empty() &&
            v.is<array>() &&
            v.get<array>().size() == 1 &&
            v.get<array>()[0].is<std::string>())
        {
            s = v.get<array>()[0].get<std::string>();
        }
        return true;
    }
};

REGISTER_TEST(PicojsonTest);

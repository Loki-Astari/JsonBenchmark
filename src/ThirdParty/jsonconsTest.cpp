#include "test.h"

#include "jsoncons/json.hpp"

using namespace jsoncons;

static void GenStat(Stat& stat, const json& v) {
    switch (v.type()) {
    case json_type::array_value:
        for (json::const_array_iterator itr = v.array_range().begin(); itr != v.array_range().end(); ++itr)
            GenStat(stat, *itr);
        stat.arrayCount++;
        stat.elementCount += v.size();
        break;

    // case json_major_type::empty_object_t:
    case json_type::object_value:
        for (json::const_object_iterator itr = v.object_range().begin(); itr != v.object_range().end(); ++itr) {
            GenStat(stat, itr->value());
            stat.stringLength += itr->key().size();
        }
        stat.objectCount++;
        stat.memberCount += v.size();
        stat.stringCount += v.size();
        break;

    case json_type::byte_string_value: 
        stat.stringCount++;
        stat.stringLength += v.as_string().size();
        break;
    case json_type::string_value: 
        stat.stringCount++;
        stat.stringLength += v.as_string().size();
        break;
    case json_type::double_value:
    case json_type::int64_value:
    case json_type::uint64_value:
    case json_type::half_value:
        stat.numberCount++;
        break;

    case json_type::bool_value:
        if (v.as_bool())
            stat.trueCount++;
        else
            stat.falseCount++;
        break;

    case json_type::null_value:
        stat.nullCount++;
        break;
    }
}

class JsonconsParseResult : public ParseResultBase {
public:
    json root;
};

class JsonconsStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return s.c_str(); }

    std::string s;
};
class JsonconsTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "jsoncons"; }
    virtual const char* Type()        const override { return "C++";}
    virtual const char* GetFilename() const override { return __FILE__; }

    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        (void)length;
        std::unique_ptr<JsonconsParseResult> pr = std::make_unique<JsonconsParseResult>();
        try {
            pr->root = json::parse(json);
            reply = std::move(pr);
        }
        catch (...) {}
    	return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const JsonconsParseResult& pr = static_cast<const JsonconsParseResult&>(parseResult);
        std::unique_ptr<JsonconsStringResult> sr = std::make_unique<JsonconsStringResult>();
        sr->s = pr.root.to_string();
        reply = std::move(sr);
        return true;
    }

    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const JsonconsParseResult& pr = static_cast<const JsonconsParseResult&>(parseResult);
        std::unique_ptr<JsonconsStringResult> sr = std::make_unique<JsonconsStringResult>();
        pr.root.dump(sr->s);
        reply = std::move(sr);
        return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const JsonconsParseResult& pr = static_cast<const JsonconsParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
        GenStat(stat, pr.root);
        return true;
    }

    virtual bool ParseDouble(const char* json_, long double& d) const override
    {
        try {
            json root = json::parse(json_);
            d = root.at(0).as_double();
        }
        catch (...) {}
        return true;
    }

    virtual bool ParseString(const char* json_, std::string& s) const override
    {
        try {
            json root = json::parse(json_);
            s = root.at(0).as_string();
        }
        catch (...) {}
        return true;
    }
};

REGISTER_TEST(JsonconsTest);


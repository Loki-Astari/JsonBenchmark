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
    virtual const char* GetName() const { return "jsoncons-C++"; }
    virtual const char* GetFilename() const { return __FILE__; }

    virtual ParseResultBase* Parse(const char* json, size_t length) const {
        (void)length;
        JsonconsParseResult* pr = new JsonconsParseResult;
        try {
            pr->root = json::parse(json);
        }
        catch (...) {
            delete pr;
            return 0;
        }
    	return pr;
    }

    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const {
        const JsonconsParseResult* pr = static_cast<const JsonconsParseResult*>(parseResult);
        JsonconsStringResult* sr = new JsonconsStringResult;
        sr->s = pr->root.to_string();
        return sr;
    }

    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const {
        const JsonconsParseResult* pr = static_cast<const JsonconsParseResult*>(parseResult);
        JsonconsStringResult* sr = new JsonconsStringResult;
        pr->root.dump(sr->s);
        //sr->s = pr->root.to_string(json_options());
        return sr;
    }

    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const {
        const JsonconsParseResult* pr = static_cast<const JsonconsParseResult*>(parseResult);
        memset(stat, 0, sizeof(Stat));
        GenStat(*stat, pr->root);
        return true;
    }

    virtual bool ParseDouble(const char* json_, double* d) const {
        try {
            json root = json::parse(json_);
            *d = root.at(0).as_double();
            return true;
        }
        catch (...) {
        }
        return false;
    }

    virtual bool ParseString(const char* json_, std::string& s) const {
        try {
            json root = json::parse(json_);
            s = root.at(0).as_string();
            return true;
        }
        catch (...) {
        }
        return false;
    }
};

REGISTER_TEST(JsonconsTest);


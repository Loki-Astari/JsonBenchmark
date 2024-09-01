#include "test.h"
#include "nlohmann/json.hpp"

using namespace nlohmann;

static void GenStat(Stat& stat, const json& v) {
    switch (v.type()) {
    case json::value_t::array:
        for (auto& element : v)
            GenStat(stat, element);
        stat.arrayCount++;
        stat.elementCount += v.size();
        break;

    case json::value_t::object:
        for (json::const_iterator it = v.begin(); it != v.end(); ++it) {
            GenStat(stat, it.value());
            stat.stringLength += it.key().size();
        }
        stat.objectCount++;
        stat.memberCount += v.size();
        stat.stringCount += v.size();
        break;

    case json::value_t::string:
        stat.stringCount++;
        stat.stringLength += v.get<std::string>().size();
        break;

    case json::value_t::number_integer:
    case json::value_t::number_unsigned:
    case json::value_t::number_float:
        stat.numberCount++;
        break;

    case json::value_t::boolean:
        if (v)
            stat.trueCount++;
        else
            stat.falseCount++;
        break;

    case json::value_t::null:
        stat.nullCount++;
        break;
    case json::value_t::discarded:
        break;
    default: break;
    }
}

class NlohmannParseResult : public ParseResultBase {
public:
    json root;
};

class NlohmannTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "nlohmann"; }
    virtual const char* Type()        const override { return "C++11";}
    virtual const char* GetFilename() const override { return __FILE__; }

    virtual bool Parse(const char* j, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        (void)length;
        std::unique_ptr<NlohmannParseResult> pr = std::make_unique<NlohmannParseResult>();
        try {
            pr->root = json::parse(j);
            reply = std::move(pr);
        }
        catch (...) {}
    	return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const NlohmannParseResult& pr = static_cast<const NlohmannParseResult&>(parseResult);
        std::unique_ptr<StringResultUsingString> sr = std::make_unique<StringResultUsingString>();
        sr->result = pr.root.dump();
        reply = std::move(sr);
        return true;
    }

    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const NlohmannParseResult& pr = static_cast<const NlohmannParseResult&>(parseResult);
        std::unique_ptr<StringResultUsingString> sr = std::make_unique<StringResultUsingString>();
        sr->result = pr.root.dump(4);
        reply = std::move(sr);
        return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const NlohmannParseResult& pr = static_cast<const NlohmannParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
        GenStat(stat, pr.root);
        return true;
    }

    virtual bool ParseDouble(const char* j, long double& d) const override
    {
        try {
            json root = json::parse(j);
            d = root[0].get<long double>();
        }
        catch (...) {}
        return true;
    }

    virtual bool ParseString(const char* j, std::string& s) const override
    {
        try {
            json root = json::parse(j);
            s = root[0].get<std::string>();
        }
        catch (...) {}
        return true;
    }
};

REGISTER_TEST(NlohmannTest);

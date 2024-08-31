#include "test.h"

#define CONFIGURU_IMPLEMENTATION 1
#include <algorithm>
#include <limits>
#include "Configuru/configuru.hpp"
#include <iostream>

using namespace configuru;

static void GenStat(Stat& stat, const Config& v) {
    switch (v.type()) {
    case Config::Array:
        for (auto& element : v.as_array()) {
            GenStat(stat, element);
        }
        stat.arrayCount++;
        stat.elementCount += v.array_size();
        break;

    case Config::Object:
        for (const auto& p : v.as_object()) {
            GenStat(stat, p.value());
            stat.stringLength += p.key().size();
        }
        stat.objectCount++;
        stat.memberCount += v.object_size();
        stat.stringCount += v.object_size();
        break;

    case Config::String:
        stat.stringCount++;
        stat.stringLength += v.as_string().size();
        break;

    case Config::Int:
    case Config::Float:
        stat.numberCount++;
        break;

    case Config::Bool:
        if (v)
            stat.trueCount++;
        else
            stat.falseCount++;
        break;

    case Config::Null:
        stat.nullCount++;
        break;
    case Config::BadLookupType:
        std::cerr << "Error BadLookupType\n";
        exit(1);
    case Config::Uninitialized:
        std::cerr << "Error Uninitialized\n";
        exit(1);
    }
}

class ConfiguruParseResult : public ParseResultBase {
public:
    Config root;
};

class ConfiguruStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return s.c_str(); }

    std::string s;
};

class ConfiguruTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "Configuru"; }
    virtual const char* Type()        const override { return "C++11";}
    virtual const char* GetFilename() const override { return __FILE__; }

    virtual bool Parse(const char* j, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        (void)length;
        std::unique_ptr<ConfiguruParseResult> pr = std::make_unique<ConfiguruParseResult>();
        try {
            pr->root = parse_string(j, configuru::JSON, "dummy_name");
    	    reply = std::move(pr);
        }
        catch (...) {}
        return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const ConfiguruParseResult& pr = static_cast<const ConfiguruParseResult&>(parseResult);
        std::unique_ptr<ConfiguruStringResult> sr = std::make_unique<ConfiguruStringResult>();
        auto format = configuru::JSON;
        format.indentation = "";
        sr->s = configuru::dump_string(pr.root, format);
        reply = std::move(sr);
        return true;
    }

    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const ConfiguruParseResult& pr = static_cast<const ConfiguruParseResult&>(parseResult);
        std::unique_ptr<ConfiguruStringResult> sr = std::make_unique<ConfiguruStringResult>();
        auto format = configuru::JSON;
        format.indentation = "\t";
        sr->s = configuru::dump_string(pr.root, format);
        reply = std::move(sr);
        return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const ConfiguruParseResult& pr = static_cast<const ConfiguruParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
        GenStat(stat, pr.root);
        return true;
    }

    virtual bool ParseDouble(const char* j, long double& d) const override
    {
        try
        {
            Config root = parse_string(j, JSON, "double_json");
            d = (double)root[0];
        }
        catch (...) {}
        return true;
    }

    virtual bool ParseString(const char* j, std::string& s) const override
    {
        try
        {
            Config root = parse_string(j, JSON, "string_json");
            s = (std::string)root[0];
        }
        catch (...) {}
        return true;
    }
};

REGISTER_TEST(ConfiguruTest);

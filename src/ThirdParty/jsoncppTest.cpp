#include "test.h"

#include "json/reader.h"
#include "json/value.h"
#include "json/writer.h"
#include <memory>
#include <string.h>

using namespace Json;

static void GenStat(Stat& stat, const Value& v) {
    switch (v.type()) {
    case arrayValue:
        for (ValueConstIterator itr = v.begin(); itr != v.end(); ++itr)
            GenStat(stat, *itr);
        stat.arrayCount++;
        stat.elementCount += v.size();
        break;

    case objectValue:
        for (ValueConstIterator itr = v.begin(); itr != v.end(); ++itr) {
            GenStat(stat, *itr);
            stat.stringLength += itr.name().size();
        }
        stat.objectCount++;
        stat.memberCount += v.size();
        stat.stringCount += v.size();   // member names
        break;

    case stringValue: 
        stat.stringCount++;
        stat.stringLength += v.asString().size();
        break;

    case intValue:
    case uintValue:
    case realValue:
        stat.numberCount++;
        break;

    case booleanValue:
        if (v.asBool())
            stat.trueCount++;
        else
            stat.falseCount++;
        break;

    case nullValue:
        stat.nullCount++;
        break;
    }
}

class JsoncppParseResult : public ParseResultBase {
public:
    Value root;
};

class JsoncppStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return s.c_str(); }

    std::string s;
};
class JsoncppTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "jsoncpp"; }
    virtual const char* Type()        const override { return "C++";}
    virtual const char* GetFilename() const override { return __FILE__; }

    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        (void)length;
        std::unique_ptr<JsoncppParseResult> pr = std::make_unique<JsoncppParseResult>();
        Json::CharReaderBuilder rbuilder;
        Json::Value settings;
        rbuilder.strictMode(&rbuilder.settings_);
        std::unique_ptr<CharReader> const reader(rbuilder.newCharReader());
        std::string errs;
        if (reader->parse(json, json + length, &pr->root, &errs)) {
            reply = std::move(pr);
        }
    	return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const JsoncppParseResult& pr = static_cast<const JsoncppParseResult&>(parseResult);
        Json::StreamWriterBuilder wbuilder;
        wbuilder.settings_["indentation"] = "";
        std::unique_ptr<JsoncppStringResult> sr = std::make_unique<JsoncppStringResult>();
        sr->s = Json::writeString(wbuilder, pr.root);
        reply = std::move(sr);
        return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const JsoncppParseResult& pr = static_cast<const JsoncppParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
        GenStat(stat, pr.root);
        return true;
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        CharReaderBuilder rbuilder;
        std::unique_ptr<Json::CharReader> const reader(rbuilder.newCharReader());
        std::size_t size = strlen(json);
        Value root;
        if (reader->parse(json, json + size, &root, nullptr) &&
            root.isArray() &&
            root.size() == 1 &&
            root[0].isDouble())
        {
            d = root[0].asDouble();
        }
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        CharReaderBuilder rbuilder;
        std::unique_ptr<Json::CharReader> const reader(rbuilder.newCharReader());
        std::size_t size = strlen(json);
        Value root;
        if (reader->parse(json, json + size, &root, nullptr) &&
            root.isArray() &&
            root.size() == 1 &&
            root[0].isString())
        {
            s = root[0].asString();
        }
        return true;
    }
};

REGISTER_TEST(JsoncppTest);

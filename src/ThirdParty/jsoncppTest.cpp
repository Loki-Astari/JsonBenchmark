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
    virtual const char* GetName() const { return "jsoncpp"; }
    virtual const char* Type()    const { return "C++";}
    virtual const char* GetFilename() const { return __FILE__; }

    virtual ParseResultBase* Parse(const char* json, size_t length) const {
        (void)length;
        JsoncppParseResult* pr = new JsoncppParseResult;
        Json::CharReaderBuilder rbuilder;
        Json::Value settings;
        rbuilder.strictMode(&rbuilder.settings_);
        std::unique_ptr<CharReader> const reader(rbuilder.newCharReader());
        std::string errs;
        if (!reader->parse(json, json + length, &pr->root, &errs)) {
            delete pr;
            return 0;
        }
    	return pr;
    }

    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const {
        const JsoncppParseResult* pr = static_cast<const JsoncppParseResult*>(parseResult);
        //FastWriter writer;
        Json::StreamWriterBuilder wbuilder;
        wbuilder.settings_["indentation"] = "";
        //std::unique_ptr<Json::StreamWriter> const writer(wbuilder.newStreamWriter());
        //writer->omitEndingLineFeed();
        JsoncppStringResult* sr = new JsoncppStringResult;
        sr->s = Json::writeString(wbuilder, pr->root);
        return sr;
    }

    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const {
        const JsoncppParseResult* pr = static_cast<const JsoncppParseResult*>(parseResult);
        memset(stat, 0, sizeof(Stat));
        GenStat(*stat, pr->root);
        return true;
    }

    virtual bool ParseDouble(const char* json, double* d) const {
        CharReaderBuilder rbuilder;
        std::unique_ptr<Json::CharReader> const reader(rbuilder.newCharReader());
        std::size_t size = strlen(json);
        Value root;
        if (reader->parse(json, json + size, &root, nullptr) &&
            root.isArray() &&
            root.size() == 1 &&
            root[0].isDouble())
        {
            *d = root[0].asDouble();
            return true;
        }
        else
            return false;
    }

    virtual bool ParseString(const char* json, std::string& s) const {
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
            return true;
        }
        else
            return false;
    }
};

REGISTER_TEST(JsoncppTest);

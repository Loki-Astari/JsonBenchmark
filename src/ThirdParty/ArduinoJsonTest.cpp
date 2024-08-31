#include "test.h"

#include <string>
#include <sstream>
#include <memory>

#define ARDUINOJSON_ENABLE_STD_STREAM 1
#include "ArduinoJson.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

static void GenStat(Stat& stat, const JsonVariant& v) {
    if (v.is<const JsonArray&>()) {
        const JsonArray& a = v.as<JsonArray>();
        for (JsonArray::const_iterator itr = a.begin(); itr != a.end(); ++itr)
            GenStat(stat, *itr);
        stat.arrayCount++;
        stat.elementCount += a.size();
    }
    else if (v.is<const JsonObject&>()) {
        const JsonObject& o = v.as<JsonObject>();
        for (JsonObject::const_iterator itr = o.begin(); itr != o.end(); ++itr) {
            GenStat(stat, itr->value);
            stat.stringLength += strlen(itr->key);
        }
        stat.objectCount++;
        stat.memberCount += o.size();
        stat.stringCount += o.size();
    }
    else if (v.is<const char*>()) {
        if (v.as<char*>()) {
            stat.stringCount++;
            stat.stringLength += strlen(v.as<char*>());
        }
        else
            stat.nullCount++; // JSON null value is treat as string null pointer
    }
    else if (v.is<long>() || v.is<double>())
        stat.numberCount++;
    else if (v.is<bool>()) {
        if ((bool)v)
            stat.trueCount++;
        else
            stat.falseCount++;
    }
}

class ArduinojsonParseResult : public ParseResultBase {
public:
    ArduinojsonParseResult(char const* json, size_t length)
    {
        buffer = (char*)malloc(length);
        memcpy(buffer, json, length);
    }
    ~ArduinojsonParseResult()
    {
        free(buffer);
    }

    char* buffer;
    DynamicJsonBuffer jsonBuffer;
    JsonVariant root;
};

class ArduinojsonTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "ArduinoJson"; }
    virtual const char* Type()        const override { return "C++";}
    virtual const char* GetFilename() const override { return __FILE__; }

    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& result) const override{
        std::unique_ptr<ArduinojsonParseResult> pr = std::make_unique<ArduinojsonParseResult>(json, length);

        // Determine object or array
        for (size_t i = 0; i < length; i++) {
            switch (json[i]) {
                case '{':
                    {
                        JsonObject& o = pr->jsonBuffer.parseObject(pr->buffer);
                        if (!o.success()) {
                            return true;
                        }
                        pr->root = o;
                    }
                    result = std::move(pr);
                    return true;
                case '[':
                    {
                        JsonArray& a = pr->jsonBuffer.parseArray(pr->buffer);
                        if (!a.success()) {
                            return true;
                        }
                        pr->root = a;
                    }
                    result = std::move(pr);
                    return true;
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    continue;
            }
            // Unknown first non-whitespace character
            break;
        }
        return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& result) const override
    {
        const ArduinojsonParseResult& pr = static_cast<const ArduinojsonParseResult&>(parseResult);
        std::unique_ptr<StringResultUsingStream> sr = std::make_unique<StringResultUsingStream>();
        StreamPrintAdapter adapter(sr->stream);
        pr.root.printTo(adapter);
        result = std::move(sr);
        return true;
    }

    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& result) const override
    {
        const ArduinojsonParseResult& pr = static_cast<const ArduinojsonParseResult&>(parseResult);
        std::unique_ptr<StringResultUsingStream> sr = std::make_unique<StringResultUsingStream>();
        StreamPrintAdapter adapter(sr->stream);
        pr.root.printTo(adapter);
        result = std::move(sr);
        return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const ArduinojsonParseResult& pr = static_cast<const ArduinojsonParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
        GenStat(stat, pr.root);
        return true;
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        ArduinojsonParseResult pr(json, strlen(json));
        JsonArray& a = pr.jsonBuffer.parseArray(pr.buffer);
        if (a.success() && a.size() == 1) {
            d = (double)a[0];
        }
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        ArduinojsonParseResult pr(json, strlen(json));
        JsonArray& a = pr.jsonBuffer.parseArray(pr.buffer);
        if (a.success() && a.size() == 1) {
            s = a[0].as<char*>();
        }
        return true;
    }
};

REGISTER_TEST(ArduinojsonTest);

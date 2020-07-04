#include "test.h"

#include "JsonBox/Convert.h"
#include "JsonBox/Escaper.h"
#include "JsonBox/IndentCanceller.h"
#include "JsonBox/Indenter.h"
#include "JsonBox/JsonParsingError.h"
#include "JsonBox/JsonWritingError.h"
#include "JsonBox/SolidusEscaper.h"
#include "JsonBox/Value.h"

#include <sstream>
#include <memory>
#include <string.h>

using namespace JsonBox;

static void GenStat(Stat& stat, const Value& v) {
    switch (v.getType()) {
    case Value::ARRAY:
        {
            const Array& a = v.getArray();
            for (Array::const_iterator itr = a.begin(); itr != a.end(); ++itr)
                GenStat(stat, *itr);
            stat.arrayCount++;
            stat.elementCount += a.size();
        }
        break;

    case Value::OBJECT:
        {
            const Object& o = v.getObject();
            for (Object::const_iterator itr = o.begin(); itr != o.end(); ++itr) {
                GenStat(stat, itr->second);
                stat.stringLength += itr->first.size();
            }
            stat.objectCount++;
            stat.memberCount += o.size();
            stat.stringCount += o.size();
        }
        break;

    case Value::STRING: 
        stat.stringCount++;
        stat.stringLength += v.getString().size();
        break;

    case Value::INTEGER:
    case Value::DOUBLE:
        stat.numberCount++;
        break;

    case Value::BOOLEAN:
        if (v.getBoolean())
            stat.trueCount++;
        else
            stat.falseCount++;
        break;

    case Value::NULL_VALUE:
        stat.nullCount++;
        break;
    case Value::UNKNOWN:
        break;
    }
}

class JsonboxParseResult : public ParseResultBase {
public:
    Value root;
};

class JsonboxStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return s.c_str(); }

    std::string s;
};
class JsonboxTest : public TestBase {
public:
    virtual const char* GetName() const { return "JsonBox-C++"; }
    virtual const char* GetFilename() const { return __FILE__; }

    virtual ParseResultBase* Parse(const char* json, size_t length) const {
        (void)length;
        JsonboxParseResult* pr = new JsonboxParseResult;
        try {
            pr->root.loadFromString(json);
        }
        catch (...) {
            delete pr;
            return 0;
        }
    	return pr;
    }

    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const {
        const JsonboxParseResult* pr = static_cast<const JsonboxParseResult*>(parseResult);
        JsonboxStringResult* sr = new JsonboxStringResult;
        std::ostringstream os;
        pr->root.writeToStream(os, false, false);
        sr->s = os.str();
        return sr;
    }

    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const {
        const JsonboxParseResult* pr = static_cast<const JsonboxParseResult*>(parseResult);
        JsonboxStringResult* sr = new JsonboxStringResult;
        std::ostringstream os;
        pr->root.writeToStream(os, true, false);
        sr->s = os.str();
        return sr;
    }

    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const {
        const JsonboxParseResult* pr = static_cast<const JsonboxParseResult*>(parseResult);
        memset(stat, 0, sizeof(Stat));
        GenStat(*stat, pr->root);
        return true;
    }

    virtual bool ParseDouble(const char* json, double* d) const {
        try {
            Value root;
            root.loadFromString(json);
            *d = root.getArray()[0].getDouble();
            return true;
        }
        catch (...) {
        }
        return false;
    }

    virtual bool ParseString(const char* json, std::string& s) const {
        try {
            Value root;
            root.loadFromString(json);
            s = root.getArray()[0].getString();
            return true;
        }
        catch (...) {
        }
        return false;
    }
};

REGISTER_TEST(JsonboxTest);


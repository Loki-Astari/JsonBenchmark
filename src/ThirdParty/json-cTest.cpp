#include "test.h"

#include "json-c/json.h"
#include <memory>
#include <string>
#include <string.h>

static void GenStat(Stat* s, json_object* v) {
    switch (json_object_get_type(v)) {
    case json_type_object:
        {
            json_object_object_foreach(v, key, val) {
                GenStat(s, val);
                s->stringCount++;
                s->stringLength += strlen(key);
                s->memberCount++;
            }
            s->objectCount++;
        }
        break;

    case json_type_array:
        for (int i = 0; i < json_object_array_length(v); i++)
            GenStat(s, json_object_array_get_idx(v, i));
        s->elementCount += json_object_array_length(v);
        s->arrayCount++;
        break;

    case json_type_string:
        s->stringCount++;
        s->stringLength += json_object_get_string_len(v);
        break;

    case json_type_int:
    case json_type_double:
        s->numberCount++;
        break;

    case json_type_boolean:
        if (json_object_get_boolean(v))
            s->trueCount++;
        else
            s->falseCount++;
        break;

        break;

    case json_type_null:
        s->nullCount++;
        break;
    }
}

class JsoncParseResult : public ParseResultBase {
public:
    JsoncParseResult() : root() {}
    virtual ~JsoncParseResult() { json_object_put(root); }

    json_object *root;
};

class JsoncStringResult : public StringResultBase {
public:
    JsoncStringResult() : s() {}
    ~JsoncStringResult() { free(s); }

    virtual const char* c_str() const { return s; }

    char* s;
};

class JsoncTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "json-c"; }
    virtual const char* Type()        const override { return "C";}
    virtual const char* GetFilename() const override { return __FILE__; }
	
    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        (void)length;
        std::unique_ptr<JsoncParseResult> pr = std::make_unique<JsoncParseResult>();
        pr->root = json_tokener_parse(json);
        if (pr->root) {
            reply = std::move(pr);
        }
    	return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const JsoncParseResult& pr = static_cast<const JsoncParseResult&>(parseResult);
        std::unique_ptr<JsoncStringResult> sr = std::make_unique<JsoncStringResult>();
        sr->s = strdup(json_object_to_json_string_ext(pr.root, JSON_C_TO_STRING_PLAIN));
        reply = std::move(sr);
        return true;
    }

    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const JsoncParseResult& pr = static_cast<const JsoncParseResult&>(parseResult);
        std::unique_ptr<JsoncStringResult> sr = std::make_unique<JsoncStringResult>();
        sr->s = strdup(json_object_to_json_string_ext(pr.root, JSON_C_TO_STRING_PRETTY));
        reply = std::move(sr);
        return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const JsoncParseResult& pr = static_cast<const JsoncParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
        GenStat(&stat, (json_object*)pr.root);
        return true;
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        JsoncParseResult pr;
        pr.root = json_tokener_parse(json);
        if (pr.root && 
            json_object_get_type(pr.root) == json_type_array &&
            json_object_array_length(pr.root) == 1 &&
            json_object_get_type(json_object_array_get_idx(pr.root, 0)) == json_type_double) 
        {
            d = json_object_get_double(json_object_array_get_idx(pr.root, 0));
        }
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        JsoncParseResult pr;
        pr.root = json_tokener_parse(json);
        if (pr.root && 
            json_object_get_type(pr.root) == json_type_array &&
            json_object_array_length(pr.root) == 1 &&
            json_object_get_type(json_object_array_get_idx(pr.root, 0)) == json_type_string) 
        {
            s = std::string(
                json_object_get_string(json_object_array_get_idx(pr.root, 0)),
                json_object_get_string_len(json_object_array_get_idx(pr.root, 0)));
        }
        return true;
    }
};

REGISTER_TEST(JsoncTest);

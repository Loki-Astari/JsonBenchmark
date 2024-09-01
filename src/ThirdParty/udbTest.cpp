#include "test.h"

#include "json-parser/json.h"
#include "json-builder.h"
#include <memory>
#include <string>

static void GenStat(Stat* s, const json_value* v) {
    switch (v->type) {
    case json_object:
        for (size_t i = 0; i < v->u.object.length; i++) {
            const json_object_entry* e = v->u.object.values + i;
            GenStat(s, e->value);
            s->stringLength += e->name_length;
        }
        s->stringCount += v->u.object.length;
        s->memberCount += v->u.object.length;
        s->objectCount++;
        break;

    case json_array:
        for (size_t i = 0; i < v->u.array.length; i++)
            GenStat(s, v->u.array.values[i]);
        s->arrayCount++;
        s->elementCount += v->u.array.length;
        break;

    case json_string:
        s->stringCount++;
        s->stringLength += v->u.string.length;
        break;

    case json_integer:
    case json_double:
        s->numberCount++; 
        break;

    case json_boolean:
        if (v->u.boolean)
            s->trueCount++;
        else
            s->falseCount++;
        break;

    case json_null:
        s->nullCount++;
        break;

    default:
        break;
    }
}

class UdpjsonParseResult : public ParseResultBase {
public:
    UdpjsonParseResult() : root() {}
    ~UdpjsonParseResult() { json_value_free(root); }

    json_value *root;
};

class UdpjsonStringResult : public StringResultBase {
public:
    UdpjsonStringResult() : s() {}
    ~UdpjsonStringResult() { free(s); }

    virtual const char* c_str() const { return s; }

    char* s;
};

class UdbTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "udb-jsason-parser";}
    virtual const char* Type()        const override { return "C";}
    virtual const char* GetFilename() const override { return __FILE__; }

    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        std::unique_ptr<UdpjsonParseResult> pr = std::make_unique<UdpjsonParseResult>();
        json_settings settings = json_settings();
        settings.value_extra = json_builder_extra;  /* space for json-builder state */
        char error[128];
        pr->root = json_parse_ex(&settings, json, length, error);
        if (pr->root) {
            reply = std::move(pr);
        }
    	return true;
    }

    // Very slow in the current version.
    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const UdpjsonParseResult& pr = static_cast<const UdpjsonParseResult&>(parseResult);
        std::unique_ptr<UdpjsonStringResult> sr = std::make_unique<UdpjsonStringResult>();
        json_serialize_opts opts = { json_serialize_mode_packed, 0, 0 };
        sr->s = (char*)malloc(json_measure_ex(pr.root, opts));
        json_serialize_ex(sr->s, pr.root, opts);
        reply = std::move(sr);
        return true;
    }

    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const UdpjsonParseResult& pr = static_cast<const UdpjsonParseResult&>(parseResult);
        std::unique_ptr<UdpjsonStringResult> sr = std::make_unique<UdpjsonStringResult>();
        json_serialize_opts opts = { json_serialize_mode_multiline, 0, 4 };
        sr->s = (char*)malloc(json_measure_ex(pr.root, opts));
        json_serialize_ex(sr->s, pr.root, opts);
        reply = std::move(sr);
        return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const UdpjsonParseResult& pr = static_cast<const UdpjsonParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
        GenStat(&stat, pr.root);
        return true;
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        UdpjsonParseResult pr;
        json_settings settings = json_settings();
        settings.value_extra = json_builder_extra;  /* space for json-builder state */
        char error[128];
        pr.root = json_parse_ex(&settings, json, strlen(json), error);
        if (pr.root &&
            pr.root->type == json_array &&
            pr.root->u.array.length == 1 &&
            pr.root->u.array.values[0]->type == json_double)
        {
            d = pr.root->u.array.values[0]->u.dbl;
        }
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        UdpjsonParseResult pr;
        json_settings settings = json_settings();
        settings.value_extra = json_builder_extra;  /* space for json-builder state */
        char error[128];
        pr.root = json_parse_ex(&settings, json, strlen(json), error);
        if (pr.root &&
            pr.root->type == json_array &&
            pr.root->u.array.length == 1 &&
            pr.root->u.array.values[0]->type == json_string)
        {
            s = std::string(
                pr.root->u.array.values[0]->u.string.ptr,
                pr.root->u.array.values[0]->u.string.length);
        }
        return true;
    }
};

REGISTER_TEST(UdbTest);

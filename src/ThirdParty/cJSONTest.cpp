#include "test.h"
#include "cJSON/cJSON.h"
#include <memory>
#include <string>
#include <stdlib.h>
#include <string.h>

static void GenStat(Stat* s, const cJSON* v) {
    if (cJSON_IsObject(v)) {
        for (cJSON* child = v->child; child != 0; child = child->next) {
            GenStat(s, child);
            s->stringCount++;
            s->stringLength += strlen(child->string);
            s->memberCount++;
        }
        s->objectCount++;
    } else if (cJSON_IsArray(v)) {
        for (cJSON* child = v->child; child != 0; child = child->next) {
            GenStat(s, child);
            s->elementCount++;
        }
        s->arrayCount++;
    } else if (cJSON_IsString(v)) {
        s->stringCount++;
        s->stringLength += strlen(v->valuestring);
    } else if (cJSON_IsNumber(v)) {
        s->numberCount++;
    } else if (cJSON_IsTrue(v)) {
        s->trueCount++;
    } else if (cJSON_IsFalse(v)) {
        s->falseCount++;
    } else if (cJSON_IsNull(v)) {
        s->nullCount++;
    }
}

class CjsonParseResult : public ParseResultBase {
public:
    CjsonParseResult() : root() {}
    ~CjsonParseResult() { cJSON_Delete(root); }

    cJSON *root;
};

class CjsonStringResult : public StringResultBase {
public:
    CjsonStringResult() : s() {}
    ~CjsonStringResult() { free(s); }

    virtual const char* c_str() const { return s; }

    char* s;
};

class CjsonTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "cJSON"; }
    virtual const char* Type()        const override { return "C";}
    virtual const char* GetFilename() const override { return __FILE__; }

    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        (void)length;
        std::unique_ptr<CjsonParseResult> pr = std::make_unique<CjsonParseResult>();
        pr->root = cJSON_ParseWithOpts(json, nullptr, static_cast<cJSON_bool>(true));
        if (pr->root != nullptr) {
            reply = std::move(pr);
        }
    	return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const CjsonParseResult& pr = static_cast<const CjsonParseResult&>(parseResult);
        std::unique_ptr<CjsonStringResult> sr = std::make_unique<CjsonStringResult>();
        sr->s = cJSON_PrintBuffered(pr.root, 4096, static_cast<cJSON_bool>(false));
        reply = std::move(sr);
        return true;
    }

    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const CjsonParseResult& pr = static_cast<const CjsonParseResult&>(parseResult);
        std::unique_ptr<CjsonStringResult> sr = std::make_unique<CjsonStringResult>();
        sr->s = cJSON_PrintBuffered(pr.root, 4096, static_cast<cJSON_bool>(true));
        reply = std::move(sr);
        return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const CjsonParseResult& pr = static_cast<const CjsonParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
        GenStat(&stat, pr.root);
        return true;
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        CjsonParseResult pr;
        pr.root = cJSON_Parse(json);
        if ((pr.root != nullptr) && cJSON_IsArray(pr.root) && cJSON_IsNumber(pr.root->child)) {
            d = pr.root->child->valuedouble;
        }
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        CjsonParseResult pr;
        pr.root = cJSON_Parse(json);
        if ((pr.root != nullptr) && cJSON_IsArray(pr.root) && cJSON_IsString(pr.root->child)) {
            s = pr.root->child->valuestring;
        }
        return true;
    }
};

REGISTER_TEST(CjsonTest);

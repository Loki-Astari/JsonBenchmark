#include "test.h"

extern "C" {
#include "ccan/json/json.h"
}

#include <memory>
#include <string>
#include <stdlib.h>
#include <string.h>

static void GenStat(Stat* s, const JsonNode* v) {
    switch (v->tag) {
    case JSON_OBJECT:
        {
            JsonNode* child;
            json_foreach(child, v) {
                GenStat(s, child);
                s->stringCount++;
                s->stringLength += strlen(child->key);
                s->memberCount++;
            }
        }
        s->objectCount++;
        break;

    case JSON_ARRAY:
        {
            JsonNode* child;
            json_foreach(child, v) {
                GenStat(s, child);
                s->elementCount++;
            }
        }
        s->arrayCount++;
        break;

    case JSON_STRING:
        s->stringCount++;
        s->stringLength += strlen(v->string_);
        break;

    case JSON_NUMBER:
        s->numberCount++; 
        break;

    case JSON_BOOL:
        if (v->bool_)
            s->trueCount++;
        else
            s->falseCount++;
        break;

    case JSON_NULL:
        s->nullCount++;
        break;
    }
}

class CcanParseResult : public ParseResultBase {
public:
    CcanParseResult() : root() {}
    ~CcanParseResult() { json_delete(root); }

    JsonNode *root;
};

class CcanStringResult : public StringResultBase {
public:
    CcanStringResult() : s() {}
    ~CcanStringResult() { free(s); }

    virtual const char* c_str() const { return s; }

    char* s;
};

class CcanTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "ccan"; }
    virtual const char* Type()        const override { return "C";}
    virtual const char* GetFilename() const override { return __FILE__; }
	
    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        (void)length;
        std::unique_ptr<CcanParseResult> pr = std::make_unique<CcanParseResult>();
        pr->root = json_decode(json);
        if (pr->root) {
            reply = std::move(pr);
        }
    	return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const CcanParseResult& pr = static_cast<const CcanParseResult&>(parseResult);
        std::unique_ptr<CcanStringResult> sr = std::make_unique<CcanStringResult>();
        sr->s = json_encode(pr.root);
        reply = std::move(sr);
        return true;
    }

    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const CcanParseResult& pr = static_cast<const CcanParseResult&>(parseResult);
        std::unique_ptr<CcanStringResult> sr = std::make_unique<CcanStringResult>();
        sr->s = json_stringify(pr.root, "    ");
        reply = std::move(sr);
        return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const CcanParseResult& pr = static_cast<const CcanParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
        GenStat(&stat, pr.root);
        return true;
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        CcanParseResult pr;
        pr.root = json_decode(json);
        if (pr.root && pr.root->tag == JSON_ARRAY && pr.root->children.head->tag == JSON_NUMBER) {
            d = pr.root->children.head->number_;
        }
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        CcanParseResult pr;
        pr.root = json_decode(json);
        if (pr.root && pr.root->tag == JSON_ARRAY && pr.root->children.head->tag == JSON_STRING) {
            s = pr.root->children.head->string_;
        }
        return true;
    }
};

REGISTER_TEST(CcanTest);

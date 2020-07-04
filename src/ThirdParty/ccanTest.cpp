#include "test.h"

extern "C" {
#include "ccan/json.h"
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
    virtual const char* GetName() const { return "ccan-C"; }
    virtual const char* GetFilename() const { return __FILE__; }
	
    virtual ParseResultBase* Parse(const char* json, size_t length) const {
        (void)length;
        CcanParseResult* pr = new CcanParseResult;
        pr->root = json_decode(json);
        if (!pr->root) {
            delete pr;
            return 0;
        }
    	return pr;
    }

    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const {
        const CcanParseResult* pr = static_cast<const CcanParseResult*>(parseResult);
        CcanStringResult* sr = new CcanStringResult;
        sr->s = json_encode(pr->root);
        return sr;
    }

    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const {
        const CcanParseResult* pr = static_cast<const CcanParseResult*>(parseResult);
        CcanStringResult* sr = new CcanStringResult;
        sr->s = json_stringify(pr->root, "    ");
        return sr;
    }

    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const {
        const CcanParseResult* pr = static_cast<const CcanParseResult*>(parseResult);
        memset(stat, 0, sizeof(Stat));
        GenStat(stat, pr->root);
        return true;
    }

    virtual bool ParseDouble(const char* json, double* d) const {
        CcanParseResult pr;
        pr.root = json_decode(json);
        if (pr.root && pr.root->tag == JSON_ARRAY && pr.root->children.head->tag == JSON_NUMBER) {
            *d = pr.root->children.head->number_;
            return true;
        }
        else
            return false;
    }

    virtual bool ParseString(const char* json, std::string& s) const {
        CcanParseResult pr;
        pr.root = json_decode(json);
        if (pr.root && pr.root->tag == JSON_ARRAY && pr.root->children.head->tag == JSON_STRING) {
            s = pr.root->children.head->string_;
            return true;
        }
        else
            return false;
    }
};

REGISTER_TEST(CcanTest);

#include "test.h"
#include "jsmn/jsmn.h"
#include <memory>
#include <string>
#include <stdlib.h>
#include <string.h>

static void GenStat(Stat* s, const char* json, const jsmntok_t* tokens, int count) {
    for (int i = 0; i < count; i++)
    switch (tokens[i].type) {
    case JSMN_OBJECT:
        s->memberCount += tokens[i].size;
        s->objectCount++;
        break;

    case JSMN_ARRAY:
        s->elementCount += tokens[i].size;
        s->arrayCount++;
        break;

    case JSMN_STRING:
        s->stringCount++;
        s->stringLength += tokens[i].end - tokens[i].start;
        break;

    case JSMN_PRIMITIVE:
        if (json[tokens[i].start] == 't')
            s->trueCount++;
        else if (json[tokens[i].start] == 'f')
            s->falseCount++;
        else if (json[tokens[i].start] == 'n')
            s->nullCount++;
        else
            s->numberCount++; 
        break;

    default:;
    }
}

// Since jsmn does not parse numbers, emulate here in order to compare with other parsers.
static void ParseNumbers(const char* json, const jsmntok_t* tokens, int count) {
    for (int i = 0; i < count; i++)
        if (tokens[i].type == JSMN_PRIMITIVE) {
            const char* s = json + tokens[i].start;
            if (*s != 't' && *s != 'f' && *s != 'n')
                (void)atof(s);
        }
}

class JsmnParseResult : public ParseResultBase {
public:
    JsmnParseResult() : json(), tokens() {}
    ~JsmnParseResult() { free(json); free(tokens); }

    char* json;
    jsmntok_t* tokens;
    int count;
};

class JsmnTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "jsmn"; }
    virtual const char* Type()        const override { return "C";}
    virtual const char* GetFilename() const override { return __FILE__; }
	
    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        std::unique_ptr<JsmnParseResult> pr = std::make_unique<JsmnParseResult>();
        jsmn_parser parser;
        jsmn_init(&parser);
        pr->count = jsmn_parse(&parser, json, length, NULL, 0);
        if (pr->count < 0) {
            return true;
        }

        jsmn_init(&parser);
        pr->tokens = (jsmntok_t*)malloc(pr->count * sizeof(jsmntok_t));
        int error = jsmn_parse(&parser, json, length, pr->tokens, pr->count);
        if (error < 0) {
            return true;
        }

        // need a copy of JSON in order to determine the types
        pr->json = strdup(json);

        // Since jsmn does not parse numbers, emulate here in order to compare with other parsers.
        ParseNumbers(json, pr->tokens, pr->count);
        reply = std::move(pr);
    	return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const JsmnParseResult& pr = static_cast<const JsmnParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
        GenStat(&stat, pr.json, pr.tokens, pr.count);
        return true;
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        jsmn_parser parser;
        jsmn_init(&parser);
        jsmntok_t tokens[2];
        int count = jsmn_parse(&parser, json, strlen(json), tokens, 2);
        if (count == 2 && tokens[0].type == JSMN_ARRAY && tokens[0].size == 1 && tokens[1].type == JSMN_PRIMITIVE) {
            char*   strend = nullptr;
            d = strtold(json + tokens[1].start, &strend);
        }
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        jsmn_parser parser;
        jsmn_init(&parser);
        jsmntok_t tokens[2];
        int count = jsmn_parse(&parser, json, strlen(json), tokens, 2);
        if (count == 2 && tokens[0].type == JSMN_ARRAY && tokens[0].size == 1 && tokens[1].type == JSMN_STRING) {
            s = std::string(json + tokens[1].start, json + tokens[1].end);
        }
        return true;
    }
};

REGISTER_TEST(JsmnTest);

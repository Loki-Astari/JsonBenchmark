#include "test.h"

#include "Jzon.h"
#include <memory>
#include <string.h>

using namespace Jzon;

static void GenStat(Stat& stat, const Node& v) {
    switch (v.getType()) {
    case Node::T_ARRAY:
        for (Node::const_iterator itr = v.begin(); itr != v.end(); ++itr)
            GenStat(stat, (*itr).second);
        stat.arrayCount++;
        stat.elementCount += v.getCount();
        break;

    case Node::T_OBJECT:
        for (Node::const_iterator itr = v.begin(); itr != v.end(); ++itr) {
            GenStat(stat, (*itr).second);
            stat.stringLength += (*itr).first.size();
        }
        stat.objectCount++;
        stat.memberCount += v.getCount();
        stat.stringCount += v.getCount();
        break;

    case Node::T_STRING: 
        stat.stringCount++;
        stat.stringLength += v.toString().size();
        break;

    case Node::T_NUMBER:
        stat.numberCount++;
        break;

    case Node::T_BOOL:
        if (v.toBool())
            stat.trueCount++;
        else
            stat.falseCount++;
        break;

    case Node::T_NULL:
        stat.nullCount++;
        break;
    case Node::T_INVALID:
        break;
    }
}

class JzonParseResult : public ParseResultBase {
public:
    Node root;
};

class JzonStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return s.c_str(); }
    std::string s;
};

class JzonTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "Jzon"; }
    virtual const char* Type()        const override { return "C++";}
    virtual const char* GetFilename() const override { return __FILE__; }

    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        (void)length;
        std::unique_ptr<JzonParseResult> pr = std::make_unique<JzonParseResult>();
        Parser parser;
        pr->root = parser.parseString(json);
        if (parser.getError().empty()) {
            reply = std::move(pr);
        }
        return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const JzonParseResult& pr = static_cast<const JzonParseResult&>(parseResult);
        std::unique_ptr<JzonStringResult> sr = std::make_unique<JzonStringResult>();
        Writer writer;
        writer.writeString(pr.root, sr->s);
        reply = std::move(sr);
        return true;
    }

    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const JzonParseResult& pr = static_cast<const JzonParseResult&>(parseResult);
        std::unique_ptr<JzonStringResult> sr = std::make_unique<JzonStringResult>();
        const Format format = { true, true, true, 4 };
        Writer writer(format);
        writer.writeString(pr.root, sr->s);
        reply = std::move(sr);
        return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const JzonParseResult& pr = static_cast<const JzonParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
        GenStat(stat, pr.root);
        return true;
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        Parser parser;
        Node root = parser.parseString(json);
        if (parser.getError().empty() && root.isArray() && root.getCount() == 1 && root.get(0).isNumber()) {
            d = root.get(0).toDouble();
        }
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        Parser parser;
        Node root = parser.parseString(json);
        if (parser.getError().empty() && root.isArray() && root.getCount() == 1 && root.get(0).isString()) {
            s = root.get(0).toString();
        }
        return true;
    }
};

REGISTER_TEST(JzonTest);

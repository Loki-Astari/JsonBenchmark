#include "test.h"

#include "jvar/include/jvar.h"
#include "jvar/include/util.h"
#include "jvar/include/var.h"
#include "jvar/include/str.h"
#include "jvar/include/json.h"
#include "jvar/include/arr.h"

#include <memory>

using namespace jvar;

static void GenStat(Stat& stat, Variant* v)
{
    switch (v->type())
    {
    case Variant::V_ARRAY:
        {
            for (Iter<Variant> i; v->forEach(i); )
            {
                GenStat(stat, &i);
            }
            stat.arrayCount++;
            stat.elementCount += v->length();
        }
        break;

    case Variant::V_OBJECT:
        for (Iter<Variant> i; v->forEach(i); )
        {
            GenStat(stat, &i);
            stat.stringLength += strlen(i.key());
        }

        stat.objectCount++;
        stat.memberCount += v->length();
        stat.stringCount += v->length();
        break;

    case Variant::V_STRING:
        stat.stringCount++;
        stat.stringLength += v->s().length();
        break;

    case Variant::V_INT:
    case Variant::V_DOUBLE:
        stat.numberCount++;
        break;

    case Variant::V_BOOL:
        if (v->toBool())
        {
            stat.trueCount++;
        }
        else
        {
            stat.falseCount++;
        }
        break;

    case Variant::V_NULL:
    case Variant::V_EMPTY:
        stat.nullCount++;
        break;

    default:
        break;
    }
}

class JvarParseResult : public ParseResultBase
{
public:
    Variant mVar;
};

class JvarStringResult : public StringResultBase
{
public:
    virtual const char* c_str() const
    {
        return mStr.c_str();
    }

    std::string mStr;
};

class JvarTest : public TestBase {
public:
    virtual const char* GetName() const { return "jvar"; }
    virtual const char* Type()    const { return "C++";}
    virtual const char* GetFilename() const { return __FILE__; }

    virtual ParseResultBase* Parse(const char* json, size_t /*length*/) const
    {
        JvarParseResult* p = new JvarParseResult;
        if (!p->mVar.parseJson(json))
        {
            delete p;
            p = NULL;;
        }
        return p;
    }

    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const
    {
        const JvarParseResult* p = (const JvarParseResult*)parseResult;
        JvarStringResult* sr = new JvarStringResult;
        sr->mStr = p->mVar.toString();
        return sr;
    }

    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const
    {
        const JvarParseResult* p = (const JvarParseResult*)parseResult;
        memset(stat, 0, sizeof(Stat));
        GenStat(*stat, (Variant*)&p->mVar);
        return true;
    }

    virtual bool ParseDouble(const char* json, double* d) const
    {
        Variant v;
        if (v.parseJson(json) &&
            v.isArray() &&
            v.length() == 1)
        {
            if (v[0].type() == Variant::V_DOUBLE || v[0].type() == Variant::V_INT)
            {
                *d = v[0].toDouble();
                return true;
            }
        }
        return false;

    }

    virtual bool ParseString(const char* json, std::string& s) const
    {
        Variant v;
        if (v.parseJson(json) &&
            v.isArray() &&
            v.length() == 1 &&
            v[0].type() == Variant::V_STRING)
        {
            s = v[0].toString();
            return true;
        }
        else
        {
            return false;
        }
    }
};

REGISTER_TEST(JvarTest);

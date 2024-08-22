#ifndef THORS_SERIALIZER_COMMON_H
#define THORS_SERIALIZER_COMMON_H

class TestAction
{
    public:
    virtual ParseResultBase* Parse(const char*, size_t) const {
        return nullptr;
    }
    virtual bool ParseDouble(const char*, double*) const {
        return false;
    }
    virtual bool ParseString(const char*, std::string&) const {
        return false;
    }
    virtual StringResultBase* SaxRoundtrip(const char*, size_t) const {
        return nullptr;
    }
    virtual StringResultBase* Stringify(const ParseResultBase*) const {
        return nullptr;
    }
    virtual StringResultBase* Prettify(const ParseResultBase*) const {
        return nullptr;
    }
    virtual bool Statistics(const ParseResultBase*, Stat*) const {
        return false;
    }
    virtual bool SaxStatistics(const char*, size_t, Stat*) const {
        return false;
    }
};

using ThorsAnvil::Serialize::jsonExporter;
using ThorsAnvil::Serialize::jsonImporter;
using ThorsAnvil::Serialize::ParseType;
using ThorsAnvil::Serialize::OutputType;

class VectorDouble: public TestAction
{
    public:
    virtual bool ParseDouble(const char* json, double* d) const {
        std::stringstream stream(json);
        std::vector<double> result;
        stream >> jsonImporter(result);
        if (stream && result.size() == 1) {
            *d = result[0];
            return true;
        }
        return false;
    }
};
class VectorString: public TestAction
{
    public:
    virtual bool ParseString(const char* json, std::string& output) const {
        std::stringstream stream(json);
        std::vector<std::string> result;
        stream >> jsonImporter(result);
        if (stream && result.size() == 1) {
            output = result[0];
            return true;
        }
        return false;
    }
};


template<typename Value>
struct GetValueResult: public ParseResultBase
{
    Value       data;
    GetValueResult()
        : data{}
    {}
};
struct GetValueStream: public StringResultBase {
    std::stringstream   stream;
    virtual const char* c_str() const   {return stream.str().c_str();}
};
template<typename Value>
class GetValue: public TestAction
{
    public:
    virtual ParseResultBase* Parse(const char* json, size_t) const {
        GetValueResult<Value>* result = new GetValueResult<Value>();
        std::stringstream stream(json);
        stream >> jsonImporter(result->data);
        char bad;
        if (!stream || stream >> bad) {
            delete result;
            result = nullptr;
        }
        return result;
    }
    virtual StringResultBase* Stringify(ParseResultBase const* value) const {
        GetValueStream* result = new GetValueStream;
        GetValueResult<Value> const* inputValue = dynamic_cast<GetValueResult<Value> const*>(value);
        result->stream << jsonExporter(inputValue->data, OutputType::Stream);
        return result;
    }
    virtual StringResultBase* Prettify(const ParseResultBase* value) const {
        GetValueStream* result = new GetValueStream;
        GetValueResult<Value> const* inputValue = dynamic_cast<GetValueResult<Value> const*>(value);
        result->stream << jsonExporter(inputValue->data, OutputType::Config);
        return result;
    }
    virtual bool Statistics(const ParseResultBase* value, Stat* stat) const {
        GetValueResult<Value> const* inputValue = dynamic_cast<GetValueResult<Value> const*>(value);
        getStats(stat, inputValue->data);
        return true;
    }
    virtual StringResultBase* SaxRoundtrip(const char* json, size_t length) const {
        std::unique_ptr<ParseResultBase> dom(Parse(json, length));
        if (!dom)
        {   return nullptr;
        }
        return Stringify(dom.get());
    }
    virtual bool SaxStatistics(const char* json, size_t length, Stat* stat) const {
        std::unique_ptr<ParseResultBase> dom(Parse(json, length));
        if (!dom)
        {   return false;
        }
        return Statistics(dom.get(), stat);
    }
};

using ActionMap = std::map<std::string, TestAction*>;
using   L01= std::vector<std::string>;
using   L02= std::vector<L01>;
using   L03= std::vector<L02>;
using   L04= std::vector<L03>;
using   L05= std::vector<L04>;
using   L06= std::vector<L05>;
using   L07= std::vector<L06>;
using   L08= std::vector<L07>;
using   L09= std::vector<L08>;
using   L10= std::vector<L09>;
using   L11= std::vector<L10>;
using   L12= std::vector<L11>;
using   L13= std::vector<L12>;
using   L14= std::vector<L13>;
using   L15= std::vector<L14>;
using   L16= std::vector<L15>;
using   L17= std::vector<L16>;
using   L18= std::vector<L17>;
using   M01= std::map<std::string, std::string>;

struct Empty {};
ThorsAnvil_MakeTrait(Empty);
struct Special
{
    int                 integer;
    double              real;
    double              e;
    double              E;
    double              emptyKey;      // "":  23456789012E66,
    int                 zero;
    int                 one;
    std::string         space;
    std::string         quote;
    std::string         backslash;
    std::string         controls;
    std::string         slash;
    std::string         alpha;
    std::string         ALPHA;
    std::string         digit;
    std::string         number;     // "0123456789": "digit",
    std::string         special;
    std::string         hex;
    bool                aTrue;      // "true": true,
    bool                aFalse;     // "false": false,
    int*                null;
    std::vector<int>    array;
    Empty               object;
    std::string         address;
    std::string         url;
    std::string         comment;
    std::string         commentKey; // "# -- --> */": " ",
    std::vector<int>    spaced;     // " s p a c e d " :[1,2 , 3
    std::vector<int>    compact;
    std::string         jsontext;
    std::string         quotes;
    std::string         key;        // "\/\\\"\uCAFE\uBABE\uAB98\uFCDE\ubcda\uef4A\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?" : "A key can be any string"
};
ThorsAnvil_MakeOverride(Special,    {"emptyKey", ""},
                                    {"number", "0123456789"},
                                    {"aTrue", "true"},
                                    {"aFalse", "false"},
                                    {"commentKey", "# -- --> */"},
                                    {"spaced", " s p a c e d "},
                                    {"key", "/\\\"\uCAFE\uBABE\uAB98\uFCDE\ubcda\uef4A\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?"}
                       );
ThorsAnvil_MakeTrait(Special,       integer, real, e, E, emptyKey, zero, one, space, quote, backslash,
                                    controls, slash, alpha, ALPHA, digit, number, special, hex,
                                    aTrue, aFalse, null, array, object, address, url, comment,
                                    commentKey, spaced, compact, jsontext, quotes, key);
using Pass01 = std::tuple<
    std::string,
    std::map<std::string, std::vector<std::string>>,
    Empty,
    std::vector<int>,
    int,
    bool,
    bool,
    int*,
    Special,
    double, double,
    double,
    int,
    double,
    double,
    double,
    double,double,double,
    std::string
>;


inline void getStats(Stat* stat, Special const& value)
{
    stat->objectCount   += 1;   // Empty
    stat->arrayCount    += 3;   // std::vector<int>
    stat->numberCount   += 7;
    stat->stringCount   += 18;
    stat->trueCount     += 1;
    stat->falseCount    += 1;
    stat->nullCount     += 1;

    stat->memberCount   += 0;
    stat->elementCount  += value.array.size() + value.spaced.size() + value.compact.size();
    stat->stringLength  += value.space.size() + value.quote.size() + value.backslash.size() +
                           value.controls.size() + value.slash.size() + value.alpha.size() +
                           value.ALPHA.size() + value.digit.size() + value.number.size() +
                           value.special.size() + value.hex.size() + value.address.size() +
                           value.url.size() + value.comment.size() + value.commentKey.size() +
                           value.spaced.size() + value.compact.size() + value.jsontext.size() +
                           value.quotes.size() + value.key.size();
}

inline void getStats(Stat* stat, Pass01 const& value)
{
    stat->objectCount   += 3;   // std::map<std::string, std::vector<std::string>> + Empty + Special
    stat->arrayCount    += 1;   // std::vector<int>
    stat->numberCount   += 11;
    stat->stringCount   += 2;
    stat->trueCount     += 1;
    stat->falseCount    += 1;
    stat->nullCount     += 1;

    stat->memberCount   += std::get<1>(value).size() + 0 + /*Special*/32;   // Number of members in all objects
    stat->elementCount  += std::get<3>(value).size();  // Number of elements in all arrays
    stat->stringLength  += std::get<0>(value).size() + std::get<19>(value).size();  // Number of code units in all strings

    getStats(stat, std::get<8>(value));
}

inline void getStats(Stat* stat, std::map<std::string, M01> const& value)
{
    stat->objectCount++;
    stat->memberCount += value.size();
    for(auto const& da: value) {
        stat->stringLength += da.first.size();
        getStats(stat, da.second);
    }
}

struct Obj2
{
    std::string     foo;
};
ThorsAnvil_MakeTrait(Obj2, foo);
inline void getStats(Stat* stat, Obj2 const& value)
{
    stat->objectCount++;
    stat->elementCount++;
    stat->stringCount += 2; /* Key + Value */
    stat->stringLength += (3 /*foo*/ + value.foo.size());
}

struct Obj3
{
    Opt<int>        a;
    std::string     foo;
};
ThorsAnvil_MakeTrait(Obj3, a, foo);
inline void getStats(Stat* stat, Obj3 const& value)
{
    stat->objectCount++;
    stat->elementCount += 2;
    ((value.a) ? stat->numberCount : stat->nullCount)++;
    stat->stringCount += 3; /*Key + Key + Value*/
    stat->stringLength += (3 /*foo*/ + 1 /*a*/ + value.foo.size());
}

#endif

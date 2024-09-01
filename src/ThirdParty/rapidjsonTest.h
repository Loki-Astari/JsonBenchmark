#ifndef RAPID_JSON_TEST_H
#define RAPID_JSON_TEST_H

#include "test.h"
#include <string>
#include <memory>

// __SSE2__ and __SSE4_2__ are recognized by gcc, clang, and the Intel compiler.
// We use -march=native with gmake to enable -msse2 and -msse4.2, if supported.
#if defined(__SSE4_2__)
#  define RAPIDJSON_SSE42      
#elif defined(__SSE2__)        
#  define RAPIDJSON_SSE2
#elif defined(_MSC_VER) // Turn on SSE4.2 for VC
#  define RAPIDJSON_SSE42
#endif

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace rapidjson;

template <typename Encoding = UTF8<> >
class StatHandler : public BaseReaderHandler<Encoding, StatHandler<Encoding> > {
public:
    typedef typename Encoding::Ch Ch;

    StatHandler(Stat& stat) : stat_(stat) {}

    bool Null() { stat_.nullCount++; return true; }
    bool Bool(bool b) { if (b) stat_.trueCount++; else stat_.falseCount++; return true; }
    bool Int(int) { stat_.numberCount++; return true; }
    bool Uint(unsigned) { stat_.numberCount++; return true; }
    bool Int64(int64_t) { stat_.numberCount++; return true; }
    bool Uint64(uint64_t) { stat_.numberCount++; return true; }
    bool Double(double) { stat_.numberCount++; return true; }
    bool String(const Ch*, SizeType length, bool) { stat_.stringCount++; stat_.stringLength += length; return true; }
    bool StartObject() { return true; }
    bool EndObject(SizeType memberCount) { stat_.objectCount++; stat_.memberCount += memberCount; return true; }
    bool StartArray() { return true; }
    bool EndArray(SizeType elementCount) { stat_.arrayCount++; stat_.elementCount += elementCount; return true; }

private:
    StatHandler& operator=(const StatHandler&);

    Stat& stat_;
};

static void GenStat(Stat& stat, const Value& v) {
    switch (v.GetType()) {
    case kNullType:  stat.nullCount++; break;
    case kFalseType: stat.falseCount++; break;
    case kTrueType:  stat.trueCount++; break;

    case kObjectType:
        for (Value::ConstMemberIterator m = v.MemberBegin(); m != v.MemberEnd(); ++m) {
            stat.stringLength += m->name.GetStringLength();
            GenStat(stat, m->value);
        }
        stat.objectCount++;
        stat.memberCount += (v.MemberEnd() - v.MemberBegin());
        stat.stringCount += (v.MemberEnd() - v.MemberBegin()); // Key
        break;

    case kArrayType:
        for (Value::ConstValueIterator i = v.Begin(); i != v.End(); ++i)
            GenStat(stat, *i);
        stat.arrayCount++;
        stat.elementCount += v.Size();
        break;

    case kStringType:
        stat.stringCount++;
        stat.stringLength += v.GetStringLength();
        break;

    case kNumberType:
        stat.numberCount++;
        break;
    }
}

class RapidjsonParseResult : public ParseResultBase {
public:
    RapidjsonParseResult(const char* json, size_t length) : buffer() {
        (void)json;
        (void)length;
#ifdef TEST_INSITU
        buffer = (char*)malloc(length + 1);
        memcpy(buffer, json, length + 1);
#endif
    }
    ~RapidjsonParseResult() { free(buffer); }
    Document document;
    char* buffer;
};

class RapidjsonStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return sb.GetString(); }

    StringBuffer sb;
};

class TEST_CLASS : public TestBase {
public:
    virtual const char* GetName()     const override { return TEST_NAME; }
    virtual const char* Type()        const override { return "C++";}
    virtual const char* GetFilename() const override { return __FILE__; }

    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        (void)length;
        std::unique_ptr<RapidjsonParseResult> pr = std::make_unique<RapidjsonParseResult>(json, length);
#ifdef TEST_INSITU
        pr->document.ParseInsitu<TEST_PARSE_FLAG>(pr->buffer);
#else
        pr->document.Parse<TEST_PARSE_FLAG>(json);
#endif
        if (!pr->document.HasParseError()) {
            reply = std::move(pr);
        }
        return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const RapidjsonParseResult& pr = static_cast<const RapidjsonParseResult&>(parseResult);
        std::unique_ptr<RapidjsonStringResult> sr = std::make_unique<RapidjsonStringResult>();
        Writer<StringBuffer> writer(sr->sb);
        pr.document.Accept(writer);
        reply = std::move(sr);
        return true;
    }

    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const RapidjsonParseResult& pr = static_cast<const RapidjsonParseResult&>(parseResult);
        std::unique_ptr<RapidjsonStringResult> sr = std::make_unique<RapidjsonStringResult>();
        PrettyWriter<StringBuffer> writer(sr->sb);
        pr.document.Accept(writer);
        reply = std::move(sr);
        return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const RapidjsonParseResult& pr = static_cast<const RapidjsonParseResult&>(parseResult);
        memset(&stat, 0, sizeof(Stat));
#if SLOWER_STAT
        StatHandler<> h(stat);
        doc->Accept(h);
#else
        GenStat(stat, pr.document);
#endif
        return true;
    }

    virtual bool SaxRoundtrip(const char* json, size_t length, std::unique_ptr<StringResultBase>& reply) const override
    {
        (void)length;
        Reader reader;
        std::unique_ptr<RapidjsonStringResult> sr = std::make_unique<RapidjsonStringResult>();
        Writer<StringBuffer> writer(sr->sb);

#ifdef TEST_INSITU
        RapidjsonParseResult pr(json, length);
        InsituStringStream is(pr.buffer);
#else
        StringStream is(json);
#endif

        if (reader.Parse<TEST_PARSE_FLAG>(is, writer)) {
            reply = std::move(sr);
        }
        return true;
    }

    virtual bool SaxStatistics(const char* json, size_t length, Stat& stat) const override
    {
        (void)length;
        memset(&stat, 0, sizeof(Stat));
        Reader reader;

#ifdef TEST_INSITU
        RapidjsonParseResult pr(json, length);
        InsituStringStream is(pr.buffer);
#else
        StringStream is(json);
#endif

        StatHandler<> handler(stat);
        return reader.Parse<TEST_PARSE_FLAG>(is, handler);
    }

    virtual bool SaxStatisticsUTF16(const char* json, size_t length, Stat& stat) const override
    {
        (void)length;
        memset(&stat, 0, sizeof(Stat));
        GenericReader<UTF8<>, UTF16<> > reader;

#ifdef TEST_INSITU
        RapidjsonParseResult pr(json, length);
        InsituStringStream is(pr.buffer);
#else
        StringStream is(json);
#endif
        StatHandler<UTF16<> > handler(stat);
        return reader.Parse<TEST_PARSE_FLAG>(is, handler);
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        Document doc;
#ifdef TEST_INSITU
        RapidjsonParseResult pr(json, strlen(json));
        doc.ParseInsitu<TEST_PARSE_FLAG>(pr.buffer);
#else
        doc.Parse<TEST_PARSE_FLAG>(json);
#endif
        if (!doc.HasParseError() && doc.IsArray() && doc.Size() == 1 && doc[0].IsNumber()) {
            d = doc[0].GetDouble();
        }
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        Document doc;
#ifdef TEST_INSITU
        RapidjsonParseResult pr(json, strlen(json));
        doc.ParseInsitu<TEST_PARSE_FLAG>(pr.buffer);
#else
        doc.Parse<TEST_PARSE_FLAG>(json);
#endif
        if (!doc.HasParseError() && doc.IsArray() && doc.Size() == 1 && doc[0].IsString()) {
            s = std::string(doc[0].GetString(), doc[0].GetStringLength());
        }
        return true;
    }
};

#endif


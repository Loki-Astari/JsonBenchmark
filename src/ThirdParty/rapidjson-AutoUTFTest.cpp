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

#ifndef TEST_PARSE_FLAG
#define TEST_PARSE_FLAG kParseDefaultFlags
#endif

#include "rapidjson/document.h"
#include "rapidjson/encodedstream.h"
#include "rapidjson/memorystream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/memorybuffer.h"
#include "rapidjson/writer.h"

using namespace rapidjson;

template <typename Encoding = AutoUTF<unsigned> >
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

class RapidjsonAutoUTFParseResult : public ParseResultBase {
public:
    Document document;
    UTFType type;
    bool hasBOM;
};

class RapidjsonAutoUTFStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return mb.GetBuffer(); }

    MemoryBuffer mb;
};

class RapidjsonAutoUTFTest : public TestBase {
public:
    virtual const char* GetName()     const override { return "rapidjsonAutoUTF"; }
    virtual const char* Type()        const override { return "C++";}
    virtual const char* GetFilename() const override { return __FILE__; }

    virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
    {
        std::unique_ptr<RapidjsonAutoUTFParseResult> pr = std::make_unique<RapidjsonAutoUTFParseResult>();
        MemoryStream ms(json, length);
        AutoUTFInputStream<unsigned, MemoryStream> is(ms);
        if (!pr->document.ParseStream<TEST_PARSE_FLAG, AutoUTF<unsigned> >(is).HasParseError()) {
            pr->type = is.GetType();
            pr->hasBOM = is.HasBOM();
            reply = std::move(pr);
        }
        return true;
    }

    virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const RapidjsonAutoUTFParseResult& pr = static_cast<const RapidjsonAutoUTFParseResult&>(parseResult);
        std::unique_ptr<RapidjsonAutoUTFStringResult> sr = std::make_unique<RapidjsonAutoUTFStringResult>();
        AutoUTFOutputStream<unsigned, MemoryBuffer> os(sr->mb, pr.type, pr.hasBOM);
        Writer<AutoUTFOutputStream<unsigned, MemoryBuffer> > writer(os);
        pr.document.Accept(writer);
        os.Put('\0');
        reply = std::move(sr);
        return true;
    }

    virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
    {
        const RapidjsonAutoUTFParseResult& pr = static_cast<const RapidjsonAutoUTFParseResult&>(parseResult);
        std::unique_ptr<RapidjsonAutoUTFStringResult> sr = std::make_unique<RapidjsonAutoUTFStringResult>();
        AutoUTFOutputStream<unsigned, MemoryBuffer> os(sr->mb, pr.type, pr.hasBOM);
        PrettyWriter<AutoUTFOutputStream<unsigned, MemoryBuffer> > writer(os);
        pr.document.Accept(writer);
        os.Put('\0');
        reply = std::move(sr);
        return true;
    }

    virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
    {
        const RapidjsonAutoUTFParseResult& pr = static_cast<const RapidjsonAutoUTFParseResult&>(parseResult);
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
        GenericReader<AutoUTF<unsigned>, UTF8<> > reader;
        std::unique_ptr<RapidjsonAutoUTFStringResult> sr = std::make_unique<RapidjsonAutoUTFStringResult>();
        MemoryStream ms(json, length);
        AutoUTFInputStream<unsigned, MemoryStream> is(ms);
        MemoryBuffer mb;
        AutoUTFOutputStream<unsigned, MemoryBuffer> os(mb, is.GetType(), is.HasBOM());
        Writer<AutoUTFOutputStream<unsigned, MemoryBuffer> > writer(os);

        if (reader.Parse<TEST_PARSE_FLAG>(is, writer)) {
            os.Put('\0');
            reply = std::move(sr);
        }
        return true;
    }

    virtual bool SaxStatistics(const char* json, size_t length, Stat& stat) const override
    {
        (void)length;
        memset(&stat, 0, sizeof(Stat));
        GenericReader<AutoUTF<unsigned>, UTF8<> > reader;
        MemoryStream ms(json, length);
        AutoUTFInputStream<unsigned, MemoryStream> is(ms);
        StatHandler<UTF8<> > handler(stat);
        return reader.Parse<TEST_PARSE_FLAG>(is, handler);
    }

    virtual bool SaxStatisticsUTF16(const char* json, size_t length, Stat& stat) const override
    {
        (void)length;
        memset(&stat, 0, sizeof(Stat));
        GenericReader<UTF8<>, UTF16<> > reader;
        StringStream is(json);
        StatHandler<UTF16<> > handler(stat);
        reader.Parse(is, handler);
        return true;
    }

    virtual bool ParseDouble(const char* json, long double& d) const override
    {
        MemoryStream ms(json, strlen(json));
        AutoUTFInputStream<unsigned, MemoryStream> is(ms);
        Document doc;
        if (!(doc.ParseStream<kParseDefaultFlags, AutoUTF<unsigned> >(is).HasParseError() || !doc.IsArray() || doc.Size() != 1 || !doc[0].IsNumber())) {
            d = doc[0].GetDouble();
        }
        return true;
    }

    virtual bool ParseString(const char* json, std::string& s) const override
    {
        MemoryStream ms(json, strlen(json));
        AutoUTFInputStream<unsigned, MemoryStream> is(ms);
        Document doc;
        if (!(doc.ParseStream<kParseDefaultFlags, AutoUTF<unsigned> >(is).HasParseError() || !doc.IsArray() || doc.Size() != 1 || !doc[0].IsString())) {
            s = std::string(doc[0].GetString(), doc[0].GetStringLength());
        }
        return true;
    }
};

REGISTER_TEST(RapidjsonAutoUTFTest);

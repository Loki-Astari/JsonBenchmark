#include "test.h"

#include "jeayeson/jeayeson.hpp"

using namespace jeayeson;

static void GenStat(Stat& stat, const value& v) {
    switch (v.get_type()) {
    case value::type::array:
        {
            const array_t& a = v.as<array_t>();
            for (array_t::const_iterator itr = a.begin(); itr != a.end(); ++itr)
                GenStat(stat, *itr);
            stat.arrayCount++;
            stat.elementCount += a.size();
        }
        break;

    case value::type::map:
        {
            const map_t& o = v.as<map_t>();
            for (map_t::const_iterator itr = o.begin(); itr != o.end(); ++itr) {
                GenStat(stat, itr->second);
                stat.stringLength += itr->first.size();
            }
            stat.objectCount++;
            stat.memberCount += o.size();
            stat.stringCount += o.size();
        }
        break;

    case value::type::string: 
        stat.stringCount++;
        stat.stringLength += v.as<std::string>().size();
        break;

    case value::type::integer:
    case value::type::real:
        stat.numberCount++;
        break;

    case value::type::boolean:
        if (v.as<bool>())
            stat.trueCount++;
        else
            stat.falseCount++;
        break;

    case value::type::null:
        stat.nullCount++;
        break;
    }
}

class JeayesonParseResult : public ParseResultBase {
public:
    value root;
};

class JeayesonStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return s.c_str(); }

    std::string s;
};
class JeayesonTest : public TestBase {
public:
    virtual const char* GetName() const { return "jeayeson-C++14"; }
    virtual const char* GetFilename() const { return __FILE__; }

    virtual ParseResultBase* Parse(const char* json, size_t length) const {
        (void)length;
        JeayesonParseResult* pr = new JeayesonParseResult;
        // Determine object or array
        try {
            for (size_t i = 0; i < length; i++) {
                switch (json[i]) {
                    case '{':
                        pr->root = json_map{json_data{json}};
                        return pr;
                    case '[':
                        pr->root = json_array{json_data{json}};
                        return pr;
                    case ' ':
                    case '\t':
                    case '\n':
                    case '\r':
                        continue;
                }
                break; // Unknown first non-whitespace character
            }
        }
        catch (...)
        {
        }
        delete pr;
        return 0;
    }

    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const {
        const JeayesonParseResult* pr = static_cast<const JeayesonParseResult*>(parseResult);
        JeayesonStringResult* sr = new JeayesonStringResult;
        std::ostringstream os;
        os << pr->root;
        sr->s = os.str();
        return sr;
    }

    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const {
        const JeayesonParseResult* pr = static_cast<const JeayesonParseResult*>(parseResult);
        memset(stat, 0, sizeof(Stat));
        GenStat(*stat, pr->root);
        return true;
    }

    virtual bool ParseDouble(const char* json, double* d) const {
        try {
            *d = json_array{json_data{json}}[0].as<json_float>();
            return true;
        }
        catch (...) {
            return false;
        }
    }

    virtual bool ParseString(const char* json, std::string& s) const {
        try {
            s = json_array{json_data{json}}[0].as<std::string>();
            return true;
        }
        catch (...) {
            return false;
        }
    }
};

REGISTER_TEST(JeayesonTest);

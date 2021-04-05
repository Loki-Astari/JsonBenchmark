THORSANVIL_ROOT             = $(realpath ../../)

TARGET                      = ThirdParty.slib

VERA                        = true
LDLIBS                      += ../../thirdparty/ccan/libccan.a
LDLIBS                      += ../../thirdparty/cJSON/libcjson.a
LDLIBS                      += ../../thirdparty/fastjson/libfastjson.a
LDLIBS                      += ../../thirdparty/JsonBox/libJsonBox.a
LDLIBS                      += ../../thirdparty/json-c/libjson-c.a
LDLIBS                      += ../../thirdparty/jsoncpp/lib/libjsoncpp.a
LDLIBS                      += ../../thirdparty/json-voorhees/libjsonv.a
LDLIBS                      += ../../thirdparty/juson/libjuson.a
LDLIBS                      += ../../thirdparty/jvar/lib/libjvar.a
LDLIBS                      += ../../thirdparty/pjson/libpjson.a
LDLIBS                      += ../../thirdparty/udb-jsason-parser/usr/local/lib/libjsonparser.a
LDLIBS                      += ../../thirdparty/udb-jsason-parser/usr/local/lib/libjsonbuilder.a
LDLIBS                      += ../../thirdparty/ujson/ujson/libujson.a
LDLIBS                      += ../../thirdparty/ujson4c/libujson4c.a
LDLIBS                      += ../../thirdparty/vincenthz-libjson/libvincenthz-libjson.a
LDLIBS                      += ../../thirdparty/yajl/yajl-2.1.0/lib/libyajl_s.a
LDLIBS                      += ../../thirdparty/jsonxx/libjsonxx.a
LDLIBS                      += ../../thirdparty/gason/libgason.a
LDLIBS                      += ../../thirdparty/Jzon/libJzon.a

JzonTest_CXXFLAGS           += -I../../thirdparty/Jzon/
cJSONTest_CXXFLAGS			+= -I../../thirdparty -Wno-nonportable-include-path
gasonTest_CXXFLAGS			+= -I../../thirdparty/gason/ -Wno-range-loop-analysis
sajsonTest_CXXFLAGS			+= -I../../thirdparty/sajson -Wno-unused-parameter -Wno-pedantic
udbTest_CXXFLAGS			+= -I../../thirdparty/udb-jsason-parser/usr/local/include/ -I../../thirdparty/udb-jsason-parser/usr/local/include/json-parser

json-cTest_CXXFLAGS			+= -I../../thirdparty/ -Wno-sign-compare
double-conversion_CXXFLAGS	+= -Wno-strict-overflow
# Extra Headers for makefiles
cajun-jsonapiTest_CXXFLAGS	+= -I../../thirdparty/cajun-jsonapi/include/cajun/ -Wno-unused-parameter
ccanTest_CXXFLAGS			+= -I../../thirdparty/ccan -Wno-nested-anon-types
fastjsonTest_CXXFLAGS		+= -I../../thirdparty/fastjson/include/ -Wno-return-type
jsonconsTest_CXXFLAGS		+= -I../../thirdparty/jsoncons/include/ -Wno-unused-parameter -Wno-missing-braces
jsonxxTest_CXXFLAGS			+= -I../../thirdparty/jsonxx -Wno-dollar-in-identifier-extension
jusonTest_CXXFLAGS			+= -I../../thirdparty/ -Wno-pedantic -Wno-gnu-anonymous-struct -Wno-nested-anon-types
jvarTest_CXXFLAGS			+= -I../../thirdparty/ -Wno-gnu-zero-variadic-macro-arguments -Wno-ignored-qualifiers -Wno-nested-anon-types -D_GLIBCXX_USE_CXX11_ABI=0
nlohmannTest_CXXFLAGS		+= -I../../thirdparty/nlohmann/include/ -Wno-unused-local-typedef
pjsonTest_CXXFLAGS			+= -I../../thirdparty/pjson -Wno-nested-anon-types
ujsonTest_CXXFLAGS			+= -I../../thirdparty/ujson/ -Wno-unused-parameter -Wno-potentially-evaluated-expression
ArduinoJsonTest_CXXFLAGS	+= -I../../thirdparty/ArduinoJson/src
JsonBoxTest_CXXFLAGS		+= -I../../thirdparty/JsonBox/include/ -I../../thirdparty/JsonBox/
ThorsSerializerTest_CXXFLAGS+= -I../../thirdparty/ThorsSerializer/ -Wno-return-stack-address -Wno-range-loop-analysis
jeayesonTest_CXXFLAGS		+= -I../../thirdparty/jeayeson/include/ -Wno-c11-extensions
jsmnTest_CXXFLAGS			+= -I../../thirdparty/
json-voorheesTest_CXXFLAGS	+= -I../../thirdparty/json-voorhees/include/
jsoncppTest_CXXFLAGS		+= -I../../thirdparty/jsoncpp/include/
rapidjson-AutoUTFTest_CXXFLAGS		+= -I../../thirdparty/rapidjson/include/
rapidjson-FullPrecTest_CXXFLAGS		+= -I../../thirdparty/rapidjson/include/
rapidjson-InsituTest_CXXFLAGS		+= -I../../thirdparty/rapidjson/include/
rapidjson-IterativeTest_CXXFLAGS	+= -I../../thirdparty/rapidjson/include/
rapidjsonTest_CXXFLAGS		+= -I../../thirdparty/rapidjson/include/
ujson4cTest_CXXFLAGS		+= -I../../thirdparty/ujson4c/
vincenthz-libjsonTest_CXXFLAGS		+= -I../../thirdparty/
yajlTest_CXXFLAGS			+= -I../../thirdparty/yajl//yajl-2.1.0/include/
ConfiguruTest_CXXFLAGS		+= -I../../thirdparty/
picojsonTest_CXXFLAGS		+= -I../../thirdparty/picojson/

EXCLUDE_HEADERS             += ThorsSerializer_Canada.h  ThorsSerializer_citm_catalog.h  ThorsSerializer_Common.h    \
                               ThorsSerializer_GetStats.h  ThorsSerializer_StringNumber.h  ThorsSerializer_Twitter.h \
                               ArduinoJson.h ArduinoJson.hpp	\
                               jsonxx.h                         \
                               Jzon.h							\
							   rapidjsonTest.h					\
							   gason.h							\
							   sajson.h							\
							   ujson.hpp double-conversion.h	\



FILE_WARNING_FLAGS          += -Wno-unknown-pragmas

include $(THORSANVIL_ROOT)/build/tools/Makefile

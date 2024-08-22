THORSANVIL_ROOT             = $(realpath ../../)

TARGET                      = ThirdParty.slib

VERA                        = true
LDLIBS                      += ../../thirdparty/ccan/libccan.a
LDLIBS                      += ../../thirdparty/cJSON/libcjson.a
LDLIBS                      += ../../thirdparty/json-c/libjson-c.a
LDLIBS                      += ../../thirdparty/jsoncpp/build/lib/libjsoncpp.a
LDLIBS                      += ../../thirdparty/json-voorhees/libjsonv.a
LDLIBS                      += ../../thirdparty/udb-jsason-parser/lib/libjsonparser.a
LDLIBS                      += ../../thirdparty/udb-jsason-parser/lib/libjsonbuilder.a
LDLIBS                      += ../../thirdparty/jsonxx/libjsonxx.a
LDLIBS                      += ../../thirdparty/Jzon/libJzon.a
LDLIBS                      += -lThorSerialize -lThorsLogging


JzonTest_CXXFLAGS           += -I../../thirdparty/Jzon/ -Wno-deprecated-declarations
cJSONTest_CXXFLAGS			+= -I../../thirdparty -Wno-nonportable-include-path
sajsonTest_CXXFLAGS			+= -I../../thirdparty/sajson -Wno-unused-parameter -Wno-pedantic
udbTest_CXXFLAGS			+= -I../../thirdparty/udb-jsason-parser/include/ -I../../thirdparty/udb-jsason-parser/include/json-parser

json-cTest_CXXFLAGS			+= -I../../thirdparty/ -Wno-sign-compare
double-conversion_CXXFLAGS	+= -Wno-strict-overflow
# Extra Headers for makefiles
cajun-jsonapiTest_CXXFLAGS	+= -I../../thirdparty/cajun-jsonapi/include/cajun/ -Wno-unused-parameter -Wno-deprecated-declarations -Wno-deprecated-copy-with-user-provided-dtor
ccanTest_CXXFLAGS			+= -I../../thirdparty/ccan -Wno-nested-anon-types
jsonconsTest_CXXFLAGS		+= -I../../thirdparty/jsoncons/include/ -Wno-unused-parameter -Wno-missing-braces
jsonxxTest_CXXFLAGS			+= -I../../thirdparty/jsonxx -Wno-dollar-in-identifier-extension
nlohmannTest_CXXFLAGS		+= -I../../thirdparty/nlohmann/include/ -Wno-unused-local-typedef
ArduinoJsonTest_CXXFLAGS	+= -I../../thirdparty/ArduinoJson/src -Wno-deprecated-dynamic-exception-spec
ThorsSerializerTest_CXXFLAGS+= -Wno-return-stack-address -Wno-range-loop-analysis -Wno-deprecated-declarations
jsmnTest_CXXFLAGS			+= -I../../thirdparty/
json-voorheesTest_CXXFLAGS	+= -I../../thirdparty/json-voorhees/include/ -Wno-deprecated-declarations -Wno-deprecated-copy-with-user-provided-dtor
jsoncppTest_CXXFLAGS		+= -I../../thirdparty/jsoncpp/include/
rapidjson-AutoUTFTest_CXXFLAGS		+= -I../../thirdparty/rapidjson/include/
rapidjson-FullPrecTest_CXXFLAGS		+= -I../../thirdparty/rapidjson/include/
rapidjson-InsituTest_CXXFLAGS		+= -I../../thirdparty/rapidjson/include/
rapidjson-IterativeTest_CXXFLAGS	+= -I../../thirdparty/rapidjson/include/
rapidjsonTest_CXXFLAGS		+= -I../../thirdparty/rapidjson/include/
ConfiguruTest_CXXFLAGS		+= -I../../thirdparty/ -Wno-unknown-warning-option -Wno-maybe-uninitialized -Wno-uninitialized 
picojsonTest_CXXFLAGS		+= -I../../thirdparty/picojson/

EXCLUDE_HEADERS             += ThorsSerializer_Canada.h  ThorsSerializer_citm_catalog.h  ThorsSerializer_Common.h    \
                               ThorsSerializer_GetStats.h  ThorsSerializer_StringNumber.h  ThorsSerializer_Twitter.h \
                               ArduinoJson.h ArduinoJson.hpp	\
                               jsonxx.h                         \
                               Jzon.h							\
							   rapidjsonTest.h					\
							   sajson.h							\



FILE_WARNING_FLAGS          += -Wno-unknown-pragmas

include $(THORSANVIL_ROOT)/build/tools/Makefile

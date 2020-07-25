THORSANVIL_ROOT             = $(realpath ../../)

TARGET                      = ThirdParty.slib

VERA                        = true

LDLIBS                      += -L../../thirdparty/ccan/                 -lccan
LDLIBS                      += -L../../thirdparty/cJSON/                -lcjson
LDLIBS                      += -L../../thirdparty/fastjson              -lfastjson
LDLIBS                      += -L../../thirdparty/JsonBox/              -lJsonBox
LDLIBS                      += -L../../thirdparty/json-c/               -ljson-c
LDLIBS                      += -L../../thirdparty/jsoncpp/lib/          -ljsoncpp
LDLIBS                      += -L../../thirdparty/json-voorhees         -ljsonv
LDLIBS                      += -L../../thirdparty/juson                 -ljuson
LDLIBS                      += -L../../thirdparty/jvar/lib              -ljvar
LDLIBS                      += -L../../thirdparty/pjson                 -lpjson
LDLIBS                      += -L../../thirdparty/udb-jsason-parser     -ljsonparser -L../../thirdparty/udb-json-builder -ljsonbuilder
LDLIBS                      += -L../../thirdparty/ujson/ujson           -lujson
LDLIBS                      += -L../../thirdparty/ujson4c               -lujson4c
LDLIBS                      += -L../../thirdparty/vincenthz-libjson     -lvincenthz-libjson
LDLIBS                      += -L../../thirdparty/yajl/yajl-2.1.1/lib   -lyajl_s

Jzon_CXXFLAGS               += -Wno-implicit-fallthrough
cJSONTest_CXXFLAGS			+= -I../../thirdparty -Wno-nonportable-include-path
gason_CXXFLAGS				+= -Wno-implicit-fallthrough
sajsonTest_CXXFLAGS			+= -I../../thirdparty/sajson -Wno-unused-parameter -Wno-pedantic
udbTest_CXXFLAGS			+= -I../../thirdparty/udb-jsason-parser

json-cTest_CXXFLAGS			+= -I../../thirdparty/ -Wno-sign-compare
jvarTest_CXXFLAGS			+= -D_GLIBCXX_USE_CXX11_ABI=0
double-conversion_CXXFLAGS	+= -Wno-strict-overflow
# Extra Headers for makefiles
cajun-jsonapiTest_CXXFLAGS	+= -I../../thirdparty/cajun-jsonapi/include/ -Wno-unused-parameter
ccanTest_CXXFLAGS			+= -I../../thirdparty/ccan -Wno-nested-anon-types
fastjsonTest_CXXFLAGS		+= -I../../thirdparty/fastjson/include/ -Wno-return-type
jsonconsTest_CXXFLAGS		+= -I../../thirdparty/jsoncons/include/ -Wno-unused-parameter
jsonxx_CXXFLAGS				+= -Wno-dollar-in-identifier-extension
jsonxxTest_CXXFLAGS			+= -Wno-dollar-in-identifier-extension
jusonTest_CXXFLAGS			+= -I../../thirdparty/ -Wno-gnu-anonymous-struct -Wno-nested-anon-types
jvarTest_CXXFLAGS			+= -I../../thirdparty/ -Wno-gnu-zero-variadic-macro-arguments -Wno-nested-anon-types
nlohmannTest_CXXFLAGS		+= -I../../thirdparty/nlohmann/include/ -Wno-unused-local-typedef
pjsonTest_CXXFLAGS			+= -I../../thirdparty/pjson -Wno-nested-anon-types
ujsonTest_CXXFLAGS			+= -I../../thirdparty/ujson/ -Wno-unused-parameter -Wno-potentially-evaluated-expression
ArduinoJsonTest_CXXFLAGS	+= -I../../thirdparty/ArduinoJson/src
JsonBoxTest_CXXFLAGS		+= -I../../thirdparty/JsonBox/include/ -I../../thirdparty/JsonBox/
ThorsSerializerTest_CXXFLAGS+= -I../../thirdparty/ThorsSerializer/
jeayesonTest_CXXFLAGS		+= -I../../thirdparty/jeayeson/include/
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
yajlTest_CXXFLAGS			+= -I../../thirdparty/yajl//yajl-2.1.1/include/
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

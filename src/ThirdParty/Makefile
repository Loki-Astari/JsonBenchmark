THORSANVIL_ROOT             = $(realpath ../../)

THIRD_PARTY                 = \
							  \
							  \
							  \
							  \
							  jsmn

TARGET                      = ThirdParty.slib

VERA                        = true

LDLIBS                      += -lccan
LDLIBS                      += -lcjson
LDLIBS                      += -lfastjson
LDLIBS                      += -L../../thirdparty/JsonBox/  -lJsonBox
LDLIBS                      += -ljson-c
LDLIBS                      += -ljsoncpp
LDLIBS                      += -ljsonv
LDLIBS                      += -ljuson
LDLIBS                      += -ljvar
LDLIBS                      += -lpjson
LDLIBS                      += -ljsonparser -ljsonbuilder
LDLIBS                      += -lujson4c
LDLIBS                      += -lvincenthz-libjson
LDLIBS                      += -lyajl_s

Jzon_CXXFLAGS               += -Wno-implicit-fallthrough
gason_CXXFLAGS				+= -Wno-implicit-fallthrough
sajsonTest_CXXFLAGS			+= -Wno-unused-parameter -Wno-pedantic
ujson_CXXFLAGS				+= -Wno-implicit-fallthrough -Wno-missing-field-initializers
udbTest_CXXFLAGS			+= -I /usr/local/include/json-parser/
json-cTest_CXXFLAGS			+= -Wno-sign-compare
jvarTest_CXXFLAGS			+= -D_GLIBCXX_USE_CXX11_ABI=0
double-conversion_CXXFLAGS	+= -Wno-strict-overflow
# Extra Headers for makefiles
cajun-jsonapiTest_CXXFLAGS	+= -Wno-unused-parameter
ccanTest_CXXFLAGS			+= -Wno-nested-anon-types
fastjsonTest_CXXFLAGS		+= -Wno-return-type
jsonconsTest_CXXFLAGS		+= -Wno-unused-parameter
jsonxx_CXXFLAGS				+= -Wno-dollar-in-identifier-extension
jsonxxTest_CXXFLAGS			+= -Wno-dollar-in-identifier-extension
jusonTest_CXXFLAGS			+= -Wno-gnu-anonymous-struct -Wno-nested-anon-types
jvarTest_CXXFLAGS			+= -Wno-gnu-zero-variadic-macro-arguments -Wno-nested-anon-types
nlohmannTest_CXXFLAGS		+= -Wno-unused-local-typedef
pjsonTest_CXXFLAGS			+= -Wno-nested-anon-types
ujson_CXXFLAGS				+= -Wno-unused-parameter -Wno-potentially-evaluated-expression
ujsonTest_CXXFLAGS			+= -Wno-unused-parameter -Wno-potentially-evaluated-expression
ArduinoJsonTest_CXXFLAGS	+= -I../../thirdparty/ArduinoJson/src
JsonBoxTest_CXXFLAGS		+= -I../../thirdparty/JsonBox/include/ -I../../thirdparty/JsonBox/

EXCLUDE_HEADERS             += ThorsSerializer_Canada.h  ThorsSerializer_citm_catalog.h  ThorsSerializer_Common.h    \
                               ThorsSerializer_GetStats.h  ThorsSerializer_StringNumber.h  ThorsSerializer_Twitter.h \
                               ArduinoJson.h ArduinoJson.hpp	\
                               configuru.hpp                    \
                               jsonxx.h                         \
                               Jzon.h							\
							   picojson.h						\
							   rapidjsonTest.h					\
							   gason.h							\
							   sajson.h							\
							   ujson.hpp double-conversion.h	\



FILE_WARNING_FLAGS          += -Wno-unknown-pragmas

include $(THORSANVIL_ROOT)/build/tools/Makefile

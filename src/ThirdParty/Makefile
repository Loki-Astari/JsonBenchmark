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
LDLIBS                      += -lJsonBox
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

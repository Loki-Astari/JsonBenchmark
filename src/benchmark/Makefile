THORSANVIL_ROOT				= $(realpath ../../)

CXXFLAGS					= -D THORSANVIL_ROOT=$(THORSANVIL_ROOT) -D DATA_DIR=$(realpath $(THORSANVIL_ROOT)/data) -D LIBEXT=$(BUILD_EXTENSION).$(SO)

FILE_WARNING_FLAGS			+= -Wno-unknown-pragmas

TARGET						= benchmark.prog memory.prog

LDFLAGS						+= -L$(THORSANVIL_ROOT)/thirdparty/ThorsSerializer/build/lib/
LDLIBS						+= -lThirdParty -lThorSerialize -lThorsLogging
memory_LINK_LIBS			= Memory
memory_LDLIBS				= -ldl

include $(THORSANVIL_ROOT)/build/tools/Makefile

THORSANVIL_ROOT             = $(realpath ../../)

FILE_WARNING_FLAGS          += -Wno-unknown-pragmas
CXXFLAGS                    += -D DATA_DIR=$(realpath $(THORSANVIL_ROOT)/data)
TARGET                      = Memory.slib
LDLIBS                      = -ldl

include $(THORSANVIL_ROOT)/build/tools/Makefile

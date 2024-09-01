THORSANVIL_ROOT             = $(realpath ./)

TARGET                      = src

run:	release-only

include $(THORSANVIL_ROOT)/build/tools/Project.Makefile

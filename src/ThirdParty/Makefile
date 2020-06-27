THORSANVIL_ROOT             = $(realpath ../../)

THIRD_PARTY                 = ThorsSerializer ArduinoJson cajun-jsonapi Configuru fastjson	\
							  jeayeson JsonBox jsoncons jsoncpp json-voorhees jsonxx jvar	\
							  Jzon nlohmann picojson rapidjson gason sajson ujson			\
							  rapidjsonAutoUTF  rapidjsonFullPrec  rapidjsonInsitu			\
							  rapidjsonIterative ujson4c pjson udb juson ccan cJSON			\
							  vincenthz-libjson yajl json-c jsmn
THIRDPARTY_PROJECTS         = $(foreach target,$(THIRD_PARTY),$(target).third)

TARGET                      = $(THIRDPARTY_PROJECTS) ThirdParty.slib
ThirdParty.slib:              $(THIRDPARTY_PROJECTS)

LINK_LIBS                   =  $(foreach target,$(THIRD_PARTY), $(target)Test)

%.third:
	@if [ "$(TARGET_MODE)" == "debug" ]; then											\
		echo "Building Third Party $*";													\
		if test -d $*; then																\
			$(MAKE) --silent -C $* $(ACTION);											\
		fi;																				\
		echo "Building Third Party $* Done";											\
	fi


FILE_WARNING_FLAGS          += -Wno-unknown-pragmas
ACTION                      ?= build
VERA                        = true

include $(THORSANVIL_ROOT)/build/tools/Makefile

veryclean:	ACTION          = veryclean
veryclean:                  $(THIRDPARTY_PROJECTS)





FEATURES ?= tva
COMMA := ,
FEATURES_LIST := $(subst $(COMMA), ,$(FEATURES))

SUPPORTED_FEATURES := tva none

UNKNOWN_FEATURES := $(filter-out $(SUPPORTED_FEATURES),$(FEATURES_LIST))
ifneq ($(UNKNOWN_FEATURES),)
    $(warning unknown features: $(UNKNOWN_FEATURES))
endif

HAS_TVA := $(filter tva,$(FEATURES_LIST))

FEATURE_CFLAGS      :=
FEATURE_LIBS        :=
FEATURE_OBJS_STATIC :=
FEATURE_OBJS_SHARED :=

ifeq ($(HAS_TVA),tva)
	SDL3_CFLAGS := $(shell pkg-config --cflags sdl3 sdl3-image)
	SDL3_LIBS   := $(shell pkg-config --libs sdl3 sdl3-image)

	FEATURE_CFLAGS       += -DTC48_CONFIG_TVA=1 $(SDL3_CFLAGS)
	FEATURE_LIBS         += $(SDL3_LIBS)
	FEATURE_OBJS_STATIC  += $(TVA_OBJS_STATIC)
	FEATURE_OBJS_SHARED  += $(TVA_OBJS_SHARED)
endif

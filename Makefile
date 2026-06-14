CC ?= cc
AR ?= gcc-ar
BUILD ?= release

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

ifeq ($(OS),Windows_NT)
	PLATFORM := windows
else
	PLATFORM := posix
endif

SRC_DIR     := src
TEST_DIR    := tests
INCLUDE_DIR := include

OBJ_ROOT_DIR := build/$(BUILD)/obj
DEP_ROOT_DIR := build/$(BUILD)/dep

OUT_DIR     := out/$(BUILD)
LIB_DIR     := $(OUT_DIR)/lib
BIN_DIR     := $(OUT_DIR)/bin

EMU_LIB_NAME   := tc48emu
EMU_LIB_STATIC := $(LIB_DIR)/lib$(EMU_LIB_NAME).a
EMU_LIB_SHARED := $(LIB_DIR)/lib$(EMU_LIB_NAME).so

TVA_LIB_NAME    := tva
TVA_LIB_STATIC  := $(LIB_DIR)/lib$(TVA_LIB_NAME).a
TVA_LIB_SHARED  := $(LIB_DIR)/lib$(TVA_LIB_NAME).so

EXE_EXT :=
ifeq ($(PLATFORM),windows)
	EXE_EXT := .exe
else ifeq ($(PLATFORM),posix)
	EXE_EXT :=
endif

EMU_EXE := $(BIN_DIR)/tc48-emu$(EXE_EXT)

CSTD       := -std=c11
WARNINGS   := -Wall -Wextra -Werror=implicit-fallthrough -Wno-old-style-declaration
PIC_CFLAGS := -fPIC

COMMON_CFLAGS := $(CSTD) $(WARNINGS) -I$(INCLUDE_DIR)

ifeq ($(PLATFORM),windows)
	CMD_MKDIR_P = powershell -NoProfile -Command "New-Item -ItemType Directory -Force -Path '$(subst /,\,$(1))'"
	CMD_RM_RF   = powershell -NoProfile -Command "Remove-Item -Recurse -Force -Path '$(subst /,\,$(1))'"
else
	CMD_MKDIR_P = mkdir -p "$(1)"
	CMD_RM_RF   = rm -rf "$(1)"
endif

GENERATED_FILES  := src/tc48/gen/pow3.c include/tc48/gen/word-lits.h include/tc48/gen/version.h include/tc48/gen/comptime-pow3.h
GENERATED_C_SRCS := $(filter %.c, $(GENERATED_FILES))

MAIN_SRC := src/main.c
EMU_SRCS := $(call rwildcard,src/tc48,*.c) $(GENERATED_C_SRCS)
TVA_SRCS := $(call rwildcard,src/tva,*.c)

EMU_LIB_CORE_OBJS_STATIC := $(patsubst %.c,$(OBJ_ROOT_DIR)/%.o,$(EMU_SRCS))
EMU_LIB_CORE_OBJS_SHARED := $(patsubst %.c,$(OBJ_ROOT_DIR)/shared/%.o,$(EMU_SRCS))

EMU_EXE_OBJS             := $(patsubst %.c,$(OBJ_ROOT_DIR)/%.o,$(MAIN_SRC))
TVA_OBJS_STATIC          := $(patsubst %.c,$(OBJ_ROOT_DIR)/%.o,$(TVA_SRCS))
TVA_OBJS_SHARED          := $(patsubst %.c,$(OBJ_ROOT_DIR)/shared/%.o,$(TVA_SRCS))

# Include feature configuration
include make/features.mk

COMMON_CFLAGS += $(FEATURE_CFLAGS)

ifeq ($(BUILD),debug)
	CFLAGS  := $(COMMON_CFLAGS) -O0 -g -DTC48_DEBUG -fsanitize=address,undefined
	LDFLAGS := -fsanitize=address,undefined
else ifeq ($(BUILD),release)
	CFLAGS  := $(COMMON_CFLAGS) -O3 -DNDEBUG
	LDFLAGS := -flto
else
	$(error Unknown BUILD=$(BUILD))
endif

EMU_LIB_FULL_OBJS_STATIC := $(EMU_LIB_CORE_OBJS_STATIC) $(FEATURE_OBJS_STATIC)
EMU_LIB_FULL_OBJS_SHARED := $(EMU_LIB_CORE_OBJS_SHARED) $(FEATURE_OBJS_SHARED)

ALL_C_SRCS := $(MAIN_SRC) $(EMU_SRCS) $(TVA_SRCS)
DEPS := $(patsubst %.c,$(DEP_ROOT_DIR)/%.d,$(ALL_C_SRCS)) \
        $(patsubst %.c,$(DEP_ROOT_DIR)/shared/%.d,$(filter-out $(MAIN_SRC),$(ALL_C_SRCS)))

.PHONY: all clean run test sharedlib libtva libtc48emu

all: $(GENERATED_FILES) $(EMU_EXE) $(EMU_LIB_STATIC) $(EMU_LIB_SHARED) test

libtc48emu: $(EMU_LIB_STATIC) $(EMU_LIB_SHARED)
libtva: $(TVA_LIB_STATIC) $(TVA_LIB_SHARED)

src/tc48/gen/pow3.c: scripts/gen/pow3.py
include/tc48/gen/word-lits.h: scripts/gen/word-lits.py include/tc48/gen/comptime-pow3.h
include/tc48/gen/version.h: scripts/gen/version.py
include/tc48/gen/comptime-pow3.h: scripts/gen/comptime-pow3.py

$(GENERATED_FILES):
	@echo "Generating $@..."
	@$(call CMD_MKDIR_P,$(dir $@))
	@python3 $< > $@

# libtc48emu
$(EMU_LIB_STATIC): $(EMU_LIB_FULL_OBJS_STATIC)
	@$(call CMD_MKDIR_P,$(dir $@))
	$(AR) rcs $@ $^

$(EMU_LIB_SHARED): $(EMU_LIB_FULL_OBJS_SHARED)
	@$(call CMD_MKDIR_P,$(dir $@))
	$(CC) -shared $^ $(LDFLAGS) $(FEATURE_LIBS) -o $@

# libtva
$(TVA_LIB_STATIC): $(TVA_OBJS_STATIC)
	@$(call CMD_MKDIR_P,$(dir $@))
	$(AR) rcs $@ $^

$(TVA_LIB_SHARED): $(TVA_OBJS_SHARED)
	@$(call CMD_MKDIR_P,$(dir $@))
	$(CC) -shared $^ $(LDFLAGS) $(FEATURE_LIBS) -o $@

# main tc48-emu executable
$(EMU_EXE): $(EMU_EXE_OBJS) $(EMU_LIB_STATIC)
	@$(call CMD_MKDIR_P,$(dir $@))
	$(CC) $^ $(LDFLAGS) $(FEATURE_LIBS) -o $@

$(OBJ_ROOT_DIR)/%.o: %.c
	@$(call CMD_MKDIR_P,$(dir $@))
	@$(call CMD_MKDIR_P,$(DEP_ROOT_DIR)/$(dir $<))
	$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_ROOT_DIR)/$*.d -c $< -o $@

$(OBJ_ROOT_DIR)/shared/%.o: %.c
	@$(call CMD_MKDIR_P,$(dir $@))
	@$(call CMD_MKDIR_P,$(DEP_ROOT_DIR)/shared/$(dir $<))
	$(CC) $(CFLAGS) $(PIC_CFLAGS) -MMD -MP -MF $(DEP_ROOT_DIR)/shared/$*.d -c $< -o $@

$(EMU_EXE_OBJS) $(EMU_LIB_CORE_OBJS_STATIC) \
$(EMU_LIB_CORE_OBJS_SHARED) $(TVA_OBJS_STATIC) \
$(TVA_OBJS_SHARED): $(GENERATED_FILES)

run: all
	$(EMU_EXE)

sharedlib: $(EMU_LIB_SHARED)

include tests/build.mk

-include $(DEPS)

clean:
	@$(call CMD_RM_RF,build)
	@$(call CMD_RM_RF,out)
	@$(call CMD_RM_RF,src/tc48/gen)
	@$(call CMD_RM_RF,include/tc48/gen)

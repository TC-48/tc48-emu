CC ?= cc
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

LIB_NAME    := tc48emu
LIB_STATIC  := $(LIB_DIR)/lib$(LIB_NAME).a
LIB_SHARED  := $(LIB_DIR)/lib$(LIB_NAME).so

EXE_EXT :=
ifeq ($(PLATFORM),windows)
	EXE_EXT := .exe
else ifeq ($(PLATFORM),posix)
	EXE_EXT :=
endif

TARGET := $(BIN_DIR)/tc48-emu$(EXE_EXT)

CSTD       := -std=c11
WARNINGS   := -Wall -Wextra -Werror=implicit-fallthrough
PIC_CFLAGS := -fPIC

COMMON_CFLAGS := $(CSTD) $(WARNINGS) -I$(INCLUDE_DIR)

ifeq ($(BUILD),debug)
	CFLAGS := $(COMMON_CFLAGS) -O0 -g -DTC48_DEBUG -fsanitize=address,undefined
	LDFLAGS := -fsanitize=address,undefined
else ifeq ($(BUILD),release)
	CFLAGS := $(COMMON_CFLAGS) -O3 -DNDEBUG
	LDFLAGS := -flto
else
	$(error Unknown BUILD=$(BUILD))
endif

ifeq ($(PLATFORM),windows)
	CMD_MKDIR_P = powershell -NoProfile -Command "New-Item -ItemType Directory -Force -Path '$(subst /,\,$(1))'"
	CMD_RM_RF   = powershell -NoProfile -Command "Remove-Item -Recurse -Force -Path '$(subst /,\,$(1))'"
else
	CMD_MKDIR_P = mkdir -p "$(1)"
	CMD_RM_RF   = rm -rf "$(1)"
endif

ALL_C_SRCS := $(call rwildcard,$(SRC_DIR),*.c)

GENERATED_FILES := src/tc48/gen/pow3.c include/tc48/gen/word-lits.h include/tc48/gen/version.h
GENERATED_C_SRCS := $(filter %.c, $(GENERATED_FILES))
ALL_C_SRCS := $(sort $(ALL_C_SRCS) $(GENERATED_C_SRCS))

MAIN_C_SRC := $(SRC_DIR)/tc48/main.c
LIB_C_SRCS := $(filter $(SRC_DIR)/%, $(filter-out $(MAIN_C_SRC), $(ALL_C_SRCS)))

MAIN_OBJ := $(patsubst %.c,$(OBJ_ROOT_DIR)/%.o,$(MAIN_C_SRC))
LIB_OBJ_STATIC := $(patsubst %.c,$(OBJ_ROOT_DIR)/%.o,$(LIB_C_SRCS))
LIB_OBJ_SHARED := $(patsubst %.c,$(OBJ_ROOT_DIR)/shared/%.o,$(LIB_C_SRCS))

DEPS := $(patsubst %.c,$(DEP_ROOT_DIR)/%.d,$(ALL_C_SRCS)) \
        $(patsubst %.c,$(DEP_ROOT_DIR)/shared/%.d,$(LIB_C_SRCS))

JFLAG := $(filter -j%,$(MAKEFLAGS))
ifeq ($(JFLAG),)
	JFLAG := -j1
endif

.PHONY: all dirs clean run test sharedlib

all: $(GENERATED_FILES) $(TARGET) $(LIB_STATIC) $(LIB_SHARED) test

src/tc48/gen/pow3.c: scripts/gen/pow3.py
include/tc48/gen/word-lits.h: scripts/gen/word-lits.py
include/tc48/gen/version.h: scripts/gen/version.py

$(GENERATED_FILES):
	@echo "Generating $@..."
	@$(call CMD_MKDIR_P,$(dir $@))
	@python3 $< > $@

$(LIB_STATIC): $(LIB_OBJ_STATIC)
	@$(call CMD_MKDIR_P,$(dir $@))
	ar rcs $@ $^

$(LIB_SHARED): $(LIB_OBJ_SHARED)
	@$(call CMD_MKDIR_P,$(dir $@))
	$(CC) -shared $^ $(LDFLAGS) -o $@

$(TARGET): $(LIB_STATIC) $(MAIN_OBJ)
	@$(call CMD_MKDIR_P,$(dir $@))
	$(CC) $(MAIN_OBJ) $(LIB_STATIC) $(LDFLAGS) -o $@

$(OBJ_ROOT_DIR)/%.o: %.c
	@$(call CMD_MKDIR_P,$(dir $@))
	@$(call CMD_MKDIR_P,$(DEP_ROOT_DIR)/$(dir $<))
	$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_ROOT_DIR)/$*.d -c $< -o $@

$(OBJ_ROOT_DIR)/shared/%.o: %.c
	@$(call CMD_MKDIR_P,$(dir $@))
	@$(call CMD_MKDIR_P,$(DEP_ROOT_DIR)/shared/$(dir $<))
	$(CC) $(CFLAGS) $(PIC_CFLAGS) -MMD -MP -MF $(DEP_ROOT_DIR)/shared/$*.d -c $< -o $@

$(MAIN_OBJ) $(LIB_OBJ_STATIC) $(LIB_OBJ_SHARED): $(GENERATED_FILES)

run: all
	$(TARGET)

sharedlib: $(LIB_SHARED)

include tests/build.mk

-include $(DEPS)

clean:
	@$(call CMD_RM_RF,build)
	@$(call CMD_RM_RF,out)
	@$(call CMD_RM_RF,src/tc48/gen)
	@$(call CMD_RM_RF,include/tc48/gen)

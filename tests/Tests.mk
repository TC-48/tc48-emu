ifeq ($(PLATFORM),posix)
	TEST_C_SRCS := $(shell find $(TEST_DIR) -name "test-*.c")
else
	TEST_C_SRCS := $(shell powershell -NoProfile -Command "Get-ChildItem -Path '$(TEST_DIR)' -Recurse -Include test-*.c | ForEach-Object { $_.FullName -replace '\\\\','/' }")
endif

TEST_OBJS := $(patsubst %.c,$(OBJ_ROOT_DIR)/%.o,$(TEST_C_SRCS))
TEST_BIN  := $(BIN_DIR)/tests$(EXE_EXT)

$(TEST_OBJS): $(GENERATED_FILES)

.PHONY: tests build-tests

tests: $(TARGET) $(LIB_STATIC) build-tests
	@echo "Linking $@..."
	$(TEST_BIN)

build-tests: $(TEST_BIN)

$(TEST_BIN): $(TEST_OBJS) $(LIB_STATIC)
	@$(call CMD_MKDIR_P,$(dir $@))
	$(CC) $(TEST_OBJS) $(LIB_STATIC) $(LDFLAGS) -lcriterion -o $@

DEPS += $(patsubst %.c,$(DEP_ROOT_DIR)/%.d,$(TEST_C_SRCS))

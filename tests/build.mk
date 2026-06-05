TEST_C_SRCS := $(call rwildcard,$(TEST_DIR),*.c)
TEST_BINS   := $(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/tests/%$(EXE_EXT),$(TEST_C_SRCS))
TEST_OBJS   := $(patsubst %.c,$(OBJ_ROOT_DIR)/%.o,$(TEST_C_SRCS))

$(TEST_BINS): $(EMU_LIB_STATIC)
$(TEST_OBJS): $(GENERATED_FILES)

.PHONY: test build-tests

test: build-tests
	@for test in $(TEST_BINS); do \
		echo "Running $$test..."; \
		./$$test || exit 1; \
	done

build-tests: $(TEST_BINS)

$(BIN_DIR)/tests/%$(EXE_EXT): $(OBJ_ROOT_DIR)/$(TEST_DIR)/%.o
	@$(call CMD_MKDIR_P,$(dir $@))
	@echo "Building $@..."
	@$(CC) $< $(EMU_LIB_STATIC) $(LDFLAGS) -lcriterion -o $@

.PHONY: test-%
test-%: $(BIN_DIR)/tests/test-%$(EXE_EXT)
	@echo "Running $<..."
	@$<

DEPS += $(patsubst %.c,$(DEP_ROOT_DIR)/%.d,$(TEST_C_SRCS))

# Test Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I../include -Iunity
TEST_DIR = .
BUILD_DIR = build
UNITY_DIR = unity

# Unity source files
UNITY_SRC = $(UNITY_DIR)/unity.c

# Test source files
TEST_SOURCES = $(wildcard $(TEST_DIR)/test_*.c)
TEST_EXECUTABLES = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%,$(TEST_SOURCES))

.PHONY: all clean test

all: $(BUILD_DIR) $(TEST_EXECUTABLES)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%: $(TEST_DIR)/%.c $(UNITY_SRC)
	$(CC) $(CFLAGS) $< $(UNITY_SRC) -o $@

test: all
	@echo "Running all tests..."
	@for test in $(TEST_EXECUTABLES); do \
		$$test || exit 1; \
	done
	@echo ""
	@echo "All tests completed successfully!"

clean:
	rm -rf $(BUILD_DIR)

# Run individual test
run-%: $(BUILD_DIR)/%
	@$<


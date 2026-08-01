###############################################################################
# Advanced Scientific Calculator
# Production Makefile
###############################################################################

# ---------------------------------------------------------------------------
# Compiler
# ---------------------------------------------------------------------------
CC      := gcc
CXX     := g++

# ---------------------------------------------------------------------------
# Directories
# ---------------------------------------------------------------------------
SRC_DIR  := Src
OBJ_DIR  := Obj
INC_DIR  := Inc
TEST_DIR := Tests

# ---------------------------------------------------------------------------
# Executables
# ---------------------------------------------------------------------------
TARGET      := calculator
TEST_TARGET := run_tests

# ---------------------------------------------------------------------------
# Warning Flags
# ---------------------------------------------------------------------------
WARNINGS := \
	-Wall \
	-Wextra \
	-Wpedantic \
	-Wshadow \
	-Wformat=2

# ---------------------------------------------------------------------------
# Compiler Flags
# ---------------------------------------------------------------------------
CFLAGS   := $(WARNINGS) -std=c11 -I$(INC_DIR)
CXXFLAGS := $(WARNINGS) -std=c++17 -I$(INC_DIR)

# Dependency generation
DEPFLAGS := -MMD -MP

# Linker
LDFLAGS := -lm

# ---------------------------------------------------------------------------
# Source Files
# ---------------------------------------------------------------------------
SRCS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/stack.c \
	$(SRC_DIR)/tokenizer.c \
	$(SRC_DIR)/postfix.c \
	$(SRC_DIR)/evaluate.c \
	$(SRC_DIR)/parser.c \
	$(SRC_DIR)/validator.c \
	$(SRC_DIR)/history.c \
	$(SRC_DIR)/memory.c \
	$(SRC_DIR)/variables.c \
	$(SRC_DIR)/functions.c \
	$(SRC_DIR)/function_info.c \
	$(SRC_DIR)/constants.c \
	$(SRC_DIR)/angle_mode.c \
	$(SRC_DIR)/units.c \
	$(SRC_DIR)/stats.c \
	$(SRC_DIR)/base.c \
	$(SRC_DIR)/plot.c \
       $(SRC_DIR)/error.c

CXX_SRCS := \
	$(SRC_DIR)/Complex.cpp \
	$(SRC_DIR)/complex_eval.cpp \
	$(SRC_DIR)/Matrix.cpp \
	$(SRC_DIR)/matrix_eval.cpp

# ---------------------------------------------------------------------------
# Object Files
# ---------------------------------------------------------------------------
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CXX_OBJS := $(CXX_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# ---------------------------------------------------------------------------
# Headers
# ---------------------------------------------------------------------------
HEADERS := \
	$(INC_DIR)/calculator.h \
	$(INC_DIR)/stack.h \
	$(INC_DIR)/validator.h \
	$(INC_DIR)/history.h \
	$(INC_DIR)/memory.h \
	$(INC_DIR)/variables.h \
	$(INC_DIR)/functions.h \
	$(INC_DIR)/function_info.h \
	$(INC_DIR)/token.h \
	$(INC_DIR)/constants.h \
	$(INC_DIR)/angle_mode.h \
	$(INC_DIR)/units.h \
	$(INC_DIR)/complex_eval.h \
	$(INC_DIR)/matrix_eval.h \
	$(INC_DIR)/stats.h \
	$(INC_DIR)/base.h \
	$(INC_DIR)/plot.h \
       $(INC_DIR)/error.h

CXX_HEADERS := \
	$(INC_DIR)/Complex.hpp \
	$(INC_DIR)/Matrix.hpp

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
TEST_SRCS := \
	$(TEST_DIR)/test_main.c \
	$(TEST_DIR)/test_stack.c \
	$(TEST_DIR)/test_tokenizer.c \
	$(TEST_DIR)/test_validator.c \
	$(TEST_DIR)/test_parser.c \
	$(TEST_DIR)/test_postfix.c \
	$(TEST_DIR)/test_functions.c \
	$(TEST_DIR)/test_variables.c \
	$(TEST_DIR)/test_history.c \
	$(TEST_DIR)/test_memory.c

TESTED_SRCS := $(filter-out $(SRC_DIR)/main.c,$(SRCS))

# ---------------------------------------------------------------------------
# Default Target
# ---------------------------------------------------------------------------
all: $(TARGET)

# ---------------------------------------------------------------------------
# Main Executable
# ---------------------------------------------------------------------------
$(TARGET): $(OBJS) $(CXX_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# ---------------------------------------------------------------------------
# Compile C
# ---------------------------------------------------------------------------
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# ---------------------------------------------------------------------------
# Compile C++
# ---------------------------------------------------------------------------
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(CXX_HEADERS) \
	$(INC_DIR)/complex_eval.h \
	$(INC_DIR)/matrix_eval.h
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

# ---------------------------------------------------------------------------
# Run
# ---------------------------------------------------------------------------
run: $(TARGET)
	./$(TARGET)

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRCS) $(TESTED_SRCS)
	$(CC) $(CFLAGS) -I$(TEST_DIR) \
	-o $@ \
	$(TEST_SRCS) \
	$(TESTED_SRCS) \
	$(LDFLAGS)

# ---------------------------------------------------------------------------
# Debug Build
# ---------------------------------------------------------------------------
debug: CFLAGS += -O0 -g3
debug: CXXFLAGS += -O0 -g3
debug: rebuild

# ---------------------------------------------------------------------------
# Release Build
# ---------------------------------------------------------------------------
release: CFLAGS += -O3 -DNDEBUG
release: CXXFLAGS += -O3 -DNDEBUG
release: rebuild

# ---------------------------------------------------------------------------
# Address Sanitizer
# ---------------------------------------------------------------------------
asan: CFLAGS += -fsanitize=address -g
asan: CXXFLAGS += -fsanitize=address -g
asan: LDFLAGS += -fsanitize=address
asan: rebuild

# ---------------------------------------------------------------------------
# Undefined Behavior Sanitizer
# ---------------------------------------------------------------------------
ubsan: CFLAGS += -fsanitize=undefined -g
ubsan: CXXFLAGS += -fsanitize=undefined -g
ubsan: LDFLAGS += -fsanitize=undefined
ubsan: rebuild

# ---------------------------------------------------------------------------
# Documentation
# ---------------------------------------------------------------------------
docs:
	doxygen Doxyfile

# ---------------------------------------------------------------------------
# Clean
# ---------------------------------------------------------------------------
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(OBJ_DIR)/*.d
	rm -f $(TARGET)
	rm -f $(TEST_TARGET)

distclean: clean

# ---------------------------------------------------------------------------
# Rebuild
# ---------------------------------------------------------------------------
rebuild: clean all

# ---------------------------------------------------------------------------
# Dependency Files
# ---------------------------------------------------------------------------
-include $(OBJS:.o=.d)
-include $(CXX_OBJS:.o=.d)

# ---------------------------------------------------------------------------
# Phony Targets
# ---------------------------------------------------------------------------
.PHONY: \
	all \
	run \
	test \
	debug \
	release \
	asan \
	ubsan \
	clean \
	distclean \
	rebuild \
	docs
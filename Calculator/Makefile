# ==================================================================
# Calculator - Makefile
#
# Build types (select with BUILD=<type>, default is debug):
#   debug    -g -O0, no optimization, best for local development
#   release  -O2 -DNDEBUG, optimized binary for actual use
#   asan     debug build + AddressSanitizer + UndefinedBehaviorSanitizer
#            (catches buffer overruns, use-after-free, UB, etc.)
#   ubsan    debug build + UndefinedBehaviorSanitizer only
#
# Each build type gets its own object directory (Obj/<type>) so
# switching between them never links stale objects from another
# configuration.
#
# Header dependencies are tracked automatically via -MMD -MP, so a
# change to any header correctly triggers a rebuild of everything
# that includes it -- no manually-maintained header list to forget
# to update.
#
# Common commands:
#   make                 -> debug build
#   make release         -> optimized build
#   make asan && make run BUILD=asan   -> run under sanitizers
#   make test            -> build and run the unit test suite
#   make docs            -> generate Doxygen HTML docs (see docs/Doxyfile)
#   make clean           -> remove the current BUILD's objects/binaries
#   make distclean       -> remove every build type's objects/binaries
# ==================================================================

CC  = gcc
CXX = g++

SRC_DIR  = Src
INC_DIR  = Inc
TEST_DIR = Tests
DOCS_DIR = docs

TARGET      = calculator
TEST_TARGET = run_tests

# ------------------------------------------------------------------
# Build type configuration
# ------------------------------------------------------------------
BUILD ?= debug

ifeq ($(BUILD),debug)
    OPT_FLAGS = -g -O0 -DDEBUG
    SAN_FLAGS =
else ifeq ($(BUILD),release)
    OPT_FLAGS = -O2 -DNDEBUG
    SAN_FLAGS =
else ifeq ($(BUILD),asan)
    OPT_FLAGS = -g -O0 -fno-omit-frame-pointer
    SAN_FLAGS = -fsanitize=address,undefined
else ifeq ($(BUILD),ubsan)
    OPT_FLAGS = -g -O0 -fno-omit-frame-pointer
    SAN_FLAGS = -fsanitize=undefined
else
    $(error Unknown BUILD '$(BUILD)'. Use one of: debug, release, asan, ubsan)
endif

OBJ_DIR      = Obj/$(BUILD)
TEST_OBJ_DIR = Obj/$(BUILD)-tests

WARN_FLAGS = -Wall -Wextra
DEPFLAGS   = -MMD -MP

CFLAGS   = $(WARN_FLAGS) -std=c11   -I$(INC_DIR) $(OPT_FLAGS) $(SAN_FLAGS) $(DEPFLAGS)
CXXFLAGS = $(WARN_FLAGS) -std=c++17 -I$(INC_DIR) $(OPT_FLAGS) $(SAN_FLAGS) $(DEPFLAGS)
LDFLAGS  = -lm $(SAN_FLAGS)

# ------------------------------------------------------------------
# Sources
# ------------------------------------------------------------------
SRCS = $(SRC_DIR)/main.c \
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
       $(SRC_DIR)/plot.c

# C++ sources, isolated to the complex-number / matrix features --
# see complex_eval.h / matrix_eval.h for the C-facing interfaces.
CXX_SRCS = $(SRC_DIR)/Complex.cpp \
           $(SRC_DIR)/complex_eval.cpp \
           $(SRC_DIR)/Matrix.cpp \
           $(SRC_DIR)/matrix_eval.cpp

OBJS     = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CXX_OBJS = $(CXX_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# ------------------------------------------------------------------
# Phase 20 -- Unit tests
#
# Tests link the real module .c files directly (not through the
# library), swapping in Tests/test_main.c in place of Src/main.c so
# there's exactly one main(). This keeps the tests exercising the
# actual production code, not a reimplementation of it.
# ------------------------------------------------------------------
TEST_SRCS = $(TEST_DIR)/test_main.c \
            $(TEST_DIR)/test_stack.c \
            $(TEST_DIR)/test_tokenizer.c \
            $(TEST_DIR)/test_validator.c \
            $(TEST_DIR)/test_parser.c \
            $(TEST_DIR)/test_postfix.c \
            $(TEST_DIR)/test_functions.c \
            $(TEST_DIR)/test_variables.c \
            $(TEST_DIR)/test_history.c \
            $(TEST_DIR)/test_memory.c

# Every module under test, EXCEPT main.c (Tests/test_main.c supplies
# main() instead).
TESTED_SRCS = $(filter-out $(SRC_DIR)/main.c,$(SRCS))

TEST_OBJS      = $(TEST_SRCS:$(TEST_DIR)/%.c=$(TEST_OBJ_DIR)/test_%.o)
TESTED_OBJS    = $(TESTED_SRCS:$(SRC_DIR)/%.c=$(TEST_OBJ_DIR)/%.o)

# ------------------------------------------------------------------
# Primary targets
# ------------------------------------------------------------------
.PHONY: all debug release asan ubsan run test docs clean distclean rebuild

all: $(TARGET)

debug:
	$(MAKE) BUILD=debug all

release:
	$(MAKE) BUILD=release all

asan:
	$(MAKE) BUILD=asan all

ubsan:
	$(MAKE) BUILD=ubsan all

# Link (uses g++ since the C++ objects need libstdc++)
$(TARGET): $(OBJS) $(CXX_OBJS)
	$(CXX) -o $@ $(OBJS) $(CXX_OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run: $(TARGET)
	./$(TARGET)

# ------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS) $(TESTED_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_OBJ_DIR)/test_%.o: $(TEST_DIR)/%.c | $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) -I$(TEST_DIR) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) -I$(TEST_DIR) -c $< -o $@

$(TEST_OBJ_DIR):
	mkdir -p $(TEST_OBJ_DIR)

# ------------------------------------------------------------------
# Docs (Doxygen) -- see docs/Doxyfile
# ------------------------------------------------------------------
docs:
	doxygen $(DOCS_DIR)/Doxyfile

# ------------------------------------------------------------------
# Housekeeping
# ------------------------------------------------------------------
clean:
	rm -rf $(OBJ_DIR) $(TEST_OBJ_DIR) $(TARGET) $(TEST_TARGET)

distclean:
	rm -rf Obj $(TARGET) $(TEST_TARGET)
	rm -rf $(DOCS_DIR)/html $(DOCS_DIR)/latex

rebuild: clean all

# Pull in auto-generated header dependencies, if any exist yet.
-include $(wildcard $(OBJ_DIR)/*.d)
-include $(wildcard $(TEST_OBJ_DIR)/*.d)
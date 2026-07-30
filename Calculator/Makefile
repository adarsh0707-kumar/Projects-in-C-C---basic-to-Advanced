# Compiler
CC = gcc
CXX = g++

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -IInc
CXXFLAGS = -Wall -Wextra -std=c++17 -IInc

# Linker flags
LDFLAGS = -lm

# Directories
SRC_DIR = Src
OBJ_DIR = Obj
INC_DIR = Inc

# Target executable
TARGET = calculator

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/stack.c \
       $(SRC_DIR)/tokenizer.c \
       $(SRC_DIR)/postfix.c \
       $(SRC_DIR)/evaluate.c\
       $(SRC_DIR)/parser.c\
       $(SRC_DIR)/validator.c\
       $(SRC_DIR)/history.c\
       $(SRC_DIR)/memory.c\
       $(SRC_DIR)/variables.c\
       $(SRC_DIR)/functions.c\
       $(SRC_DIR)/function_info.c\
       $(SRC_DIR)/constants.c\
       $(SRC_DIR)/angle_mode.c\
       $(SRC_DIR)/units.c\
       $(SRC_DIR)/stats.c\
       $(SRC_DIR)/base.c \
       $(SRC_DIR)/plot.c

# C++ source files (isolated to the complex-number feature —
# see complex_eval.h for the C-facing interface)
CXX_SRCS = $(SRC_DIR)/Complex.cpp \
           $(SRC_DIR)/complex_eval.cpp \
           $(SRC_DIR)/Matrix.cpp \
           $(SRC_DIR)/matrix_eval.cpp

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CXX_OBJS = $(CXX_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Header files
HEADERS = \
       $(INC_DIR)/calculator.h \
       $(INC_DIR)/stack.h \
       $(INC_DIR)/validator.h \
       $(INC_DIR)/history.h \
       $(INC_DIR)/memory.h \
       $(INC_DIR)/variables.h\
       $(INC_DIR)/functions.h\
       $(INC_DIR)/function_info.h\
       $(INC_DIR)/token.h\
       $(INC_DIR)/constants.h\
       $(INC_DIR)/angle_mode.h\
       $(INC_DIR)/units.h\
       $(INC_DIR)/complex_eval.h\
       $(INC_DIR)/matrix_eval.h\
       $(INC_DIR)/stats.h\
       $(INC_DIR)/base.h \
       $(INC_DIR)/plot.h

CXX_HEADERS = $(INC_DIR)/Complex.hpp $(INC_DIR)/Matrix.hpp

# ---------------------------------------------------------------
# Phase 20 -- Unit tests
#
# Tests link the real module .c files directly (not through the
# library), swapping in Tests/test_main.c in place of Src/main.c so
# there's exactly one main(). This keeps the tests exercising the
# actual production code, not a reimplementation of it.
# ---------------------------------------------------------------
TEST_DIR = Tests
TEST_TARGET = run_tests

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

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRCS) $(TESTED_SRCS) $(HEADERS) $(TEST_DIR)/test_framework.h $(TEST_DIR)/tests.h
	$(CC) $(CFLAGS) -I$(TEST_DIR) -o $@ $(TEST_SRCS) $(TESTED_SRCS) $(LDFLAGS)

# Default target
all: $(TARGET)

# Link object files (uses g++ as the linker since CXX_OBJS need libstdc++)
$(TARGET): $(OBJS) $(CXX_OBJS)
	$(CXX) -o $@ $(OBJS) $(CXX_OBJS) $(LDFLAGS)

# Compile C source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(CXX_HEADERS) $(INC_DIR)/complex_eval.h $(INC_DIR)/matrix_eval.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run program
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET) $(TEST_TARGET)

# Rebuild everything
rebuild: clean all

.PHONY: all clean run rebuild test
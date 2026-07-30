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
       $(SRC_DIR)/stats.c

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
       $(INC_DIR)/stats.h

CXX_HEADERS = $(INC_DIR)/Complex.hpp $(INC_DIR)/Matrix.hpp

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
	rm -f $(OBJ_DIR)/*.o $(TARGET)

# Rebuild everything
rebuild: clean all

.PHONY: all clean run rebuild
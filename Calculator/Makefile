# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -IInc

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
       $(SRC_DIR)/angle_mode.c

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

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
       $(INC_DIR)/angle_mode.h

# Default target
all: $(TARGET)

# Link object files
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Run program
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

# Rebuild everything
rebuild: clean all

.PHONY: all clean run rebuild
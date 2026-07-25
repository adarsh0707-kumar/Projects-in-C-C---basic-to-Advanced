# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -IInc

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
       $(SRC_DIR)/memory.c

# Object files
OBJS = $(OBJ_DIR)/main.o \
       $(OBJ_DIR)/stack.o \
       $(OBJ_DIR)/tokenizer.o \
       $(OBJ_DIR)/postfix.o \
       $(OBJ_DIR)/evaluate.o\
	$(OBJ_DIR)/parser.o\
       $(OBJ_DIR)/validator.o\
       $(OBJ_DIR)/history.o\
       $(SRC_DIR)/memory.o

# Header files
HEADERS = $(INC_DIR)/stack.h $(INC_DIR)/calculator.h

# Default target
all: $(TARGET)

# Link object files
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

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
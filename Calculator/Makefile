CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = calculator

SRCS = main.c \
       stack.c \
       tokenizer.c \
       postfix.c \
       evaluate.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c stack.h calculator.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
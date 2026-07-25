CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = calculator

SRCS = main.c \
       Sum.c \
       Sub.c \
       Mult.c \
       Div.c \
       Modulus.c \
#        Percentage.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c main.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
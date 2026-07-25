#include"stack.h"
#include<stdio.h>
#include<stdlib.h>

// Function to initialize the Double stack
void initDoubleStack(DoubleStack *s){
    s->top = -1;
}

// Function to push an Double onto the stack
void pushDouble(DoubleStack *s, double value){
    if (s->top == MAX - 1)
    {
        printf("Error: Stack Overflow\n");
        exit(EXIT_FAILURE);
    }

    s->items[++s->top] = value;
}

// Function to pop an integer from the stack
double popDouble(DoubleStack *s){
    if (s->top == -1)
    {
        printf("Error: Stack Underflow\n");
        exit(EXIT_FAILURE);
    }

    return s->items[s->top--];
}

// Function to peek at the top integer of the stack without popping it
double peekDouble(DoubleStack *s){
    if (s->top == -1)
    {
        printf("Error: Stack is Empty\n");
        exit(EXIT_FAILURE);
    }

    return s->items[s->top];
}

// Function to check if the Double stack is empty
int isEmptyDoubleStack(DoubleStack *s){
    return s->top == -1;
}


// Function to initialize the Character stack
void initCharStack(CharStack *s){
    s->top = -1;
}

// Function to push a character onto the stack
void pushChar(CharStack *s, char value){
    if (s->top == MAX - 1)
    {
        printf("Error: Stack Overflow\n");
        exit(EXIT_FAILURE);
    }

    s->items[++s->top] = value;
}

// Function to pop a character from the stack
char popChar(CharStack *s){
    if (s->top == -1)
    {
        printf("Error: Stack Underflow\n");
        exit(EXIT_FAILURE);
    }

    return s->items[s->top--];
}

// Function to peek at the top character of the stack without popping it
char peekChar(CharStack *s){
    if (s->top == -1)
    {
        printf("Error: Stack is Empty\n");
        exit(EXIT_FAILURE);
    }

    return s->items[s->top];
}

// Function to check if the Character stack is empty
int isEmptyCharStack(CharStack *s){
    return s->top == -1;
}

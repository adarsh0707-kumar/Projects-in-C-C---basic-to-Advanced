#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Inc/error.h"

/*==============================
    Double Stack Functions
==============================*/

// Function to initialize the Double stack
void initDoubleStack(DoubleStack *s)
{
    s->top = -1;
}

// Function to push an Double onto the stack
void pushDouble(DoubleStack *s, double value)
{
    if (s->top == MAX - 1)
    {
        calculatorSetLastError(CALC_ERR_STACK_OVERFLOW);
        return;
    }

    s->items[++s->top] = value;
}

// Function to pop an integer from the stack
double popDouble(DoubleStack *s)
{
    if (s->top == -1)
    {
        calculatorSetLastError(CALC_ERR_STACK_UNDERFLOW);
    }

    return s->items[s->top--];
}

// Function to peek at the top integer of the stack without popping it
double peekDouble(DoubleStack *s)
{
    if (s->top == -1)
    {
        printf("Error: Stack is Empty\n");
        exit(EXIT_FAILURE);
    }

    return s->items[s->top];
}

// Function to check if the Double stack is empty
int isEmptyDoubleStack(DoubleStack *s)
{
    return s->top == -1;
}

/*==============================
    Char Stack Functions
==============================*/

// Function to initialize the Character stack
void initCharStack(CharStack *s)
{
    s->top = -1;
}

// Function to push a character onto the stack
void pushChar(CharStack *s, char value)
{
    if (s->top == MAX - 1)
    {
        calculatorSetLastError(CALC_ERR_STACK_OVERFLOW);
        return;
    }

    s->items[++s->top] = value;
}

// Function to pop a character from the stack
char popChar(CharStack *s)
{
    if (s->top == -1)
    {
        calculatorSetLastError(CALC_ERR_STACK_UNDERFLOW);
    }

    return s->items[s->top--];
}

// Function to peek at the top character of the stack without popping it
char peekChar(CharStack *s)
{
    if (s->top == -1)
    {
        printf("Error: Stack is Empty\n");
        exit(EXIT_FAILURE);
    }

    return s->items[s->top];
}

// Function to check if the Character stack is empty
int isEmptyCharStack(CharStack *s)
{
    return s->top == -1;
}

/*==============================
    String Stack Functions
==============================*/

void initStringStack(StringStack *s)
{
    s->top = -1;
}

void pushString(StringStack *s, const char str[])
{
    if (s->top == MAX - 1)
    {
        calculatorSetLastError(CALC_ERR_STACK_OVERFLOW);
        return;
    }

    strcpy(s->items[++s->top], str);
}

char *popString(StringStack *s)
{
    if (s->top == -1)
    {
        calculatorSetLastError(CALC_ERR_STACK_UNDERFLOW);
    }

    return s->items[s->top--];
}

char *peekString(StringStack *s)
{
    if (s->top == -1)
    {
        printf("Error: String Stack Empty\n");
        exit(EXIT_FAILURE);
    }

    return s->items[s->top];
}

int isEmptyStringStack(StringStack *s)
{
    return s->top == -1;
}

/*==============================
    Token Stack Functions
==============================*/

void initTokenStack(TokenStack *s)
{
    s->top = -1;
}

void pushToken(TokenStack *s, Token value)
{
    if (s->top == MAX - 1)
    {
        calculatorSetLastError(CALC_ERR_STACK_OVERFLOW);
        return;
    }

    s->items[++s->top] = value;
}

Token popToken(TokenStack *s)
{
    if (s->top == -1)
    {
        calculatorSetLastError(CALC_ERR_STACK_UNDERFLOW);
    }

    return s->items[s->top--];
}

Token peekToken(TokenStack *s)
{
    if (s->top == -1)
    {
        printf("Token Stack Empty\n");
        exit(EXIT_FAILURE);
    }

    return s->items[s->top];
}

int isEmptyTokenStack(TokenStack *s)
{
    return s->top == -1;
}

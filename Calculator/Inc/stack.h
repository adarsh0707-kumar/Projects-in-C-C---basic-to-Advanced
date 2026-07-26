#ifndef STACK_H
#define STACK_H

#include "token.h"

#define MAX 100

typedef struct{
    double items[MAX];
    int top;
} DoubleStack;

typedef struct{
    char items[MAX];
    int top;
} CharStack;

typedef struct
{
    char items[MAX][32];
    int top;
} StringStack;

typedef struct
{
    Token items[MAX];
    int top;
} TokenStack;

// Function to initialize the Double stack
void initDoubleStack(DoubleStack *s);
void pushDouble(DoubleStack *s, double value);
double popDouble(DoubleStack *s);
double peekDouble(DoubleStack *s);
int isEmptyDoubleStack(DoubleStack *s);

// Function to initialize the Character stack
void initCharStack(CharStack *s);
void pushChar(CharStack *s, char value);
char popChar(CharStack *s);
char peekChar(CharStack *s);
int isEmptyCharStack(CharStack *s);

// Function to initialize the Strings stack
void initStringStack(StringStack *s);
void pushString(StringStack *s, const char str[]);
char *popString(StringStack *s);
char *peekString(StringStack *s);
int isEmptyStringStack(StringStack *s);

// Function to initialize the Token stack
void initTokenStack(TokenStack *s);
void pushToken(TokenStack *s, Token value);
Token popToken(TokenStack *s);
Token peekToken(TokenStack *s);
int isEmptyTokenStack(TokenStack *s);
#endif

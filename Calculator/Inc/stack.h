/**
 * @file stack.h
 * @brief Fixed-capacity stacks used throughout the expression pipeline:
 * doubles (postfix evaluation), operators, string tokens, and full
 * Token values (shunting-yard conversion).
 *
 * All four stack types share the same fail-loud convention: pushing
 * past #MAX or popping/peeking an empty stack prints an error and
 * calls exit() rather than silently corrupting memory or returning
 * garbage (see docs/RULES.md and Tests/test_stack.c).
 */
#ifndef STACK_H
#define STACK_H

#include "token.h"

/** Maximum number of elements any stack type below can hold. */
#define MAX 100

/** A stack of doubles, used while evaluating a postfix expression. */
typedef struct
{
    double items[MAX]; /**< Backing storage. */
    int top;           /**< Index of the top element, -1 when empty. */
} DoubleStack;

/** A stack of single characters, e.g. pending operators. */
typedef struct
{
    char items[MAX]; /**< Backing storage. */
    int top;         /**< Index of the top element, -1 when empty. */
} CharStack;

/** A stack of short strings (function/variable names, etc.). */
typedef struct
{
    char items[MAX][32]; /**< Backing storage. */
    int top;             /**< Index of the top element, -1 when empty. */
} StringStack;

/** A stack of full Token values, used while converting infix to postfix. */
typedef struct
{
    Token items[MAX]; /**< Backing storage. */
    int top;          /**< Index of the top element, -1 when empty. */
} TokenStack;

/** @brief Initializes an empty DoubleStack. */
void initDoubleStack(DoubleStack *s);
/** @brief Pushes a value. Exits the program on overflow. */
void pushDouble(DoubleStack *s, double value);
/** @brief Pops and returns the top value. Exits the program on underflow. */
double popDouble(DoubleStack *s);
/** @brief Returns the top value without removing it. Exits on empty. */
double peekDouble(DoubleStack *s);
/** @brief Returns 1 if the stack has no elements, 0 otherwise. */
int isEmptyDoubleStack(DoubleStack *s);

/** @brief Initializes an empty CharStack. */
void initCharStack(CharStack *s);
/** @brief Pushes a character. Exits the program on overflow. */
void pushChar(CharStack *s, char value);
/** @brief Pops and returns the top character. Exits on underflow. */
char popChar(CharStack *s);
/** @brief Returns the top character without removing it. Exits on empty. */
char peekChar(CharStack *s);
/** @brief Returns 1 if the stack has no elements, 0 otherwise. */
int isEmptyCharStack(CharStack *s);

/** @brief Initializes an empty StringStack. */
void initStringStack(StringStack *s);
/** @brief Pushes a copy of @p str (must fit in 31 chars + null). Exits
 *  the program on overflow. */
void pushString(StringStack *s, const char str[]);
/** @brief Pops and returns a pointer to the top string. Exits on
 *  underflow. */
char *popString(StringStack *s);
/** @brief Returns a pointer to the top string without removing it.
 *  Exits on empty. */
char *peekString(StringStack *s);
/** @brief Returns 1 if the stack has no elements, 0 otherwise. */
int isEmptyStringStack(StringStack *s);

/** @brief Initializes an empty TokenStack. */
void initTokenStack(TokenStack *s);
/** @brief Pushes a token. Exits the program on overflow. */
void pushToken(TokenStack *s, Token value);
/** @brief Pops and returns the top token. Exits the program on underflow. */
Token popToken(TokenStack *s);
/** @brief Returns the top token without removing it. Exits on empty. */
Token peekToken(TokenStack *s);
/** @brief Returns 1 if the stack has no elements, 0 otherwise. */
int isEmptyTokenStack(TokenStack *s);

#endif

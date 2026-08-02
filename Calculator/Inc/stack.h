/**
 * @file stack.h
 * @brief Fixed-capacity stacks used throughout the expression pipeline:
 * doubles (postfix evaluation), operators (parenthesis validation),
 * string tokens, and full Token values (shunting-yard conversion).
 *
 * Push/pop/peek all return 1 on success and 0 on failure (stack full
 * for push, stack empty for pop/peek), recording *why* via
 * calculatorSetLastError() (see error.h) rather than calling exit().
 * This is the module most directly exposed to adversarial or simply
 * very large input (e.g. a pathologically deep expression), so it's
 * the first one migrated off the project's earlier fail-loud-via-
 * exit() convention -- see docs/CHANGELOG.md.
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

/** A stack of single characters, e.g. pending operators/parentheses. */
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
/** @brief Pushes a value.
 *  @return 1 on success, 0 if the stack is already full
 *  (CALC_ERR_STACK_OVERFLOW). */
int pushDouble(DoubleStack *s, double value);
/** @brief Pops the top value into *out.
 *  @return 1 on success, 0 if the stack is empty
 *  (CALC_ERR_STACK_UNDERFLOW); *out is left unmodified on failure. */
int popDouble(DoubleStack *s, double *out);
/** @brief Copies the top value into *out without removing it.
 *  @return 1 on success, 0 if the stack is empty
 *  (CALC_ERR_STACK_UNDERFLOW); *out is left unmodified on failure. */
int peekDouble(DoubleStack *s, double *out);
/** @brief Returns 1 if the stack has no elements, 0 otherwise. */
int isEmptyDoubleStack(DoubleStack *s);

/** @brief Initializes an empty CharStack. */
void initCharStack(CharStack *s);
/** @brief Pushes a character.
 *  @return 1 on success, 0 if the stack is already full. */
int pushChar(CharStack *s, char value);
/** @brief Pops the top character into *out.
 *  @return 1 on success, 0 if the stack is empty; *out is left
 *  unmodified on failure. */
int popChar(CharStack *s, char *out);
/** @brief Copies the top character into *out without removing it.
 *  @return 1 on success, 0 if the stack is empty; *out is left
 *  unmodified on failure. */
int peekChar(CharStack *s, char *out);
/** @brief Returns 1 if the stack has no elements, 0 otherwise. */
int isEmptyCharStack(CharStack *s);

/** @brief Initializes an empty StringStack. */
void initStringStack(StringStack *s);
/** @brief Pushes a copy of @p str (must fit in 31 chars + null).
 *  @return 1 on success, 0 if the stack is already full. */
int pushString(StringStack *s, const char str[]);
/** @brief Copies the top string into @p out (a buffer of at least
 *  @p outSize bytes) and removes it from the stack.
 *  @return 1 on success, 0 if the stack is empty. */
int popString(StringStack *s, char out[], int outSize);
/** @brief Copies the top string into @p out without removing it.
 *  @return 1 on success, 0 if the stack is empty. */
int peekString(StringStack *s, char out[], int outSize);
/** @brief Returns 1 if the stack has no elements, 0 otherwise. */
int isEmptyStringStack(StringStack *s);

/** @brief Initializes an empty TokenStack. */
void initTokenStack(TokenStack *s);
/** @brief Pushes a token.
 *  @return 1 on success, 0 if the stack is already full. */
int pushToken(TokenStack *s, Token value);
/** @brief Pops the top token into *out.
 *  @return 1 on success, 0 if the stack is empty; *out is left
 *  unmodified on failure. */
int popToken(TokenStack *s, Token *out);
/** @brief Copies the top token into *out without removing it.
 *  @return 1 on success, 0 if the stack is empty; *out is left
 *  unmodified on failure. */
int peekToken(TokenStack *s, Token *out);
/** @brief Returns 1 if the stack has no elements, 0 otherwise. */
int isEmptyTokenStack(TokenStack *s);

#endif

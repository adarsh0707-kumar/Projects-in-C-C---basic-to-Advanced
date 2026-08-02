#include "stack.h"
#include "../Inc/error.h"
#include <string.h>

/*==============================
    Double Stack Functions
==============================*/

void initDoubleStack(DoubleStack *s)
{
    s->top = -1;
}

int pushDouble(DoubleStack *s, double value)
{
    if (s->top == MAX - 1)
    {
        calculatorSetLastError(CALC_ERR_STACK_OVERFLOW);
        return 0;
    }

    s->items[++s->top] = value;
    return 1;
}

int popDouble(DoubleStack *s, double *out)
{
    if (s->top == -1)
    {
        calculatorSetLastError(CALC_ERR_STACK_UNDERFLOW);
        return 0;
    }

    *out = s->items[s->top--];
    return 1;
}

int peekDouble(DoubleStack *s, double *out)
{
    if (s->top == -1)
    {
        calculatorSetLastError(CALC_ERR_STACK_UNDERFLOW);
        return 0;
    }

    *out = s->items[s->top];
    return 1;
}

int isEmptyDoubleStack(DoubleStack *s)
{
    return s->top == -1;
}

/*==============================
    Char Stack Functions
==============================*/

void initCharStack(CharStack *s)
{
    s->top = -1;
}

int pushChar(CharStack *s, char value)
{
    if (s->top == MAX - 1)
    {
        calculatorSetLastError(CALC_ERR_STACK_OVERFLOW);
        return 0;
    }

    s->items[++s->top] = value;
    return 1;
}

int popChar(CharStack *s, char *out)
{
    if (s->top == -1)
    {
        calculatorSetLastError(CALC_ERR_STACK_UNDERFLOW);
        return 0;
    }

    *out = s->items[s->top--];
    return 1;
}

int peekChar(CharStack *s, char *out)
{
    if (s->top == -1)
    {
        calculatorSetLastError(CALC_ERR_STACK_UNDERFLOW);
        return 0;
    }

    *out = s->items[s->top];
    return 1;
}

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

int pushString(StringStack *s, const char str[])
{
    if (s->top == MAX - 1)
    {
        calculatorSetLastError(CALC_ERR_STACK_OVERFLOW);
        return 0;
    }

    strncpy(s->items[s->top + 1], str, sizeof(s->items[0]) - 1);
    s->items[s->top + 1][sizeof(s->items[0]) - 1] = '\0';
    s->top++;
    return 1;
}

int popString(StringStack *s, char out[], int outSize)
{
    if (s->top == -1)
    {
        calculatorSetLastError(CALC_ERR_STACK_UNDERFLOW);
        return 0;
    }

    strncpy(out, s->items[s->top--], outSize - 1);
    out[outSize - 1] = '\0';
    return 1;
}

int peekString(StringStack *s, char out[], int outSize)
{
    if (s->top == -1)
    {
        calculatorSetLastError(CALC_ERR_STACK_UNDERFLOW);
        return 0;
    }

    strncpy(out, s->items[s->top], outSize - 1);
    out[outSize - 1] = '\0';
    return 1;
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

int pushToken(TokenStack *s, Token value)
{
    if (s->top == MAX - 1)
    {
        calculatorSetLastError(CALC_ERR_STACK_OVERFLOW);
        return 0;
    }

    s->items[++s->top] = value;
    return 1;
}

int popToken(TokenStack *s, Token *out)
{
    if (s->top == -1)
    {
        calculatorSetLastError(CALC_ERR_STACK_UNDERFLOW);
        return 0;
    }

    *out = s->items[s->top--];
    return 1;
}

int peekToken(TokenStack *s, Token *out)
{
    if (s->top == -1)
    {
        calculatorSetLastError(CALC_ERR_STACK_UNDERFLOW);
        return 0;
    }

    *out = s->items[s->top];
    return 1;
}

int isEmptyTokenStack(TokenStack *s)
{
    return s->top == -1;
}

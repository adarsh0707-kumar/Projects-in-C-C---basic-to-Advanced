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

    /*
    outSize is the caller's word for how big out[] is, and nothing checked
    it. At zero, "outSize - 1" is -1, which strncpy takes as a size_t: it
    becomes SIZE_MAX and the copy runs until it happens upon a NUL. The
    following line then writes out[-1], before the buffer.

    Every caller today passes sizeof(buffer), so this was a hazard rather
    than a live defect -- but it is the callee's job to refuse a size it
    cannot honour, and the cast below is only safe because of this check.
    */
    if (outSize <= 0)
    {
        calculatorSetLastError(CALC_ERR_INTERNAL);
        return 0;
    }

    strncpy(out, s->items[s->top--], (size_t)(outSize - 1));
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

    /* Same unguarded size as popString above. */
    if (outSize <= 0)
    {
        calculatorSetLastError(CALC_ERR_INTERNAL);
        return 0;
    }

    strncpy(out, s->items[s->top], (size_t)(outSize - 1));
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

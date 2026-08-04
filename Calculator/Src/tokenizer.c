#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "error.h"

double readNumber(char expression[], int *index)
{
    int dotCount = 0;
    int start = *index;

    /* Consume an optional leading '-' (evaluate.c only calls this
       function when it has already confirmed a digit or '.' follows) */
    if (expression[*index] == '-')
        (*index)++;

    while (isdigit(expression[*index]) || expression[*index] == '.')
    {
        if (expression[*index] == '.')
            dotCount++;

        if (dotCount > 1)
        {
            /* Unreachable through the normal CLI pipeline --
               infixToPostfix() already rejects a number with more
               than one decimal point before it's ever written into
               the postfix string this function reads from. Kept as
               a safety net for any other caller. */
            calculatorSetLastError(CALC_ERR_INVALID_TOKEN);
            return NAN;
        }

        (*index)++;
    }

    char temp[64];
    int len = *index - start;

    if (len >= (int)sizeof(temp))
    {
        /* Unreachable through the normal CLI pipeline -- numbers this
           long can't occur in valid input. Kept as a safety net. */
        calculatorSetLastError(CALC_ERR_INVALID_TOKEN);
        return NAN;
    }

    for (int i = 0; i < len; i++)
        temp[i] = expression[start + i];

    temp[len] = '\0';

    return atof(temp);
}
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "validator.h"
#include <stdlib.h>

int validateExpression(char expression[])
{
    int i = 0;
    int dotCourt = 0;
    int expectiongOperand = 1;

    while (expression[i] != '\0')
    {
        char c = expression[i];
        if (c == ' ')
        {
            i++;
            continue;
        }
        // Numer
        if (isdigit(c))
        {
            dotCourt = 0;
            while (isdigit(expression[i]) || expression[i] == '.')
            {
                if (expression[i] == '.')
                {
                    dotCourt++;
                    if (dotCourt > 1)
                    {
                        printf("Error: Invalid number formate.\n");
                        return 0;
                    }
                }
                i++;
            }
            expectiongOperand = 0;
            continue;
        }

        // Decimal beginning with '.'
        if (c == '.')
        {
            printf("Error: Number cannot start with '.'\n");
            return 0;
        }

        // Operators
        if (c == '+' || c == '*' || c == '/' || c == '%')
        {
            if (expectiongOperand)
            {
                printf("Error: Missing operand.\n");
                return 0;
            }
            expectiongOperand = 1;
            i++;
            continue;
        }

        // Minus
        if (c == '-')
        {
            i++;
            continue;
        }

        // Parentheses
        if (c == '(' || c == ')')
        {
            i++;
            continue;
        }

        printf("Error: Invalid character '%c'\n", c);
        return 0;
    }
    return 1;
}
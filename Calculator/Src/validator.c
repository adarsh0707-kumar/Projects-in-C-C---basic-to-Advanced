#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "validator.h"
#include <stdlib.h>

typedef enum
{
    EXPECT_OPERAND,
    AFTER_OPERAND
} ParserState;

int validateExpression(char expression[])
{
    ParserState state = EXPECT_OPERAND;

    int i = 0;
    int balance = 0;

    while (expression[i] != '\0')
    {
        char c = expression[i];

        if (isspace(c))
        {
            i++;
            continue;
        }

        switch (state)
        {
        case EXPECT_OPERAND:

            /* Number or identifier */
            if (isdigit(c) || c == '.' || isalpha(c))
            {
                if (isalpha(c))
                {
                    while (isalnum(expression[i]) || expression[i] == '_')
                    {
                        i++;
                    }
                }
                else
                {
                    int dotCount = 0;

                    while (isdigit(expression[i]) || expression[i] == '.')
                    {
                        if (expression[i] == '.')
                            dotCount++;

                        if (dotCount > 1)
                        {
                            printf("Error: Invalid number format.\n");
                            return 0;
                        }

                        i++;
                    }
                }

                state = AFTER_OPERAND;
                continue;
            }

            if (c == '-')
            {
                i++;
                continue;
            }

            if (c == '(')
            {
                balance++;
                i++;
                continue;
            }

            printf("Error: Operand expected.\n");
            return 0;

        case AFTER_OPERAND:

            if (c == ')')
            {
                balance--;

                if (balance < 0)
                {
                    printf("Error: Too many ')'\n");
                    return 0;
                }

                i++;
                continue;
            }

            if (c == '+' || c == '-' ||
                c == '*' || c == '/' ||
                c == '%' || c == '^'
            )
            {
                state = EXPECT_OPERAND;
                i++;
                continue;
            }

            printf("Error: Operator expected.\n");
            return 0;
        }
    }

    if (balance != 0)
    {
        printf("Error: Mismatched parentheses.\n");
        return 0;
    }

    if (state == EXPECT_OPERAND)
    {
        printf("Error: Expression cannot end with an operator.\n");
        return 0;
    }

    return 1;
}
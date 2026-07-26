#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "validator.h"
#include "functions.h"
#include "function_info.h"

typedef enum
{
    EXPECT_OPERAND,
    AFTER_OPERAND
} ParserState;

typedef enum
{
    EXPECT_OPERAND,
    AFTER_OPERAND
} ParserState;

/* NEW: one frame per nesting depth, tracks function-call arity */
#define MAX_PAREN_DEPTH 64

typedef struct
{
    int isFunctionCall; /* 1 if this '(' opened a function, 0 if plain grouping */
    int expectedArgs;   /* commas expected = argc - 1 */
    int commasSeen;
    char funcName[32]; /* for the error message */
} ParenFrame;

int validateExpression(char expression[])
{
    ParserState state = EXPECT_OPERAND;

    int i = 0;
    int balance = 0;

    ParenFrame frames[MAX_PAREN_DEPTH]; /* NEW */

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

            /* Number */
            if (isdigit(c) || c == '.')
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

                state = AFTER_OPERAND;
                continue;
            }

            /* Variable or Function */
            if (isalpha(c))
            {
                char identifier[32];
                int k = 0;

                while (isalnum(expression[i]) || expression[i] == '_')
                {
                    identifier[k++] = expression[i++];
                }

                identifier[k] = '\0';

                while (isspace(expression[i]))
                    i++;

                /* Function call */
                if (expression[i] == '(' && isFunction(identifier))
                {
                    balance++;

                    if (balance >= MAX_PAREN_DEPTH)
                    {
                        printf("Error: Too many nested parentheses.\n");
                        return 0;
                    }

                    frames[balance].isFunctionCall = 1;
                    frames[balance].expectedArgs = functionArgumentCount(identifier) - 1;
                    frames[balance].commasSeen = 0;
                    strcpy(frames[balance].funcName, identifier);

                    i++; /* consume '(' */
                    state = EXPECT_OPERAND;
                    continue;
                }
                /* Variable */
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

                if (balance >= MAX_PAREN_DEPTH)
                {
                    printf("Error: Too many nested parentheses.\n");
                    return 0;
                }

                frames[balance].isFunctionCall = 0; /* NEW: mark as non-function */

                i++;
                continue;
            }

            printf("Error: Operand expected.\n");
            return 0;

        case AFTER_OPERAND:

            if (c == ')')
            {
                if (balance == 0)
                {
                    printf("Error: Too many ')'\n");
                    return 0;
                }

                if (frames[balance].isFunctionCall)
                {
                    int actualArgs = frames[balance].commasSeen + 1;
                    int expectedArgs = frames[balance].expectedArgs + 1;

                    if (actualArgs != expectedArgs)
                    {
                        printf("Error: Function '%s' expects %d argument(s), got %d.\n",
                               frames[balance].funcName,
                               expectedArgs,
                               actualArgs);
                        return 0;
                    }
                }

                balance--;
                i++;
                continue;
            }

            /* Postfix factorial */
            if (c == '!')
            {
                i++;
                continue;
            }

            /* Function argument separator */
            if (c == ',')
            {
                if (balance == 0 || !frames[balance].isFunctionCall)
                {
                    printf("Error: ',' used outside of a function call.\n");
                    return 0;
                }

                frames[balance].commasSeen++;

                state = EXPECT_OPERAND;
                i++;
                continue;
            }

            /* Binary operators */
            if (c == '+' ||
                c == '-' ||
                c == '*' ||
                c == '/' ||
                c == '%' ||
                c == '^') 
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
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "calculator.h"
#include "stack.h"
#include "functions.h"
#include "function_info.h"

double applyOperation(double a, double b, char op)
{
    switch (op)
    {
    case '+':
        return a + b;

    case '-':
        return a - b;

    case '*':
        return a * b;

    case '/':
        if (b == 0.0)
        {
            printf("Error: Division by zero!\n");
            exit(EXIT_FAILURE);
        }
        return a / b;

    case '%':

        printf("Error: Modulus is supported only for integers.\n");
        exit(EXIT_FAILURE);

    case '^':
        return pow(a, b);

    default:
        printf("Error: Invalid operator '%c'\n", op);
        exit(EXIT_FAILURE);
    }
}

double evaluatePostfix(char postfix[])
{
    DoubleStack s;
    initDoubleStack(&s);

    int i = 0;

    while (postfix[i] != '\0')
    {
        /* Skip spaces */
        if (postfix[i] == ' ')
        {
            i++;
            continue;
        }

        /* Read number */
        if (isdigit(postfix[i]) ||
            postfix[i] == '.' ||
            (postfix[i] == '-' &&
             (isdigit(postfix[i + 1]) || postfix[i + 1] == '.')))
        {
            double number = readNumber(postfix, &i);
            pushDouble(&s, number);
            continue;
        }

        /* Read function name */
        if (isalpha(postfix[i]))
        {
            char function[32];
            int j = 0;

            while (isalpha(postfix[i]) || isdigit(postfix[i]))
            {
                if (j >= (int)sizeof(function) - 1)
                {
                    printf("Error: Function name too long.\n");
                    exit(EXIT_FAILURE);
                }

                function[j++] = postfix[i++];
            }

            function[j] = '\0';

            int argc = functionArgumentCount(function);

            if (argc == -1)
            {
                printf("Unknown function '%s'\n", function);
                exit(EXIT_FAILURE);
            }

            if (argc == 1)
            {
                double a = popDouble(&s);

                pushDouble(&s,
                           applyFunction(function, a));
            }
            else if (argc == 2)
            {
                double b = popDouble(&s);
                double a = popDouble(&s);

                pushDouble(&s,
                           applyBinaryFunction(function, a, b));
            }
            else
            {
                printf("Error: Unsupported function '%s'\n", function);
                exit(EXIT_FAILURE);
            }

            continue;
        }

        /* Factorial */
        if (postfix[i] == '!')
        {
            double value = popDouble(&s);

            pushDouble(&s, factorial(value));

            i++;
            continue;
        }

        /* Binary operator */
        double b = popDouble(&s);
        double a = popDouble(&s);

        double result = applyOperation(a, b, postfix[i]);

        pushDouble(&s, result);

        i++;
    }

    return popDouble(&s);
}
#include <stdio.h>
#include <ctype.h>
#include "calculator.h"
#include "stack.h"
#include <stdlib.h>

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
        if (postfix[i] == ' ')
        {
            i++;
            continue;
        }

        /* Read a decimal number */
        if (isdigit(postfix[i]) || postfix[i] == '.')
        {
            char *endPtr;

            double number = strtod(&postfix[i], &endPtr);

            pushDouble(&s, number);

            i = endPtr - postfix;
            continue;
        }

        double b = popDouble(&s);
        double a = popDouble(&s);

        double result = applyOperation(a, b, postfix[i]);

        pushDouble(&s, result);
        i++;
    }

    return popDouble(&s);
}

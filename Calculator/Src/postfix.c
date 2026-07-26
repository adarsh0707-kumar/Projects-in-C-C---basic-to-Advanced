#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "calculator.h"
#include "stack.h"
#include "variables.h"

double precedence(char op)
{
    switch (op)
    {
    case '+':
    case '-':
        return 1;

    case '*':
    case '/':
    case '%':
        return 2;
    
    case '^':
        return 3;

    default:
        return 0;
    }
}

int previousNonSpace(char expression[], int index)
{
    index--;

    while (index >= 0 && expression[index] == ' ')
    {
        index--;
    }

    return index;
}

void infixToPostfix(char infix[], char postfix[])
{
    CharStack s;
    initCharStack(&s);

    int i = 0;
    int j = 0;

    while (infix[i] != '\0')
    {
        /* Ignore spaces */
        if (infix[i] == ' ')
        {
            i++;
            continue;
        }

        /* Variable (ans, x, y, radius, etc.) */
        if (isalpha(infix[i]))
        {
            char variable[32];
            int k = 0;

            while (isalnum(infix[i]) || infix[i] == '_')
            {
                variable[k++] = infix[i++];
            }

            variable[k] = '\0';

            double value;

            if (!getVariable(variable, &value))
            {
                printf("Error: Undefined variable '%s'\n", variable);
                exit(EXIT_FAILURE);
            }

            j += sprintf(&postfix[j], "%g ", value);
            continue;
        }

        /* Number (including unary minus) */
        int prev = previousNonSpace(infix, i);

        if (isdigit(infix[i]) ||
            infix[i] == '.' ||
            (infix[i] == '-' &&
             (prev < 0 ||
              infix[prev] == '(' ||
              infix[prev] == '+' ||
              infix[prev] == '-' ||
              infix[prev] == '*' ||
              infix[prev] == '/' ||
              infix[prev] == '%')))
        {
            if (infix[i] == '-')
            {
                postfix[j++] = infix[i++];
            }

            int dotCount = 0;

            while (isdigit(infix[i]) || infix[i] == '.')
            {
                if (infix[i] == '.')
                    dotCount++;

                if (dotCount > 1)
                {
                    printf("Error: Invalid number format.\n");
                    exit(EXIT_FAILURE);
                }

                postfix[j++] = infix[i++];
            }

            postfix[j++] = ' ';
            continue;
        }

        /* Left Parenthesis */
        if (infix[i] == '(')
        {
            pushChar(&s, '(');
        }

        /* Right Parenthesis */
        else if (infix[i] == ')')
        {
            while (!isEmptyCharStack(&s) &&
                   peekChar(&s) != '(')
            {
                postfix[j++] = popChar(&s);
                postfix[j++] = ' ';
            }

            if (!isEmptyCharStack(&s))
                popChar(&s);
        }

        /* Operator */
        else
        {
            while (!isEmptyCharStack(&s) &&
                   peekChar(&s) != '(' &&
                   (
                        precedence(peekChar(&s)) > precedence(infix[i]) ||
                    (
                        precedence(peekChar(&s)) == precedence(infix[i]) &&
                        infix[i] != '^')
                )
                )
            {
                postfix[j++] = popChar(&s);
                postfix[j++] = ' ';
            }

            pushChar(&s, infix[i]);
        }

        i++;
    }

    while (!isEmptyCharStack(&s))
    {
        postfix[j++] = popChar(&s);
        postfix[j++] = ' ';
    }

    postfix[j] = '\0';
}
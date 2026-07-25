#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "calculator.h"
#include "stack.h"

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

    default:
        return 0;
    }
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

        /* Number (integer or decimal) */
        if (isdigit(infix[i]) || infix[i] == '.')
        {
            while (isdigit(infix[i]) || infix[i] == '.')
            {
                postfix[j++] = infix[i++];
            }

            postfix[j++] = ' ';
            continue;
        }

        /* Left Parenthesis */
        if (infix[i] == '(')
        {
            pushChar(&s, infix[i]);
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
            {
                popChar(&s);
            }
        }

        /* Operator */
        else
        {
            while (!isEmptyCharStack(&s) &&
                   peekChar(&s) != '(' &&
                   precedence(peekChar(&s)) >= precedence(infix[i]))
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
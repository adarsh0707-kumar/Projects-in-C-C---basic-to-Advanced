#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "calculator.h"
#include "stack.h"

int precedence(char op)
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

    int i = 0, j = 0;

    while (infix[i] != '\0')
    {
        if (infix[i] == ' ')
        {
            i++;
            continue;
        }

        if (isdigit(infix[i]))
        {
            while (isdigit(infix[i]))
            {
                postfix[j++] = infix[i++];
            }

            postfix[j++] = ' ';
            continue;
        }

        if (infix[i] == '(')
        {
            pushChar(&s, '(');
        }
        else if (infix[i] == ')')
        {
            while (!isEmptyCharStack(&s) && peekChar(&s) != '(')
            {
                postfix[j++] = popChar(&s);
                postfix[j++] = ' ';
            }

            if (!isEmptyCharStack(&s))
                popChar(&s);
        }
        else
        {
            while (!isEmptyCharStack(&s) && peekChar(&s) != '(' && precedence(peekChar(&s)) >= precedence(infix[i]))
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

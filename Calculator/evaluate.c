#include <stdio.h>
#include <ctype.h>
#include "calculator.h"
#include "stack.h"

int applyOperation(int a, int b, char op)
{
    switch (op)
    {
    case '+':
        return a + b;
        break;

    case '-':
        return a - b;
        break;

    case '*':
        return a * b;
        break;

    case '/':
        return a / b;
        break;

    case '%':
        return a % b;
        break;
    default:
        break;
    }
}

int evaluatePostfix(char postfix[])
{
    IntStack s;
    initIntStack(&s);

    int i = 0;

    while (postfix[i] != '\0')
    {
        if (postfix[i] == ' ')
        {
            i++;
            continue;
        }

        if (isdigit(postfix[i]))
        {
            int number = 0;
            while (isdigit(postfix[i]))
            {
                number = number * 10 + (postfix[i] - '0');
                i++;
            }

            pushInt(&s, number);
            continue;
        }

        int b = popInt(&s);
        int a = popInt(&s);

        int result = applyOperation(a, b, postfix[i]);

        pushInt(&s, result);
        i++;
    }

    return popInt(&s);
}

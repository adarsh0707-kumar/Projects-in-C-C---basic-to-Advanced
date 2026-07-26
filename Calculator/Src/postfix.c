#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "calculator.h"
#include "stack.h"
#include "variables.h"
#include "functions.h"
#include "constants.h"
#include "function_info.h"

int isOperator(char ch)
{
    return ch == '+' ||
           ch == '-' ||
           ch == '*' ||
           ch == '/' ||
           ch == '%' ||
           ch == '^' ||
           ch == '!';
}

Token makeOperatorToken(char op)
{
    Token t;

    t.type = TOKEN_OPERATOR;

    t.text[0] = op;
    t.text[1] = '\0';

    return t;
}

Token makeLeftParenToken(void)
{
    Token t;

    t.type = TOKEN_LEFT_PAREN;

    strcpy(t.text, "(");

    return t;
}

Token makeFunctionToken(char name[])
{
    Token t;

    t.type = TOKEN_FUNCTION;

    strcpy(t.text, name);

    return t;
}

Token makeCommaToken(void)
{
    Token t;

    t.type = TOKEN_COMMA;

    strcpy(t.text, ",");

    return t;
}

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

    case '!':
        return 4;

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
    TokenStack operators;
    initTokenStack(&operators);

    int i = 0;
    int j = 0;

    while (infix[i] != '\0')
    {
        /* Skip spaces */
        if (isspace(infix[i]))
        {
            i++;
            continue;
        }

        /* ---------------------------
           Variables / Functions
        ----------------------------*/
        if (isalpha(infix[i]))
        {
            char identifier[32];
            int k = 0;

            /* Read complete identifier */

            while (isalnum(infix[i]) || infix[i] == '_')
            {
                identifier[k++] = infix[i++];
            }

            identifier[k] = '\0';

            /* Built-in constants */

            if (strcmp(identifier, "pi") == 0)
            {
                j += sprintf(&postfix[j], "%g ", getConstant("pi"));
                continue;
            }

            if (strcmp(identifier, "e") == 0)
            {
                j += sprintf(&postfix[j], "%g ", getConstant("e"));
                continue;
            }

            /* Skip spaces after identifier */

            int temp = i;

            while (isspace(infix[temp]))
                temp++;

            /* Function */

            if (infix[temp] == '(' && isFunction(identifier))
            {
                pushToken(&operators, makeFunctionToken(identifier));
                i = temp;
                continue;
            }

            /* Variable */

            double value;

            if (!getVariable(identifier, &value))
            {
                printf("Error: Undefined variable '%s'\n", identifier);
                exit(EXIT_FAILURE);
            }

            j += sprintf(&postfix[j], "%g ", value);

            continue;
        }

        /* ---------------------------
           Number
        ----------------------------*/

        int prev = previousNonSpace(infix, i);

        if (isdigit(infix[i]) ||
            infix[i] == '.' ||
            (infix[i] == '-' &&
             (prev < 0 ||
              infix[prev] == '(' ||
              isOperator(infix[prev]))))
        {
            if (infix[i] == '-')
                postfix[j++] = infix[i++];

            int dotCount = 0;

            while (isdigit(infix[i]) || infix[i] == '.')
            {
                if (infix[i] == '.')
                    dotCount++;

                if (dotCount > 1)
                {
                    printf("Invalid number.\n");
                    exit(EXIT_FAILURE);
                }

                postfix[j++] = infix[i++];
            }

            postfix[j++] = ' ';
            continue;
        }

        /* ---------------------------
           Left Parenthesis
        ----------------------------*/

        if (infix[i] == '(')
        {
            pushToken(&operators, makeLeftParenToken());
            i++;
            continue;
        }

        /* ---------------------------
           Right Parenthesis
        ----------------------------*/

        if (infix[i] == ')')
        {
            while (!isEmptyTokenStack(&operators))
            {
                Token top = peekToken(&operators);

                if (top.type == TOKEN_LEFT_PAREN)
                    break;

                top = popToken(&operators);

                j += sprintf(&postfix[j], "%s ", top.text);
            }

            if (isEmptyTokenStack(&operators))
            {
                printf("Error: Mismatched parentheses.\n");
                exit(EXIT_FAILURE);
            }

            /* Remove '(' */
            popToken(&operators);

            /* If a function is on top, output it */
            if (!isEmptyTokenStack(&operators))
            {
                Token top = peekToken(&operators);

                if (top.type == TOKEN_FUNCTION)
                {
                    top = popToken(&operators);
                    j += sprintf(&postfix[j], "%s ", top.text);
                }
            }

            i++;
            continue;
        }

        /* Postfix factorial */
        if (infix[i] == '!')
        {
            while (!isEmptyTokenStack(&operators))
            {
                Token top = peekToken(&operators);

                if (top.type != TOKEN_OPERATOR)
                    break;

                if (precedence(top.text[0]) >= precedence('!'))
                {
                    top = popToken(&operators);
                    j += sprintf(&postfix[j], "%s ", top.text);
                }
                else
                {
                    break;
                }
            }

            pushToken(&operators, makeOperatorToken('!'));

            i++;
            continue;
        }

        /* ---------------------------
            Function argument separator
            ----------------------------*/
        if (infix[i] == ',')
        {
            while (!isEmptyTokenStack(&operators))
            {
                Token top = peekToken(&operators);

                if (top.type == TOKEN_LEFT_PAREN)
                    break;

                top = popToken(&operators);

                j += sprintf(&postfix[j],
                             "%s ",
                             top.text);
            }

            if (isEmptyTokenStack(&operators))
            {
                printf("Error: Misplaced comma.\n");
                exit(EXIT_FAILURE);
            }

            i++;
            continue;
        }

        /* ---------------------------
           Operator
        ----------------------------*/

        Token current = makeOperatorToken(infix[i]);

        while (!isEmptyTokenStack(&operators))
        {
            Token top = peekToken(&operators);

            if (top.type != TOKEN_OPERATOR)
                break;

            char topOp = top.text[0];

            if (precedence(topOp) > precedence(current.text[0]) ||
                (precedence(topOp) == precedence(current.text[0]) &&
                 current.text[0] != '^'))
            {
                top = popToken(&operators);
                j += sprintf(&postfix[j], "%s ", top.text);
            }
            else
            {
                break;
            }
        }

        pushToken(&operators, current);
        i++;
    }

    /* Empty stack */

    while (!isEmptyTokenStack(&operators))
    {
        Token top = popToken(&operators);

        if (top.type == TOKEN_LEFT_PAREN)
            continue;

        j += sprintf(&postfix[j], "%s ", top.text);
    }

    postfix[j] = '\0';
}
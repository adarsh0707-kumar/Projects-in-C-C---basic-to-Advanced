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
#include "../Inc/error.h"

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
    case '>':
    case '<':
        return 0;

    case '=':
        return 0;
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

<<<<<<< Updated upstream
void infixToPostfix(char infix[], char postfix[])
=======
/* Both of these "should never happen" given how infixToPostfix() drives
   the stack (it never pops/peeks without having just confirmed the
   stack isn't empty, and never pushes more tokens than there are
   characters in the input) -- but stack.c can still fail if that
   invariant is ever violated by a future change, so every call site
   below checks anyway rather than assuming success. */
static int reportInternalStackError(void)
{
    printf("Error: Internal stack error while parsing expression.\n");
    calculatorSetLastError(CALC_ERR_INTERNAL);
    return 0;
}

static int reportTooComplex(void)
{
    printf("Error: Expression is too complex to parse (too many pending operators/parentheses).\n");
    calculatorSetLastError(CALC_ERR_STACK_OVERFLOW);
    return 0;
}

/**
 * Converts an infix expression into postfix (Reverse Polish) form.
 * Returns 1 on success, 0 on failure -- see calculator.h.
 */
int infixToPostfix(char infix[], char postfix[])
>>>>>>> Stashed changes
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
                if (k >= (int)sizeof(identifier) - 1)
                {
                    printf("Error: Identifier name too long.\n");
                    calculatorSetLastError(CALC_ERR_INVALID_TOKEN);
                    return 0;
                }

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
<<<<<<< Updated upstream
                pushToken(&operators, makeFunctionToken(identifier));
=======
                if (!pushToken(&operators, makeFunctionToken(identifier)))
                    return reportTooComplex();

>>>>>>> Stashed changes
                i = temp;
                continue;
            }

            /* Variable */

            double value;

            if (!getVariable(identifier, &value))
            {
                printf("Error: Undefined variable '%s'\n", identifier);
                calculatorSetLastError(CALC_ERR_INVALID_VARIABLE);
                return 0;
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
                    calculatorSetLastError(CALC_ERR_INVALID_TOKEN);
                    return 0;
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
<<<<<<< Updated upstream
            pushToken(&operators, makeLeftParenToken());
=======
            if (!pushToken(&operators, makeLeftParenToken()))
                return reportTooComplex();

>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
                Token top = peekToken(&operators);
=======
                Token top;

                if (!peekToken(&operators, &top))
                    return reportInternalStackError();
>>>>>>> Stashed changes

                if (top.type == TOKEN_LEFT_PAREN)
                    break;

<<<<<<< Updated upstream
                top = popToken(&operators);
=======
                if (!popToken(&operators, &top))
                    return reportInternalStackError();
>>>>>>> Stashed changes

                j += sprintf(&postfix[j], "%s ", top.text);
            }

            if (isEmptyTokenStack(&operators))
            {
                printf("Error: Mismatched parentheses.\n");
                calculatorSetLastError(CALC_ERR_INVALID_EXPRESSION);
                return 0;
            }

            /* Remove '(' */
<<<<<<< Updated upstream
            popToken(&operators);
=======
            Token discarded;
            if (!popToken(&operators, &discarded))
                return reportInternalStackError();
>>>>>>> Stashed changes

            /* If a function is on top, output it */
            if (!isEmptyTokenStack(&operators))
            {
<<<<<<< Updated upstream
                Token top = peekToken(&operators);

                if (top.type == TOKEN_FUNCTION)
                {
                    top = popToken(&operators);
=======
                Token top;

                if (!peekToken(&operators, &top))
                    return reportInternalStackError();

                if (top.type == TOKEN_FUNCTION)
                {
                    if (!popToken(&operators, &top))
                        return reportInternalStackError();

>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
                Token top = peekToken(&operators);
=======
                Token top;

                if (!peekToken(&operators, &top))
                    return reportInternalStackError();
>>>>>>> Stashed changes

                if (top.type != TOKEN_OPERATOR)
                    break;

                if (precedence(top.text[0]) >= precedence('!'))
                {
<<<<<<< Updated upstream
                    top = popToken(&operators);
=======
                    if (!popToken(&operators, &top))
                        return reportInternalStackError();

>>>>>>> Stashed changes
                    j += sprintf(&postfix[j], "%s ", top.text);
                }
                else
                {
                    break;
                }
            }

<<<<<<< Updated upstream
            pushToken(&operators, makeOperatorToken('!'));
=======
            if (!pushToken(&operators, makeOperatorToken('!')))
                return reportTooComplex();
>>>>>>> Stashed changes

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
<<<<<<< Updated upstream
                Token top = peekToken(&operators);
=======
                Token top;

                if (!peekToken(&operators, &top))
                    return reportInternalStackError();
>>>>>>> Stashed changes

                if (top.type == TOKEN_LEFT_PAREN)
                    break;

<<<<<<< Updated upstream
                top = popToken(&operators);
=======
                if (!popToken(&operators, &top))
                    return reportInternalStackError();
>>>>>>> Stashed changes

                j += sprintf(&postfix[j],
                             "%s ",
                             top.text);
            }

            if (isEmptyTokenStack(&operators))
            {
                printf("Error: Misplaced comma.\n");
                calculatorSetLastError(CALC_ERR_INVALID_EXPRESSION);
                return 0;
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
<<<<<<< Updated upstream
            Token top = peekToken(&operators);
=======
            Token top;

            if (!peekToken(&operators, &top))
                return reportInternalStackError();
>>>>>>> Stashed changes

            if (top.type != TOKEN_OPERATOR)
                break;

            char topOp = top.text[0];

            if (precedence(topOp) > precedence(current.text[0]) ||
                (precedence(topOp) == precedence(current.text[0]) &&
                 current.text[0] != '^'))
            {
<<<<<<< Updated upstream
                top = popToken(&operators);
=======
                if (!popToken(&operators, &top))
                    return reportInternalStackError();

>>>>>>> Stashed changes
                j += sprintf(&postfix[j], "%s ", top.text);
            }
            else
            {
                break;
            }
        }

<<<<<<< Updated upstream
        pushToken(&operators, current);
=======
        if (!pushToken(&operators, current))
            return reportTooComplex();

>>>>>>> Stashed changes
        i++;
    }

    /* Empty stack */

    while (!isEmptyTokenStack(&operators))
    {
<<<<<<< Updated upstream
        Token top = popToken(&operators);
=======
        Token top;

        if (!popToken(&operators, &top))
            return reportInternalStackError();
>>>>>>> Stashed changes

        if (top.type == TOKEN_LEFT_PAREN)
            continue;

        j += sprintf(&postfix[j], "%s ", top.text);
    }

    postfix[j] = '\0';
    return 1;
}

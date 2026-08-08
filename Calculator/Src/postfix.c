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
#include "error.h"

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
/*
Every token written into postfix[] goes through these.

Twelve writes used to go in unbounded, and a long enough expression ran
past the caller's buffer: sixty chained "ans" terms was sufficient, which
AddressSanitizer reported as a stack-buffer-overflow on main.c's
postfix[1024]. The expression before that produced a silently wrong answer
and no error at all.

Both macros return 0 from the enclosing function on overflow, so the
caller is told the expression was too long instead of being handed
whatever survived.
*/
#define POSTFIX_EMIT(...)                                                  \
    do                                                                     \
    {                                                                      \
        const int remaining_ = CALC_POSTFIX_SIZE - j;                      \
        if (remaining_ <= 1)                                               \
        {                                                                  \
            printf("Error: Expression is too long to convert.\n");         \
            calculatorSetLastError(CALC_ERR_INTERNAL);                     \
            return 0;                                                      \
        }                                                                  \
        const int written_ =                                               \
            snprintf(&postfix[j], (size_t)remaining_, __VA_ARGS__);        \
        if (written_ < 0 || written_ >= remaining_)                        \
        {                                                                  \
            printf("Error: Expression is too long to convert.\n");         \
            calculatorSetLastError(CALC_ERR_INTERNAL);                     \
            return 0;                                                      \
        }                                                                  \
        j += written_;                                                     \
    } while (0)

#define POSTFIX_EMIT_CHAR(character)                                       \
    do                                                                     \
    {                                                                      \
        if (j + 1 >= CALC_POSTFIX_SIZE)                                    \
        {                                                                  \
            printf("Error: Expression is too long to convert.\n");         \
            calculatorSetLastError(CALC_ERR_INTERNAL);                     \
            return 0;                                                      \
        }                                                                  \
        postfix[j++] = (character);                                        \
    } while (0)

int infixToPostfix(char infix[], char postfix[])
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

            /*
            %.17g, not %g. The postfix form is text, so every constant and
            variable is printed here and parsed again by evaluatePostfix().
            %g defaults to six significant figures, which quantised the
            value on the way through: pi became 3.14159, so sin(pi) came
            back as 2.65e-06 instead of 1.22e-16, and x=1/3 followed by
            x*3 gave 0.999999.

            Seventeen significant figures is the shortest precision that
            round-trips an IEEE-754 double exactly, so the text carries the
            same value the double held.
            */
            if (strcmp(identifier, "pi") == 0)
            {
                POSTFIX_EMIT("%.17g ", getConstant("pi"));
                continue;
            }

            if (strcmp(identifier, "e") == 0)
            {
                POSTFIX_EMIT("%.17g ", getConstant("e"));
                continue;
            }

            /* Skip spaces after identifier */

            int temp = i;

            while (isspace(infix[temp]))
                temp++;

            /* Function */

            if (infix[temp] == '(' && isFunction(identifier))
            {
                if (!pushToken(&operators, makeFunctionToken(identifier)))
                    return reportTooComplex();

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

            /* Round-trips exactly; see the note on the constants above. */
            POSTFIX_EMIT("%.17g ", value);

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
                POSTFIX_EMIT_CHAR(infix[i++]);

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

                POSTFIX_EMIT_CHAR(infix[i++]);
            }

            POSTFIX_EMIT_CHAR(' ');
            continue;
        }

        /* ---------------------------
           Left Parenthesis
        ----------------------------*/

        if (infix[i] == '(')
        {
            if (!pushToken(&operators, makeLeftParenToken()))
                return reportTooComplex();

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
                Token top;

                if (!peekToken(&operators, &top))
                    return reportInternalStackError();

                if (top.type == TOKEN_LEFT_PAREN)
                    break;

                if (!popToken(&operators, &top))
                    return reportInternalStackError();

                POSTFIX_EMIT("%s ", top.text);
            }

            if (isEmptyTokenStack(&operators))
            {
                printf("Error: Mismatched parentheses.\n");
                calculatorSetLastError(CALC_ERR_INVALID_EXPRESSION);
                return 0;
            }

            /* Remove '(' */
            Token discarded;
            if (!popToken(&operators, &discarded))
                return reportInternalStackError();

            /* If a function is on top, output it */
            if (!isEmptyTokenStack(&operators))
            {
                Token top;

                if (!peekToken(&operators, &top))
                    return reportInternalStackError();

                if (top.type == TOKEN_FUNCTION)
                {
                    if (!popToken(&operators, &top))
                        return reportInternalStackError();

                    POSTFIX_EMIT("%s ", top.text);
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
                Token top;

                if (!peekToken(&operators, &top))
                    return reportInternalStackError();

                if (top.type != TOKEN_OPERATOR)
                    break;

                if (precedence(top.text[0]) >= precedence('!'))
                {
                    if (!popToken(&operators, &top))
                        return reportInternalStackError();

                    POSTFIX_EMIT("%s ", top.text);
                }
                else
                {
                    break;
                }
            }

            if (!pushToken(&operators, makeOperatorToken('!')))
                return reportTooComplex();

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
                Token top;

                if (!peekToken(&operators, &top))
                    return reportInternalStackError();

                if (top.type == TOKEN_LEFT_PAREN)
                    break;

                if (!popToken(&operators, &top))
                    return reportInternalStackError();

                POSTFIX_EMIT("%s ",
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
            Token top;

            if (!peekToken(&operators, &top))
                return reportInternalStackError();

            if (top.type != TOKEN_OPERATOR)
                break;

            char topOp = top.text[0];

            if (precedence(topOp) > precedence(current.text[0]) ||
                (precedence(topOp) == precedence(current.text[0]) &&
                 current.text[0] != '^'))
            {
                if (!popToken(&operators, &top))
                    return reportInternalStackError();

                POSTFIX_EMIT("%s ", top.text);
            }
            else
            {
                break;
            }
        }

        if (!pushToken(&operators, current))
            return reportTooComplex();

        i++;
    }

    /* Empty stack */

    while (!isEmptyTokenStack(&operators))
    {
        Token top;

        if (!popToken(&operators, &top))
            return reportInternalStackError();

        /* A '(' still on the stack here was never closed. This used to
           `continue`, silently dropping it and returning success: "(2+3"
           converted to "2 3 +" and "(()" to an empty string, despite
           Inc/calculator.h documenting a 0 return for mismatched
           parentheses. Reported the same way as the surplus-')' case
           above, so both directions behave alike. */
        if (top.type == TOKEN_LEFT_PAREN)
        {
            printf("Error: Mismatched parentheses.\n");
            calculatorSetLastError(CALC_ERR_INVALID_EXPRESSION);
            return 0;
        }

        POSTFIX_EMIT("%s ", top.text);
    }

    postfix[j] = '\0';
    return 1;
}

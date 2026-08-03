#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "calculator.h"
#include "stack.h"
#include "functions.h"
#include "function_info.h"
<<<<<<< Updated upstream
=======
#include "../Inc/error.h"

/* Last non-fatal evaluation error, set by applyOperation()/evaluatePostfix()
   whenever they return NAN instead of a real result. Callers that loop over
   many evaluations (e.g. plot.c sampling many x values) can check
   isfinite(result) and, if it fails, report getLastEvalError() instead of
   the whole process dying on a single bad sample -- see RULES.md's own
   "never call exit() from library modules" guidance. */
static char lastEvalError[128] = "";

static void setEvalError(const char msg[])
{
    strncpy(lastEvalError, msg, sizeof(lastEvalError) - 1);
    lastEvalError[sizeof(lastEvalError) - 1] = '\0';
}

const char *getLastEvalError(void)
{
    return lastEvalError;
}

/* Wraps stack.c's status-code pop/push so evaluatePostfix() can stay
   written in the natural "value in, value out" style while still
   degrading gracefully: a malformed postfix expression (e.g. one
   with more operators than operands) now produces NAN + a recorded
   error instead of exit()ing. NAN propagates through every
   subsequent arithmetic op, so a single bad pop/push naturally
   surfaces as a NAN final result without extra bookkeeping. */
static double safePopDouble(DoubleStack *s)
{
    double value;

    if (!popDouble(s, &value))
    {
        setEvalError(calculatorErrorString(calculatorGetLastError()));
        return NAN;
    }

    return value;
}

static void safePushDouble(DoubleStack *s, double value)
{
    if (!pushDouble(s, value))
    {
        setEvalError(calculatorErrorString(calculatorGetLastError()));
    }
}
>>>>>>> Stashed changes

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
                    /* Unreachable through the normal CLI pipeline --
                       infixToPostfix() already rejects an identifier
                       this long before it's ever written into the
                       postfix string. Kept as a safety net. */
                    calculatorSetLastError(CALC_ERR_INTERNAL);
                    setEvalError(calculatorErrorString(CALC_ERR_INTERNAL));
                    return NAN;
                }

                function[j++] = postfix[i++];
            }

            function[j] = '\0';

            int argc = functionArgumentCount(function);

            if (argc == -1)
            {
                /* Unreachable through the normal CLI pipeline --
                   infixToPostfix() only ever emits functions the
                   registry (function_info.c) already confirmed exist.
                   Kept as a safety net. */
                calculatorSetLastError(CALC_ERR_INVALID_FUNCTION);
                setEvalError(calculatorErrorString(CALC_ERR_INVALID_FUNCTION));
                return NAN;
            }

            if (argc == 1)
            {
                double a = popDouble(&s);

<<<<<<< Updated upstream
                pushDouble(&s,
                           applyFunction(function, a));
=======
                /* factorial() (reached via "fact") now fails
                   gracefully on its own -- see functions.c -- so
                   there's no need to pre-validate its argument here
                   the way earlier revisions of this function did. */
                double result = applyFunction(function, a);

                if (!isfinite(result) && strcmp(function, "fact") == 0)
                    setEvalError("Factorial only works for non-negative integers.");

                safePushDouble(&s, result);
>>>>>>> Stashed changes
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
                /* Unreachable through the normal CLI pipeline -- every
                   registered function takes 1 or 2 arguments. Kept as
                   a safety net. */
                calculatorSetLastError(CALC_ERR_INTERNAL);
                setEvalError(calculatorErrorString(CALC_ERR_INTERNAL));
                return NAN;
            }

            continue;
        }

        /* Factorial */
        if (postfix[i] == '!')
        {
<<<<<<< Updated upstream
            double value = popDouble(&s);

            pushDouble(&s, factorial(value));
=======
            double value = safePopDouble(&s);
            double result = factorial(value);

            if (!isfinite(result))
                setEvalError("Factorial only works for non-negative integers.");

            safePushDouble(&s, result);
>>>>>>> Stashed changes

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
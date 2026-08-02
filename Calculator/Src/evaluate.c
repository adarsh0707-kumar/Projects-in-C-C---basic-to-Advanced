#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "calculator.h"
#include "stack.h"
#include "functions.h"
#include "function_info.h"
#include "error.h"

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
            setEvalError("Division by zero.");
            return NAN;
        }
        return a / b;

    case '%':
        if (b == 0.0 || floor(a) != a || floor(b) != b)
        {
            setEvalError("Modulus is supported only for non-zero integers.");
            return NAN;
        }
        return fmod(a, b);

    case '^':
        return pow(a, b);

    default:
        setEvalError("Invalid operator.");
        return NAN;
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
            safePushDouble(&s, number);
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
                double a = safePopDouble(&s);

                /* factorial() itself intentionally exit()s on invalid
                   input (see functions.c / test_functions.c) -- that's
                   correct for a single top-level calculation, but would
                   be fatal for callers like plot.c that evaluate many
                   x values in a loop. Pre-validate here so a bad sample
                   degrades to NaN instead of killing the process. */
                if (strcmp(function, "fact") == 0 && (a < 0 || floor(a) != a))
                {
                    setEvalError("Factorial only works for non-negative integers.");
                    safePushDouble(&s, NAN);
                }
                else
                {
                    safePushDouble(&s,
                                   applyFunction(function, a));
                }
            }
            else if (argc == 2)
            {
                double b = safePopDouble(&s);
                double a = safePopDouble(&s);

                safePushDouble(&s,
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
            double value = safePopDouble(&s);

            /* Same reasoning as the 'fact' function above: validate
               before calling the (intentionally exit()-on-error)
               factorial() so one bad sample can't kill the process. */
            if (value < 0 || floor(value) != value)
            {
                setEvalError("Factorial only works for non-negative integers.");
                safePushDouble(&s, NAN);
            }
            else
            {
                safePushDouble(&s, factorial(value));
            }

            i++;
            continue;
        }

        /* Binary operator */
        double b = safePopDouble(&s);
        double a = safePopDouble(&s);

        double result = applyOperation(a, b, postfix[i]);

        safePushDouble(&s, result);

        i++;
    }

    return safePopDouble(&s);
}
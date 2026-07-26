#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "functions.h"

double applyFunction(const char name[], double value)
{
    if (strcmp(name, "sqrt") == 0)
        return sqrt(value);

    if (strcmp(name, "sin") == 0)
        return sin(value);

    if (strcmp(name, "cos") == 0)
        return cos(value);

    if (strcmp(name, "tan") == 0)
        return tan(value);

    if (strcmp(name, "log") == 0)
        return log10(value);

    if (strcmp(name, "ln") == 0)
        return log(value);

    if (strcmp(name, "exp") == 0)
        return exp(value);

    if (strcmp(name, "abs") == 0)
        return fabs(value);

    if (strcmp(name, "cbrt") == 0)
        return cbrt(value);

    if (strcmp(name, "asin") == 0)
        return asin(value);

    if (strcmp(name, "acos") == 0)
        return acos(value);

    printf("Unknown function: %s\n", name);

    return 0;
}

double applyBinaryFunction(const char name[],
                           double a,
                           double b)
{
    if (strcmp(name, "pow") == 0)
        return pow(a, b);

    if (strcmp(name, "max") == 0)
        return a > b ? a : b;

    if (strcmp(name, "min") == 0)
        return a < b ? a : b;

    if (strcmp(name, "hypot") == 0)
        return hypot(a, b);

    if (strcmp(name, "atan2") == 0)
        return atan2(a, b);

    printf("Unknown function %s\n", name);
    exit(EXIT_FAILURE);
}

double factorial(double n)
{
    if (n < 0 || floor(n) != n)
    {
        printf("Factorial only works for non-negative integers.\n");
        exit(EXIT_FAILURE);
    }

    double result = 1;

    for (int i = 2; i <= (int)n; i++)
        result *= i;

    return result;
}
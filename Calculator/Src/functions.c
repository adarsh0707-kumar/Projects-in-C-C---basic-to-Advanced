#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "functions.h"
#include "angle_mode.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Angle-mode conversion helpers. Only used by the circular trig
   functions (sin/cos/tan/asin/acos/atan2) — hyperbolic functions
   (sinh/cosh/tanh/...) operate on plain reals and are never
   affected by degree/radian mode. */
static double toRadians(double degrees)
{
    return degrees * M_PI / 180.0;
}

static double toDegrees(double radians)
{
    return radians * 180.0 / M_PI;
}

double applyFunction(const char name[], double value)
{
    if (strcmp(name, "sqrt") == 0)
        return sqrt(value);

    if (strcmp(name, "sin") == 0)
        return sin(getAngleMode() == MODE_DEGREE ? toRadians(value) : value);

    if (strcmp(name, "cos") == 0)
        return cos(getAngleMode() == MODE_DEGREE ? toRadians(value) : value);

    if (strcmp(name, "tan") == 0)
        return tan(getAngleMode() == MODE_DEGREE ? toRadians(value) : value);

    if (strcmp(name, "log") == 0)
        return log10(value);

    if (strcmp(name, "ln") == 0)
        return log(value);

    if (strcmp(name, "exp") == 0)
        return exp(value);

    /* 'abs' and 'fabs' are intentional aliases for the same operation:
       'abs' is the name a casual user types; 'fabs' matches the
       underlying C library function name. Both resolve to fabs()
       here, so there's no duplicated logic — just two entry points. */
    if (strcmp(name, "abs") == 0)
        return fabs(value);

    if (strcmp(name, "cbrt") == 0)
        return cbrt(value);

    if (strcmp(name, "asin") == 0)
    {
        double result = asin(value);
        return getAngleMode() == MODE_DEGREE ? toDegrees(result) : result;
    }

    if (strcmp(name, "acos") == 0)
    {
        double result = acos(value);
        return getAngleMode() == MODE_DEGREE ? toDegrees(result) : result;
    }

    /* Rounding (Phase 11) */
    if (strcmp(name, "floor") == 0)
        return floor(value);

    if (strcmp(name, "ceil") == 0)
        return ceil(value);

    if (strcmp(name, "round") == 0)
        return round(value);

    if (strcmp(name, "trunc") == 0)
        return trunc(value);

    if (strcmp(name, "fabs") == 0)
        return fabs(value);

    /* Hyperbolic (Phase 11) */
    if (strcmp(name, "sinh") == 0)
        return sinh(value);

    if (strcmp(name, "cosh") == 0)
        return cosh(value);

    if (strcmp(name, "tanh") == 0)
        return tanh(value);

    if (strcmp(name, "asinh") == 0)
        return asinh(value);

    if (strcmp(name, "acosh") == 0)
        return acosh(value);

    if (strcmp(name, "atanh") == 0)
        return atanh(value);

    /* Misc (Phase 11) */
    if (strcmp(name, "fact") == 0)
        return factorial(value);

    if (strcmp(name, "gamma") == 0)
        return tgamma(value);

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
    {
        double result = atan2(a, b);
        return getAngleMode() == MODE_DEGREE ? toDegrees(result) : result;
    }

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
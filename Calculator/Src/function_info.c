#include <string.h>
#include <stddef.h>
#include "function_info.h"

/*
 * Single source of truth for every known function name and its
 * required argument count. functionArgumentCount() and isFunction()
 * both derive from this one table, so adding a function only ever
 * means adding one line here.
 */
static const FunctionInfo functions[] =
    {
        /* Unary — basic */
        {"sqrt", 1},
        {"sin", 1},
        {"cos", 1},
        {"tan", 1},
        {"asin", 1},
        {"acos", 1},
        {"log", 1},
        {"ln", 1},
        {"exp", 1},
        {"abs", 1},
        {"cbrt", 1},

        /* Unary — rounding (Phase 11) */
        {"floor", 1},
        {"ceil", 1},
        {"round", 1},
        {"trunc", 1},
        {"fabs", 1},

        /* Unary — hyperbolic (Phase 11) */
        {"sinh", 1},
        {"cosh", 1},
        {"tanh", 1},
        {"asinh", 1},
        {"acosh", 1},
        {"atanh", 1},

        /* Unary — misc (Phase 11) */
        {"fact", 1},
        {"gamma", 1},

        /* Binary */
        {"pow", 2},
        {"max", 2},
        {"min", 2},
        {"hypot", 2},
        {"atan2", 2}};

#define FUNCTION_COUNT (sizeof(functions) / sizeof(functions[0]))

int functionArgumentCount(const char name[])
{
    for (size_t i = 0; i < FUNCTION_COUNT; i++)
    {
        if (strcmp(functions[i].name, name) == 0)
            return functions[i].argc;
    }

    return -1; /* sentinel: unknown function (see evaluate.c) */
}

int isFunction(const char name[])
{
    for (size_t i = 0; i < FUNCTION_COUNT; i++)
    {
        if (strcmp(functions[i].name, name) == 0)
            return 1;
    }

    return 0;
}
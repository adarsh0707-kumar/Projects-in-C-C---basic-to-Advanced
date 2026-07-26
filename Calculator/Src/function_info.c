#include <string.h>
#include "function_info.h"
/*
static FunctionInfo functions[] =
    {
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

        {"pow", 2},
        {"max", 2},
        {"min", 2},
        {"hypot", 2},
        {"atan2", 2}
    };
"""
// #define FUNCTION_COUNT \
//     (sizeof(functions) / sizeof(functions[0]))
*/

int functionArgumentCount(const char name[])
{
    if (!strcmp(name, "sqrt"))
        return 1;
    if (!strcmp(name, "sin"))
        return 1;
    if (!strcmp(name, "cos"))
        return 1;
    if (!strcmp(name, "tan"))
        return 1;
    if (!strcmp(name, "log"))
        return 1;
    if (!strcmp(name, "ln"))
        return 1;
    if (!strcmp(name, "exp"))
        return 1;
    if (!strcmp(name, "abs"))
        return 1;
    if (!strcmp(name, "asin"))
        return 1;
    if (!strcmp(name, "acos"))
        return 1;
    if (!strcmp(name, "cbrt"))
        return 1;

    if (!strcmp(name, "pow"))
        return 2;
    if (!strcmp(name, "max"))
        return 2;
    if (!strcmp(name, "min"))
        return 2;
    if (!strcmp(name, "hypot"))
        return 2;

    return 0;
}

int isFunction(const char name[])
{
    return strcmp(name, "sqrt") == 0 ||
           strcmp(name, "sin") == 0 ||
           strcmp(name, "cos") == 0 ||
           strcmp(name, "tan") == 0 ||
           strcmp(name, "log") == 0 ||
           strcmp(name, "ln") == 0 ||
           strcmp(name, "exp") == 0 ||
           strcmp(name, "abs") == 0 ||
           strcmp(name, "asin") == 0 ||
           strcmp(name, "acos") == 0 ||
           strcmp(name, "cbrt") == 0 ||

           /* Binary functions */

           strcmp(name, "pow") == 0 ||
           strcmp(name, "max") == 0 ||
           strcmp(name, "min") == 0 ||
           strcmp(name, "hypot") == 0;
}
#include <string.h>

int functionArgumentCount(const char name[])
{
    if (strcmp(name, "pow") == 0)
        return 2;

    if (strcmp(name, "max") == 0)
        return 2;

    if (strcmp(name, "min") == 0)
        return 2;

    if (strcmp(name, "hypot") == 0)
        return 2;

    return 1;
}
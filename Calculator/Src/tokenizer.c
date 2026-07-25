#include <stdlib.h>

double readNumber(char expression[], int *index)
{
    char *endPtr;

    double number = strtod(&expression[*index], &endPtr);

    *index = endPtr - expression;

    return number;
}
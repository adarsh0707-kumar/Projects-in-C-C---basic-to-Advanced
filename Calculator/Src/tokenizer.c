#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

double readNumber(char expression[], int *index)
{
    int dotCount = 0;
    int start = *index;

    while (isdigit(expression[*index]) || expression[*index] == '.')
    {
        if (expression[*index] == '.')
            dotCount++;

        if (dotCount > 1)
        {
            printf("Error: Invalid number format.\n");
            exit(EXIT_FAILURE);
        }

        (*index)++;
    }

    char temp[64];
    int len = *index - start;

    for (int i = 0; i < len; i++)
        temp[i] = expression[start + i];

    temp[len] = '\0';

    return atof(temp);
}
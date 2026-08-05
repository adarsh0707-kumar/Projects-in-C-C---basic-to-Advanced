#include <stdio.h>
#include <string.h>
#include <math.h>
#include "variables.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

static Variable variables[MAX_VARIABLES] =
    {
        {"ans", 0},
        {"pi", M_PI},
        {"e", M_E}
    };

static int variableCount = 3;

/*-------------------------
    Built-in variable ans
-------------------------*/

void setAns(double value)
{
    setVariable("ans", value);
}

double getAns(void)
{
    double value = 0;

    getVariable("ans", &value);

    return value;
}

/*-------------------------
    User Variables
-------------------------*/

int setVariable(const char name[], double value)
{
    if (strcmp(name, "pi") == 0 ||
        strcmp(name, "e") == 0)
    {
        printf("Error: '%s' is a read-only constant.\n", name);
        return 0;
    }
    
    /* Update existing variable */
    for (int i = 0; i < variableCount; i++)
    {
        if (strcmp(variables[i].name, name) == 0)
        {
            variables[i].value = value;
            return 1;
        }
    }

    /* Variable table full */
    if (variableCount >= MAX_VARIABLES)
    {
        return 0;
    }

    /* Create new variable */
    strcpy(variables[variableCount].name, name);
    variables[variableCount].value = value;

    variableCount++;

    return 1;
}

int getVariable(const char name[], double *value)
{
    for (int i = 0; i < variableCount; i++)
    {
        if (strcmp(variables[i].name, name) == 0)
        {
            *value = variables[i].value;
            return 1;
        }
    }

    return 0;
}

void showVariables(void)
{
    printf("\n===== VARIABLES =====\n");

    for (int i = 0; i < variableCount; i++)
    {
        printf("%s = %g\n",
               variables[i].name,
               variables[i].value);
    }
}

int getVariableCount(void)
{
    return variableCount;
}

int getVariableByIndex(int index, char outName[], int outSize, double *outValue)
{
    if (index < 0 || index >= variableCount)
        return 0;

    strncpy(outName, variables[index].name, (size_t)outSize - 1);
    outName[outSize - 1] = '\0';
    *outValue = variables[index].value;

    return 1;
}
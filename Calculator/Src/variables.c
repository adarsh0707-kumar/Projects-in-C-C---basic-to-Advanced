#include <stdio.h>
#include <string.h>
#include "variables.h"

static Variable variables[MAX_VARIABLES] =
    {
        {"ans", 0}};

static int variableCount = 1;

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
#ifndef VARIABLES_H
#define VARIABLES_H

#define MAX_VARIABLES 100

typedef struct
{
    char name[32];
    double value;
} Variable;

void setAns(double value);
double getAns(void);

int setVariable(const char name[], double value);
int getVariable(const char name[], double *value);
void showVariables(void);

#endif
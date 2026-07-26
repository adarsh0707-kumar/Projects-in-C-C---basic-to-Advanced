#ifndef FUNCTION_INFO_H
#define FUNCTION_INFO_H

typedef struct
{
    const char *name;
    int argc;
} FunctionInfo;

int functionArgumentCount(const char name[]);

int isFunction(const char name[]);

#endif
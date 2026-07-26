#include<string.h>
#include<math.h>
#include"constants.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

double getConstant(const char name[]){
    if(strcmp(name, "pi") == 0){
        return M_PI;
    }
    if (strcmp(name, "e") == 0)
        return M_E;

    return 0;
}

int isConstant(const char name[]){
    return strcmp(name, "pi") == 0 ||
           strcmp(name, "e") == 0;
}
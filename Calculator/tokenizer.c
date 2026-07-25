#include <ctype.h>

int readNumber(char expression[], int *index){
    int number = 0;

    while(isdigit(expression[*index])){
        number = number * 10 + (expression[*index] - '0');
        (*index)++;
    }
}

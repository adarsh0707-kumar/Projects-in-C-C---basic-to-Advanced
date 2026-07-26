#include "stack.h"
#include"calculator.h"

int validateParentheses(char expression[])
{
    CharStack s;
    initCharStack(&s);

    int i = 0;

    while(expression[i] != '\0')
    {
        if(expression[i] == '('){
            pushChar(&s, '(');
        }
        else if(expression[i] == ')'){
            if(isEmptyCharStack(&s)){
                return 0;
            }
            popChar(&s);
        }
        i++;
    }
    return isEmptyCharStack(&s);
}

int isFunction(char name[])
{
    return strcmp(name, "sqrt") == 0 ||
           strcmp(name, "sin") == 0 ||
           strcmp(name, "cos") == 0 ||
           strcmp(name, "tan") == 0 ||
           strcmp(name, "log") == 0 ||
           strcmp(name, "ln") == 0 ||
           strcmp(name, "exp") == 0 ||
           strcmp(name, "abs") == 0;
}
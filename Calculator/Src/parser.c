#include "stack.h"

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
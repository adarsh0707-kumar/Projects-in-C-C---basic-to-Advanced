#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int num;
char operator;
int result = 0;
char expression[256] = "";

int main()
{

    do
    {
        printf("Enter a number for Calculation: ");
        scanf("%d", &num);
        printf("\nEnter an operator (+, -, *, /, %%, =): ");
        scanf(" %c", &operator);

        char temp[32];

        sprintf(temp, "%d %c ", num, operator);
        strcat(expression, temp);

        if (operator == '=')
        {
            printf("Exiting the calculator.\n");
            

            result = Calculate(result, num, operator);
            printf("The Result of %s = %d\n", expression, result);
            break;
        };
        printf("\n");
    } while (operator != '=');
}

int Calculate(int result, int num, char operator)
{
    switch (operator)
    {

    case '+':
        result = Sum(result, num);
        break;
    case '-':
        result = Subtract(result, num);
        break;
    case '*':
        result = Multiply(result, num);
        break;
    case '/':
        result = Divide(result, num);
        break;
    case '%':
        result = Modulus(result, num);
        break;
    default:
        printf("Invalid operator. Please try again.\n");
    }

    return result;
}
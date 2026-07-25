#include <stdio.h>
#include <string.h>
#include "calculator.h"

int main(void)
{
    int firstNumber;
    int nextNumber;
    char op;

    char infix[256] = "";
    char postfix[256];

    printf("Enter first number: ");

    if (scanf("%d", &firstNumber) != 1)
    {
        printf("Invalid input!\n");
        return 1;
    }

    snprintf(infix, sizeof(infix), "%d ", firstNumber);

    while (1)
    {
        printf("Operator (+, -, *, /, %%, =): ");

        if (scanf(" %c", &op) != 1)
        {
            printf("Invalid input!\n");
            return 1;
        }

        if (op == '=')
            break;

        printf("Next number: ");

        if (scanf("%d", &nextNumber) != 1)
        {
            printf("Invalid input!\n");
            return 1;
        }

        char temp[50];
        snprintf(temp, sizeof(temp), "%c %d ", op, nextNumber);

        strcat(infix, temp);
    }

    printf("\nInfix Expression   : %s\n", infix);

    infixToPostfix(infix, postfix);

    printf("Postfix Expression : %s\n", postfix);

    int result = evaluatePostfix(postfix);

    printf("Result             : %d\n", result);

    return 0;
}
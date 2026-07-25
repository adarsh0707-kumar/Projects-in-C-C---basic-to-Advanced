#include <stdio.h>
#include <string.h>
#include "calculator.h"

int main(void)
{
    char infix[256];
    char postfix[256];

    printf("Enter expression: ");

    if (fgets(infix, sizeof(infix), stdin) == NULL)
    {
        printf("Error reading input.\n");
        return 1;
    }

    /* Remove trailing newline */
    infix[strcspn(infix, "\n")] = '\0';

    /* Validate parentheses */
    if (!validateParentheses(infix))
    {
        printf("Error: Mismatched parentheses.\n");
        return 1;
    }

    /* Convert infix to postfix */
    infixToPostfix(infix, postfix);

    printf("\nInfix Expression   : %s\n", infix);
    printf("Postfix Expression : %s\n", postfix);

    /* Evaluate postfix */
    double result = evaluatePostfix(postfix);

    printf("Result             : %g\n", result);

    return 0;
}
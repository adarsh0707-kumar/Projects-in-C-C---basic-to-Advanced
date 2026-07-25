#include <stdio.h>
#include <string.h>
#include "calculator.h"
#include "history.h"

int main(void)
{
    char infix[256];
    char postfix[256];

    int choice;

    while (1)
    {
        printf("\n===== CALCULATOR =====\n\n");
        printf("1. New Calculation\n");
        printf("2. View History\n");
        printf("3. Clear History\n");
        printf("4. Exit\n");
        printf("Choice: ");

        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice)
        {
        case 1:
            /* Existing calculator code goes here */
            printf("Enter expression: ");

            if (fgets(infix, sizeof(infix), stdin) == NULL)
            {
                printf("Error reading input.\n");
                return 1;
            }

            /* Remove trailing newline */
            infix[strcspn(infix, "\n")] = '\0';
            if (!validateExpression(infix))
            {
                return 1;
            }

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
            addHistory(infix, result);
            break;

        case 2:
            showHistory();
            break;

        case 3:
            clearHistory();
            break;

        case 4:
            return 0;

        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}
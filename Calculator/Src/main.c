#include <stdio.h>
#include <string.h>
#include "calculator.h"
#include "history.h"
#include "memory.h"

int main(void)
{
    char infix[256];
    char postfix[256];

    int choice;
    double lastResult = 0.0;

    while (1)
    {
        printf("\n===== CALCULATOR =====\n\n");
        printf("1. New Calculation\n");
        printf("2. View History\n");
        printf("3. Clear History\n");
        printf("4. Memory Function\n");
        printf("5. Exit\n");
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
            lastResult = evaluatePostfix(postfix);

            printf("Result             : %g\n", lastResult);
            addHistory(infix, lastResult);
            break;

        case 2:
            showHistory();
            break;

        case 3:
            clearHistory();
            break;

        case 4:
        {
            int memChoice;
            double value;

            while (1)
            {
                printf("\n===== MEMORY =====\n");
                printf("1. MS (Store Last Result)\n");
                printf("2. MR (Recall Memory)\n");
                printf("3. M+ (Add to Memory)\n");
                printf("4. M- (Subtract from Memory)\n");
                printf("5. MC (Clear Memory)\n");
                printf("6. Back\n");
                printf("Choice: ");

                scanf("%d", &memChoice);

                switch (memChoice)
                {
                case 1:
                    memoryStore(lastResult);
                    break;

                case 2:
                    printf("Memory = %g\n", memoryRecall());
                    break;

                case 3:
                    printf("Enter value: ");
                    scanf("%lf", &value);
                    memoryAdd(value);
                    break;

                case 4:
                    printf("Enter value: ");
                    scanf("%lf", &value);
                    memorySubtract(value);
                    break;

                case 5:
                    memoryClear();
                    break;

                case 6:
                    goto exitMemoryMenu;

                default:
                    printf("Invalid choice!\n");
                }
            }

            exitMemoryMenu:
            break;
        }
        case 5:
            return 0;

        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}
#include <stdio.h>
#include <string.h>
#include "calculator.h"
#include "history.h"
#include "memory.h"
#include"variables.h"

int main(void)
{
    char infix[256];
    char postfix[256];

    char variableName[32];
    char expression[256];

    int choice;

    double lastResult = 0.0;

    while (1)
    {
        printf("\n===== CALCULATOR =====\n\n");
        printf("1. New Calculation\n");
        printf("2. View History\n");
        printf("3. Clear History\n");
        printf("4. Memory Function\n");
        printf("5. View Variables\n");
        printf("6. Exit\n");

        printf("\nChoice: ");

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
                break;
            }

            /* Remove trailing newline */
            infix[strcspn(infix, "\n")] = '\0';

            /* ----------------------------
               Variable Assignment
               Example:
               x=10
               y=5+3
               total=(x+y)*2
               ---------------------------- */
            char *equal = strchr(infix, '=');

            if (equal != NULL)
            {
                int len = equal - infix;

                strncpy(variableName, infix, len);
                variableName[len] = '\0';

                strcpy(expression, equal + 1);

                infixToPostfix(expression, postfix);

                lastResult = evaluatePostfix(postfix);

                setVariable(variableName, lastResult);
                setAns(lastResult);

                printf("\n%s = %g\n",
                       variableName,
                       lastResult);

                addHistory(infix, lastResult);

                break;
            }

            /* Normal expression */

            if (!validateExpression(infix))
            {
                break;
            }

            /* Validate parentheses */
            if (!validateParentheses(infix))
            {
                printf("Error: Mismatched parentheses.\n");
                break;
            }

            /* Convert infix to postfix */
            infixToPostfix(infix, postfix);

            printf("\nInfix Expression   : %s\n", infix);
            printf("Postfix Expression : %s\n", postfix);

            /* Evaluate postfix */
            lastResult = evaluatePostfix(postfix);

            setAns(lastResult);

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
            showVariables();
            break;

        case 6:
            printf("Goodbye!\n");
            return 0;

        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}
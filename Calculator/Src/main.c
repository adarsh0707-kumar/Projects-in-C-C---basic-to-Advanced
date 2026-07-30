#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "calculator.h"
#include "history.h"
#include "memory.h"
#include "variables.h"
#include "angle_mode.h"
#include "units.h"
#include "complex_eval.h"
#include "matrix_eval.h"
#include "stats.h"
#include "base.h"
#include "plot.h"

int main(void)
{
    char infix[256];
    char postfix[256];

    char variableName[32];
    char expression[256];
    char processed[512];

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
        printf("6. Unit Converter\n");
        printf("7. Complex Number Calculator\n");
        printf("8. Matrix Calculator\n");
        printf("9. Statistics\n");
        printf("10. Base Converter\n");
        printf("11. Plot Graph\n");
        printf("12. Exit\n");

        printf("\nChoice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ; // discard invalid input
            printf("Invalid choice!\n");
            continue;
        }

        while (getchar() != '\n')
            ; // discard remaining characters

        switch (choice)
        {
        case 1:
            /* Existing calculator code goes here */
            printf("Enter expression [%s]: ", angleModeName());

            if (fgets(expression, sizeof(expression), stdin) == NULL)
            {
                printf("Error reading input.\n");
                break;
            }

            expression[strcspn(expression, "\n")] = '\0';

            /* ----------------------------
               Expression history shortcuts
               Example:
               !!            (repeat the most recent calculation)
               !5            (repeat calculation #5 from history)
               history(10)   (show only the last 10 history entries)
               ---------------------------- */
            if (expression[0] == '!')
            {
                char resolved[256];

                if (expression[1] == '!' && expression[2] == '\0')
                {
                    if (!getLastHistoryExpression(resolved, sizeof(resolved)))
                    {
                        printf("Error: No history available.\n");
                        break;
                    }
                }
                else
                {
                    char *endptr;
                    long n = strtol(expression + 1, &endptr, 10);

                    if (endptr == expression + 1 || *endptr != '\0' || n <= 0)
                    {
                        printf("Error: Invalid history reference. "
                               "Use '!!' or '!<number>'.\n");
                        break;
                    }

                    if (!getHistoryExpressionByNumber((int)n, resolved, sizeof(resolved)))
                    {
                        printf("Error: History entry #%ld not found.\n", n);
                        break;
                    }
                }

                printf("Repeating: %s\n", resolved);
                strcpy(expression, resolved);
            }
            else if (strncmp(expression, "history(", 8) == 0)
            {
                char *closeParen = strchr(expression, ')');

                if (closeParen == NULL || *(closeParen + 1) != '\0')
                {
                    printf("Error: Invalid syntax. Use 'history(<count>)'.\n");
                    break;
                }

                char countStr[16];
                int len = (int)(closeParen - (expression + 8));

                if (len <= 0 || len >= (int)sizeof(countStr))
                {
                    printf("Error: Invalid syntax. Use 'history(<count>)'.\n");
                    break;
                }

                strncpy(countStr, expression + 8, (size_t)len);
                countStr[len] = '\0';

                char *endptr;
                long n = strtol(countStr, &endptr, 10);

                if (*endptr != '\0' || n <= 0)
                {
                    printf("Error: 'history(<count>)' requires a positive integer.\n");
                    break;
                }

                showRecentHistory((int)n);
                break;
            }

            /* ----------------------------
               Angle mode command
               Example:
               mode deg
               mode rad
               mode        (shows current mode)
               ---------------------------- */
            if (strncmp(expression, "mode", 4) == 0 &&
                (expression[4] == '\0' || isspace((unsigned char)expression[4])))
            {
                char arg[16] = "";

                sscanf(expression + 4, "%15s", arg);

                if (strcmp(arg, "deg") == 0)
                {
                    setAngleMode(MODE_DEGREE);
                    printf("Angle mode set to DEGREE.\n");
                }
                else if (strcmp(arg, "rad") == 0)
                {
                    setAngleMode(MODE_RADIAN);
                    printf("Angle mode set to RADIAN.\n");
                }
                else if (arg[0] == '\0')
                {
                    printf("Current angle mode: %s\n", angleModeName());
                }
                else
                {
                    printf("Unknown mode '%s'. Use 'mode deg' or 'mode rad'.\n", arg);
                }

                break;
            }

            /* Insert implicit '*' operators */
            insertImplicitMultiplication(expression, processed);

            /* Copy processed expression */
            strcpy(infix, processed);

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

                char *start = variableName;

                while (*start == ' ')
                    start++;

                char *end = start + strlen(start) - 1;

                while (end > start && *end == ' ')
                {
                    *end = '\0';
                    end--;
                }

                strcpy(variableName, start);

                strcpy(expression, equal + 1);

                insertImplicitMultiplication(expression, processed);

                if (!validateExpression(processed))
                    break;

                if (!validateParentheses(processed))
                {
                    printf("Error: Mismatched parentheses.\n");
                    break;
                }

                infixToPostfix(processed, postfix);

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

                if (scanf("%d", &memChoice) != 1)
                {
                    while (getchar() != '\n')
                        ;
                    printf("Invalid choice!\n");
                    continue;
                }

                while (getchar() != '\n')
                    ;

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
                    if (scanf("%lf", &value) != 1)
                    {
                        while (getchar() != '\n')
                            ;
                        printf("Invalid number!\n");
                        continue;
                    }

                    while (getchar() != '\n')
                        ;
                    memoryAdd(value);
                    break;

                case 4:
                    printf("Enter value: ");
                    if (scanf("%lf", &value) != 1)
                    {
                        while (getchar() != '\n')
                            ;
                        printf("Invalid number!\n");
                        continue;
                    }

                    while (getchar() != '\n')
                        ;
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
        {
            char convInput[64];
            double convValue;
            char convFromUnit[16];
            char convToUnit[16];

            printf("Enter value with unit (e.g. 10km, 30C, or 10km to miles): ");

            if (fgets(convInput, sizeof(convInput), stdin) == NULL)
            {
                printf("Error reading input.\n");
                break;
            }

            convInput[strcspn(convInput, "\n")] = '\0';

            if (!parseConversion(convInput, &convValue, convFromUnit, convToUnit))
            {
                printf("Error: Could not parse '%s'. Expected a format like "
                       "'10km' or '10km to miles'.\n",
                       convInput);
                break;
            }

            if (convToUnit[0] == '\0')
                convertAndPrint(convValue, convFromUnit);
            else
                convertToSingleUnit(convValue, convFromUnit, convToUnit);

            break;
        }

        case 7:
        {
            char complexInput[128];
            char complexResult[128];

            printf("Enter complex expression (e.g. (2+3i)*(4-5i), sqrt(-1)): ");

            if (fgets(complexInput, sizeof(complexInput), stdin) == NULL)
            {
                printf("Error reading input.\n");
                break;
            }

            complexInput[strcspn(complexInput, "\n")] = '\0';

            if (evaluateComplexExpression(complexInput, complexResult, sizeof(complexResult)))
                printf("%s = %s\n", complexInput, complexResult);
            else
                printf("Error: %s\n", complexResult);

            break;
        }

        case 8:
        {
            char matrixInput[256];
            char matrixResult[256];

            printf("Enter matrix expression (e.g. det([[1,2],[3,4]]), "
                   "inverse([[1,2],[3,4]]), transpose([[1,2],[3,4]])): ");

            if (fgets(matrixInput, sizeof(matrixInput), stdin) == NULL)
            {
                printf("Error reading input.\n");
                break;
            }

            matrixInput[strcspn(matrixInput, "\n")] = '\0';

            if (evaluateMatrixExpression(matrixInput, matrixResult, sizeof(matrixResult)))
                printf("%s = %s\n", matrixInput, matrixResult);
            else
                printf("Error: %s\n", matrixResult);

            break;
        }

        case 9:
        {
            char statsInput[256];
            char statsResult[64];

            printf("Enter statistics expression (e.g. mean(1,2,3,4), "
                   "median(2,7,5), stddev(4,8,6,5,3,7)): ");

            if (fgets(statsInput, sizeof(statsInput), stdin) == NULL)
            {
                printf("Error reading input.\n");
                break;
            }

            statsInput[strcspn(statsInput, "\n")] = '\0';

            if (evaluateStatsExpression(statsInput, statsResult, sizeof(statsResult)))
                printf("%s = %s\n", statsInput, statsResult);
            else
                printf("%s\n", statsResult);

            break;
        }

        case 10:
        {
            char baseInput[64];
            char baseResult[64];

            printf("Enter base expression (e.g. bin(25), hex(255), "
                   "oct(64), dec(1111b)): ");

            if (fgets(baseInput, sizeof(baseInput), stdin) == NULL)
            {
                printf("Error reading input.\n");
                break;
            }

            baseInput[strcspn(baseInput, "\n")] = '\0';

            if (evaluateBaseExpression(baseInput, baseResult, sizeof(baseResult)))
                printf("%s = %s\n", baseInput, baseResult);
            else
                printf("%s\n", baseResult);

            break;
        }

        case 11:
        {
            char plotInput[256];
            char plotError[128];

            printf("Enter plot expression (e.g. plot(sin(x)), plot(x^2), plot(log(x))): ");

            if (fgets(plotInput, sizeof(plotInput), stdin) == NULL)
            {
                printf("Error reading input.\n");
                break;
            }

            plotInput[strcspn(plotInput, "\n")] = '\0';

            if (!evaluatePlotExpression(plotInput, plotError, sizeof(plotError)))
            {
                if (plotError[0] != '\0')
                    printf("%s\n", plotError);
            }

            break;
        }

        case 12:
            printf("\nThank you for using the Scientific Calculator.\n");
            printf("Goodbye!\n");
            return 0;

        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}
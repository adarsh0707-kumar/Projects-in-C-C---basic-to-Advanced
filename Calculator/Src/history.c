#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "history.h"

#define HISTORY_FILE "./Build/history.txt"

void addHistory(char expression[], double result)
{
    FILE *fp = fopen(HISTORY_FILE, "a");

    if (fp == NULL)
    {
        fprintf(stderr,
                "Error opening '%s': %s\n",
                HISTORY_FILE,
                strerror(errno));
        return;
    }
    fprintf(fp, "%s = %g\n", expression, result);

    fclose(fp);
}

void showHistory(void)
{
    FILE *fp = fopen(HISTORY_FILE, "r");

    if (fp == NULL)
    {
        printf("No history available!\n");
        return;
    }

    char line[256];
    int count = 1;

    printf("\n========== HISTORY ==========\n");

    while (fgets(line, sizeof(line), fp))
    {
        printf("%d. %s", count++, line);
    }

    printf("=============================\n");

    fclose(fp);
}

void clearHistory(void)
{
    FILE *fp = fopen(HISTORY_FILE, "w");
    if (fp != NULL)
        fclose(fp);

    printf("History cleared.\n");
}

/* History lines are stored as "<expression> = <result>\n"; this
   pulls just the expression part back out. */
static int extractExpressionFromLine(const char line[], char outExpr[], int outSize)
{
    const char *eq = strstr(line, " = ");

    if (eq == NULL)
        return 0;

    int len = (int)(eq - line);

    if (len <= 0 || len >= outSize)
        return 0;

    strncpy(outExpr, line, (size_t)len);
    outExpr[len] = '\0';
    return 1;
}

int getLastHistoryExpression(char outExpr[], int outSize)
{
    FILE *fp = fopen(HISTORY_FILE, "r");
    if (fp == NULL)
        return 0;

    char line[256];
    char lastLine[256] = "";
    int found = 0;

    while (fgets(line, sizeof(line), fp))
    {
        strcpy(lastLine, line);
        found = 1;
    }

    fclose(fp);

    if (!found)
        return 0;

    return extractExpressionFromLine(lastLine, outExpr, outSize);
}

int getHistoryExpressionByNumber(int n, char outExpr[], int outSize)
{
    if (n <= 0)
        return 0;

    FILE *fp = fopen(HISTORY_FILE, "r");
    if (fp == NULL)
        return 0;

    char line[256];
    int count = 0;
    int found = 0;
    char target[256] = "";

    while (fgets(line, sizeof(line), fp))
    {
        count++;
        if (count == n)
        {
            strcpy(target, line);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found)
        return 0;

    return extractExpressionFromLine(target, outExpr, outSize);
}

void showRecentHistory(int n)
{
    FILE *fp = fopen(HISTORY_FILE, "r");

    if (fp == NULL)
    {
        printf("No history available!\n");
        return;
    }

    char line[256];
    int total = 0;

    while (fgets(line, sizeof(line), fp))
        total++;

    if (total == 0)
    {
        printf("No history available!\n");
        fclose(fp);
        return;
    }

    int startLine = total - n + 1;
    if (startLine < 1)
        startLine = 1;

    rewind(fp);

    printf("\n===== LAST %d HISTORY ENTRIES =====\n", total - startLine + 1);

    int count = 0;
    while (fgets(line, sizeof(line), fp))
    {
        count++;
        if (count >= startLine)
            printf("%d. %s", count, line);
    }

    printf("===================================\n");

    fclose(fp);
}
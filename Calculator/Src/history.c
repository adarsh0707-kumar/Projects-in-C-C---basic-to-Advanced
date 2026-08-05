#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

/*
 * The history file used to be the literal path "./Build/history.txt",
 * which only works when the calculator is launched from the project
 * directory. A user who downloads a release binary and runs it from
 * their Downloads folder has no ./Build, so every history operation
 * silently failed. The path is now resolved at runtime to a per-user
 * data directory, which is also where each platform expects an
 * application to keep this kind of state.
 */
#if defined(_WIN32)
#include <direct.h>
#define MAKE_DIR(path) _mkdir(path)
#define PATH_SEP "\\"
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MAKE_DIR(path) mkdir((path), 0755)
#define PATH_SEP "/"
#endif

#define HISTORY_PATH_MAX 512
/* Deliberately smaller than HISTORY_PATH_MAX so appending the
   separator and "history.txt" to a full-length directory provably
   still fits, which is what -Wformat-truncation checks. */
#define HISTORY_DIR_MAX 480

/* Creates every missing component of @p path, like `mkdir -p`. An
   already-existing component reports EEXIST, which isn't an error
   here. Anything else is left for the caller's fopen() to report. */
static void ensureDirectoryExists(const char path[])
{
    char partial[HISTORY_PATH_MAX];
    size_t len = strlen(path);

    if (len == 0 || len >= sizeof(partial))
        return;

    memcpy(partial, path, len + 1);

    for (size_t i = 1; i < len; i++)
    {
        if (partial[i] == '/' || partial[i] == '\\')
        {
            char saved = partial[i];
            partial[i] = '\0';
            MAKE_DIR(partial);
            partial[i] = saved;
        }
    }

    MAKE_DIR(partial);
}

/* Set by setHistoryFilePath(); empty means "not overridden". */
static char explicitPath[HISTORY_PATH_MAX] = "";

void setHistoryFilePath(const char path[])
{
    if (path == NULL)
        explicitPath[0] = '\0';
    else
        snprintf(explicitPath, sizeof(explicitPath), "%s", path);
}

const char *historyFilePath(void)
{
    static char path[HISTORY_PATH_MAX];
    char directory[HISTORY_DIR_MAX];

    /* An explicit override wins over everything. The test suite uses
       this to keep its scratch history out of the real one. */
    if (explicitPath[0] != '\0')
    {
        snprintf(path, sizeof(path), "%s", explicitPath);
        return path;
    }

    /* Then the environment, so a user can put history wherever they
       like without recompiling. getenv() is ISO C; setenv() is not,
       which is why the override above is a function rather than the
       test suite setting this variable. */
    const char *override = getenv("CALCULATOR_HISTORY_FILE");

    if (override != NULL && override[0] != '\0')
    {
        snprintf(path, sizeof(path), "%s", override);
        return path;
    }

#if defined(_WIN32)
    const char *base = getenv("APPDATA");

    if (base != NULL && base[0] != '\0')
        snprintf(directory, sizeof(directory), "%s" PATH_SEP "Calculator", base);
    else
        directory[0] = '\0';
#elif defined(__APPLE__)
    const char *home = getenv("HOME");

    if (home != NULL && home[0] != '\0')
        snprintf(directory, sizeof(directory), "%s/Library/Application Support/Calculator", home);
    else
        directory[0] = '\0';
#else
    /* XDG Base Directory spec: $XDG_DATA_HOME, else ~/.local/share. */
    const char *xdg = getenv("XDG_DATA_HOME");
    const char *home = getenv("HOME");

    if (xdg != NULL && xdg[0] != '\0')
        snprintf(directory, sizeof(directory), "%s/calculator", xdg);
    else if (home != NULL && home[0] != '\0')
        snprintf(directory, sizeof(directory), "%s/.local/share/calculator", home);
    else
        directory[0] = '\0';
#endif

    if (directory[0] == '\0')
    {
        /* No home directory to work with (a bare container, a cron
           job with a stripped environment). Fall back to the working
           directory rather than losing history entirely. */
        snprintf(path, sizeof(path), "history.txt");
        return path;
    }

    ensureDirectoryExists(directory);
    snprintf(path, sizeof(path), "%s" PATH_SEP "history.txt", directory);

    return path;
}

void addHistory(char expression[], double result)
{
    /* Resolved once and held: historyFilePath() can itself touch errno
       (it may attempt mkdir), so calling it again inside the error
       branch below could overwrite the errno that fopen just set. */
    const char *file = historyFilePath();

    FILE *fp = fopen(file, "a");

    if (fp == NULL)
    {
        fprintf(stderr,
                "Error opening '%s': %s\n",
                file,
                strerror(errno));
        return;
    }
    fprintf(fp, "%s = %g\n", expression, result);

    fclose(fp);
}

void showHistory(void)
{
    FILE *fp = fopen(historyFilePath(), "r");

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
    FILE *fp = fopen(historyFilePath(), "w");
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
    FILE *fp = fopen(historyFilePath(), "r");
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

    FILE *fp = fopen(historyFilePath(), "r");
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
    FILE *fp = fopen(historyFilePath(), "r");

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

int getHistoryCount(void)
{
    FILE *fp = fopen(historyFilePath(), "r");
    if (fp == NULL)
        return 0;

    char line[256];
    int total = 0;

    while (fgets(line, sizeof(line), fp))
        total++;

    fclose(fp);
    return total;
}

int getHistoryLineByNumber(int n, char outLine[], int outSize)
{
    if (n <= 0)
        return 0;

    FILE *fp = fopen(historyFilePath(), "r");
    if (fp == NULL)
        return 0;

    char line[256];
    int count = 0;
    int found = 0;

    while (fgets(line, sizeof(line), fp))
    {
        count++;
        if (count == n)
        {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found)
        return 0;

    size_t len = strcspn(line, "\r\n");

    if ((int)len >= outSize)
        len = (size_t)(outSize - 1);

    strncpy(outLine, line, len);
    outLine[len] = '\0';
    return 1;
}
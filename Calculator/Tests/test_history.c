#include <stdio.h>
#include "test_framework.h"
#include "tests.h"
#include "history.h"

/*
 * history.c persists to a real file on disk (./Build/history.txt,
 * hardcoded internally -- not exposed via history.h). That's fine for
 * the interactive calculator, but it means these tests would otherwise
 * overwrite whatever history you'd actually built up while using the
 * calculator by hand. So: back the real file up before testing and
 * restore it afterward. This path string duplicates history.c's
 * internal HISTORY_FILE constant; if that ever changes, update this too.
 */
#define HISTORY_FILE "./Build/history.txt"
#define HISTORY_BACKUP_FILE "./Build/history_test_backup.txt"

static void copyFile(const char *from, const char *to)
{
    FILE *src = fopen(from, "r");
    if (src == NULL)
        return;

    FILE *dst = fopen(to, "w");
    if (dst == NULL)
    {
        fclose(src);
        return;
    }

    char buf[4096];
    size_t n;

    while ((n = fread(buf, 1, sizeof(buf), src)) > 0)
        fwrite(buf, 1, n, dst);

    fclose(src);
    fclose(dst);
}

static void backupRealHistory(void)
{
    copyFile(HISTORY_FILE, HISTORY_BACKUP_FILE);
}

static void restoreRealHistory(void)
{
    FILE *backup = fopen(HISTORY_BACKUP_FILE, "r");

    if (backup == NULL)
    {
        /* There was no pre-existing history file -- leave it cleared. */
        clearHistory();
        return;
    }

    fclose(backup);
    copyFile(HISTORY_BACKUP_FILE, HISTORY_FILE);
    remove(HISTORY_BACKUP_FILE);
}

static void test_add_and_get_last(void)
{
    clearHistory();

    char expr1[] = "2+3";
    addHistory(expr1, 5.0);

    char expr2[] = "10*2";
    addHistory(expr2, 20.0);

    char resolved[256];
    int found = getLastHistoryExpression(resolved, sizeof(resolved));

    ASSERT_TRUE(found == 1, "getLastHistoryExpression() should succeed when history isn't empty");
    ASSERT_STR_EQ(resolved, "10*2", "'!!' should resolve to the most recently added expression");
}

static void test_get_by_number(void)
{
    clearHistory();

    char expr1[] = "1+1";
    addHistory(expr1, 2.0);

    char expr2[] = "2+2";
    addHistory(expr2, 4.0);

    char expr3[] = "3+3";
    addHistory(expr3, 6.0);

    char resolved[256];

    ASSERT_TRUE(getHistoryExpressionByNumber(2, resolved, sizeof(resolved)) == 1,
                "entry #2 should exist after 3 additions");
    ASSERT_STR_EQ(resolved, "2+2", "'!2' should resolve to the 2nd expression added, in order");

    ASSERT_TRUE(getHistoryExpressionByNumber(99, resolved, sizeof(resolved)) == 0,
                "referencing a history entry that doesn't exist should fail, not crash");
}

static void test_count_and_line_by_number(void)
{
    clearHistory();

    ASSERT_TRUE(getHistoryCount() == 0, "an empty history should count as 0 entries");

    char expr1[] = "2+3";
    addHistory(expr1, 5.0);

    char expr2[] = "10*2";
    addHistory(expr2, 20.0);

    ASSERT_TRUE(getHistoryCount() == 2, "getHistoryCount() should reflect both additions");

    char line[256];

    ASSERT_TRUE(getHistoryLineByNumber(1, line, sizeof(line)) == 1, "entry #1 should exist");
    ASSERT_STR_EQ(line, "2+3 = 5", "getHistoryLineByNumber() should return the full 'expr = result' line");

    ASSERT_TRUE(getHistoryLineByNumber(2, line, sizeof(line)) == 1, "entry #2 should exist");
    ASSERT_STR_EQ(line, "10*2 = 20", "entries should stay in the order they were added");

    ASSERT_TRUE(getHistoryLineByNumber(99, line, sizeof(line)) == 0,
                "referencing a history entry that doesn't exist should fail, not crash");
    ASSERT_TRUE(getHistoryLineByNumber(0, line, sizeof(line)) == 0, "entry #0 should fail (1-indexed)");
}

static void test_clear_history(void)
{
    char expr[] = "5+5";
    addHistory(expr, 10.0);

    clearHistory();

    char resolved[256];
    int found = getLastHistoryExpression(resolved, sizeof(resolved));

    ASSERT_TRUE(found == 0, "getLastHistoryExpression() should fail right after clearHistory()");
}

void run_history_tests(void)
{
    backupRealHistory();

    test_add_and_get_last();
    test_get_by_number();
    test_count_and_line_by_number();
    test_clear_history();

    restoreRealHistory();
}
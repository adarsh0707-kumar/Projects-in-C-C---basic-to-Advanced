#include <stdio.h>
#include <stdlib.h>
#include "test_framework.h"
#include "tests.h"
#include "history.h"

/*
 * These tests exercise real file I/O, so they need somewhere to write
 * that isn't the history the user has built up by hand. history.c
 * honours $CALCULATOR_HISTORY_FILE ahead of every platform default
 * (see historyFilePath()), so pointing that at a scratch file makes
 * the suite hermetic: nothing is backed up, nothing is restored, and
 * the real history is never opened in the first place.
 *
 * This replaces an earlier backup-and-restore dance against a
 * hardcoded "./Build/history.txt" -- which duplicated a path constant
 * that has since moved, and which lost the user's history outright if
 * the suite crashed between the backup and the restore.
 */
#define TEST_HISTORY_FILE "calculator_test_history.txt"

static void useScratchHistoryFile(void)
{
    setHistoryFilePath(TEST_HISTORY_FILE);
}

static void removeScratchHistoryFile(void)
{
    remove(TEST_HISTORY_FILE);
    setHistoryFilePath(NULL);
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
    useScratchHistoryFile();

    test_add_and_get_last();
    test_get_by_number();
    test_count_and_line_by_number();
    test_clear_history();

    removeScratchHistoryFile();
}
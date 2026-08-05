#ifndef HISTORY_H
#define HISTORY_H

/** @brief Returns the absolute path of the history file, creating its
 *  parent directory if needed.
 *
 *  Resolved at runtime rather than hardcoded, so a downloaded binary
 *  works from any working directory:
 *  - `$CALCULATOR_HISTORY_FILE` if set (used by the test suite, and
 *    available to anyone who wants history somewhere specific);
 *  - Windows: `%APPDATA%\Calculator\history.txt`;
 *  - macOS: `~/Library/Application Support/Calculator/history.txt`;
 *  - elsewhere: `$XDG_DATA_HOME/calculator/history.txt`, falling back
 *    to `~/.local/share/calculator/history.txt`.
 *
 *  With no home directory in the environment at all, returns a bare
 *  "history.txt" in the working directory. The returned pointer is to
 *  static storage: it stays valid until the next call, and callers
 *  that need it across an errno check should copy or hold it. */
const char *historyFilePath(void);

/** @brief Overrides the history file path for the rest of the process,
 *  ahead of both `$CALCULATOR_HISTORY_FILE` and the platform default.
 *  Pass NULL to clear the override and go back to normal resolution.
 *
 *  Exists because the test suite needs a scratch file and `setenv()`
 *  isn't ISO C (it's unavailable under a strict `-std=c11`), so the
 *  environment variable alone couldn't be driven portably from a test. */
void setHistoryFilePath(const char path[]);

void addHistory(char expression[], double result);
void showHistory(void);
void clearHistory(void);

/* Phase 18: expression history shortcuts ('!!', '!5', 'history(10)') */

/* Writes the most recently logged expression into outExpr.
   Returns 1 on success, 0 if history is empty. */
int getLastHistoryExpression(char outExpr[], int outSize);

/* Writes the expression from history entry #n (1-indexed, same
   numbering shown by showHistory()) into outExpr.
   Returns 1 on success, 0 if entry n doesn't exist. */
int getHistoryExpressionByNumber(int n, char outExpr[], int outSize);

/* Prints only the most recent n history entries (fewer if history
   has less than n entries), preserving their original numbering. */
void showRecentHistory(int n);

/* Returns the number of entries currently in history (0 if the file
   doesn't exist yet or is empty). */
int getHistoryCount(void);

/* Writes the full formatted line for history entry #n (1-indexed,
   same numbering as showHistory()), e.g. "2+3 = 5", into outLine
   with the trailing newline stripped. Returns 1 on success, 0 if
   entry n doesn't exist. */
int getHistoryLineByNumber(int n, char outLine[], int outSize);

#endif
#ifndef HISTORY_H
#define HISTORY_H

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
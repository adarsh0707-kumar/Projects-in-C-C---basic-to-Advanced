/**
 * @file stats.h
 * @brief Descriptive statistics over a comma-separated argument list.
 */
#ifndef STATS_H
#define STATS_H

/**
 * @brief Evaluates a statistics expression, e.g. "mean(1,2,3,4)",
 * "median(2,7,5)", "stddev(4,8,6,5,3,7)".
 *
 * Supported functions:
 *   - `mean`, `median`, `sum`, `min`, `max`, `count`  (any count >= 1)
 *   - `stddev`, `variance`     (sample, divides by n-1, needs >= 2 values)
 *   - `pstddev`, `pvariance`   (population, divides by n, needs >= 1 value)
 *
 * @param expr       Expression such as "mean(1,2,3,4)".
 * @param result     Destination buffer for the formatted numeric result
 *                   (on success) or an error message (on failure).
 * @param resultSize Size of @p result in bytes.
 * @return 1 on success, 0 on failure. */
int evaluateStatsExpression(const char expr[], char result[], int resultSize);

#endif

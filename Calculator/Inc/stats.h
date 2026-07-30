#ifndef STATS_H
#define STATS_H

/*
 * Evaluates a statistics expression, e.g. "mean(1,2,3,4)",
 * "median(2,7,5)", "stddev(4,8,6,5,3,7)", and writes the formatted
 * numeric result into result[resultSize]. Returns 1 on success.
 * On failure, returns 0 and writes an error message into result
 * instead.
 *
 * Supported functions:
 *   mean, median, sum, min, max, count      (any count >= 1)
 *   stddev, variance                         (sample, /(n-1), needs >= 2)
 *   pstddev, pvariance                       (population, /n, needs >= 1)
 */
int evaluateStatsExpression(const char expr[], char result[], int resultSize);

#endif
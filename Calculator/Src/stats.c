#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "stats.h"

#define MAX_STATS_VALUES 256

/*
 * Reads a comma-separated list of numbers starting at expr[*i]
 * (which should be positioned right after the opening '('),
 * advancing *i past the last number read (stopping at ',' or the
 * closing ')'). Returns 0 on any malformed number.
 */
static int parseNumberList(const char expr[], int *i, double values[], int *count)
{
    *count = 0;

    while (1)
    {
        while (isspace((unsigned char)expr[*i]))
            (*i)++;

        int start = *i;

        if (expr[*i] == '-' || expr[*i] == '+')
            (*i)++;

        int dotCount = 0;
        int hasDigits = 0;

        while (isdigit((unsigned char)expr[*i]) || expr[*i] == '.')
        {
            if (expr[*i] == '.')
                dotCount++;
            else
                hasDigits = 1;

            if (dotCount > 1)
                return 0;

            (*i)++;
        }

        if (!hasDigits)
            return 0;

        if (*count >= MAX_STATS_VALUES)
            return 0;

        char buf[32];
        int len = *i - start;

        if (len <= 0 || len >= (int)sizeof(buf))
            return 0;

        strncpy(buf, expr + start, len);
        buf[len] = '\0';
        values[(*count)++] = atof(buf);

        while (isspace((unsigned char)expr[*i]))
            (*i)++;

        if (expr[*i] == ',')
        {
            (*i)++;
            continue;
        }

        break;
    }

    return 1;
}

static int compareDoubles(const void *a, const void *b)
{
    double da = *(const double *)a;
    double db = *(const double *)b;

    if (da < db)
        return -1;
    if (da > db)
        return 1;
    return 0;
}

static double statMean(const double values[], int n)
{
    double sum = 0.0;

    for (int i = 0; i < n; i++)
        sum += values[i];

    return sum / n;
}

static double statMedian(const double values[], int n)
{
    double sorted[MAX_STATS_VALUES];
    memcpy(sorted, values, (size_t)n * sizeof(double));
    qsort(sorted, (size_t)n, sizeof(double), compareDoubles);

    if (n % 2 == 1)
        return sorted[n / 2];

    return (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;
}

static double statSum(const double values[], int n)
{
    double sum = 0.0;

    for (int i = 0; i < n; i++)
        sum += values[i];

    return sum;
}

static double statMin(const double values[], int n)
{
    double m = values[0];

    for (int i = 1; i < n; i++)
        if (values[i] < m)
            m = values[i];

    return m;
}

static double statMax(const double values[], int n)
{
    double m = values[0];

    for (int i = 1; i < n; i++)
        if (values[i] > m)
            m = values[i];

    return m;
}

/* denominator selects sample (n-1, Bessel's correction) vs
   population (n) variance */
static double statVarianceWithDenom(const double values[], int n, int sample)
{
    double m = statMean(values, n);
    double sumSq = 0.0;

    for (int i = 0; i < n; i++)
    {
        double diff = values[i] - m;
        sumSq += diff * diff;
    }

    return sumSq / (double)(sample ? (n - 1) : n);
}

int evaluateStatsExpression(const char expr[], char result[], int resultSize)
{
    int i = 0;

    while (isspace((unsigned char)expr[i]))
        i++;

    char name[16];
    int n = 0;

    while (isalpha((unsigned char)expr[i]) && n < 15)
        name[n++] = expr[i++];

    name[n] = '\0';

    while (isspace((unsigned char)expr[i]))
        i++;

    if (n == 0 || expr[i] != '(')
    {
        snprintf(result, (size_t)resultSize,
                 "Error: Expected a function call like 'mean(1,2,3)'.");
        return 0;
    }

    i++; /* consume '(' */

    double values[MAX_STATS_VALUES];
    int count = 0;

    if (!parseNumberList(expr, &i, values, &count))
    {
        snprintf(result, (size_t)resultSize, "Error: Could not parse the argument list.");
        return 0;
    }

    while (isspace((unsigned char)expr[i]))
        i++;

    if (expr[i] != ')')
    {
        snprintf(result, (size_t)resultSize, "Error: Expected ')'.");
        return 0;
    }

    i++;

    while (isspace((unsigned char)expr[i]))
        i++;

    if (expr[i] != '\0')
    {
        snprintf(result, (size_t)resultSize, "Error: Unexpected characters after ')'.");
        return 0;
    }

    if (count == 0)
    {
        snprintf(result, (size_t)resultSize,
                 "Error: '%s' requires at least one value.", name);
        return 0;
    }

    double stat;

    if (strcmp(name, "mean") == 0)
    {
        stat = statMean(values, count);
    }
    else if (strcmp(name, "median") == 0)
    {
        stat = statMedian(values, count);
    }
    else if (strcmp(name, "sum") == 0)
    {
        stat = statSum(values, count);
    }
    else if (strcmp(name, "min") == 0)
    {
        stat = statMin(values, count);
    }
    else if (strcmp(name, "max") == 0)
    {
        stat = statMax(values, count);
    }
    else if (strcmp(name, "count") == 0)
    {
        stat = (double)count;
    }
    else if (strcmp(name, "stddev") == 0 || strcmp(name, "variance") == 0)
    {
        if (count < 2)
        {
            snprintf(result, (size_t)resultSize,
                     "Error: '%s' requires at least two values (sample statistic).", name);
            return 0;
        }

        double var = statVarianceWithDenom(values, count, 1);
        stat = (strcmp(name, "stddev") == 0) ? sqrt(var) : var;
    }
    else if (strcmp(name, "pstddev") == 0 || strcmp(name, "pvariance") == 0)
    {
        double var = statVarianceWithDenom(values, count, 0);
        stat = (strcmp(name, "pstddev") == 0) ? sqrt(var) : var;
    }
    else
    {
        snprintf(result, (size_t)resultSize,
                 "Error: Unknown statistics function '%s'.", name);
        return 0;
    }

    snprintf(result, (size_t)resultSize, "%g", stat);
    return 1;
}
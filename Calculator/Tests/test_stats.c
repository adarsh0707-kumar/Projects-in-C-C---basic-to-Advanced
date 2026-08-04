#include "test_framework.h"
#include "tests.h"
#include "stats.h"
#include <stdlib.h>

static void test_basic_stats(void)
{
    char result[64];

    ASSERT_TRUE(evaluateStatsExpression("mean(1,2,3,4)", result, sizeof(result)) == 1, "mean() should succeed");
    ASSERT_DOUBLE_EQ(atof(result), 2.5, "mean(1,2,3,4) should be 2.5");

    ASSERT_TRUE(evaluateStatsExpression("median(2,7,5)", result, sizeof(result)) == 1, "median() should succeed");
    ASSERT_DOUBLE_EQ(atof(result), 5.0, "median(2,7,5) should be 5 (the middle value once sorted)");

    ASSERT_TRUE(evaluateStatsExpression("median(1,2,3,4)", result, sizeof(result)) == 1,
                "median() with an even count should succeed");
    ASSERT_DOUBLE_EQ(atof(result), 2.5, "median(1,2,3,4) should average the two middle values");

    ASSERT_TRUE(evaluateStatsExpression("sum(1,2,3,4)", result, sizeof(result)) == 1, "sum() should succeed");
    ASSERT_DOUBLE_EQ(atof(result), 10.0, "sum(1,2,3,4) should be 10");

    ASSERT_TRUE(evaluateStatsExpression("min(4,1,3,2)", result, sizeof(result)) == 1, "min() should succeed");
    ASSERT_DOUBLE_EQ(atof(result), 1.0, "min(4,1,3,2) should be 1");

    ASSERT_TRUE(evaluateStatsExpression("max(4,1,3,2)", result, sizeof(result)) == 1, "max() should succeed");
    ASSERT_DOUBLE_EQ(atof(result), 4.0, "max(4,1,3,2) should be 4");

    ASSERT_TRUE(evaluateStatsExpression("count(4,1,3,2)", result, sizeof(result)) == 1, "count() should succeed");
    ASSERT_DOUBLE_EQ(atof(result), 4.0, "count(4,1,3,2) should be 4");
}

static void test_variance_family(void)
{
    char result[64];

    /* stddev(4,8,6,5,3,7): mean = 5.5, sample variance (n-1=5) = 3.5,
       stddev = sqrt(3.5) ~= 1.87083. Results are formatted with "%g"
       (6 significant digits), so these compare against the exact
       rounded string rather than the full-precision value -- ASSERT_
       DOUBLE_EQ's 1e-6 tolerance is tighter than %g's own rounding
       error at this magnitude, and would spuriously fail otherwise. */
    ASSERT_TRUE(evaluateStatsExpression("stddev(4,8,6,5,3,7)", result, sizeof(result)) == 1,
                "stddev() should succeed with >= 2 values");
    ASSERT_STR_EQ(result, "1.87083", "stddev(4,8,6,5,3,7) should be sqrt(3.5)");

    ASSERT_TRUE(evaluateStatsExpression("variance(4,8,6,5,3,7)", result, sizeof(result)) == 1,
                "variance() should succeed with >= 2 values");
    ASSERT_DOUBLE_EQ(atof(result), 3.5, "variance(4,8,6,5,3,7) should be 3.5 (sample, n-1)");

    /* Population variance divides by n instead of n-1: sum of squared
       diffs is 17.5, so population variance = 17.5/6 ~= 2.91667. */
    ASSERT_TRUE(evaluateStatsExpression("pvariance(4,8,6,5,3,7)", result, sizeof(result)) == 1,
                "pvariance() should succeed");
    ASSERT_STR_EQ(result, "2.91667", "pvariance(4,8,6,5,3,7) should be 17.5/6");

    ASSERT_TRUE(evaluateStatsExpression("pstddev(4,8,6,5,3,7)", result, sizeof(result)) == 1,
                "pstddev() should succeed");
    ASSERT_STR_EQ(result, "1.70783", "pstddev(4,8,6,5,3,7) should be sqrt(17.5/6)");

    /* pstddev/pvariance only need >= 1 value; stddev/variance need >= 2. */
    ASSERT_TRUE(evaluateStatsExpression("pstddev(5)", result, sizeof(result)) == 1,
                "pstddev() should accept a single value");
    ASSERT_DOUBLE_EQ(atof(result), 0.0, "pstddev() of a single value should be 0");
}

static void test_stats_error_paths(void)
{
    char result[64];

    ASSERT_TRUE(evaluateStatsExpression("nope", result, sizeof(result)) == 0,
                "an expression with no '(' should fail");
    ASSERT_TRUE(evaluateStatsExpression("mean(1,2,)", result, sizeof(result)) == 0,
                "a trailing comma with no following number should fail");
    ASSERT_TRUE(evaluateStatsExpression("mean(1,2,3", result, sizeof(result)) == 0,
                "a missing closing paren should fail");
    ASSERT_TRUE(evaluateStatsExpression("mean(1,2,3)x", result, sizeof(result)) == 0,
                "trailing characters after ')' should fail");
    ASSERT_TRUE(evaluateStatsExpression("stddev(5)", result, sizeof(result)) == 0,
                "stddev() with only one value should fail (sample statistic needs >= 2)");
    ASSERT_TRUE(evaluateStatsExpression("banana(1,2,3)", result, sizeof(result)) == 0,
                "an unknown statistics function name should fail");
}

void run_stats_tests(void)
{
    test_basic_stats();
    test_variance_family();
    test_stats_error_paths();
}

#include "test_framework.h"
#include "tests.h"
#include "functions.h"
#include "angle_mode.h"
#include "error.h"
#include <math.h>

static void test_unary_functions(void)
{
    ASSERT_DOUBLE_EQ(applyFunction("sqrt", 16.0), 4.0, "sqrt(16) should be 4");
    ASSERT_DOUBLE_EQ(applyFunction("abs", -5.0), 5.0, "abs(-5) should be 5");
    ASSERT_DOUBLE_EQ(applyFunction("fabs", -5.0), 5.0, "fabs should be an alias for abs");
    ASSERT_DOUBLE_EQ(applyFunction("floor", 2.9), 2.0, "floor(2.9) should be 2");
    ASSERT_DOUBLE_EQ(applyFunction("ceil", 2.1), 3.0, "ceil(2.1) should be 3");
    ASSERT_DOUBLE_EQ(applyFunction("round", 2.5), 3.0, "round(2.5) should be 3");
    ASSERT_DOUBLE_EQ(applyFunction("log", 100.0), 2.0, "log(100) should be base-10 log, i.e. 2");
    ASSERT_DOUBLE_EQ(applyFunction("cbrt", 27.0), 3.0, "cbrt(27) should be 3");
}

static void test_trig_respects_angle_mode(void)
{
    setAngleMode(MODE_RADIAN);
    ASSERT_DOUBLE_EQ(applyFunction("sin", 0.0), 0.0, "sin(0 rad) should be 0");

    setAngleMode(MODE_DEGREE);
    ASSERT_DOUBLE_EQ(applyFunction("sin", 90.0), 1.0, "sin(90 deg) should be 1");
    ASSERT_DOUBLE_EQ(applyFunction("cos", 180.0), -1.0, "cos(180 deg) should be -1");

    /* Reset so later suites (and any manual testing in the same run)
       see the default radian mode, not whatever this suite left behind. */
    setAngleMode(MODE_RADIAN);
}

static void test_binary_functions(void)
{
    ASSERT_DOUBLE_EQ(applyBinaryFunction("pow", 2.0, 8.0), 256.0, "pow(2,8) should be 256");
    ASSERT_DOUBLE_EQ(applyBinaryFunction("max", 10.0, 20.0), 20.0, "max(10,20) should be 20");
    ASSERT_DOUBLE_EQ(applyBinaryFunction("min", 5.0, 3.0), 3.0, "min(5,3) should be 3");
    ASSERT_DOUBLE_EQ(applyBinaryFunction("hypot", 3.0, 4.0), 5.0, "hypot(3,4) should be 5");
}

static void test_factorial(void)
{
    ASSERT_DOUBLE_EQ(factorial(0), 1.0, "0! should be 1");
    ASSERT_DOUBLE_EQ(factorial(5), 120.0, "5! should be 120");
}

/*
 * As of the Phase C error-handling migration, factorial(),
 * applyFunction(), and applyBinaryFunction() report failure via NAN +
 * calculatorSetLastError() instead of exit()ing -- see
 * docs/CHANGELOG.md. These no longer need ASSERT_EXITS_NONZERO
 * (fork+check-exit-status); a plain in-process assertion is enough,
 * and confirms the process really does keep running afterward.
 */
static void test_functions_error_paths(void)
{
    calculatorClearError();
    ASSERT_TRUE(isnan(factorial(-3)),
                "factorial(-3) should return NAN, not a wrong value");
    ASSERT_TRUE(calculatorGetLastError() == CALC_ERR_DOMAIN,
                "should record CALC_ERR_DOMAIN");

    calculatorClearError();
    ASSERT_TRUE(isnan(factorial(2.5)),
                "factorial(2.5) should return NAN, not silently truncate");
    ASSERT_TRUE(calculatorGetLastError() == CALC_ERR_DOMAIN,
                "should record CALC_ERR_DOMAIN");

    calculatorClearError();
    ASSERT_TRUE(isnan(applyBinaryFunction("notarealfunction", 1.0, 2.0)),
                "an unrecognized binary function name should return NAN");
    ASSERT_TRUE(calculatorGetLastError() == CALC_ERR_INVALID_FUNCTION,
                "should record CALC_ERR_INVALID_FUNCTION");

    calculatorClearError();
    ASSERT_TRUE(isnan(applyFunction("notarealfunction", 1.0)),
                "an unrecognized unary function name should return NAN "
                "(previously a latent bug: it silently returned 0)");
    ASSERT_TRUE(calculatorGetLastError() == CALC_ERR_INVALID_FUNCTION,
                "should record CALC_ERR_INVALID_FUNCTION");

    /* And, just as importantly: the test process is still alive to
       check these results at all, which is the entire point. */
}

void run_functions_tests(void)
{
    test_unary_functions();
    test_trig_respects_angle_mode();
    test_binary_functions();
    test_factorial();
    test_functions_error_paths();
}
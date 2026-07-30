#include "test_framework.h"
#include "tests.h"
#include "functions.h"
#include "angle_mode.h"

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

static void factorialOfNegativeCall(void)
{
    factorial(-3); /* should exit(EXIT_FAILURE): factorial is undefined for negatives */
}

static void factorialOfNonIntegerCall(void)
{
    factorial(2.5); /* should exit(EXIT_FAILURE): factorial requires an integer */
}

static void unknownBinaryFunctionCall(void)
{
    applyBinaryFunction("notarealfunction", 1.0, 2.0); /* should exit(EXIT_FAILURE) */
}

static void test_functions_error_paths(void)
{
    ASSERT_EXITS_NONZERO(factorialOfNegativeCall(), "factorial(-3) should fail loudly, not return a wrong value");
    ASSERT_EXITS_NONZERO(factorialOfNonIntegerCall(), "factorial(2.5) should fail loudly, not silently truncate");
    ASSERT_EXITS_NONZERO(unknownBinaryFunctionCall(), "an unrecognized binary function name should fail loudly");
}

void run_functions_tests(void)
{
    test_unary_functions();
    test_trig_respects_angle_mode();
    test_binary_functions();
    test_factorial();
    test_functions_error_paths();
}
/* evaluateComplexExpression() (complex_eval.h) is an extern "C"
   entry point specifically so plain C code -- including this test
   file -- can call into the C++ complex-number engine (Complex.cpp,
   complex_eval.cpp) without itself being compiled as C++. */
#include "test_framework.h"
#include "tests.h"
#include "complex_eval.h"

static void test_complex_arithmetic(void)
{
    char result[128];

    ASSERT_TRUE(evaluateComplexExpression("(2+3i)*(4-5i)", result, sizeof(result)) == 1,
                "complex multiplication should succeed");
    ASSERT_STR_EQ(result, "23+2i", "(2+3i)*(4-5i) should be 23+2i");

    ASSERT_TRUE(evaluateComplexExpression("(3+4i)+(1+1i)", result, sizeof(result)) == 1,
                "complex addition should succeed");
    ASSERT_STR_EQ(result, "4+5i", "(3+4i)+(1+1i) should be 4+5i");

    ASSERT_TRUE(evaluateComplexExpression("(3+4i)-(1+1i)", result, sizeof(result)) == 1,
                "complex subtraction should succeed");
    ASSERT_STR_EQ(result, "2+3i", "(3+4i)-(1+1i) should be 2+3i");

    ASSERT_TRUE(evaluateComplexExpression("(4+2i)/2", result, sizeof(result)) == 1,
                "division by a real number should succeed");
    ASSERT_STR_EQ(result, "2+1i", "(4+2i)/2 should be 2+1i");

    ASSERT_TRUE(evaluateComplexExpression("(1-2i)", result, sizeof(result)) == 1,
                "a negative imaginary part should format with a '-'");
    ASSERT_STR_EQ(result, "1-2i", "(1-2i) should format as '1-2i'");

    ASSERT_TRUE(evaluateComplexExpression("(2+3i)-(2+3i)", result, sizeof(result)) == 1,
                "a purely real result should succeed");
    ASSERT_STR_EQ(result, "0", "a zero result should format as plain '0', not '0+0i'");
}

static void test_complex_sqrt(void)
{
    char result[128];

    ASSERT_TRUE(evaluateComplexExpression("sqrt(-1)", result, sizeof(result)) == 1, "sqrt(-1) should succeed");
    ASSERT_STR_EQ(result, "1i", "sqrt(-1) should be 1i (i.e. i)");

    ASSERT_TRUE(evaluateComplexExpression("sqrt(4)", result, sizeof(result)) == 1,
                "sqrt() of a positive real should succeed");
    ASSERT_STR_EQ(result, "2", "sqrt(4) should be plain '2'");
}

static void test_complex_error_paths(void)
{
    char result[128];

    ASSERT_TRUE(evaluateComplexExpression("(2+3i", result, sizeof(result)) == 0,
                "a missing closing paren should fail");
    ASSERT_STR_EQ(result, "Mismatched parentheses", "should report mismatched parentheses");

    ASSERT_TRUE(evaluateComplexExpression("foo(2)", result, sizeof(result)) == 0,
                "an unknown identifier should fail");
    ASSERT_STR_EQ(result, "Unknown identifier 'foo'", "should name the unknown identifier");

    ASSERT_TRUE(evaluateComplexExpression("(2+3i))", result, sizeof(result)) == 0,
                "trailing unmatched characters should fail");
    ASSERT_STR_EQ(result, "Unexpected character ')'", "should report the unexpected trailing character");

    ASSERT_TRUE(evaluateComplexExpression("", result, sizeof(result)) == 0, "an empty expression should fail");
    ASSERT_STR_EQ(result, "Operand expected", "should report a missing operand");
}

void run_complex_tests(void)
{
    test_complex_arithmetic();
    test_complex_sqrt();
    test_complex_error_paths();
}

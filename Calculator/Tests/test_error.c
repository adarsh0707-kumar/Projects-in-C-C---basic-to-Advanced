#include "../Inc/error.h"
#include "test_framework.h"

static void test_error_state(void)
{
    calculatorClearError();
    ASSERT_INT_EQ(calculatorGetLastError(), CALC_OK, "cleared error should read as CALC_OK");

    calculatorSetLastError(CALC_ERR_DIVIDE_BY_ZERO);
    ASSERT_INT_EQ(calculatorGetLastError(), CALC_ERR_DIVIDE_BY_ZERO, "should report the last error set");

    calculatorSetLastError(CALC_ERR_DOMAIN);
    ASSERT_INT_EQ(calculatorGetLastError(), CALC_ERR_DOMAIN,
                  "setting a new error should overwrite the previous one");

    calculatorClearError();
    ASSERT_INT_EQ(calculatorGetLastError(), CALC_OK, "clearing again should return to CALC_OK");
}

static void test_error_string(void)
{
    ASSERT_STR_EQ(calculatorErrorString(CALC_OK), "No error", "CALC_OK string");
    ASSERT_STR_EQ(calculatorErrorString(CALC_ERR_DIVIDE_BY_ZERO), "Division by zero",
                  "CALC_ERR_DIVIDE_BY_ZERO string");
    ASSERT_STR_EQ(calculatorErrorString(CALC_ERR_DOMAIN), "Value outside the valid domain",
                  "CALC_ERR_DOMAIN string");
    ASSERT_STR_EQ(calculatorErrorString(CALC_ERR_INVALID_TOKEN), "Invalid token",
                  "CALC_ERR_INVALID_TOKEN string");
    ASSERT_STR_EQ(calculatorErrorString(CALC_ERR_INVALID_EXPRESSION), "Invalid expression",
                  "CALC_ERR_INVALID_EXPRESSION string");
    ASSERT_STR_EQ(calculatorErrorString(CALC_ERR_INVALID_FUNCTION), "Unknown function",
                  "CALC_ERR_INVALID_FUNCTION string");
    ASSERT_STR_EQ(calculatorErrorString(CALC_ERR_INVALID_VARIABLE), "Unknown variable",
                  "CALC_ERR_INVALID_VARIABLE string");
    ASSERT_STR_EQ(calculatorErrorString(CALC_ERR_STACK_OVERFLOW), "Stack overflow",
                  "CALC_ERR_STACK_OVERFLOW string");
    ASSERT_STR_EQ(calculatorErrorString(CALC_ERR_STACK_UNDERFLOW), "Stack underflow",
                  "CALC_ERR_STACK_UNDERFLOW string");
    ASSERT_STR_EQ(calculatorErrorString(CALC_ERR_INTERNAL), "Internal error",
                  "CALC_ERR_INTERNAL string");
}

void run_error_tests(void)
{
    test_error_state();
    test_error_string();
}

#include "../Inc/error.h"
#include "test_framework.h"

void test_error_framework(void)
{
    calculatorClearError();

    ASSERT_EQ(CALC_OK, calculatorGetLastError());

    calculatorSetLastError(CALC_ERR_MEMORY);

    ASSERT_EQ(CALC_ERR_MEMORY, calculatorGetLastError());

    ASSERT_STR_EQ(
        "Memory allocation failed",
        calculatorErrorString(CALC_ERR_MEMORY));

    calculatorClearError();

    ASSERT_EQ(CALC_OK, calculatorGetLastError());
}
#include "test_framework.h"
#include "tests.h"
#include "calculator.h"

static void test_read_simple_integer(void)
{
    char expr[] = "42+3";
    int index = 0;

    double value = readNumber(expr, &index);

    ASSERT_DOUBLE_EQ(value, 42.0, "should parse '42' as 42.0");
    ASSERT_INT_EQ(index, 2, "index should advance past the digits, stopping at '+'");
}

static void test_read_decimal(void)
{
    char expr[] = "3.14*2";
    int index = 0;

    double value = readNumber(expr, &index);

    ASSERT_DOUBLE_EQ(value, 3.14, "should parse '3.14' as 3.14");
    ASSERT_INT_EQ(index, 4, "index should stop right after the decimal digits");
}

static void test_read_negative_number(void)
{
    char expr[] = "-7.5+1";
    int index = 0;

    double value = readNumber(expr, &index);

    ASSERT_DOUBLE_EQ(value, -7.5, "should parse a leading '-' as part of the number");
    ASSERT_INT_EQ(index, 4, "index should stop right after the number, at '+'");
}

static void test_read_whole_string_number(void)
{
    char expr[] = "100";
    int index = 0;

    double value = readNumber(expr, &index);

    ASSERT_DOUBLE_EQ(value, 100.0, "should parse a number that spans the entire string");
    ASSERT_INT_EQ(index, 3, "index should land on the terminating '\\0'");
}

void run_tokenizer_tests(void)
{
    test_read_simple_integer();
    test_read_decimal();
    test_read_negative_number();
    test_read_whole_string_number();
}
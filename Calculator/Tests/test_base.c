#include "test_framework.h"
#include "tests.h"
#include "base.h"

static void test_decimal_to_base(void)
{
    char result[64];

    ASSERT_TRUE(evaluateBaseExpression("bin(25)", result, sizeof(result)) == 1, "bin(25) should succeed");
    ASSERT_STR_EQ(result, "11001", "bin(25) should be '11001'");

    ASSERT_TRUE(evaluateBaseExpression("hex(255)", result, sizeof(result)) == 1, "hex(255) should succeed");
    ASSERT_STR_EQ(result, "FF", "hex(255) should be 'FF'");

    ASSERT_TRUE(evaluateBaseExpression("oct(64)", result, sizeof(result)) == 1, "oct(64) should succeed");
    ASSERT_STR_EQ(result, "100", "oct(64) should be '100'");

    ASSERT_TRUE(evaluateBaseExpression("bin(0)", result, sizeof(result)) == 1, "bin(0) should succeed");
    ASSERT_STR_EQ(result, "0", "bin(0) should be '0'");

    ASSERT_TRUE(evaluateBaseExpression("hex(-255)", result, sizeof(result)) == 1,
                "a negative value should succeed (sign-magnitude output)");
    ASSERT_STR_EQ(result, "-FF", "hex(-255) should be '-FF'");
}

static void test_base_to_decimal(void)
{
    char result[64];

    ASSERT_TRUE(evaluateBaseExpression("dec(1111b)", result, sizeof(result)) == 1, "dec(1111b) should succeed");
    ASSERT_STR_EQ(result, "15", "dec(1111b) should be '15'");

    ASSERT_TRUE(evaluateBaseExpression("dec(377o)", result, sizeof(result)) == 1, "dec(377o) should succeed");
    ASSERT_STR_EQ(result, "255", "dec(377o) should be '255'");

    ASSERT_TRUE(evaluateBaseExpression("dec(FFh)", result, sizeof(result)) == 1, "dec(FFh) should succeed");
    ASSERT_STR_EQ(result, "255", "dec(FFh) should be '255'");

    ASSERT_TRUE(evaluateBaseExpression("dec(-1111b)", result, sizeof(result)) == 1,
                "a leading '-' before the digits should negate the decoded value");
    ASSERT_STR_EQ(result, "-15", "dec(-1111b) should be '-15'");
}

static void test_base_error_paths(void)
{
    char result[64];

    /* bin()/hex()/oct() take a plain DECIMAL integer as input (they
       convert TO that base, not validate digits already in it) --
       bin(12) legitimately succeeds as "1100". What actually fails
       is a non-digit character anywhere in that decimal argument. */
    ASSERT_TRUE(evaluateBaseExpression("bin(12a)", result, sizeof(result)) == 0,
                "bin() with a non-digit character in its decimal argument should fail");
    ASSERT_TRUE(evaluateBaseExpression("dec(129b)", result, sizeof(result)) == 0,
                "dec() with a digit outside the given base ('9' in base 2) should fail");
    ASSERT_TRUE(evaluateBaseExpression("dec(15)", result, sizeof(result)) == 0,
                "dec() with no base suffix should fail");
    ASSERT_TRUE(evaluateBaseExpression("dec(15x)", result, sizeof(result)) == 0,
                "dec() with an unrecognized suffix should fail");
    ASSERT_TRUE(evaluateBaseExpression("bin()", result, sizeof(result)) == 0,
                "a call with no argument should fail");
    ASSERT_TRUE(evaluateBaseExpression("nope(1)", result, sizeof(result)) == 0,
                "an unknown base function name should fail");
    ASSERT_TRUE(evaluateBaseExpression("bin(25", result, sizeof(result)) == 0,
                "a missing closing paren should fail");
}

void run_base_tests(void)
{
    test_decimal_to_base();
    test_base_to_decimal();
    test_base_error_paths();
}

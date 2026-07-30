#include "test_framework.h"
#include "tests.h"
#include "calculator.h"

static void assertImplicitMul(const char *input, const char *expected, const char *msg)
{
    char in[128];
    char out[256];

    strcpy(in, input);
    insertImplicitMultiplication(in, out);

    ASSERT_STR_EQ(out, expected, msg);
}

static void test_insert_implicit_multiplication(void)
{
    assertImplicitMul("2x", "2*x", "digit directly followed by a variable should get an inserted '*'");
    assertImplicitMul("3(4)", "3*(4)", "digit directly followed by '(' should get an inserted '*'");
    assertImplicitMul("(2)(3)", "(2)*(3)", "')' directly followed by '(' should get an inserted '*'");
    assertImplicitMul("(2)3", "(2)*3", "')' directly followed by a digit should get an inserted '*'");
    assertImplicitMul("2sin(3)", "2*sin(3)", "digit directly followed by a function name should get an inserted '*'");
    assertImplicitMul("2+3", "2+3", "an expression with no adjacent operand/paren pairs should be unchanged");

    /* This is the specific case the identifier-protection check exists for:
       the '2' in "atan2" must NOT be treated as a standalone number. */
    assertImplicitMul("atan2(3,4)", "atan2(3,4)",
                      "a digit that's part of a function name (atan2) must not get a '*' inserted before '('");
}

static void test_validate_parentheses(void)
{
    char e1[] = "(())";
    ASSERT_TRUE(validateParentheses(e1) == 1, "balanced nested parentheses should be valid");

    char e2[] = "(2+3)*(4-1)";
    ASSERT_TRUE(validateParentheses(e2) == 1, "two separate balanced groups should be valid");

    char e3[] = "(()";
    ASSERT_TRUE(validateParentheses(e3) == 0, "an unclosed '(' should be invalid");

    char e4[] = ")(";
    ASSERT_TRUE(validateParentheses(e4) == 0, "a ')' with nothing open yet should be invalid");

    char e5[] = "2+3";
    ASSERT_TRUE(validateParentheses(e5) == 1, "an expression with no parentheses at all is trivially balanced");
}

void run_parser_tests(void)
{
    test_insert_implicit_multiplication();
    test_validate_parentheses();
}
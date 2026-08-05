#include "test_framework.h"
#include "tests.h"
#include "calculator.h"
#include "stack.h" /* MAX -- the CharStack depth validateParentheses() can track */

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

/* validateParentheses() reports its failure reason through a
   caller-supplied buffer, so every call here needs one. */
static char err[128];

static void test_validate_parentheses(void)
{
    char e1[] = "(())";
    ASSERT_TRUE(validateParentheses(e1, err, sizeof(err)) == 1, "balanced nested parentheses should be valid");

    char e2[] = "(2+3)*(4-1)";
    ASSERT_TRUE(validateParentheses(e2, err, sizeof(err)) == 1, "two separate balanced groups should be valid");

    char e3[] = "(()";
    ASSERT_TRUE(validateParentheses(e3, err, sizeof(err)) == 0, "an unclosed '(' should be invalid");

    char e4[] = ")(";
    ASSERT_TRUE(validateParentheses(e4, err, sizeof(err)) == 0, "a ')' with nothing open yet should be invalid");

    char e5[] = "2+3";
    ASSERT_TRUE(validateParentheses(e5, err, sizeof(err)) == 1, "an expression with no parentheses at all is trivially balanced");
}

/*
 * The three ways parentheses can fail used to be indistinguishable to a
 * caller -- all three just returned 0, and every caller printed the same
 * hardcoded "Mismatched parentheses." These pin the specific message,
 * which is what the CLI and the Qt GUI now display.
 */
static void test_validate_parentheses_messages(void)
{
    char e1[] = "(()";
    validateParentheses(e1, err, sizeof(err));
    ASSERT_STR_EQ(err, "Error: Unclosed '('.", "a '(' left open should say so");

    char e2[] = ")(";
    validateParentheses(e2, err, sizeof(err));
    ASSERT_STR_EQ(err, "Error: Too many ')'.",
                  "a ')' with nothing open should be distinguished from an unclosed '('");

    char e3[] = "(2+3)";
    validateParentheses(e3, err, sizeof(err));
    ASSERT_STR_EQ(err, "", "balanced parentheses should leave the error buffer empty");

    /* One '(' deeper than the CharStack (MAX) can hold. This is the
       path that gives up mid-walk rather than exiting the process. */
    char deep[MAX + 2];
    memset(deep, '(', MAX + 1);
    deep[MAX + 1] = '\0';

    ASSERT_TRUE(validateParentheses(deep, err, sizeof(err)) == 0, "nesting past the stack's capacity should be invalid");
    ASSERT_STR_EQ(err, "Error: Too many nested parentheses.",
                  "overflowing the stack should report the depth, not a generic mismatch");
}

void run_parser_tests(void)
{
    test_insert_implicit_multiplication();
    test_validate_parentheses();
    test_validate_parentheses_messages();
}
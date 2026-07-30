#include "test_framework.h"
#include "tests.h"
#include "calculator.h"
#include "variables.h"

static void assertPostfix(const char *input, const char *expected, const char *msg)
{
    char in[128];
    char out[256];

    strcpy(in, input);
    infixToPostfix(in, out);

    ASSERT_STR_EQ(out, expected, msg);
}

static void test_infix_to_postfix(void)
{
    assertPostfix("2+3*4", "2 3 4 * + ", "'*' should bind tighter than '+' (no parens needed)");
    assertPostfix("(2+3)*4", "2 3 + 4 * ", "parentheses should force '+' to happen before '*'");
    assertPostfix("pow(2,3)", "2 3 pow ", "a binary function's args should come out in order before the function name");
    assertPostfix("2^3", "2 3 ^ ", "'^' should be emitted as its own postfix operator");

    setVariable("x", 5.0);
    assertPostfix("x+1", "5 1 + ", "a variable should be substituted with its current numeric value");
}

static void test_operator_helpers(void)
{
    ASSERT_TRUE(isOperator('+') == 1, "'+' should be recognized as an operator");
    ASSERT_TRUE(isOperator('^') == 1, "'^' should be recognized as an operator");
    ASSERT_TRUE(isOperator('!') == 1, "'!' (factorial) should be recognized as an operator");
    ASSERT_TRUE(isOperator('x') == 0, "a letter should not be recognized as an operator");

    ASSERT_TRUE(precedence('^') > precedence('*'), "'^' should bind tighter than '*'");
    ASSERT_TRUE(precedence('*') > precedence('+'), "'*' should bind tighter than '+'");
    ASSERT_TRUE(precedence('!') > precedence('^'), "postfix '!' should bind tightest of all");
}

static void test_evaluate_postfix(void)
{
    char postfix1[] = "2 3 4 * + ";
    ASSERT_DOUBLE_EQ(evaluatePostfix(postfix1), 14.0, "2+3*4 should evaluate to 14");

    char postfix2[] = "2 3 + 4 * ";
    ASSERT_DOUBLE_EQ(evaluatePostfix(postfix2), 20.0, "(2+3)*4 should evaluate to 20");

    char postfix3[] = "2 3 pow ";
    ASSERT_DOUBLE_EQ(evaluatePostfix(postfix3), 8.0, "pow(2,3) should evaluate to 8");
}

void run_postfix_tests(void)
{
    test_infix_to_postfix();
    test_operator_helpers();
    test_evaluate_postfix();
}
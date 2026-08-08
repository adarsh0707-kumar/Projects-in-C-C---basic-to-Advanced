#include "test_framework.h"
#include "tests.h"
#include "calculator.h"
#include "variables.h"
#include "error.h"

#include <math.h>

static void test_substituted_values_keep_full_precision(void);

static void assertPostfix(const char *input, const char *expected, const char *msg)
{
    char in[128];
    char out[256];

    strcpy(in, input);

    ASSERT_TRUE(infixToPostfix(in, out), "infixToPostfix should succeed on a valid expression");
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

static void test_infix_to_postfix_error_paths(void)
{
    char in[128];
    char out[256];

    /* As of the Phase C error-handling migration, infixToPostfix()
       reports failure via its return value instead of exit()ing --
       see docs/CHANGELOG.md. */
    strcpy(in, "totallyUndefinedVariable+1");
    calculatorClearError();
    ASSERT_TRUE(!infixToPostfix(in, out),
                "an undefined variable should fail, not silently substitute a wrong value");
    ASSERT_TRUE(calculatorGetLastError() == CALC_ERR_INVALID_VARIABLE,
                "should record CALC_ERR_INVALID_VARIABLE");

    strcpy(in, "1.2.3+1");
    calculatorClearError();
    ASSERT_TRUE(!infixToPostfix(in, out),
                "a malformed number (two decimal points) should fail, not silently truncate");
    ASSERT_TRUE(calculatorGetLastError() == CALC_ERR_INVALID_TOKEN,
                "should record CALC_ERR_INVALID_TOKEN");
}

/*
 * A surplus ')' was always rejected, but an unclosed '(' used to be
 * dropped silently and reported as success -- "(2+3" came back as
 * "2 3 +" -- contradicting Inc/calculator.h's documented 0 return for
 * mismatched parentheses. Nothing user-facing depended on it, since
 * validateExpression() rejects such input earlier in the pipeline, but
 * a direct caller of infixToPostfix() was misled.
 */
static void test_infix_to_postfix_unbalanced_parentheses(void)
{
    char in[128];
    char out[256];

    const char *unclosed[] = {"(2+3", "sin(2", "(()", "(", "((1+2)"};

    for (unsigned k = 0; k < sizeof(unclosed) / sizeof(unclosed[0]); k++)
    {
        strcpy(in, unclosed[k]);
        calculatorClearError();
        ASSERT_TRUE(!infixToPostfix(in, out),
                    "an unclosed '(' should fail, not silently drop the parenthesis");
        ASSERT_TRUE(calculatorGetLastError() == CALC_ERR_INVALID_EXPRESSION,
                    "an unclosed '(' should record CALC_ERR_INVALID_EXPRESSION");
    }

    /* The opposite direction was already correct; pinned so both stay
       consistent with each other. */
    const char *surplus[] = {"2+3)", ")(", ")"};

    for (unsigned k = 0; k < sizeof(surplus) / sizeof(surplus[0]); k++)
    {
        strcpy(in, surplus[k]);
        calculatorClearError();
        ASSERT_TRUE(!infixToPostfix(in, out),
                    "a surplus ')' should fail");
    }

    /* Balanced expressions must still convert, so the new check can't
       be over-eager. */
    strcpy(in, "((1+2)*3)");
    calculatorClearError();
    ASSERT_TRUE(infixToPostfix(in, out),
                "fully balanced nested parentheses should still succeed");
    ASSERT_DOUBLE_EQ(evaluatePostfix(out), 9.0,
                     "((1+2)*3) should still evaluate to 9");
}

void run_postfix_tests(void)
{
    test_infix_to_postfix();
    test_infix_to_postfix_error_paths();
    test_infix_to_postfix_unbalanced_parentheses();
    test_operator_helpers();
    test_evaluate_postfix();
    test_substituted_values_keep_full_precision();
}
/*
 * Precision of substituted constants and variables.
 *
 * The postfix form is text, so every constant and variable is printed into
 * it and parsed back out. Printing with %g quantised each one to six
 * significant figures, and none of the 486 tests noticed: the suite checked
 * that getVariable("pi") returned the right double -- which it always did --
 * and never evaluated the expression "pi". Coverage was what pointed here,
 * by reporting constants.c as entirely unexecuted while the calculator
 * plainly understood pi.
 *
 * Every assertion below fails against the %g version.
 */
static double evaluateInfix(const char *expression)
{
    char in[256];
    char out[512];

    strcpy(in, expression);

    if (!infixToPostfix(in, out))
        return NAN;

    return evaluatePostfix(out);
}

static void test_substituted_values_keep_full_precision(void)
{
    calculatorClearError();

    /* The constant itself, to the last bit a double can hold. */
    ASSERT_DOUBLE_NEAR(evaluateInfix("pi"), 3.14159265358979323846, 1e-15,
                       "the expression 'pi' should evaluate to full precision");
    ASSERT_DOUBLE_NEAR(evaluateInfix("e"), 2.71828182845904523536, 1e-15,
                       "the expression 'e' should evaluate to full precision");

    /*
    Amplified, so the failure is unmistakable rather than a rounding
    quibble: with pi truncated to 3.14159 this returns -2.
    */
    ASSERT_DOUBLE_NEAR(evaluateInfix("pi*1000000-3141592"),
                       0.65358979301527, 1e-6,
                       "pi scaled up should still carry its later digits");

    /*
    sin(pi) is the classic probe. Correct to a double it is ~1.2e-16;
    with a six-figure pi it comes back as 2.65e-06, ten orders of
    magnitude adrift.
    */
    ASSERT_DOUBLE_NEAR(evaluateInfix("sin(pi)"), 0.0, 1e-12,
                       "sin(pi) should be zero to within double precision");

    /* A variable must survive the round trip through the postfix text. */
    setVariable("third", 1.0 / 3.0);

    ASSERT_DOUBLE_NEAR(evaluateInfix("third*3"), 1.0, 1e-15,
                       "a variable holding 1/3, tripled, should give exactly 1");

    /* And a value with digits well past the sixth. */
    setVariable("precise", 1.2345678901234567);

    ASSERT_DOUBLE_NEAR(evaluateInfix("precise"), 1.2345678901234567, 1e-15,
                       "a variable should evaluate to the value it was set to");

    /*
    ans is a variable too, so every chained calculation went through the
    same truncation -- the most user-visible form of this defect.
    */
    setAns(1.0 / 3.0);

    ASSERT_DOUBLE_NEAR(evaluateInfix("ans*3"), 1.0, 1e-15,
                       "chaining through 'ans' should not lose precision");
}

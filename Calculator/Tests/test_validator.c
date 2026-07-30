#include "test_framework.h"
#include "tests.h"
#include "validator.h"

static void test_valid_expressions(void)
{
    char e1[] = "2+3*4";
    ASSERT_TRUE(validateExpression(e1) == 1, "'2+3*4' should be valid");

    char e2[] = "sin(3.14)";
    ASSERT_TRUE(validateExpression(e2) == 1, "'sin(3.14)' should be valid");

    char e3[] = "pow(2,3)";
    ASSERT_TRUE(validateExpression(e3) == 1, "'pow(2,3)' (correct arg count) should be valid");

    char e4[] = "(2+3)*4";
    ASSERT_TRUE(validateExpression(e4) == 1, "balanced parentheses with valid content should be valid");

    char e5[] = "5!+3";
    ASSERT_TRUE(validateExpression(e5) == 1, "postfix factorial followed by an operator should be valid");

    char e6[] = "x+1";
    ASSERT_TRUE(validateExpression(e6) == 1,
                "an identifier used as a variable should be syntactically valid "
                "(validateExpression checks syntax only, not whether 'x' is defined)");
}

static void test_invalid_expressions(void)
{
    char e1[] = "2+";
    ASSERT_TRUE(validateExpression(e1) == 0, "an expression can't end with an operator");

    char e2[] = "+2";
    ASSERT_TRUE(validateExpression(e2) == 0, "an expression can't start with a binary '+'");

    char e3[] = "sin(2,3)";
    ASSERT_TRUE(validateExpression(e3) == 0, "sin() takes 1 argument, not 2");

    char e4[] = "(2+3";
    ASSERT_TRUE(validateExpression(e4) == 0, "unclosed '(' should be invalid");

    char e5[] = "2,3";
    ASSERT_TRUE(validateExpression(e5) == 0, "a comma outside of a function call should be invalid");

    char e6[] = "2 3";
    ASSERT_TRUE(validateExpression(e6) == 0, "two operands with no operator between them should be invalid");
}

void run_validator_tests(void)
{
    test_valid_expressions();
    test_invalid_expressions();
}
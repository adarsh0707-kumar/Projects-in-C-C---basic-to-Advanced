#include "test_framework.h"
#include "tests.h"
#include "validator.h"

/* validateExpression() reports diagnostics through a caller-supplied
   buffer rather than printing them, so every call here needs one. */
static char err[128];

static void test_valid_expressions(void)
{
    char e1[] = "2+3*4";
    ASSERT_TRUE(validateExpression(e1, err, sizeof(err)) == 1, "'2+3*4' should be valid");

    char e2[] = "sin(3.14)";
    ASSERT_TRUE(validateExpression(e2, err, sizeof(err)) == 1, "'sin(3.14)' should be valid");

    char e3[] = "pow(2,3)";
    ASSERT_TRUE(validateExpression(e3, err, sizeof(err)) == 1, "'pow(2,3)' (correct arg count) should be valid");

    char e4[] = "(2+3)*4";
    ASSERT_TRUE(validateExpression(e4, err, sizeof(err)) == 1, "balanced parentheses with valid content should be valid");

    char e5[] = "5!+3";
    ASSERT_TRUE(validateExpression(e5, err, sizeof(err)) == 1, "postfix factorial followed by an operator should be valid");

    char e6[] = "x+1";
    ASSERT_TRUE(validateExpression(e6, err, sizeof(err)) == 1,
                "an identifier used as a variable should be syntactically valid "
                "(validateExpression checks syntax only, not whether 'x' is defined)");
}

static void test_invalid_expressions(void)
{
    char e1[] = "2+";
    ASSERT_TRUE(validateExpression(e1, err, sizeof(err)) == 0, "an expression can't end with an operator");

    char e2[] = "+2";
    ASSERT_TRUE(validateExpression(e2, err, sizeof(err)) == 0, "an expression can't start with a binary '+'");

    char e3[] = "sin(2,3)";
    ASSERT_TRUE(validateExpression(e3, err, sizeof(err)) == 0, "sin() takes 1 argument, not 2");

    char e4[] = "(2+3";
    ASSERT_TRUE(validateExpression(e4, err, sizeof(err)) == 0, "unclosed '(' should be invalid");

    char e5[] = "2,3";
    ASSERT_TRUE(validateExpression(e5, err, sizeof(err)) == 0, "a comma outside of a function call should be invalid");

    char e6[] = "2 3";
    ASSERT_TRUE(validateExpression(e6, err, sizeof(err)) == 0, "two operands with no operator between them should be invalid");
}

/*
 * The diagnostic used to go straight to stdout, which meant the Qt GUI
 * could only ever show a generic "Error: Invalid expression." These
 * check the message actually reaches the caller's buffer, since that's
 * the entire point of the out-parameter.
 */
static void test_error_messages(void)
{
    char e1[] = "2+";
    validateExpression(e1, err, sizeof(err));
    ASSERT_STR_EQ(err, "Error: Expression cannot end with an operator.",
                  "a trailing operator should report its own specific message");

    char e2[] = "2 3";
    validateExpression(e2, err, sizeof(err));
    ASSERT_STR_EQ(err, "Error: Operator expected.",
                  "two adjacent operands should report a missing operator");

    char e3[] = "sin(2,3)";
    validateExpression(e3, err, sizeof(err));
    ASSERT_STR_EQ(err, "Error: Function 'sin' expects 1 argument(s), got 2.",
                  "an arity mismatch should name the function and both counts");

    char e4[] = "2+3";
    validateExpression(e4, err, sizeof(err));
    ASSERT_STR_EQ(err, "", "a valid expression should leave the error buffer empty");

    /* A caller passing a buffer too small for the message must still get
       a terminated string back, not a truncated-and-unterminated one. */
    char tiny[8];
    char e5[] = "sin(2,3)";
    ASSERT_TRUE(validateExpression(e5, tiny, sizeof(tiny)) == 0,
                "a short error buffer shouldn't change the return value");
    ASSERT_TRUE(strlen(tiny) < sizeof(tiny), "the message must stay null-terminated when truncated");
}

void run_validator_tests(void)
{
    test_valid_expressions();
    test_invalid_expressions();
    test_error_messages();
}

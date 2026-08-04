#include "test_framework.h"
#include "tests.h"
#include "plot.h"
#include <string.h>

/* evaluatePlotExpression() prints the actual ASCII graph straight to
   stdout on success (see plot.h) -- there's no returned buffer to
   assert its content against, so these tests focus on the return
   value and the syntax-error paths that do fill errorMsg. Letting the
   graph print during a test run matches how the rest of this suite
   handles other stdout-printing functions (e.g. memory.c's tests). */

static void test_plot_success(void)
{
    char errorMsg[128];

    ASSERT_TRUE(evaluatePlotExpression("plot(sin(x))", errorMsg, sizeof(errorMsg)) == 1,
                "a well-formed plot expression should succeed");
    ASSERT_STR_EQ(errorMsg, "", "errorMsg should be untouched on success");

    ASSERT_TRUE(evaluatePlotExpression("  plot( x^2 )  ", errorMsg, sizeof(errorMsg)) == 1,
                "surrounding whitespace should be tolerated");
}

static void test_plot_asymptote_does_not_crash(void)
{
    char errorMsg[128];

    /* This is the exact regression this project's docs call out by
       name (see docs/CHANGELOG.md, docs/ERROR_HANDLING.md): plot(1/x)
       samples x=0 and must render that column as a gap, not crash the
       whole process. Simply returning at all (rather than aborting)
       is the assertion. */
    ASSERT_TRUE(evaluatePlotExpression("plot(1/x)", errorMsg, sizeof(errorMsg)) == 1,
                "plot(1/x) should succeed overall despite the x=0 asymptote");
}

static void test_plot_syntax_error_paths(void)
{
    char errorMsg[128];

    ASSERT_TRUE(evaluatePlotExpression("sin(x)", errorMsg, sizeof(errorMsg)) == 0,
                "input without a 'plot(' prefix should be rejected");
    ASSERT_STR_EQ(errorMsg, "Error: Use 'plot(<expression in x>)'.", "should explain the required form");

    ASSERT_TRUE(evaluatePlotExpression("plot(sin(x)", errorMsg, sizeof(errorMsg)) == 0,
                "a missing closing ')' should be rejected");
    ASSERT_STR_EQ(errorMsg, "Error: Missing closing ')' in plot(...).", "should explain the missing paren");

    ASSERT_TRUE(evaluatePlotExpression("plot()", errorMsg, sizeof(errorMsg)) == 0,
                "an empty plot() should be rejected");
    ASSERT_STR_EQ(errorMsg, "Error: plot() requires an expression, e.g. plot(sin(x)).",
                  "should explain that an expression is required");

    ASSERT_TRUE(evaluatePlotExpression("plot(x) extra", errorMsg, sizeof(errorMsg)) == 0,
                "trailing characters after the closing ')' should be rejected");
    ASSERT_STR_EQ(errorMsg, "Error: Unexpected characters after plot(...).",
                  "should explain the unexpected trailing characters");
}

static void test_plot_inner_expression_error(void)
{
    char errorMsg[128];

    /* An undefined variable is a *semantic* failure, caught inside
       the per-sample infixToPostfix() call, not by the syntax-only
       checks above -- infixToPostfix() already prints its own
       message, so errorMsg is deliberately left empty here (see the
       comment in plot.c and plot.h). */
    ASSERT_TRUE(evaluatePlotExpression("plot(y)", errorMsg, sizeof(errorMsg)) == 0,
                "an undefined variable inside plot(...) should be rejected");
    ASSERT_STR_EQ(errorMsg, "",
                  "errorMsg should stay empty since infixToPostfix() already printed its own message");
}

void run_plot_tests(void)
{
    test_plot_success();
    test_plot_asymptote_does_not_crash();
    test_plot_syntax_error_paths();
    test_plot_inner_expression_error();
}

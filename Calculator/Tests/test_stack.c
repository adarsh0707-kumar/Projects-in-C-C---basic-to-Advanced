#include "test_framework.h"
#include "tests.h"
#include "stack.h"
#include "error.h"

static void test_double_stack_push_pop_order(void)
{
    DoubleStack s;
    initDoubleStack(&s);

    ASSERT_TRUE(isEmptyDoubleStack(&s), "fresh stack should be empty");

    ASSERT_TRUE(pushDouble(&s, 1.5), "push should succeed on a non-full stack");
    ASSERT_TRUE(pushDouble(&s, 2.5), "push should succeed on a non-full stack");
    ASSERT_TRUE(pushDouble(&s, 3.5), "push should succeed on a non-full stack");

    ASSERT_TRUE(!isEmptyDoubleStack(&s), "stack with items should not be empty");

    double peeked;
    ASSERT_TRUE(peekDouble(&s, &peeked), "peek should succeed on a non-empty stack");
    ASSERT_DOUBLE_EQ(peeked, 3.5, "peek should return the last pushed value");

    double a, b, c;
    ASSERT_TRUE(popDouble(&s, &a), "pop should succeed on a non-empty stack");
    ASSERT_TRUE(popDouble(&s, &b), "pop should succeed on a non-empty stack");
    ASSERT_TRUE(popDouble(&s, &c), "pop should succeed on a non-empty stack");

    ASSERT_DOUBLE_EQ(a, 3.5, "pop order should be LIFO (3.5 first)");
    ASSERT_DOUBLE_EQ(b, 2.5, "pop order should be LIFO (2.5 second)");
    ASSERT_DOUBLE_EQ(c, 1.5, "pop order should be LIFO (1.5 last)");

    ASSERT_TRUE(isEmptyDoubleStack(&s), "stack should be empty after popping everything");
}

/*
 * As of the Phase C error-handling migration, stack.c reports
 * overflow/underflow via a returned status code + calculatorSetLastError()
 * instead of exit()ing -- see docs/CHANGELOG.md. These tests replace the
 * old ASSERT_EXITS_NONZERO checks: a full, empty-or-full stack is no
 * longer able to crash the process, just report why it refused.
 */
static void test_double_stack_error_paths(void)
{
    DoubleStack s;
    initDoubleStack(&s);

    double value = 999.0; /* sentinel, to confirm it's left untouched */
    calculatorClearError();
    ASSERT_TRUE(!popDouble(&s, &value),
                "popping an empty DoubleStack should fail, not return garbage");
    ASSERT_TRUE(calculatorGetLastError() == CALC_ERR_STACK_UNDERFLOW,
                "underflow should record CALC_ERR_STACK_UNDERFLOW");
    ASSERT_DOUBLE_EQ(value, 999.0,
                     "a failed pop must not modify the output value");

    calculatorClearError();
    ASSERT_TRUE(!peekDouble(&s, &value),
                "peeking an empty DoubleStack should fail, not return garbage");
    ASSERT_TRUE(calculatorGetLastError() == CALC_ERR_STACK_UNDERFLOW,
                "underflow should record CALC_ERR_STACK_UNDERFLOW");

    for (int i = 0; i < MAX; i++)
    {
        ASSERT_TRUE(pushDouble(&s, (double)i),
                    "pushing up to MAX items should succeed");
    }

    calculatorClearError();
    ASSERT_TRUE(!pushDouble(&s, 12345.0),
                "pushing past MAX capacity should fail, not silently corrupt memory");
    ASSERT_TRUE(calculatorGetLastError() == CALC_ERR_STACK_OVERFLOW,
                "overflow should record CALC_ERR_STACK_OVERFLOW");
}

static void test_char_stack(void)
{
    CharStack s;
    initCharStack(&s);

    ASSERT_TRUE(isEmptyCharStack(&s), "fresh char stack should be empty");

    ASSERT_TRUE(pushChar(&s, '('), "push should succeed on a non-full stack");
    ASSERT_TRUE(pushChar(&s, '['), "push should succeed on a non-full stack");

    char peeked, first, second;
    ASSERT_TRUE(peekChar(&s, &peeked), "peek should succeed on a non-empty stack");
    ASSERT_TRUE(peeked == '[', "peek should return the last pushed char");

    ASSERT_TRUE(popChar(&s, &first), "pop should succeed on a non-empty stack");
    ASSERT_TRUE(first == '[', "pop order should be LIFO");

    ASSERT_TRUE(popChar(&s, &second), "pop should succeed on a non-empty stack");
    ASSERT_TRUE(second == '(', "pop order should be LIFO");

    ASSERT_TRUE(isEmptyCharStack(&s), "char stack should be empty after popping everything");

    calculatorClearError();
    char discarded;
    ASSERT_TRUE(!popChar(&s, &discarded), "popping an empty CharStack should fail");
    ASSERT_TRUE(calculatorGetLastError() == CALC_ERR_STACK_UNDERFLOW,
                "underflow should record CALC_ERR_STACK_UNDERFLOW");
}

static void test_string_stack(void)
{
    StringStack s;
    initStringStack(&s);

    ASSERT_TRUE(isEmptyStringStack(&s), "fresh string stack should be empty");

    ASSERT_TRUE(pushString(&s, "first"), "push should succeed on a non-full stack");
    ASSERT_TRUE(pushString(&s, "second"), "push should succeed on a non-full stack");

    char buf[32];
    ASSERT_TRUE(peekString(&s, buf, sizeof(buf)), "peek should succeed on a non-empty stack");
    ASSERT_STR_EQ(buf, "second", "peek should return the last pushed string");

    ASSERT_TRUE(popString(&s, buf, sizeof(buf)), "pop should succeed on a non-empty stack");
    ASSERT_STR_EQ(buf, "second", "pop order should be LIFO");

    ASSERT_TRUE(popString(&s, buf, sizeof(buf)), "pop should succeed on a non-empty stack");
    ASSERT_STR_EQ(buf, "first", "pop order should be LIFO");

    ASSERT_TRUE(isEmptyStringStack(&s), "string stack should be empty after popping everything");
}

static void test_token_stack(void)
{
    TokenStack s;
    initTokenStack(&s);

    ASSERT_TRUE(isEmptyTokenStack(&s), "fresh token stack should be empty");

    Token a = {TOKEN_OPERATOR, "+"};
    Token b = {TOKEN_FUNCTION, "sin"};

    ASSERT_TRUE(pushToken(&s, a), "push should succeed on a non-full stack");
    ASSERT_TRUE(pushToken(&s, b), "push should succeed on a non-full stack");

    Token top;
    ASSERT_TRUE(peekToken(&s, &top), "peek should succeed on a non-empty stack");
    ASSERT_TRUE(top.type == TOKEN_FUNCTION, "peek should return the last pushed token's type");
    ASSERT_STR_EQ(top.text, "sin", "peek should return the last pushed token's text");

    Token popped1, popped2;
    ASSERT_TRUE(popToken(&s, &popped1), "pop should succeed on a non-empty stack");
    ASSERT_STR_EQ(popped1.text, "sin", "pop order should be LIFO");

    ASSERT_TRUE(popToken(&s, &popped2), "pop should succeed on a non-empty stack");
    ASSERT_STR_EQ(popped2.text, "+", "pop order should be LIFO");

    ASSERT_TRUE(isEmptyTokenStack(&s), "token stack should be empty after popping everything");
}

void run_stack_tests(void)
{
    test_double_stack_push_pop_order();
    test_double_stack_error_paths();
    test_char_stack();
    test_string_stack();
    test_token_stack();
}

#include "test_framework.h"
#include "tests.h"
#include "stack.h"

static void test_double_stack_push_pop_order(void)
{
    DoubleStack s;
    initDoubleStack(&s);

    ASSERT_TRUE(isEmptyDoubleStack(&s), "fresh stack should be empty");

    pushDouble(&s, 1.5);
    pushDouble(&s, 2.5);
    pushDouble(&s, 3.5);

    ASSERT_TRUE(!isEmptyDoubleStack(&s), "stack with items should not be empty");
    ASSERT_DOUBLE_EQ(peekDouble(&s), 3.5, "peek should return the last pushed value");

    ASSERT_DOUBLE_EQ(popDouble(&s), 3.5, "pop order should be LIFO (3.5 first)");
    ASSERT_DOUBLE_EQ(popDouble(&s), 2.5, "pop order should be LIFO (2.5 second)");
    ASSERT_DOUBLE_EQ(popDouble(&s), 1.5, "pop order should be LIFO (1.5 last)");

    ASSERT_TRUE(isEmptyDoubleStack(&s), "stack should be empty after popping everything");
}

static void doubleStackUnderflowCall(void)
{
    DoubleStack s;
    initDoubleStack(&s);
    popDouble(&s); /* should exit(EXIT_FAILURE) -- nothing to pop */
}

static void doubleStackOverflowCall(void)
{
    DoubleStack s;
    initDoubleStack(&s);

    for (int i = 0; i < MAX + 1; i++)
        pushDouble(&s, (double)i); /* should exit(EXIT_FAILURE) on the (MAX+1)th push */
}

static void test_double_stack_error_paths(void)
{
    ASSERT_EXITS_NONZERO(doubleStackUnderflowCall(),
                         "popping an empty DoubleStack should fail loudly, not return garbage");
    ASSERT_EXITS_NONZERO(doubleStackOverflowCall(),
                         "pushing past MAX capacity should fail loudly, not silently corrupt memory");
}

static void test_char_stack(void)
{
    CharStack s;
    initCharStack(&s);

    ASSERT_TRUE(isEmptyCharStack(&s), "fresh char stack should be empty");

    pushChar(&s, '(');
    pushChar(&s, '[');

    ASSERT_TRUE(peekChar(&s) == '[', "peek should return the last pushed char");
    ASSERT_TRUE(popChar(&s) == '[', "pop order should be LIFO");
    ASSERT_TRUE(popChar(&s) == '(', "pop order should be LIFO");
    ASSERT_TRUE(isEmptyCharStack(&s), "char stack should be empty after popping everything");
}

static void test_string_stack(void)
{
    StringStack s;
    initStringStack(&s);

    ASSERT_TRUE(isEmptyStringStack(&s), "fresh string stack should be empty");

    pushString(&s, "first");
    pushString(&s, "second");

    ASSERT_STR_EQ(peekString(&s), "second", "peek should return the last pushed string");
    ASSERT_STR_EQ(popString(&s), "second", "pop order should be LIFO");
    ASSERT_STR_EQ(popString(&s), "first", "pop order should be LIFO");
    ASSERT_TRUE(isEmptyStringStack(&s), "string stack should be empty after popping everything");
}

static void test_token_stack(void)
{
    TokenStack s;
    initTokenStack(&s);

    ASSERT_TRUE(isEmptyTokenStack(&s), "fresh token stack should be empty");

    Token a = {TOKEN_OPERATOR, "+"};
    Token b = {TOKEN_FUNCTION, "sin"};

    pushToken(&s, a);
    pushToken(&s, b);

    Token top = peekToken(&s);
    ASSERT_TRUE(top.type == TOKEN_FUNCTION, "peek should return the last pushed token's type");
    ASSERT_STR_EQ(top.text, "sin", "peek should return the last pushed token's text");

    Token popped1 = popToken(&s);
    ASSERT_STR_EQ(popped1.text, "sin", "pop order should be LIFO");

    Token popped2 = popToken(&s);
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
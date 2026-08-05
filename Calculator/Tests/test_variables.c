#include "test_framework.h"
#include "tests.h"
#include "variables.h"

static void test_builtin_constants(void)
{
    double value;

    ASSERT_TRUE(getVariable("pi", &value) == 1, "'pi' should exist by default");
    ASSERT_DOUBLE_EQ(value, 3.14159265358979323846, "'pi' should hold the correct value");

    ASSERT_TRUE(getVariable("e", &value) == 1, "'e' should exist by default");
    ASSERT_DOUBLE_EQ(value, 2.71828182845904523536, "'e' should hold the correct value");
}

static void test_constants_are_read_only(void)
{
    double before, after;
    getVariable("pi", &before);

    int result = setVariable("pi", 3.0);

    ASSERT_TRUE(result == 0, "setVariable() should refuse to overwrite 'pi'");

    getVariable("pi", &after);
    ASSERT_DOUBLE_EQ(after, before, "'pi' should be unchanged after the rejected write");
}

static void test_set_and_get_user_variable(void)
{
    double value;

    int result = setVariable("testVarA", 42.0);
    ASSERT_TRUE(result == 1, "setVariable() should succeed for a new, non-reserved name");

    ASSERT_TRUE(getVariable("testVarA", &value) == 1, "the newly set variable should be retrievable");
    ASSERT_DOUBLE_EQ(value, 42.0, "the retrieved value should match what was set");

    /* Updating an existing variable should overwrite, not create a duplicate */
    setVariable("testVarA", 99.0);
    getVariable("testVarA", &value);
    ASSERT_DOUBLE_EQ(value, 99.0, "setting an existing variable again should update its value");
}

static void test_get_undefined_variable(void)
{
    double value;
    int result = getVariable("thisVariableDoesNotExistAnywhere", &value);

    ASSERT_TRUE(result == 0, "getVariable() should return 0 for a name that was never set");
}

static void test_variable_enumeration(void)
{
    int countBefore = getVariableCount();

    ASSERT_TRUE(setVariable("testVarEnumOnly", 7.5) == 1, "setVariable() should succeed for a new name");

    int countAfter = getVariableCount();
    ASSERT_TRUE(countAfter == countBefore + 1, "getVariableCount() should increase by 1 after adding a new variable");

    /* setVariable() appends new names to the end of the internal
       table, so the new variable should be the last one enumerated. */
    char name[32];
    double value;
    ASSERT_TRUE(getVariableByIndex(countAfter - 1, name, sizeof(name), &value) == 1,
                "the last index should be retrievable");
    ASSERT_STR_EQ(name, "testVarEnumOnly", "the last-added variable should be the last one enumerated");
    ASSERT_DOUBLE_EQ(value, 7.5, "the enumerated value should match what was set");

    /* Updating (not adding) an existing variable shouldn't change the count. */
    setVariable("testVarEnumOnly", 9.0);
    ASSERT_TRUE(getVariableCount() == countAfter, "updating an existing variable shouldn't increase the count");

    ASSERT_TRUE(getVariableByIndex(-1, name, sizeof(name), &value) == 0, "a negative index should fail");
    ASSERT_TRUE(getVariableByIndex(countAfter, name, sizeof(name), &value) == 0,
                "an index at/past the count should fail");
}

static void test_ans(void)
{
    setAns(123.5);
    ASSERT_DOUBLE_EQ(getAns(), 123.5, "getAns() should return whatever setAns() last stored");
}

void run_variables_tests(void)
{
    test_builtin_constants();
    test_constants_are_read_only();
    test_set_and_get_user_variable();
    test_get_undefined_variable();
    test_variable_enumeration();
    test_ans();
}
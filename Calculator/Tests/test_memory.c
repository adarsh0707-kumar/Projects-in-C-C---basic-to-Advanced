#include "test_framework.h"
#include "tests.h"
#include "memory.h"

static void test_store_and_recall(void)
{
    memoryStore(10.0);
    ASSERT_DOUBLE_EQ(memoryRecall(), 10.0, "memoryRecall() should return what memoryStore() just set");
}

static void test_add_and_subtract(void)
{
    memoryStore(10.0);

    memoryAdd(5.0);
    ASSERT_DOUBLE_EQ(memoryRecall(), 15.0, "M+ should add to the current memory value");

    memorySubtract(3.0);
    ASSERT_DOUBLE_EQ(memoryRecall(), 12.0, "M- should subtract from the current memory value");
}

static void test_clear(void)
{
    memoryStore(999.0);
    memoryClear();
    ASSERT_DOUBLE_EQ(memoryRecall(), 0.0, "MC should reset memory back to 0");
}

void run_memory_tests(void)
{
    test_store_and_recall();
    test_add_and_subtract();
    test_clear();
}
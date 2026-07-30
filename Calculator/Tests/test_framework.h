#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

/* Shared counters, defined once in test_main.c */
extern int testsRun;
extern int testsFailed;

#define ASSERT_TRUE(cond, msg)                                      \
    do                                                              \
    {                                                               \
        testsRun++;                                                 \
        if (!(cond))                                                \
        {                                                           \
            testsFailed++;                                          \
            printf("  FAIL [%s:%d] %s\n", __func__, __LINE__, msg); \
        }                                                           \
    } while (0)

#define ASSERT_INT_EQ(actual, expected, msg) \
    ASSERT_TRUE((actual) == (expected), msg)

#define ASSERT_DOUBLE_EQ(actual, expected, msg) \
    ASSERT_TRUE(fabs((actual) - (expected)) < 1e-6, msg)

#define ASSERT_STR_EQ(actual, expected, msg) \
    ASSERT_TRUE(strcmp((actual), (expected)) == 0, msg)

/*
 * Several functions in this codebase call exit(EXIT_FAILURE) on invalid
 * input (stack overflow/underflow, division by zero, factorial() on a
 * negative/non-integer, an unknown binary function...). That's an
 * existing, intentional design choice elsewhere in the app -- not
 * something this test suite should paper over -- but it means we can't
 * just call those code paths directly in-process, or the whole test
 * run would die with them.
 *
 * ASSERT_EXITS_NONZERO forks a child, runs `stmt` in it, and checks
 * that the child terminated with a non-zero exit status -- i.e. it
 * confirms the failure path really does fail loudly, without taking
 * the test binary down with it.
 */
#define ASSERT_EXITS_NONZERO(stmt, msg)                                              \
    do                                                                               \
    {                                                                                \
        testsRun++;                                                                  \
        fflush(NULL);                                                                \
        pid_t _pid = fork();                                                         \
        if (_pid == 0)                                                               \
        {                                                                            \
            freopen("/dev/null", "w", stdout);                                       \
            freopen("/dev/null", "w", stderr);                                       \
            stmt;                                                                    \
            _exit(0); /* only reached if stmt did NOT exit/crash */                  \
        }                                                                            \
        else                                                                         \
        {                                                                            \
            int _status;                                                             \
            waitpid(_pid, &_status, 0);                                              \
            int _failedAsExpected = WIFEXITED(_status) && WEXITSTATUS(_status) != 0; \
            if (!_failedAsExpected)                                                  \
            {                                                                        \
                testsFailed++;                                                       \
                printf("  FAIL [%s:%d] %s (expected non-zero exit)\n",               \
                       __func__, __LINE__, msg);                                     \
            }                                                                        \
        }                                                                            \
    } while (0)

#define RUN_SUITE(fn)              \
    do                             \
    {                              \
        printf("-- %s --\n", #fn); \
        fn();                      \
    } while (0)

#endif
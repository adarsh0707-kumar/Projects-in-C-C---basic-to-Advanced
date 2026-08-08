#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <string.h>
#include <math.h>

/* fork()/waitpid() are POSIX and have no Windows equivalent, so
   ASSERT_EXITS_NONZERO below has two definitions -- see the comment
   above it for what the Windows build gives up. */
#if !defined(_WIN32)
#include <unistd.h>
#include <sys/wait.h>
#endif

/* Shared counters, defined once in test_main.c */
extern int testsRun;
extern int testsFailed;
extern int testsSkipped;

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

/*
 * Like ASSERT_DOUBLE_EQ, but with the tolerance stated rather than assumed.
 *
 * The 1e-6 above is right for most arithmetic here, and wrong for anything
 * about precision: it is the same order of magnitude as the error it would
 * need to detect. Constants and variables were being quantised to six
 * significant figures on their way through the postfix string, an error of
 * roughly 2.6e-06 in pi, and the whole suite passed.
 */
#define ASSERT_DOUBLE_NEAR(actual, expected, tolerance, msg) \
    ASSERT_TRUE(fabs((actual) - (expected)) < (tolerance), msg)

#define ASSERT_STR_EQ(actual, expected, msg) \
    ASSERT_TRUE(strcmp((actual), (expected)) == 0, msg)

/*
 * Several functions in this codebase call exit(EXIT_FAILURE) on invalid
 * input (factorial() on a negative/non-integer, an unknown binary
 * function...). That's an existing, intentional design choice elsewhere
 * in the app -- not something this test suite should paper over -- but
 * it means we can't just call those code paths directly in-process, or
 * the whole test run would die with them.
 *
 * (Stack overflow/underflow used to be in this category too, but as of
 * the Phase C error-handling migration stack.c reports those via a
 * returned status code instead -- see test_double_stack_error_paths()
 * in test_stack.c for how that's tested now.)
 *
 * ASSERT_EXITS_NONZERO forks a child, runs `stmt` in it, and checks
 * that the child terminated with a non-zero exit status -- i.e. it
 * confirms the failure path really does fail loudly, without taking
 * the test binary down with it.
 *
 * Windows has no fork(), and the alternatives (spawning the test
 * binary again with a "run only this case" flag, or CreateProcess
 * plumbing) would be a lot of machinery for a handful of assertions.
 * So on Windows these cases are *skipped and reported as skipped*
 * rather than silently passing, so a Windows summary can never claim
 * coverage it didn't have.
 *
 * As it stands there are NO call sites: every case that once used this
 * (stack overflow/underflow, out-of-domain factorial) has migrated to
 * returned status codes, so Linux, macOS and Windows all run the same
 * 471 tests and testsSkipped is always 0. The macro and its guard are
 * kept because the underlying pattern -- "this really does exit(1)" --
 * is still worth being able to assert, and because the POSIX includes
 * above would otherwise break the MSVC build the moment someone used
 * it again.
 */
#if defined(_WIN32)

#define ASSERT_EXITS_NONZERO(stmt, msg)                            \
    do                                                             \
    {                                                              \
        testsSkipped++;                                            \
        printf("  SKIP [%s:%d] %s (needs fork(); not on Windows)\n", \
               __func__, __LINE__, msg);                           \
    } while (0)

#else

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

#endif /* _WIN32 */

#define RUN_SUITE(fn)              \
    do                             \
    {                              \
        printf("-- %s --\n", #fn); \
        fn();                      \
    } while (0)

#endif
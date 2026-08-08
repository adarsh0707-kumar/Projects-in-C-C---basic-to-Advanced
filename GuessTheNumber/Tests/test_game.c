/******************************************************************************
 * @file test_game.c
 * @brief Tests for the Guess The Number rules.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The harness is a handful of macros. A third-party framework would be the
 * project's only dependency, for a program of this size.
 *
 * Input is read from a temporary file rather than a terminal, which is the
 * whole reason gtn_read_guess() takes a FILE* instead of using stdin.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"

static int checks = 0;
static int failures = 0;

#define CHECK(expression)                                                  \
    do                                                                     \
    {                                                                      \
        ++checks;                                                          \
        if (!(expression))                                                 \
        {                                                                  \
            ++failures;                                                    \
            printf("  FAIL %s:%d: %s\n", __FILE__, __LINE__, #expression); \
        }                                                                  \
    } while (0)

#define CHECK_EQ(actual, expected)                                         \
    do                                                                     \
    {                                                                      \
        ++checks;                                                          \
        const long a_ = (long)(actual);                                    \
        const long e_ = (long)(expected);                                  \
        if (a_ != e_)                                                      \
        {                                                                  \
            ++failures;                                                    \
            printf("  FAIL %s:%d: %s == %ld, expected %ld\n",              \
                   __FILE__, __LINE__, #actual, a_, e_);                   \
        }                                                                  \
    } while (0)

/**
 * @brief Returns a stream holding @p text, for feeding to the reader.
 */
static FILE *stream_of(const char *text)
{
    FILE *file = tmpfile();

    if (file == NULL)
        return NULL;

    fputs(text, file);
    rewind(file);

    return file;
}

static void test_compare(void)
{
    printf("TC-001  A guess is judged low, high or correct\n");

    CHECK(gtn_compare(10, 50) == GTN_TOO_LOW);
    CHECK(gtn_compare(90, 50) == GTN_TOO_HIGH);
    CHECK(gtn_compare(50, 50) == GTN_CORRECT);

    /* The boundaries are ordinary numbers, not special cases. */
    CHECK(gtn_compare(GTN_MIN, GTN_MAX) == GTN_TOO_LOW);
    CHECK(gtn_compare(GTN_MAX, GTN_MIN) == GTN_TOO_HIGH);
    CHECK(gtn_compare(GTN_MIN, GTN_MIN) == GTN_CORRECT);
}

static void test_range(void)
{
    printf("TC-002  Only numbers within the published range are legal\n");

    CHECK(gtn_in_range(GTN_MIN));
    CHECK(gtn_in_range(GTN_MAX));
    CHECK(gtn_in_range(50));

    CHECK(!gtn_in_range(GTN_MIN - 1));
    CHECK(!gtn_in_range(GTN_MAX + 1));
    CHECK(!gtn_in_range(0));
    CHECK(!gtn_in_range(-7));
}

static void test_pick(void)
{
    printf("TC-003  The secret number is always inside the range\n");

    /*
    A fixed seed so a failure can be reproduced. Many draws, because a
    modulus that was off by one would still produce a legal number most of
    the time -- the bug would only show at the boundaries.
    */
    srand(12345u);

    int sawMin = 0;
    int sawMax = 0;

    for (int draw = 0; draw < 20000; ++draw)
    {
        const int picked = gtn_pick();

        if (!gtn_in_range(picked))
        {
            printf("  FAIL gtn_pick returned %d, outside %d..%d\n",
                   picked, GTN_MIN, GTN_MAX);
            ++failures;
            ++checks;
            return;
        }

        if (picked == GTN_MIN)
            sawMin = 1;

        if (picked == GTN_MAX)
            sawMax = 1;
    }

    ++checks;

    /* Both ends must be reachable, or the range is not what it claims. */
    CHECK(sawMin);
    CHECK(sawMax);
}

static void test_reads_a_number(void)
{
    printf("TC-004  A valid line is read as a guess\n");

    FILE *input = stream_of("42\n");
    int guess = 0;

    CHECK(input != NULL);
    CHECK(gtn_read_guess(input, &guess) == GTN_READ_OK);
    CHECK_EQ(guess, 42);

    fclose(input);

    /* Surrounding whitespace is not the user's mistake. */
    input = stream_of("   7   \n");
    guess = 0;

    CHECK(gtn_read_guess(input, &guess) == GTN_READ_OK);
    CHECK_EQ(guess, 7);

    fclose(input);
}

static void test_rejects_bad_input(void)
{
    printf("TC-005  Nonsense is refused rather than misread\n");

    const char *const REFUSED[] = {
        "abc\n",      /* not a number at all */
        "\n",         /* an empty line */
        "12abc\n",    /* a number with a tail */
        "3.5\n",      /* not a whole number */
        "0\n",        /* below the range */
        "101\n",      /* above the range */
        "-5\n",       /* negative */
        "99999999999999999999\n" /* beyond long */
    };

    for (size_t index = 0; index < sizeof(REFUSED) / sizeof(REFUSED[0]);
         ++index)
    {
        FILE *input = stream_of(REFUSED[index]);
        int guess = -1;

        const GtnRead result = gtn_read_guess(input, &guess);

        if (result != GTN_READ_INVALID)
        {
            printf("  FAIL %s was not refused (result %d)\n",
                   REFUSED[index], (int)result);
            ++failures;
        }

        ++checks;

        /* A refused read must not scribble on the caller's variable. */
        CHECK_EQ(guess, -1);

        fclose(input);
    }
}

static void test_bad_input_does_not_loop(void)
{
    printf("TC-006  A bad line is consumed, so a retry actually retries\n");

    /*
    This is the defect the rewrite exists for. The original used
    scanf("%d"), which leaves unparsed characters in the buffer: the next
    call failed on the same "x", the guess variable kept its previous
    value, and the program printed "Your guess is too low" forever. Typing
    one letter was enough.

    Reading whole lines means a refused line is gone. Here the reader is
    called repeatedly on a stream whose first three lines are rubbish; if
    any of them were left in the buffer, the 42 at the end would never be
    reached and this test would fail rather than hang.
    */
    FILE *input = stream_of("x\nhello\n???\n42\n");

    int guess = 0;

    CHECK(gtn_read_guess(input, &guess) == GTN_READ_INVALID);
    CHECK(gtn_read_guess(input, &guess) == GTN_READ_INVALID);
    CHECK(gtn_read_guess(input, &guess) == GTN_READ_INVALID);

    /* Progress: the stream advanced past every bad line. */
    CHECK(gtn_read_guess(input, &guess) == GTN_READ_OK);
    CHECK_EQ(guess, 42);

    fclose(input);
}

static void test_end_of_input(void)
{
    printf("TC-007  Exhausted input ends the game instead of spinning\n");

    FILE *input = stream_of("");
    int guess = 0;

    CHECK(gtn_read_guess(input, &guess) == GTN_READ_EOF);

    fclose(input);

    /* And after the last real line. */
    input = stream_of("50\n");

    CHECK(gtn_read_guess(input, &guess) == GTN_READ_OK);
    CHECK(gtn_read_guess(input, &guess) == GTN_READ_EOF);

    fclose(input);

    /* A null stream is end of input, not a crash. */
    CHECK(gtn_read_guess(NULL, &guess) == GTN_READ_EOF);
    CHECK(gtn_read_guess(stdin, NULL) == GTN_READ_EOF);
}

static void test_overlong_line(void)
{
    printf("TC-008  An overlong line is refused whole, not split\n");

    /*
    Longer than the reader's buffer. Without draining the remainder, the
    tail would come back as a second guess -- so a mashed keyboard could
    produce a "valid" number the user never typed.
    */
    char line[512];

    memset(line, 'x', sizeof(line) - 2);
    line[sizeof(line) - 2] = '\n';
    line[sizeof(line) - 1] = '\0';

    FILE *input = stream_of(line);
    int guess = -1;

    CHECK(gtn_read_guess(input, &guess) == GTN_READ_INVALID);
    CHECK_EQ(guess, -1);

    /* Nothing of that line survived to be read as a guess. */
    CHECK(gtn_read_guess(input, &guess) == GTN_READ_EOF);

    fclose(input);
}

int main(void)
{
    printf("Guess The Number - test suite\n\n");

    test_compare();
    test_range();
    test_pick();
    test_reads_a_number();
    test_rejects_bad_input();
    test_bad_input_does_not_loop();
    test_end_of_input();
    test_overlong_line();

    printf("\n%d checks, %d failed\n", checks, failures);
    printf("RESULT: %s\n", (failures == 0) ? "PASSED" : "FAILED");

    return (failures == 0) ? 0 : 1;
}

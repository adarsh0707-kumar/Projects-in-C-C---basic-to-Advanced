#ifndef GAME_H
#define GAME_H

/******************************************************************************
 * @file Game.h
 * @brief Guess The Number: the rules, separated from the terminal.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Everything here is free of printf and scanf so that it can be tested. The
 * program's whole behaviour -- picking a number, judging a guess, reading one
 * from the user -- lives in these functions; main() only prints and loops.
 ******************************************************************************/

#include <stdio.h>

/** Lowest number the game will pick, and accept. */
#define GTN_MIN 1

/** Highest number the game will pick, and accept. */
#define GTN_MAX 100

/**
 * @enum GtnVerdict
 * @brief How a guess compares with the secret number.
 */
typedef enum
{
    GTN_TOO_LOW,  /**< The guess is below the secret. */
    GTN_TOO_HIGH, /**< The guess is above the secret. */
    GTN_CORRECT   /**< The guess is the secret. */
} GtnVerdict;

/**
 * @enum GtnRead
 * @brief The outcome of trying to read one guess.
 *
 * "Not a number" is a distinct outcome from "no more input". The original
 * program could not tell them apart and treated both as neither, which is
 * what made a stray letter loop forever.
 */
typedef enum
{
    GTN_READ_OK,      /**< A number in range was read. */
    GTN_READ_INVALID, /**< The line was not a number in range; ask again. */
    GTN_READ_EOF      /**< Input ended; the game cannot continue. */
} GtnRead;

/**
 * @brief Judges a guess against the secret number.
 * @param guess  The player's guess.
 * @param secret The number to find.
 * @return GtnVerdict Whether the guess was low, high or correct.
 */
GtnVerdict gtn_compare(int guess, int secret);

/**
 * @brief Reports whether a value is a legal guess.
 * @param value Value to check.
 * @return int Non-zero when GTN_MIN <= value <= GTN_MAX.
 */
int gtn_in_range(int value);

/**
 * @brief Picks a secret number using rand().
 *
 * Seeding is the caller's business: a game seeds from the clock, a test
 * seeds with a constant so the same run happens twice.
 *
 * @return int A number from GTN_MIN to GTN_MAX inclusive.
 */
int gtn_pick(void);

/**
 * @brief Reads one guess from a stream.
 *
 * Reads a whole line and parses it, rather than using scanf("%d"). A failed
 * scanf leaves the offending characters in the buffer, so the next call
 * fails on the same input and the caller spins: type a letter into the
 * original program and it printed "too low" forever. Consuming the line
 * whether or not it parses is what makes a retry actually retry.
 *
 * Trailing text is rejected, so "12abc" is refused rather than read as 12.
 *
 * @param in    Stream to read from.
 * @param guess Receives the guess; untouched unless GTN_READ_OK is returned.
 * @return GtnRead What happened.
 */
GtnRead gtn_read_guess(FILE *in, int *guess);

#endif /* GAME_H */

/******************************************************************************
 * @file main.c
 * @brief Guess The Number: the terminal front end.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Prints and loops. Every rule it applies comes from Game.h, so the game can
 * be tested without a terminal.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Game.h"

int main(void)
{
    srand((unsigned)time(NULL));

    const int secret = gtn_pick();

    int attempts = 0;
    int finished = 0;

    printf("Welcome to Guess The Number Game!\n");

    while (!finished)
    {
        int guess = 0;

        printf("\nPlease enter your guess between %d and %d: ",
               GTN_MIN, GTN_MAX);
        fflush(stdout);

        const GtnRead read = gtn_read_guess(stdin, &guess);

        if (read == GTN_READ_EOF)
        {
            /*
            Ctrl+D, or a pipe running dry. Without this the loop would spin
            on a stream that will never produce anything again.
            */
            printf("\n\nNo more input. The number was %d.\n", secret);
            return 0;
        }

        if (read == GTN_READ_INVALID)
        {
            printf("That is not a whole number between %d and %d. "
                   "Try again!\n",
                   GTN_MIN, GTN_MAX);
            continue;
        }

        ++attempts;

        switch (gtn_compare(guess, secret))
        {
        case GTN_TOO_LOW:
            printf("Your guess is too low. Try again!\n");
            break;

        case GTN_TOO_HIGH:
            printf("Your guess is too high. Try again!\n");
            break;

        case GTN_CORRECT:
            printf("\nCongratulations!! You guessed the correct number: "
                   "%d in %d attempt%s.\n",
                   secret, attempts, (attempts == 1) ? "" : "s");
            finished = 1;
            break;
        }
    }

    printf("\nThank you for playing the game!\n");
    printf("Developed by: Adarsh Kumar!\n");
    printf("Bye Bye!!\n");

    return 0;
}

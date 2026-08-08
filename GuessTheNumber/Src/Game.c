#include "Game.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

GtnVerdict gtn_compare(int guess, int secret)
{
    if (guess < secret)
        return GTN_TOO_LOW;

    if (guess > secret)
        return GTN_TOO_HIGH;

    return GTN_CORRECT;
}

int gtn_in_range(int value)
{
    return value >= GTN_MIN && value <= GTN_MAX;
}

int gtn_pick(void)
{
    /*
    The low bits of rand() are famously poor on some implementations, but
    the range here is small and the game is a toy; what matters is that the
    result is always in range, which the modulus guarantees.
    */
    return (rand() % (GTN_MAX - GTN_MIN + 1)) + GTN_MIN;
}

GtnRead gtn_read_guess(FILE *in, int *guess)
{
    char line[128];

    if (in == NULL || guess == NULL)
        return GTN_READ_EOF;

    if (fgets(line, (int)sizeof(line), in) == NULL)
        return GTN_READ_EOF;

    /*
    A line longer than the buffer would otherwise be split and its tail
    treated as a second guess. Drain the rest of it and refuse the lot.
    */
    if (strchr(line, '\n') == NULL && !feof(in))
    {
        int discarded;

        while ((discarded = fgetc(in)) != '\n' && discarded != EOF)
        {
            /* discard */
        }

        return GTN_READ_INVALID;
    }

    errno = 0;

    char *end = NULL;
    const long value = strtol(line, &end, 10);

    /* No digits at all. */
    if (end == line)
        return GTN_READ_INVALID;

    /* Anything after the number other than whitespace, such as "12abc". */
    while (*end != '\0' && isspace((unsigned char)*end))
        ++end;

    if (*end != '\0')
        return GTN_READ_INVALID;

    if (errno == ERANGE || value < INT_MIN || value > INT_MAX)
        return GTN_READ_INVALID;

    if (!gtn_in_range((int)value))
        return GTN_READ_INVALID;

    *guess = (int)value;

    return GTN_READ_OK;
}

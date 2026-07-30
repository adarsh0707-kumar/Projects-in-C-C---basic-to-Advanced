#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "base.h"

static int charToDigit(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

/* Converts a digit string (already validated to only contain
   characters < base) into a decimal value. Returns 0 via *ok on
   an empty digit string or overflow-free failure. */
static long long digitsToDecimal(const char digits[], int base, int *ok)
{
    long long value = 0;
    *ok = 1;

    if (digits[0] == '\0')
    {
        *ok = 0;
        return 0;
    }

    for (int i = 0; digits[i] != '\0'; i++)
    {
        int d = charToDigit(digits[i]);

        if (d < 0 || d >= base)
        {
            *ok = 0;
            return 0;
        }

        value = value * base + d;
    }

    return value;
}

/* Converts a (possibly negative) decimal value to a base-N string
   (sign-magnitude, not two's complement — see base.h). */
static void decimalToBaseString(long long value, int base, char out[], int outSize)
{
    if (value == 0)
    {
        snprintf(out, (size_t)outSize, "0");
        return;
    }

    int negative = value < 0;
    unsigned long long uval = negative ? (unsigned long long)(-value)
                                       : (unsigned long long)value;

    char buf[80];
    int pos = 0;
    const char *digitChars = "0123456789ABCDEF";

    while (uval > 0)
    {
        buf[pos++] = digitChars[uval % (unsigned long long)base];
        uval /= (unsigned long long)base;
    }

    int idx = 0;
    if (negative && idx < outSize - 1)
        out[idx++] = '-';

    for (int i = pos - 1; i >= 0 && idx < outSize - 1; i--)
        out[idx++] = buf[i];

    out[idx] = '\0';
}

int evaluateBaseExpression(const char expr[], char result[], int resultSize)
{
    int i = 0;

    while (isspace((unsigned char)expr[i]))
        i++;

    char name[8];
    int n = 0;

    while (isalpha((unsigned char)expr[i]) && n < 7)
        name[n++] = expr[i++];

    name[n] = '\0';

    while (isspace((unsigned char)expr[i]))
        i++;

    if (n == 0 || expr[i] != '(')
    {
        snprintf(result, (size_t)resultSize,
                 "Error: Expected a call like 'bin(25)' or 'dec(1111b)'.");
        return 0;
    }

    i++; /* consume '(' */

    while (isspace((unsigned char)expr[i]))
        i++;

    int argStart = i;
    int negative = 0;

    if (expr[i] == '-')
    {
        negative = 1;
        i++;
    }

    int digitStart = i;

    while (isalnum((unsigned char)expr[i]))
        i++;

    int digitLen = i - digitStart;

    while (isspace((unsigned char)expr[i]))
        i++;

    if (expr[i] != ')')
    {
        snprintf(result, (size_t)resultSize, "Error: Expected ')'.");
        return 0;
    }

    int closeParen = i;
    i++;

    while (isspace((unsigned char)expr[i]))
        i++;

    if (expr[i] != '\0')
    {
        snprintf(result, (size_t)resultSize, "Error: Unexpected characters after ')'.");
        return 0;
    }

    if (digitLen <= 0)
    {
        snprintf(result, (size_t)resultSize, "Error: '%s' requires an argument.", name);
        return 0;
    }

    (void)argStart;
    (void)closeParen;

    char argBuf[64];
    if (digitLen >= (int)sizeof(argBuf))
    {
        snprintf(result, (size_t)resultSize, "Error: Argument too long.");
        return 0;
    }

    strncpy(argBuf, expr + digitStart, (size_t)digitLen);
    argBuf[digitLen] = '\0';

    if (strcmp(name, "bin") == 0 || strcmp(name, "hex") == 0 || strcmp(name, "oct") == 0)
    {
        /* Argument must be a plain decimal integer */
        for (int k = 0; argBuf[k] != '\0'; k++)
        {
            if (!isdigit((unsigned char)argBuf[k]))
            {
                snprintf(result, (size_t)resultSize,
                         "Error: '%s' requires a decimal integer argument.", name);
                return 0;
            }
        }

        long long value = atoll(argBuf);
        if (negative)
            value = -value;

        int base = (strcmp(name, "bin") == 0) ? 2 : (strcmp(name, "hex") == 0) ? 16
                                                                               : 8;
        decimalToBaseString(value, base, result, resultSize);
        return 1;
    }

    if (strcmp(name, "dec") == 0)
    {
        if (negative)
        {
            /* '-' was already consumed as a candidate sign, but for
               dec() the argument is "<digits><suffix>", and a
               leading '-' in front of that is still a valid sign
               for the source value (e.g. dec(-1111b) = -15). */
        }

        if (digitLen < 2)
        {
            snprintf(result, (size_t)resultSize,
                     "Error: 'dec' requires digits followed by a base suffix "
                     "(b/o/h), e.g. dec(1111b).");
            return 0;
        }

        char suffix = argBuf[digitLen - 1];
        int base;

        if (suffix == 'b' || suffix == 'B')
            base = 2;
        else if (suffix == 'o' || suffix == 'O')
            base = 8;
        else if (suffix == 'h' || suffix == 'H')
            base = 16;
        else
        {
            snprintf(result, (size_t)resultSize,
                     "Error: Unknown base suffix '%c'. Use b (binary), "
                     "o (octal), or h (hex).",
                     suffix);
            return 0;
        }

        argBuf[digitLen - 1] = '\0'; /* strip suffix, leaving just the digits */

        int ok;
        long long value = digitsToDecimal(argBuf, base, &ok);

        if (!ok)
        {
            snprintf(result, (size_t)resultSize,
                     "Error: '%s' is not a valid base-%d number.", argBuf, base);
            return 0;
        }

        if (negative)
            value = -value;

        snprintf(result, (size_t)resultSize, "%lld", value);
        return 1;
    }

    snprintf(result, (size_t)resultSize, "Error: Unknown base function '%s'.", name);
    return 0;
}
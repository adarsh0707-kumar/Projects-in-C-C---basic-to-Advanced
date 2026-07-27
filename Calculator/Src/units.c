#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "units.h"

typedef struct
{
    const char *name;
    UnitCategory category;
    double toBase; /* multiply a value in this unit by this to get the base unit */
    int display;   /* 1 = show this unit in the conversion table, 0 = alias-only */
} UnitDef;

/*
 * Base units per category:
 *   Length      -> meters
 *   Weight      -> grams
 *   Time        -> seconds
 *   Temperature -> handled separately (not linear-from-zero, see convertAndPrint)
 *
 * Several entries share a factor on purpose (e.g. "mile"/"mi") so both
 * spellings are recognized on input; only one of each pair is marked
 * for display so the printed table doesn't show duplicate rows.
 */
static const UnitDef unitTable[] =
    {
        /* Length */
        {"km", UNIT_CAT_LENGTH, 1000.0, 1},
        {"m", UNIT_CAT_LENGTH, 1.0, 1},
        {"cm", UNIT_CAT_LENGTH, 0.01, 1},
        {"mm", UNIT_CAT_LENGTH, 0.001, 1},
        {"mile", UNIT_CAT_LENGTH, 1609.344, 1},
        {"mi", UNIT_CAT_LENGTH, 1609.344, 0},
        {"yard", UNIT_CAT_LENGTH, 0.9144, 1},
        {"yd", UNIT_CAT_LENGTH, 0.9144, 0},
        {"ft", UNIT_CAT_LENGTH, 0.3048, 1},
        {"inch", UNIT_CAT_LENGTH, 0.0254, 1},
        {"in", UNIT_CAT_LENGTH, 0.0254, 0},

        /* Weight */
        {"kg", UNIT_CAT_WEIGHT, 1000.0, 1},
        {"g", UNIT_CAT_WEIGHT, 1.0, 1},
        {"mg", UNIT_CAT_WEIGHT, 0.001, 1},
        {"lb", UNIT_CAT_WEIGHT, 453.592, 1},
        {"oz", UNIT_CAT_WEIGHT, 28.3495, 1},

        /* Time */
        {"day", UNIT_CAT_TIME, 86400.0, 1},
        {"hr", UNIT_CAT_TIME, 3600.0, 1},
        {"h", UNIT_CAT_TIME, 3600.0, 0},
        {"min", UNIT_CAT_TIME, 60.0, 1},
        {"sec", UNIT_CAT_TIME, 1.0, 1},
        {"s", UNIT_CAT_TIME, 1.0, 0},
        {"ms", UNIT_CAT_TIME, 0.001, 1}};

#define UNIT_TABLE_COUNT (sizeof(unitTable) / sizeof(unitTable[0]))

int parseValueWithUnit(const char input[], double *value, char unit[16])
{
    int i = 0;

    while (isspace((unsigned char)input[i]))
        i++;

    int start = i;

    if (input[i] == '-' || input[i] == '+')
        i++;

    int dotCount = 0;
    int hasDigits = 0;

    while (isdigit((unsigned char)input[i]) || input[i] == '.')
    {
        if (input[i] == '.')
            dotCount++;
        else
            hasDigits = 1;

        if (dotCount > 1)
            return 0;

        i++;
    }

    if (!hasDigits)
        return 0;

    char numBuf[32];
    int len = i - start;

    if (len <= 0 || len >= (int)sizeof(numBuf))
        return 0;

    strncpy(numBuf, input + start, len);
    numBuf[len] = '\0';
    *value = atof(numBuf);

    while (isspace((unsigned char)input[i]))
        i++;

    int u = 0;

    while (isalpha((unsigned char)input[i]) && u < 15)
        unit[u++] = input[i++];

    unit[u] = '\0';

    while (isspace((unsigned char)input[i]))
        i++;

    /* No unit found, or leftover garbage after the unit -> reject */
    if (u == 0 || input[i] != '\0')
        return 0;

    return 1;
}

static int isTemperatureUnit(const char unit[])
{
    return strcmp(unit, "C") == 0 ||
           strcmp(unit, "F") == 0 ||
           strcmp(unit, "K") == 0;
}

UnitCategory unitCategory(const char unit[])
{
    if (isTemperatureUnit(unit))
        return UNIT_CAT_TEMPERATURE;

    for (size_t i = 0; i < UNIT_TABLE_COUNT; i++)
    {
        if (strcmp(unitTable[i].name, unit) == 0)
            return unitTable[i].category;
    }

    return UNIT_CAT_UNKNOWN;
}

static double toBaseFactor(const char unit[])
{
    for (size_t i = 0; i < UNIT_TABLE_COUNT; i++)
    {
        if (strcmp(unitTable[i].name, unit) == 0)
            return unitTable[i].toBase;
    }

    return 0.0;
}

static double toCelsius(double value, const char unit[])
{
    if (strcmp(unit, "C") == 0)
        return value;

    if (strcmp(unit, "F") == 0)
        return (value - 32.0) * 5.0 / 9.0;

    /* K */
    return value - 273.15;
}

int convertAndPrint(double value, const char unitIn[])
{
    char unit[16];
    strncpy(unit, unitIn, sizeof(unit) - 1);
    unit[sizeof(unit) - 1] = '\0';

    /* Temperature units are conventionally single uppercase letters
       (30C, 80F); accept lowercase c/f/k too, without touching other
       single-letter units like 'g' (grams) or 'm' (meters) or 's'
       (seconds), which must stay lowercase to match the table. */
    if (strlen(unit) == 1)
    {
        char upper = (char)toupper((unsigned char)unit[0]);

        if (upper == 'C' || upper == 'F' || upper == 'K')
            unit[0] = upper;
    }

    UnitCategory cat = unitCategory(unit);

    if (cat == UNIT_CAT_UNKNOWN)
    {
        printf("Error: Unknown unit '%s'.\n", unitIn);
        return 0;
    }

    if (cat == UNIT_CAT_TEMPERATURE)
    {
        double c = toCelsius(value, unit);
        double f = c * 9.0 / 5.0 + 32.0;
        double k = c + 273.15;

        printf("%g %s = %g C = %g F = %g K\n", value, unitIn, c, f, k);
        return 1;
    }

    double base = value * toBaseFactor(unit);

    printf("%g %s =\n", value, unitIn);

    for (size_t i = 0; i < UNIT_TABLE_COUNT; i++)
    {
        if (unitTable[i].category == cat && unitTable[i].display)
        {
            printf("  %g %s\n", base / unitTable[i].toBase, unitTable[i].name);
        }
    }

    return 1;
}
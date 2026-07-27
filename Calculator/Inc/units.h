#ifndef UNITS_H
#define UNITS_H

typedef enum
{
    UNIT_CAT_LENGTH,
    UNIT_CAT_WEIGHT,
    UNIT_CAT_TIME,
    UNIT_CAT_TEMPERATURE,
    UNIT_CAT_UNKNOWN
} UnitCategory;

/* Parses a string like "10km", "10 km", "30C" into a numeric value
   and a unit suffix. Returns 1 on success, 0 if the string isn't a
   valid <number><unit> pattern (e.g. missing unit, garbage after
   the unit, or no digits at all). */
int parseValueWithUnit(const char input[], double *value, char unit[16]);

/* Looks up a unit name and reports its category.
   Returns UNIT_CAT_UNKNOWN if the unit isn't recognized. */
UnitCategory unitCategory(const char unit[]);

/* Prints a full conversion table for the given value/unit to stdout.
   Returns 0 (and prints an error) if the unit isn't recognized,
   1 on success. */
int convertAndPrint(double value, const char unit[]);

/* Parses "<value><unit>" optionally followed by "to <targetUnit>"
   (e.g. "10km", "10km to miles", "80F to C"). toUnit[0] is set to
   '\0' if no "to" clause was present. Returns 1 on success. */
int parseConversion(const char input[], double *value, char fromUnit[16], char toUnit[16]);

/* Converts value/fromUnit to a single target unit and prints the
   result (used when the user gave an explicit "to <unit>" clause).
   Returns 0 (and prints an error) on unknown unit or category
   mismatch (e.g. trying to convert km to kg), 1 on success. */
int convertToSingleUnit(double value, const char fromUnit[], const char toUnit[]);

#endif
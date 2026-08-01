/**
 * @file units.h
 * @brief Unit conversion (length, weight, time, temperature).
 */
#ifndef UNITS_H
#define UNITS_H

/** Broad category a unit belongs to; conversions only make sense
 *  within the same category (e.g. km -> miles, not km -> kg). */
typedef enum
{
   UNIT_CAT_LENGTH,
   UNIT_CAT_WEIGHT,
   UNIT_CAT_TIME,
   UNIT_CAT_TEMPERATURE,
   UNIT_CAT_UNKNOWN /**< Unit name wasn't recognized. */
} UnitCategory;

/** @brief Parses a string like "10km", "10 km", "30C" into a numeric
 *  value and a unit suffix.
 *  @param input Source string.
 *  @param value Out-parameter set to the parsed number.
 *  @param unit  Out-parameter (>= 16 bytes) set to the unit suffix.
 *  @return 1 on success, 0 if the string isn't a valid
 *  `<number><unit>` pattern (missing unit, garbage after the unit,
 *  or no digits at all). */
int parseValueWithUnit(const char input[], double *value, char unit[16]);

/** @brief Looks up a unit name and reports its category.
 *  @return UNIT_CAT_UNKNOWN if the unit isn't recognized. */
UnitCategory unitCategory(const char unit[]);

/** @brief Prints a full conversion table (to every unit in the same
 *  category) for the given value/unit to stdout.
 *  @return 0 (and prints an error) if the unit isn't recognized,
 *  1 on success. */
int convertAndPrint(double value, const char unit[]);

/** @brief Parses "<value><unit>" optionally followed by "to
 *  <targetUnit>" (e.g. "10km", "10km to miles", "80F to C").
 *  @param input   Source string.
 *  @param value   Out-parameter set to the parsed number.
 *  @param fromUnit Out-parameter (>= 16 bytes) set to the source unit.
 *  @param toUnit  Out-parameter (>= 16 bytes) set to the target unit,
 *  or "" if no "to" clause was present.
 *  @return 1 on success, 0 on a parse error. */
int parseConversion(const char input[], double *value, char fromUnit[16], char toUnit[16]);

/** @brief Converts a value between two specific units and prints the
 *  result (used when the user gave an explicit "to <unit>" clause).
 *  @return 0 (and prints an error) on an unknown unit or a category
 *  mismatch (e.g. converting km to kg), 1 on success. */
int convertToSingleUnit(double value, const char fromUnit[], const char toUnit[]);

#endif

#include "test_framework.h"
#include "tests.h"
#include "units.h"

/* parseValueWithUnit() is declared in units.h but not currently
   called anywhere in the CLI pipeline (main.c only ever calls
   parseConversion()) -- tested anyway since it's part of the public
   API and its own header promises specific behavior. */
static void test_parse_value_with_unit(void)
{
    double value;
    char unit[16];

    ASSERT_TRUE(parseValueWithUnit("10km", &value, unit) == 1, "'10km' should parse");
    ASSERT_DOUBLE_EQ(value, 10.0, "value should be 10");
    ASSERT_STR_EQ(unit, "km", "unit should be 'km'");

    ASSERT_TRUE(parseValueWithUnit("3.5 m", &value, unit) == 1, "'3.5 m' (with a space) should parse");
    ASSERT_DOUBLE_EQ(value, 3.5, "value should be 3.5");
    ASSERT_STR_EQ(unit, "m", "unit should be 'm'");

    ASSERT_TRUE(parseValueWithUnit("-40C", &value, unit) == 1, "a negative value should parse");
    ASSERT_DOUBLE_EQ(value, -40.0, "value should be -40");

    ASSERT_TRUE(parseValueWithUnit("10", &value, unit) == 0, "a number with no unit should be rejected");
    ASSERT_TRUE(parseValueWithUnit("km", &value, unit) == 0, "a unit with no number should be rejected");
    ASSERT_TRUE(parseValueWithUnit("10km!!", &value, unit) == 0, "garbage after the unit should be rejected");
}

static void test_unit_category(void)
{
    ASSERT_TRUE(unitCategory("km") == UNIT_CAT_LENGTH, "'km' should be a length unit");
    ASSERT_TRUE(unitCategory("kg") == UNIT_CAT_WEIGHT, "'kg' should be a weight unit");
    ASSERT_TRUE(unitCategory("hr") == UNIT_CAT_TIME, "'hr' should be a time unit");
    ASSERT_TRUE(unitCategory("C") == UNIT_CAT_TEMPERATURE, "'C' should be a temperature unit");
    ASSERT_TRUE(unitCategory("F") == UNIT_CAT_TEMPERATURE, "'F' should be a temperature unit");
    ASSERT_TRUE(unitCategory("K") == UNIT_CAT_TEMPERATURE, "'K' should be a temperature unit");
    ASSERT_TRUE(unitCategory("notaunit") == UNIT_CAT_UNKNOWN, "an unrecognized name should be UNKNOWN");
}

static void test_parse_conversion(void)
{
    double value;
    char fromUnit[16], toUnit[16];

    ASSERT_TRUE(parseConversion("10km", &value, fromUnit, toUnit) == 1, "bare '<value><unit>' should parse");
    ASSERT_DOUBLE_EQ(value, 10.0, "value should be 10");
    ASSERT_STR_EQ(fromUnit, "km", "fromUnit should be 'km'");
    ASSERT_STR_EQ(toUnit, "", "toUnit should be empty with no 'to' clause");

    ASSERT_TRUE(parseConversion("10km to miles", &value, fromUnit, toUnit) == 1,
                "'<value><unit> to <unit>' should parse");
    ASSERT_STR_EQ(fromUnit, "km", "fromUnit should be 'km'");
    ASSERT_STR_EQ(toUnit, "miles", "toUnit should be 'miles'");

    ASSERT_TRUE(parseConversion("80F to C", &value, fromUnit, toUnit) == 1, "temperature conversion should parse");
    ASSERT_STR_EQ(fromUnit, "F", "fromUnit should be 'F'");
    ASSERT_STR_EQ(toUnit, "C", "toUnit should be 'C'");

    ASSERT_TRUE(parseConversion("10km to", &value, fromUnit, toUnit) == 0,
                "'to' with no target unit should be rejected");
    ASSERT_TRUE(parseConversion("10km toward miles", &value, fromUnit, toUnit) == 0,
                "trailing text that isn't a valid 'to <unit>' clause should be rejected");
    ASSERT_TRUE(parseConversion("nope", &value, fromUnit, toUnit) == 0, "no digits at all should be rejected");
}

static void test_convert_to_single_unit(void)
{
    ASSERT_TRUE(convertToSingleUnit(10.0, "km", "miles") == 1,
                "converting within the same category (length) should succeed");
    ASSERT_TRUE(convertToSingleUnit(80.0, "F", "C") == 1, "temperature conversion should succeed");
    ASSERT_TRUE(convertToSingleUnit(10.0, "km", "kg") == 0,
                "converting between different categories should fail");
    ASSERT_TRUE(convertToSingleUnit(10.0, "notaunit", "km") == 0, "an unknown source unit should fail");
    ASSERT_TRUE(convertToSingleUnit(10.0, "km", "notaunit") == 0, "an unknown target unit should fail");
}

static void test_convert_and_print(void)
{
    ASSERT_TRUE(convertAndPrint(10.0, "km") == 1, "a recognized unit should succeed");
    ASSERT_TRUE(convertAndPrint(80.0, "F") == 1, "a recognized temperature unit should succeed");
    ASSERT_TRUE(convertAndPrint(10.0, "notaunit") == 0, "an unrecognized unit should fail");
}

void run_units_tests(void)
{
    test_parse_value_with_unit();
    test_unit_category();
    test_parse_conversion();
    test_convert_to_single_unit();
    test_convert_and_print();
}

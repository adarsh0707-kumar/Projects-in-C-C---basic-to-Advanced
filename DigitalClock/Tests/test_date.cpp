/******************************************************************************
 * @file test_date.cpp
 * @brief Tests for the Date core component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Covers TC-007 (current date display) from the Testing Report, plus the
 * calendar arithmetic the date formats depend on.
 ******************************************************************************/

#include "TestFramework.hpp"

#include <ctime>

#include "Date.hpp"

TEST_CASE(TC_007, "Verify current date display")
{
    Date date;

    CHECK_TRUE(date.update());
    CHECK_TRUE(date.isValid());

    const std::time_t raw = std::time(nullptr);
    std::tm local{};

#ifdef _WIN32
    localtime_s(&local, &raw);
#else
    localtime_r(&raw, &local);
#endif

    CHECK_EQ(date.getDay(), local.tm_mday);
    CHECK_EQ(date.getMonth(), local.tm_mon + 1);
    CHECK_EQ(date.getYear(), local.tm_year + 1900);
    CHECK_EQ(date.getWeekday(), local.tm_wday);

    // The long form must carry a weekday, a month name and a four-digit year.
    const std::string formatted = date.getCurrentDate();

    CHECK_CONTAINS(formatted, date.getWeekdayName());
    CHECK_CONTAINS(formatted, date.getMonthName());
    CHECK_CONTAINS(formatted, std::to_string(date.getYear()));
}

TEST_CASE(UT_020, "Date derives the correct weekday")
{
    Date date;

    // 03 August 2026 is a Monday, the example used throughout the User Manual.
    CHECK_TRUE(date.setDate(2026, 8, 3));
    CHECK_EQ(date.getWeekdayName(), std::string("Monday"));
    CHECK_EQ(date.getMonthName(), std::string("August"));
    CHECK_EQ(date.getDateLong(), std::string("Monday, 03 August 2026"));

    // A leap day, and a century boundary that is a leap year.
    CHECK_TRUE(date.setDate(2024, 2, 29));
    CHECK_EQ(date.getWeekdayName(), std::string("Thursday"));

    CHECK_TRUE(date.setDate(2000, 1, 1));
    CHECK_EQ(date.getWeekdayName(), std::string("Saturday"));
}

TEST_CASE(UT_021, "Date renders every supported numeric format")
{
    Date date;

    CHECK_TRUE(date.setDate(2026, 8, 3));

    CHECK_EQ(date.getDateDayFirst(), std::string("03-08-2026"));
    CHECK_EQ(date.getDateMonthFirst(), std::string("08-03-2026"));
    CHECK_EQ(date.getDateIso(), std::string("2026-08-03"));

    // Single-digit days and months must be zero padded.
    CHECK_TRUE(date.setDate(2026, 1, 9));
    CHECK_EQ(date.getDateDayFirst(), std::string("09-01-2026"));
}

TEST_CASE(UT_022, "Date rejects impossible calendar dates")
{
    Date date;

    CHECK_TRUE(date.setDate(2026, 8, 3));

    CHECK_FALSE(date.setDate(2026, 13, 1));
    CHECK_FALSE(date.setDate(2026, 0, 1));
    CHECK_FALSE(date.setDate(2026, 2, 30));
    CHECK_FALSE(date.setDate(2026, 4, 31));
    CHECK_FALSE(date.setDate(1800, 1, 1));

    // 2026 is not a leap year, so 29 February does not exist.
    CHECK_FALSE(date.setDate(2026, 2, 29));

    // A rejected assignment must leave the previous date intact.
    CHECK_EQ(date.getDateIso(), std::string("2026-08-03"));
}

TEST_CASE(UT_023, "Date applies the Gregorian leap year rule")
{
    CHECK_TRUE(Date::isLeapYear(2024));
    CHECK_TRUE(Date::isLeapYear(2000));
    CHECK_TRUE(Date::isLeapYear(1600));

    CHECK_FALSE(Date::isLeapYear(2026));
    CHECK_FALSE(Date::isLeapYear(1900));
    CHECK_FALSE(Date::isLeapYear(2100));

    CHECK_EQ(Date::daysInMonth(2024, 2), 29);
    CHECK_EQ(Date::daysInMonth(2026, 2), 28);
    CHECK_EQ(Date::daysInMonth(2026, 1), 31);
    CHECK_EQ(Date::daysInMonth(2026, 4), 30);

    // Out-of-range months report zero rather than reading past the table.
    CHECK_EQ(Date::daysInMonth(2026, 0), 0);
    CHECK_EQ(Date::daysInMonth(2026, 13), 0);
}

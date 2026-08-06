/******************************************************************************
 * @file test_formatter.cpp
 * @brief Tests for the TimeFormatter core component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Covers TC-005 (12-hour display format), TC-006 (24-hour display format) and
 * TC-008 (configured date format) from the Testing Report. The expected
 * strings are taken from the User Manual, section 7.3 and 7.4.
 ******************************************************************************/

#include "TestFramework.hpp"

#include "Clock.hpp"
#include "Date.hpp"
#include "TimeFormatter.hpp"

TEST_CASE(TC_005, "Verify 12-hour display format")
{
    Clock clock;
    TimeFormatter formatter;

    formatter.setTimeFormat(TimeFormatter::TimeFormat::Hour12);

    CHECK_TRUE(clock.setTime(20, 45, 30));

    // User Manual 7.3: TimeFormat=12 yields "08:45:30 PM".
    CHECK_EQ(formatter.formatTime(clock), std::string("08:45:30 PM"));

    CHECK_TRUE(clock.setTime(8, 45, 30));
    CHECK_EQ(formatter.formatTime(clock), std::string("08:45:30 AM"));

    // Boundaries required by the FR-004 acceptance criteria.
    CHECK_TRUE(clock.setTime(0, 0, 0));
    CHECK_EQ(formatter.formatTime(clock), std::string("12:00:00 AM"));

    CHECK_TRUE(clock.setTime(12, 0, 0));
    CHECK_EQ(formatter.formatTime(clock), std::string("12:00:00 PM"));

    // The wide form used on screen carries the same meridiem.
    CHECK_TRUE(clock.setTime(10, 45, 30));
    CHECK_EQ(formatter.formatTimeWide(clock), std::string("10 : 45 : 30 AM"));
}

TEST_CASE(TC_006, "Verify 24-hour display format")
{
    Clock clock;
    TimeFormatter formatter;

    formatter.setTimeFormat(TimeFormatter::TimeFormat::Hour24);

    CHECK_TRUE(clock.setTime(20, 45, 30));

    // User Manual 7.3: TimeFormat=24 yields "20:45:30".
    CHECK_EQ(formatter.formatTime(clock), std::string("20:45:30"));

    // FR-005 requires leading zeros and no meridiem indicator.
    CHECK_TRUE(clock.setTime(9, 5, 3));
    CHECK_EQ(formatter.formatTime(clock), std::string("09:05:03"));
    CHECK_NOT_CONTAINS(formatter.formatTime(clock), "AM");
    CHECK_NOT_CONTAINS(formatter.formatTime(clock), "PM");

    CHECK_TRUE(clock.setTime(0, 0, 0));
    CHECK_EQ(formatter.formatTime(clock), std::string("00:00:00"));

    // The wide form matches the screen layout in the User Manual.
    CHECK_TRUE(clock.setTime(9, 42, 16));
    CHECK_EQ(formatter.formatTimeWide(clock), std::string("09 : 42 : 16"));
}

TEST_CASE(TC_008, "Verify configured date format")
{
    Date date;
    TimeFormatter formatter;

    CHECK_TRUE(date.setDate(2026, 8, 3));

    // User Manual 7.4 lists each of these formats and its output.
    formatter.setDateFormat(TimeFormatter::DateFormat::Long);
    CHECK_EQ(formatter.formatDate(date), std::string("Monday, 03 August 2026"));

    formatter.setDateFormat(TimeFormatter::DateFormat::DayFirst);
    CHECK_EQ(formatter.formatDate(date), std::string("03-08-2026"));

    formatter.setDateFormat(TimeFormatter::DateFormat::MonthFirst);
    CHECK_EQ(formatter.formatDate(date), std::string("08-03-2026"));

    formatter.setDateFormat(TimeFormatter::DateFormat::Iso);
    CHECK_EQ(formatter.formatDate(date), std::string("2026-08-03"));
}

TEST_CASE(UT_030, "TimeFormatter parses configuration values")
{
    using TF = TimeFormatter;

    CHECK_TRUE(
        TF::timeFormatFromString("12") == TF::TimeFormat::Hour12);
    CHECK_TRUE(
        TF::timeFormatFromString("12H") == TF::TimeFormat::Hour12);
    CHECK_TRUE(
        TF::timeFormatFromString("24") == TF::TimeFormat::Hour24);
    CHECK_TRUE(
        TF::timeFormatFromString(" 24-hour ") == TF::TimeFormat::Hour24);

    // Unrecognised values keep the caller's fallback.
    CHECK_TRUE(
        TF::timeFormatFromString("36", TF::TimeFormat::Hour24) ==
        TF::TimeFormat::Hour24);
    CHECK_TRUE(
        TF::timeFormatFromString("", TF::TimeFormat::Hour12) ==
        TF::TimeFormat::Hour12);

    CHECK_TRUE(
        TF::dateFormatFromString("LONG") == TF::DateFormat::Long);
    CHECK_TRUE(
        TF::dateFormatFromString("DD-MM-YYYY") == TF::DateFormat::DayFirst);
    CHECK_TRUE(
        TF::dateFormatFromString("MM-DD-YYYY") == TF::DateFormat::MonthFirst);
    CHECK_TRUE(
        TF::dateFormatFromString("YYYY-MM-DD") == TF::DateFormat::Iso);

    // Slashes are accepted as an alternative separator.
    CHECK_TRUE(
        TF::dateFormatFromString("dd/mm/yyyy") == TF::DateFormat::DayFirst);

    CHECK_TRUE(
        TF::dateFormatFromString("nonsense", TF::DateFormat::Long) ==
        TF::DateFormat::Long);
}

TEST_CASE(UT_031, "TimeFormatter combines date and time")
{
    Clock clock;
    Date date;
    TimeFormatter formatter;

    CHECK_TRUE(clock.setTime(20, 45, 30));
    CHECK_TRUE(date.setDate(2026, 8, 3));

    formatter.setTimeFormat(TimeFormatter::TimeFormat::Hour24);
    formatter.setDateFormat(TimeFormatter::DateFormat::Iso);

    CHECK_EQ(
        formatter.formatDateTime(clock, date),
        std::string("2026-08-03 - 20:45:30"));

    // Defaults are the 24-hour clock and the long date.
    TimeFormatter fresh;

    CHECK_TRUE(fresh.timeFormat() == TimeFormatter::TimeFormat::Hour24);
    CHECK_TRUE(fresh.dateFormat() == TimeFormatter::DateFormat::Long);
}

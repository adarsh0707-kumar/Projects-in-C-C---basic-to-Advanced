/******************************************************************************
 * @file test_clock.cpp
 * @brief Tests for the Clock core component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Covers TC-003 (current system time) and TC-004 (automatic time updates)
 * from the Testing Report, plus supporting checks on the 12-hour conversion
 * boundaries required by FR-004.
 ******************************************************************************/

#include "TestFramework.hpp"

#include <chrono>
#include <ctime>
#include <thread>

#include "Clock.hpp"

TEST_CASE(TC_003, "Verify current system time")
{
    Clock clock;

    CHECK_TRUE(clock.update());
    CHECK_TRUE(clock.isValid());

    // Compare against the system clock read independently.
    const std::time_t raw = std::time(nullptr);
    std::tm local{};

#ifdef _WIN32
    localtime_s(&local, &raw);
#else
    localtime_r(&raw, &local);
#endif

    CHECK_EQ(clock.getHour(), local.tm_hour);
    CHECK_EQ(clock.getMinute(), local.tm_min);

    // Seconds may have advanced between the two reads.
    const int drift = std::abs(clock.getSecond() - local.tm_sec);
    CHECK_TRUE(drift <= 1 || drift >= 59);

    // Fields must stay inside their valid ranges.
    CHECK_TRUE(clock.getHour() >= 0 && clock.getHour() <= 23);
    CHECK_TRUE(clock.getMinute() >= 0 && clock.getMinute() <= 59);
    CHECK_TRUE(clock.getSecond() >= 0 && clock.getSecond() <= 60);
}

TEST_CASE(TC_004, "Verify automatic time updates")
{
    Clock clock;

    CHECK_TRUE(clock.setTime(10, 30, 0));

    // A snapshot must not drift on its own between updates.
    const std::string before = clock.getCurrentTime();
    CHECK_EQ(clock.getCurrentTime(), before);

    // update() must replace the snapshot with the real system time.
    CHECK_TRUE(clock.update());
    CHECK_TRUE(clock.isValid());

    // Advancing past a second boundary must be observable.
    const int first = clock.getSecond();

    std::this_thread::sleep_for(std::chrono::milliseconds(1100));

    CHECK_TRUE(clock.update());

    CHECK_NE(clock.getSecond(), first);
}

TEST_CASE(TC_006A, "Clock renders 24-hour time with leading zeros")
{
    Clock clock;

    CHECK_TRUE(clock.setTime(9, 5, 3));
    CHECK_EQ(clock.getTime24(), std::string("09:05:03"));
    CHECK_EQ(clock.getCurrentTime(), std::string("09:05:03"));

    CHECK_TRUE(clock.setTime(20, 45, 30));
    CHECK_EQ(clock.getTime24(), std::string("20:45:30"));

    // FR-005 requires no meridiem indicator in 24-hour form.
    CHECK_NOT_CONTAINS(clock.getTime24(), "PM");
    CHECK_NOT_CONTAINS(clock.getTime24(), "AM");
}

TEST_CASE(TC_005A, "Clock converts midnight and noon to 12-hour form")
{
    Clock clock;

    // Midnight must read 12 AM, not 00 AM (FR-004 acceptance criteria).
    CHECK_TRUE(clock.setTime(0, 0, 0));
    CHECK_EQ(clock.getHour12(), 12);
    CHECK_FALSE(clock.isPM());
    CHECK_EQ(clock.getTime12(), std::string("12:00:00 AM"));

    // Noon must read 12 PM.
    CHECK_TRUE(clock.setTime(12, 0, 0));
    CHECK_EQ(clock.getHour12(), 12);
    CHECK_TRUE(clock.isPM());
    CHECK_EQ(clock.getTime12(), std::string("12:00:00 PM"));

    // One minute before noon is still AM.
    CHECK_TRUE(clock.setTime(11, 59, 59));
    CHECK_EQ(clock.getHour12(), 11);
    CHECK_FALSE(clock.isPM());

    // The last minute of the day is PM.
    CHECK_TRUE(clock.setTime(23, 59, 59));
    CHECK_EQ(clock.getHour12(), 11);
    CHECK_TRUE(clock.isPM());
    CHECK_EQ(clock.getTime12(), std::string("11:59:59 PM"));
}

TEST_CASE(UT_010, "Clock rejects out-of-range times")
{
    Clock clock;

    CHECK_TRUE(clock.setTime(12, 0, 0));

    CHECK_FALSE(clock.setTime(24, 0, 0));
    CHECK_FALSE(clock.setTime(-1, 0, 0));
    CHECK_FALSE(clock.setTime(0, 60, 0));
    CHECK_FALSE(clock.setTime(0, 0, 61));

    // A rejected assignment must leave the previous snapshot intact.
    CHECK_EQ(clock.getTime24(), std::string("12:00:00"));

    // A leap second is a legitimate value.
    CHECK_TRUE(clock.setTime(23, 59, 60));
}

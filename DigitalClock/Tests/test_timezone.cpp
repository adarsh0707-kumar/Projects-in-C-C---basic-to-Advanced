/******************************************************************************
 * @file test_timezone.cpp
 * @brief Tests for the time zone support added in v1.3.0.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Covers TC-053 to TC-060.
 *
 * Fixed-offset zones are arithmetic on a UTC instant, so they are asserted
 * exactly. Named zones depend on the platform's timezone database, which is
 * not something a test can assume; those assertions are written to hold
 * whether or not the database is present, and the platform-dependent parts
 * are checked through resolves() rather than against hardcoded times.
 ******************************************************************************/

#include "TestFramework.hpp"

#include <ctime>

#include "TimeZone.hpp"
#include "WorldClock.hpp"

namespace
{
    /*
    2026-08-07 12:00:00 UTC. Chosen as a round instant well inside a day, so
    that offsets in either direction stay on the same date and the expected
    values are obvious by inspection.
    */
    const std::time_t NOON_UTC = 1786104000;
}

TEST_CASE(TC_053, "Verify UTC offset parsing")
{
    int minutes = -1;

    CHECK_TRUE(TimeZone::parseOffset("UTC", minutes));
    CHECK_EQ(minutes, 0);

    CHECK_TRUE(TimeZone::parseOffset("GMT", minutes));
    CHECK_EQ(minutes, 0);

    CHECK_TRUE(TimeZone::parseOffset("UTC+05:30", minutes));
    CHECK_EQ(minutes, 330);

    CHECK_TRUE(TimeZone::parseOffset("+05:30", minutes));
    CHECK_EQ(minutes, 330);

    CHECK_TRUE(TimeZone::parseOffset("-08:00", minutes));
    CHECK_EQ(minutes, -480);

    // Hours without minutes.
    CHECK_TRUE(TimeZone::parseOffset("+09", minutes));
    CHECK_EQ(minutes, 540);

    // Case and surrounding whitespace are insignificant.
    CHECK_TRUE(TimeZone::parseOffset("  utc-03:30  ", minutes));
    CHECK_EQ(minutes, -210);

    // The extremes of the real range.
    CHECK_TRUE(TimeZone::parseOffset("+14:00", minutes));
    CHECK_EQ(minutes, TimeZone::MAX_OFFSET_MINUTES);

    CHECK_TRUE(TimeZone::parseOffset("-12:00", minutes));
    CHECK_EQ(minutes, TimeZone::MIN_OFFSET_MINUTES);
}

TEST_CASE(TC_054, "Verify malformed offsets are rejected")
{
    int minutes = 0;

    CHECK_FALSE(TimeZone::parseOffset("", minutes));
    CHECK_FALSE(TimeZone::parseOffset("   ", minutes));
    CHECK_FALSE(TimeZone::parseOffset("nonsense", minutes));

    // An unsigned number is too ambiguous to guess at.
    CHECK_FALSE(TimeZone::parseOffset("530", minutes));
    CHECK_FALSE(TimeZone::parseOffset("05:30", minutes));

    CHECK_FALSE(TimeZone::parseOffset("+", minutes));
    CHECK_FALSE(TimeZone::parseOffset("+05:", minutes));
    CHECK_FALSE(TimeZone::parseOffset("+:30", minutes));
    CHECK_FALSE(TimeZone::parseOffset("+05:99", minutes));

    // Beyond the range any real zone occupies.
    CHECK_FALSE(TimeZone::parseOffset("+15:00", minutes));
    CHECK_FALSE(TimeZone::parseOffset("-13:00", minutes));
}

TEST_CASE(TC_055, "Verify fixed-offset zones convert exactly")
{
    TimeZone utc;
    CHECK_TRUE(utc.setFixedOffset(0));

    std::tm out{};
    CHECK_TRUE(utc.localTime(NOON_UTC, out));
    CHECK_EQ(out.tm_hour, 12);
    CHECK_EQ(out.tm_min, 0);

    // +05:30 turns 12:00 UTC into 17:30.
    TimeZone kolkata;
    CHECK_TRUE(kolkata.setFixedOffset(330));
    CHECK_TRUE(kolkata.localTime(NOON_UTC, out));
    CHECK_EQ(out.tm_hour, 17);
    CHECK_EQ(out.tm_min, 30);

    // -08:00 turns 12:00 UTC into 04:00.
    TimeZone pacific;
    CHECK_TRUE(pacific.setFixedOffset(-480));
    CHECK_TRUE(pacific.localTime(NOON_UTC, out));
    CHECK_EQ(out.tm_hour, 4);
    CHECK_EQ(out.tm_min, 0);

    // A fixed offset is always resolvable, on every platform.
    CHECK_TRUE(utc.resolves());
    CHECK_TRUE(kolkata.resolves());
    CHECK_TRUE(pacific.resolves());

    CHECK_EQ(kolkata.offsetMinutes(NOON_UTC), 330);
    CHECK_EQ(pacific.offsetMinutes(NOON_UTC), -480);

    // Out-of-range offsets are refused.
    CHECK_FALSE(utc.setFixedOffset(TimeZone::MAX_OFFSET_MINUTES + 1));
    CHECK_FALSE(utc.setFixedOffset(TimeZone::MIN_OFFSET_MINUTES - 1));
}

TEST_CASE(TC_056, "Verify zone time and offset formatting")
{
    TimeZone kolkata;
    CHECK_TRUE(kolkata.setFixedOffset(330));

    CHECK_EQ(kolkata.formatTime(NOON_UTC, false), std::string("17:30"));
    CHECK_EQ(kolkata.formatTime(NOON_UTC, true), std::string("05:30 PM"));
    CHECK_EQ(kolkata.formatOffset(NOON_UTC), std::string("+05:30"));

    TimeZone pacific;
    CHECK_TRUE(pacific.setFixedOffset(-480));

    CHECK_EQ(pacific.formatTime(NOON_UTC, false), std::string("04:00"));
    CHECK_EQ(pacific.formatTime(NOON_UTC, true), std::string("04:00 AM"));
    CHECK_EQ(pacific.formatOffset(NOON_UTC), std::string("-08:00"));

    TimeZone utc;
    CHECK_EQ(utc.formatOffset(NOON_UTC), std::string("+00:00"));

    // Midnight and noon read correctly in 12-hour form.
    TimeZone midnight;
    CHECK_TRUE(midnight.setFixedOffset(-12 * 60));
    CHECK_EQ(midnight.formatTime(NOON_UTC, true), std::string("12:00 AM"));

    TimeZone noon;
    CHECK_TRUE(noon.setFixedOffset(0));
    CHECK_EQ(noon.formatTime(NOON_UTC, true), std::string("12:00 PM"));
}

TEST_CASE(TC_057, "Verify zone specification parsing")
{
    TimeZone zone;

    CHECK_TRUE(TimeZone::parse("UTC", zone));
    CHECK_TRUE(zone.kind() == TimeZone::Kind::FixedOffset);
    CHECK_EQ(zone.label(), std::string("UTC"));

    // A label after a pipe.
    CHECK_TRUE(TimeZone::parse("+05:30 | Home", zone));
    CHECK_TRUE(zone.kind() == TimeZone::Kind::FixedOffset);
    CHECK_EQ(zone.label(), std::string("Home"));
    CHECK_EQ(zone.offsetMinutes(NOON_UTC), 330);

    // A '/' marks an IANA zone id.
    CHECK_TRUE(TimeZone::parse("Asia/Kolkata", zone));
    CHECK_TRUE(zone.kind() == TimeZone::Kind::Named);
    CHECK_EQ(zone.name(), std::string("Asia/Kolkata"));
    CHECK_EQ(zone.label(), std::string("Asia/Kolkata"));

    CHECK_TRUE(TimeZone::parse("America/New_York | NYC", zone));
    CHECK_TRUE(zone.kind() == TimeZone::Kind::Named);
    CHECK_EQ(zone.label(), std::string("NYC"));

    // Malformed specifications are rejected.
    CHECK_FALSE(TimeZone::parse("", zone));
    CHECK_FALSE(TimeZone::parse("   ", zone));
    CHECK_FALSE(TimeZone::parse("not-a-zone", zone));
    CHECK_FALSE(TimeZone::parse("| OnlyALabel", zone));
    CHECK_FALSE(TimeZone::parse("+99:00 | Silly", zone));
}

TEST_CASE(TC_058, "Verify the world clock loads a zone list")
{
    WorldClock world;

    CHECK_EQ(world.count(), static_cast<std::size_t>(0));
    CHECK_FALSE(world.load(""));

    CHECK_TRUE(world.load("UTC, +05:30 | Home, -08:00 | Pacific"));
    CHECK_EQ(world.count(), static_cast<std::size_t>(3));
    CHECK_EQ(world.invalidCount(), static_cast<std::size_t>(0));

    CHECK_EQ(world.at(0).label(), std::string("UTC"));
    CHECK_EQ(world.at(1).label(), std::string("Home"));
    CHECK_EQ(world.at(2).label(), std::string("Pacific"));

    // Fixed offsets always resolve, so none of these is unavailable.
    CHECK_EQ(world.unresolvedCount(), static_cast<std::size_t>(0));

    // Whitespace and empty entries between commas are tolerated.
    CHECK_TRUE(world.load("  UTC ,, +01:00 | Berlin  "));
    CHECK_EQ(world.count(), static_cast<std::size_t>(2));
}

TEST_CASE(TC_059, "Verify malformed zone entries are skipped, not fatal")
{
    WorldClock world;

    CHECK_TRUE(world.load("UTC, garbage, +05:30 | Home, +99:00"));

    // The two good entries survive; the two bad ones are counted.
    CHECK_EQ(world.count(), static_cast<std::size_t>(2));
    CHECK_EQ(world.invalidCount(), static_cast<std::size_t>(2));

    CHECK_EQ(world.at(0).label(), std::string("UTC"));
    CHECK_EQ(world.at(1).label(), std::string("Home"));

    // A list of nothing but bad entries loads nothing and says so.
    CHECK_FALSE(world.load("garbage, +99:00, ---"));
    CHECK_EQ(world.count(), static_cast<std::size_t>(0));

    // The zone count is capped.
    std::string many;
    for (int index = 0; index < 20; ++index)
        many += (index > 0 ? "," : "") + std::string("UTC");

    CHECK_TRUE(world.load(many));
    CHECK_TRUE(world.count() <= WorldClock::MAX_ZONES);
}

TEST_CASE(TC_060, "Verify world clock rows are aligned and labelled")
{
    WorldClock world;

    CHECK_TRUE(world.load("UTC, +05:30 | Home, -08:00 | Pacific"));

    const std::vector<std::string> rows = world.rows(NOON_UTC, false);

    CHECK_EQ(rows.size(), static_cast<std::size_t>(3));

    // Each row carries its label, its time and its offset.
    CHECK_CONTAINS(rows[0], "UTC");
    CHECK_CONTAINS(rows[0], "12:00");
    CHECK_CONTAINS(rows[0], "(UTC+00:00)");

    CHECK_CONTAINS(rows[1], "Home");
    CHECK_CONTAINS(rows[1], "17:30");
    CHECK_CONTAINS(rows[1], "(UTC+05:30)");

    CHECK_CONTAINS(rows[2], "Pacific");
    CHECK_CONTAINS(rows[2], "04:00");
    CHECK_CONTAINS(rows[2], "(UTC-08:00)");

    // Labels are padded to a common width, so the times form a column.
    const std::size_t first = rows[0].find("12:00");
    CHECK_EQ(rows[1].find("17:30"), first);
    CHECK_EQ(rows[2].find("04:00"), first);

    // Fixed offsets are never marked unavailable.
    for (const std::string &row : rows)
        CHECK_NOT_CONTAINS(row, "unavailable");

    // 12-hour rendering carries a meridiem.
    const std::vector<std::string> twelve = world.rows(NOON_UTC, true);
    CHECK_CONTAINS(twelve[1], "05:30 PM");

    // No zones means no rows.
    WorldClock empty;
    CHECK_EQ(empty.rows(NOON_UTC, false).size(),
             static_cast<std::size_t>(0));
}

TEST_CASE(UT_120, "Named zones behave consistently with platform support")
{
    TimeZone named;
    CHECK_TRUE(named.setNamedZone("Asia/Kolkata"));
    CHECK_TRUE(named.kind() == TimeZone::Kind::Named);

    std::tm out{};
    const bool converted = named.localTime(NOON_UTC, out);

    /*
    Whether this resolves is a property of the platform, not of the code, so
    the assertion is the invariant that must hold either way: a zone that
    reports itself resolvable must convert, and one that does not must still
    produce a usable time rather than leaving the caller with nothing.
    */
    if (named.resolves())
    {
        CHECK_TRUE(converted);

        // Asia/Kolkata is +05:30 year round; it observes no daylight saving.
        CHECK_EQ(named.offsetMinutes(NOON_UTC), 330);
        CHECK_EQ(named.formatTime(NOON_UTC, false), std::string("17:30"));
    }
    else
    {
        // The fallback is UTC, and the row renderer marks it.
        CHECK_EQ(out.tm_hour, 12);

        WorldClock world;
        CHECK_TRUE(world.load("Asia/Kolkata | Home"));
        CHECK_CONTAINS(world.rows(NOON_UTC, false)[0], "unavailable");
    }

    // An empty name is refused outright.
    TimeZone blank;
    CHECK_FALSE(blank.setNamedZone(""));
    CHECK_FALSE(blank.setNamedZone("   "));
}

TEST_CASE(UT_121, "Resolving a named zone leaves the process TZ untouched")
{
    /*
    Reading a named zone works by swapping the TZ environment variable and
    calling tzset(). If the swap failed to restore, every subsequent call to
    localtime() in the process would silently use the wrong zone -- so this
    asserts the restoration rather than trusting it.

    The mechanism is POSIX-only; on Windows a named zone reports itself
    unresolvable and never touches the environment at all.
    */
    const std::time_t probe = std::time(nullptr);

    std::tm before{};
    std::tm after{};

#ifdef _WIN32
    CHECK_TRUE(localtime_s(&before, &probe) == 0);
#else
    CHECK_TRUE(localtime_r(&probe, &before) != nullptr);
#endif

    TimeZone named;
    CHECK_TRUE(named.setNamedZone("Asia/Kolkata"));

    std::tm ignored{};
    named.localTime(probe, ignored);
    named.offsetMinutes(probe);
    named.resolves();

#ifdef _WIN32
    CHECK_TRUE(localtime_s(&after, &probe) == 0);
#else
    CHECK_TRUE(localtime_r(&probe, &after) != nullptr);
#endif

    CHECK_EQ(after.tm_hour, before.tm_hour);
    CHECK_EQ(after.tm_min, before.tm_min);
}

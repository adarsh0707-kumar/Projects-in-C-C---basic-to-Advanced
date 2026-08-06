/******************************************************************************
 * @file test_utility.cpp
 * @brief Unit tests for the Utility helper class.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Utility helpers underpin parsing throughout the project, so they are
 * covered directly rather than only through the components that use them.
 * These supporting tests carry UT identifiers to distinguish them from the
 * numbered cases in the Testing Report.
 ******************************************************************************/

#include "TestFramework.hpp"

#include "Utility.hpp"

TEST_CASE(UT_001, "Utility::trim removes surrounding whitespace")
{
    CHECK_EQ(Utility::trim("  Clock  "), std::string("Clock"));
    CHECK_EQ(Utility::trim("\t\nClock\r\n"), std::string("Clock"));
    CHECK_EQ(Utility::trim("Clock"), std::string("Clock"));

    // Interior whitespace must survive.
    CHECK_EQ(Utility::trim("  Digital Clock  "), std::string("Digital Clock"));

    // A string of nothing but whitespace collapses to empty.
    CHECK_EQ(Utility::trim("   \t  "), std::string(""));
    CHECK_EQ(Utility::trim(""), std::string(""));
}

TEST_CASE(UT_002, "Utility case conversion is stable")
{
    CHECK_EQ(Utility::toUpper("Dark"), std::string("DARK"));
    CHECK_EQ(Utility::toUpper("DARK"), std::string("DARK"));
    CHECK_EQ(Utility::toLower("Dark"), std::string("dark"));
    CHECK_EQ(Utility::toLower("dark"), std::string("dark"));

    // Non-alphabetic characters pass through untouched.
    CHECK_EQ(Utility::toUpper("dd-mm-yyyy"), std::string("DD-MM-YYYY"));
}

TEST_CASE(UT_003, "Utility::padZero pads to the requested width")
{
    CHECK_EQ(Utility::padZero(0), std::string("00"));
    CHECK_EQ(Utility::padZero(7), std::string("07"));
    CHECK_EQ(Utility::padZero(23), std::string("23"));

    // Values wider than the field are never truncated.
    CHECK_EQ(Utility::padZero(2026, 2), std::string("2026"));
    CHECK_EQ(Utility::padZero(5, 4), std::string("0005"));
}

TEST_CASE(UT_004, "Utility::toInt rejects values that are not integers")
{
    CHECK_EQ(Utility::toInt("1000", 1), 1000);
    CHECK_EQ(Utility::toInt("  250  ", 1), 250);
    CHECK_EQ(Utility::toInt("-5", 1), -5);

    // Trailing text must not be silently ignored.
    CHECK_EQ(Utility::toInt("100abc", 42), 42);
    CHECK_EQ(Utility::toInt("abc", 42), 42);
    CHECK_EQ(Utility::toInt("", 42), 42);

    // Values beyond int range fall back rather than wrapping.
    CHECK_EQ(Utility::toInt("99999999999999999999", 42), 42);
}

TEST_CASE(UT_005, "Utility::toBool understands the documented spellings")
{
    CHECK_TRUE(Utility::toBool("true", false));
    CHECK_TRUE(Utility::toBool("Enabled", false));
    CHECK_TRUE(Utility::toBool("YES", false));
    CHECK_TRUE(Utility::toBool("1", false));

    CHECK_FALSE(Utility::toBool("false", true));
    CHECK_FALSE(Utility::toBool("Disabled", true));
    CHECK_FALSE(Utility::toBool("no", true));
    CHECK_FALSE(Utility::toBool("0", true));

    // Anything unrecognised keeps the caller's default.
    CHECK_TRUE(Utility::toBool("maybe", true));
    CHECK_FALSE(Utility::toBool("maybe", false));
}

TEST_CASE(UT_006, "Utility::center pads shorter text and leaves long text")
{
    CHECK_EQ(Utility::center("abc", 9), std::string("   abc"));
    CHECK_EQ(Utility::center("abc", 3), std::string("abc"));

    // Text wider than the field must not be cropped.
    CHECK_EQ(Utility::center("abcdef", 4), std::string("abcdef"));
}

TEST_CASE(UT_007, "Utility::fileExists distinguishes present from absent")
{
    const std::string path =
        TestFramework::writeTempFile("utility-exists.txt", "present");

    CHECK_TRUE(Utility::fileExists(path));
    CHECK_FALSE(Utility::fileExists(path + ".missing"));
    CHECK_FALSE(Utility::fileExists(""));
}

TEST_CASE(UT_008, "Utility::currentDateTime returns a full timestamp")
{
    const std::string stamp = Utility::currentDateTime();

    // Expected shape: YYYY-MM-DD HH:MM:SS
    CHECK_EQ(stamp.size(), static_cast<std::size_t>(19));
    CHECK_EQ(stamp[4], '-');
    CHECK_EQ(stamp[7], '-');
    CHECK_EQ(stamp[10], ' ');
    CHECK_EQ(stamp[13], ':');
    CHECK_EQ(stamp[16], ':');
}

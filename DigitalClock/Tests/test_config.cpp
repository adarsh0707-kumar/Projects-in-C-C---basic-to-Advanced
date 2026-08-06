/******************************************************************************
 * @file test_config.cpp
 * @brief Tests for the ConfigurationManager service.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Covers TC-009 (configuration loading), TC-010 (default configuration),
 * TC-011 (invalid configuration handling), TC-021 (empty configuration file)
 * and TC-022 (invalid refresh interval) from the Testing Report.
 ******************************************************************************/

#include "TestFramework.hpp"

#include "ConfigurationManager.hpp"

TEST_CASE(TC_009, "Verify configuration loading")
{
    // The configuration shipped with the project must load as written.
    ConfigurationManager config;

    CHECK_TRUE(config.load("Config/config.ini"));
    CHECK_TRUE(config.size() > 0);

    CHECK_EQ(config.getValue("TimeFormat"), std::string("24"));
    CHECK_EQ(config.getValue("DateFormat"), std::string("LONG"));
    CHECK_EQ(config.getValue("Theme"), std::string("Dark"));
    CHECK_EQ(config.getInt("RefreshInterval"), 1000);
    CHECK_TRUE(config.getBool("Logging"));

    // Keys are matched case-insensitively.
    CHECK_EQ(config.getValue("THEME"), std::string("Dark"));
    CHECK_EQ(config.getValue("theme"), std::string("Dark"));

    CHECK_EQ(config.filePath(), std::string("Config/config.ini"));
}

TEST_CASE(TC_010, "Verify default configuration")
{
    ConfigurationManager config;

    // A file that does not exist must report failure, not throw.
    CHECK_FALSE(config.load("Config/definitely-not-here.ini"));
    CHECK_EQ(config.size(), static_cast<std::size_t>(0));

    // Every getter must then fall back to the supplied default.
    CHECK_EQ(config.getValue("Theme", "Dark"), std::string("Dark"));
    CHECK_EQ(config.getInt("RefreshInterval", 1000), 1000);
    CHECK_TRUE(config.getBool("Logging", true));
    CHECK_FALSE(config.getBool("ConsoleLog", false));

    // A getter without a default returns an empty string rather than failing.
    CHECK_EQ(config.getValue("Theme"), std::string(""));
    CHECK_FALSE(config.hasValue("Theme"));
}

TEST_CASE(TC_011, "Verify invalid configuration handling")
{
    const std::string path = TestFramework::writeTempFile(
        "invalid.ini",
        "# a comment\n"
        "; another comment\n"
        "this line has no separator\n"
        "=valueWithoutKey\n"
        "\n"
        "   \n"
        "Theme=Blue\n"
        "RefreshInterval=notanumber\n"
        "TrailingSpaces   =   Dark   \n");

    ConfigurationManager config;

    // Malformed lines are skipped; the file as a whole still loads.
    CHECK_TRUE(config.load(path));

    // Valid entries either side of the malformed ones survive.
    CHECK_EQ(config.getValue("Theme"), std::string("Blue"));
    CHECK_EQ(config.getValue("TrailingSpaces"), std::string("Dark"));

    // A non-numeric value falls back rather than throwing.
    CHECK_EQ(config.getInt("RefreshInterval", 1000), 1000);

    // Nothing was invented from the malformed lines.
    CHECK_FALSE(config.hasValue("this line has no separator"));
    CHECK_FALSE(config.hasValue(""));
}

TEST_CASE(TC_021, "Empty configuration file applies default settings")
{
    const std::string path = TestFramework::writeTempFile("empty.ini", "");

    ConfigurationManager config;

    // An empty file is a valid file: the load succeeds with nothing in it.
    CHECK_TRUE(config.load(path));
    CHECK_EQ(config.size(), static_cast<std::size_t>(0));

    CHECK_EQ(config.getValue("Theme", "Dark"), std::string("Dark"));
    CHECK_EQ(config.getInt("RefreshInterval", 1000), 1000);

    // A file of nothing but comments behaves the same way.
    const std::string commentsOnly =
        TestFramework::writeTempFile("comments.ini", "# nothing here\n\n; nor here\n");

    CHECK_TRUE(config.load(commentsOnly));
    CHECK_EQ(config.size(), static_cast<std::size_t>(0));
    CHECK_EQ(config.getValue("Theme", "Dark"), std::string("Dark"));
}

TEST_CASE(TC_022, "Invalid refresh interval is rejected by validation")
{
    const std::string path = TestFramework::writeTempFile(
        "intervals.ini",
        "TooSmall=0\n"
        "Negative=-500\n"
        "TooLarge=999999\n"
        "NotANumber=fast\n"
        "Valid=250\n");

    ConfigurationManager config;

    CHECK_TRUE(config.load(path));

    const int minimum = 50;
    const int maximum = 60000;

    // Values outside the accepted range fall back to the default.
    CHECK_EQ(config.getInt("TooSmall", 1000, minimum, maximum), 1000);
    CHECK_EQ(config.getInt("Negative", 1000, minimum, maximum), 1000);
    CHECK_EQ(config.getInt("TooLarge", 1000, minimum, maximum), 1000);
    CHECK_EQ(config.getInt("NotANumber", 1000, minimum, maximum), 1000);

    // A value inside the range is accepted unchanged.
    CHECK_EQ(config.getInt("Valid", 1000, minimum, maximum), 250);

    // The range boundaries themselves are accepted.
    CHECK_EQ(config.getInt("TooSmall", minimum, 0, maximum), 0);
}

TEST_CASE(UT_040, "ConfigurationManager reads sectioned files")
{
    const std::string path = TestFramework::writeTempFile(
        "sections.ini",
        "[DARK]\n"
        "Foreground=White\n"
        "Header=Cyan\n"
        "[LIGHT]\n"
        "Foreground=Black\n");

    ConfigurationManager config;

    CHECK_TRUE(config.load(path));

    // Sectioned keys are reachable by their qualified name.
    CHECK_EQ(config.getValue("DARK.Header"), std::string("Cyan"));
    CHECK_EQ(config.getValue("LIGHT.Foreground"), std::string("Black"));

    // The unqualified name holds the last value seen, as in a flat file.
    CHECK_EQ(config.getValue("Foreground"), std::string("Black"));
}

TEST_CASE(UT_041, "ConfigurationManager writes settings back to disk")
{
    const std::string path = TestFramework::writeTempFile(
        "roundtrip.ini",
        "Theme=Dark\nRefreshInterval=1000\n");

    ConfigurationManager config;

    CHECK_TRUE(config.load(path));

    CHECK_TRUE(config.setValue("Theme", "Blue"));
    CHECK_TRUE(config.setValue("NewKey", "NewValue"));
    CHECK_FALSE(config.setValue("", "rejected"));

    CHECK_TRUE(config.save());

    // Reloading must produce exactly what was saved.
    ConfigurationManager reloaded;

    CHECK_TRUE(reloaded.load(path));
    CHECK_EQ(reloaded.getValue("Theme"), std::string("Blue"));
    CHECK_EQ(reloaded.getValue("NewKey"), std::string("NewValue"));
    CHECK_EQ(reloaded.getInt("RefreshInterval"), 1000);

    // save() without a prior load has no destination and must report failure.
    ConfigurationManager unsaved;
    CHECK_FALSE(unsaved.save());
}

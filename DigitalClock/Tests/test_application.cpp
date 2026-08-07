/******************************************************************************
 * @file test_application.cpp
 * @brief Tests for the Application lifecycle.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Covers TC-001 (application startup), TC-002 (module initialization) and
 * TC-020 (graceful shutdown) from the Testing Report.
 *
 * These tests drive the real Application, so std::cout is captured for the
 * duration: the presentation layer writes escape sequences that would
 * otherwise scramble the test report.
 ******************************************************************************/

#include "TestFramework.hpp"

#include <filesystem>

#include "Application.hpp"
#include "Version.hpp"

TEST_CASE(TC_001, "Verify successful application startup")
{
    Application application;

    bool started = false;

    {
        TestFramework::OutputCapture capture;

        started = application.initialize("Config/config.ini");

        application.shutdown();
    }

    CHECK_TRUE(started);

    // The configuration shipped with the project drives the refresh interval.
    CHECK_EQ(application.refreshInterval(), 1000);

    // Startup must be recorded in the log (FR-007 acceptance criteria).
    CHECK_TRUE(std::filesystem::exists("Logs/application.log"));
}

TEST_CASE(TC_002, "Verify initialization of all modules")
{
    Application application;

    {
        TestFramework::OutputCapture capture;

        CHECK_TRUE(application.initialize("Config/config.ini"));

        // A frame must render straight after startup, which exercises the
        // clock, date, formatter, theme, screen and console together.
        application.renderFrame();

        const std::string drawn = capture.text();

        // The composed frame carries the date and the status fields.
        CHECK_CONTAINS(drawn, "Theme");
        CHECK_CONTAINS(drawn, "Refresh Rate");
        CHECK_CONTAINS(drawn, "Status");
        CHECK_CONTAINS(drawn, "Press Q or Ctrl+C to Exit");

        application.shutdown();
    }

    // Startup with a missing configuration must still succeed on defaults.
    Application withDefaults;

    {
        TestFramework::OutputCapture capture;

        CHECK_TRUE(withDefaults.initialize("Config/no-such-config.ini"));

        withDefaults.shutdown();
    }

    CHECK_EQ(withDefaults.refreshInterval(), 1000);
}

TEST_CASE(TC_020, "Verify graceful shutdown")
{
    Application application;

    {
        TestFramework::OutputCapture capture;

        CHECK_TRUE(application.initialize("Config/config.ini"));

        application.renderFrame();

        // Asking the loop to stop must be observable before shutdown.
        application.requestStop();
        CHECK_FALSE(application.isRunning());

        application.shutdown();

        // Repeated shutdown must be safe (FR-009).
        application.shutdown();
        application.shutdown();
    }

    CHECK_FALSE(application.isRunning());

    // run() on an application that was never initialized reports the failure
    // rather than entering an endless loop.
    Application uninitialized;

    CHECK_EQ(uninitialized.run(), Application::EXIT_STARTUP_FAILED);
}

TEST_CASE(UT_090, "Application validates the configured refresh interval")
{
    // A refresh interval outside the accepted range falls back to the default.
    const std::string path = TestFramework::writeTempFile(
        "app-interval.ini",
        "Theme=Dark\n"
        "RefreshInterval=0\n"
        "Logging=Disabled\n");

    Application application;

    {
        TestFramework::OutputCapture capture;

        CHECK_TRUE(application.initialize(path));

        application.shutdown();
    }

    CHECK_EQ(application.refreshInterval(), 1000);

    // A value inside the range is honoured.
    const std::string valid = TestFramework::writeTempFile(
        "app-interval-valid.ini",
        "RefreshInterval=250\n"
        "Logging=Disabled\n");

    Application fast;

    {
        TestFramework::OutputCapture capture;

        CHECK_TRUE(fast.initialize(valid));

        fast.shutdown();
    }

    CHECK_EQ(fast.refreshInterval(), 250);
}

TEST_CASE(UT_091, "Application honours the configured formats and theme")
{
    const std::string path = TestFramework::writeTempFile(
        "app-formats.ini",
        "TimeFormat=12\n"
        "DateFormat=YYYY-MM-DD\n"
        "Theme=HighContrast\n"
        "RefreshInterval=1000\n"
        "Logging=Disabled\n");

    Application application;

    std::string drawn;

    {
        TestFramework::OutputCapture capture;

        CHECK_TRUE(application.initialize(path));

        application.renderFrame();

        drawn = capture.text();

        application.shutdown();
    }

    // The 12-hour format puts a meridiem indicator on the clock line.
    const bool hasMeridiem =
        drawn.find(" AM") != std::string::npos ||
        drawn.find(" PM") != std::string::npos;

    CHECK_TRUE(hasMeridiem);

    // The status bar reports the theme that was actually loaded.
    CHECK_CONTAINS(drawn, "HighContrast");
}

TEST_CASE(UT_092, "Application reports its identity")
{
    CHECK_EQ(Application::defaultConfigPath(), std::string("Config/config.ini"));
    CHECK_EQ(Application::EXIT_OK, 0);
    CHECK_EQ(Application::EXIT_STARTUP_FAILED, 1);

    CHECK_EQ(Version::VERSION, std::string("1.1.0"));
    CHECK_FALSE(Version::NAME.empty());
    CHECK_FALSE(Version::AUTHOR.empty());
}

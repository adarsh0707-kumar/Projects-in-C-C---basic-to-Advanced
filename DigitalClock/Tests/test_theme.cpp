/******************************************************************************
 * @file test_theme.cpp
 * @brief Tests for the ThemeManager service and Theme primitives.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Covers TC-012 (theme loading), TC-013 (missing theme handling) and TC-025
 * (unsupported theme name) from the Testing Report.
 ******************************************************************************/

#include "TestFramework.hpp"

#include "Theme.hpp"
#include "ThemeManager.hpp"

TEST_CASE(TC_012, "Verify theme loading")
{
    ThemeManager theme;

    // Every theme named in the User Manual must load from Resources/themes.
    for (const std::string &name : ThemeManager::availableThemes())
    {
        if (!theme.loadTheme(name))
        {
            FAIL_TEST("theme '" + name + "' failed to load");
            continue;
        }

        CHECK_EQ(theme.currentTheme(), name);
    }

    CHECK_TRUE(theme.loadTheme("Dark"));
    CHECK_EQ(theme.currentTheme(), std::string("Dark"));

    // The dark theme defines a distinct colour for each major element.
    CHECK_TRUE(
        theme.color(ThemeManager::Element::Header) == Theme::Color::Cyan);
    CHECK_TRUE(
        theme.color(ThemeManager::Element::Time) == Theme::Color::BrightGreen);
    CHECK_TRUE(
        theme.color(ThemeManager::Element::Date) == Theme::Color::Yellow);

    CHECK_TRUE(theme.applyTheme());
    CHECK_TRUE(theme.isApplied());

    // Theme names are matched without regard to case.
    CHECK_TRUE(theme.loadTheme("dark"));
    CHECK_TRUE(theme.loadTheme("HIGHCONTRAST"));
}

TEST_CASE(TC_013, "Verify missing theme handling")
{
    ThemeManager theme;

    CHECK_TRUE(theme.loadTheme("Blue"));
    CHECK_EQ(theme.currentTheme(), std::string("Blue"));

    // A theme file that is not present must fall back to the default.
    CHECK_FALSE(theme.loadTheme("Midnight"));
    CHECK_EQ(theme.currentTheme(), std::string("Default"));

    // The fallback must still supply a complete, usable colour set.
    CHECK_TRUE(
        theme.color(ThemeManager::Element::Header) == Theme::Color::Cyan);
    CHECK_TRUE(
        theme.color(ThemeManager::Element::Time) == Theme::Color::Green);
    CHECK_TRUE(
        theme.color(ThemeManager::Element::Date) == Theme::Color::Yellow);

    CHECK_FALSE(theme.colorFor(ThemeManager::Element::Time).empty());
}

TEST_CASE(TC_025, "Unsupported theme name loads the default theme")
{
    ThemeManager theme;

    // Empty, whitespace and nonsense names all resolve to the default.
    CHECK_FALSE(theme.loadTheme(""));
    CHECK_EQ(theme.currentTheme(), std::string("Default"));

    CHECK_FALSE(theme.loadTheme("   "));
    CHECK_EQ(theme.currentTheme(), std::string("Default"));

    CHECK_FALSE(theme.loadTheme("NotAThemeAtAll"));
    CHECK_EQ(theme.currentTheme(), std::string("Default"));

    // A path traversal attempt is treated as an unknown name, not followed.
    CHECK_FALSE(theme.loadTheme("../../etc/passwd"));
    CHECK_EQ(theme.currentTheme(), std::string("Default"));

    // The application remains renderable throughout.
    CHECK_TRUE(theme.applyTheme());
    CHECK_FALSE(theme.reset().empty());
}

TEST_CASE(UT_060, "ThemeManager honours the colour switch")
{
    ThemeManager theme;

    CHECK_TRUE(theme.isColorEnabled());
    CHECK_FALSE(theme.colorFor(ThemeManager::Element::Time).empty());
    CHECK_FALSE(theme.reset().empty());

    // With colour disabled, nothing may be emitted into the output stream.
    theme.setColorEnabled(false);

    CHECK_FALSE(theme.isColorEnabled());
    CHECK_EQ(theme.colorFor(ThemeManager::Element::Time), std::string(""));
    CHECK_EQ(theme.reset(), std::string(""));

    // applyTheme() reports that the theme has no visible effect.
    CHECK_FALSE(theme.applyTheme());

    theme.setColorEnabled(true);
    CHECK_TRUE(theme.applyTheme());
}

TEST_CASE(UT_061, "Theme maps colour names to ANSI sequences")
{
    CHECK_EQ(Theme::foreground(Theme::Color::Red), std::string("\033[31m"));
    CHECK_EQ(Theme::foreground(Theme::Color::Green), std::string("\033[32m"));
    CHECK_EQ(
        Theme::foreground(Theme::Color::BrightCyan), std::string("\033[96m"));
    CHECK_EQ(Theme::background(Theme::Color::Blue), std::string("\033[44m"));
    CHECK_EQ(Theme::style(Theme::Style::Bold), std::string("\033[1m"));
    CHECK_EQ(Theme::reset(), std::string("\033[0m"));

    // Names are parsed case-insensitively.
    CHECK_TRUE(Theme::colorFromName("cyan") == Theme::Color::Cyan);
    CHECK_TRUE(Theme::colorFromName("CYAN") == Theme::Color::Cyan);
    CHECK_TRUE(Theme::colorFromName(" Yellow ") == Theme::Color::Yellow);

    // Spellings used by the bundled theme files resolve sensibly.
    CHECK_TRUE(Theme::colorFromName("DarkBlue") == Theme::Color::Blue);
    CHECK_TRUE(Theme::colorFromName("DarkGray") == Theme::Color::BrightBlack);
    CHECK_TRUE(Theme::colorFromName("Grey") == Theme::Color::BrightBlack);

    // Unknown names keep the caller's fallback.
    CHECK_TRUE(
        Theme::colorFromName("chartreuse", Theme::Color::White) ==
        Theme::Color::White);

    CHECK_EQ(Theme::colorName(Theme::Color::Cyan), std::string("Cyan"));
}

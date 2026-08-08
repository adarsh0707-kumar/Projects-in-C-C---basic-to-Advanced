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

#include <vector>

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

TEST_CASE(UT_062, "Every theme colour and style maps to a distinct sequence")
{
    /*
    Coverage measurement showed this table exercised only by sample, leaving
    half of it unrun. It is not incidental code: every entry is a colour a
    theme file can name, so an entry that mapped to the wrong sequence -- or
    to another entry's sequence -- would silently mis-render a theme.
    */
    static const Theme::Color COLORS[] = {
        Theme::Color::Default,
        Theme::Color::Black,   Theme::Color::Red,
        Theme::Color::Green,   Theme::Color::Yellow,
        Theme::Color::Blue,    Theme::Color::Magenta,
        Theme::Color::Cyan,    Theme::Color::White,
        Theme::Color::BrightBlack,   Theme::Color::BrightRed,
        Theme::Color::BrightGreen,   Theme::Color::BrightYellow,
        Theme::Color::BrightBlue,    Theme::Color::BrightMagenta,
        Theme::Color::BrightCyan,    Theme::Color::BrightWhite};

    std::vector<std::string> foregrounds;
    std::vector<std::string> backgrounds;

    for (const Theme::Color color : COLORS)
    {
        const std::string fg = Theme::foreground(color);
        const std::string bg = Theme::background(color);

        // Every entry produces a real escape sequence.
        CHECK_FALSE(fg.empty());
        CHECK_FALSE(bg.empty());
        CHECK_CONTAINS(fg, "\033[");
        CHECK_CONTAINS(bg, "\033[");

        // Foreground and background are never confused for one another.
        CHECK_NE(fg, bg);

        // Each colour's name round-trips back to the same colour.
        const std::string name = Theme::colorName(color);
        CHECK_FALSE(name.empty());
        CHECK_TRUE(Theme::colorFromName(name) == color);

        foregrounds.push_back(fg);
        backgrounds.push_back(bg);
    }

    // No two colours share a sequence, which is what would make one colour
    // silently render as another.
    for (std::size_t i = 0; i < foregrounds.size(); ++i)
    {
        for (std::size_t j = i + 1; j < foregrounds.size(); ++j)
        {
            CHECK_NE(foregrounds[i], foregrounds[j]);
            CHECK_NE(backgrounds[i], backgrounds[j]);
        }
    }

    static const Theme::Style STYLES[] = {
        Theme::Style::Normal,    Theme::Style::Bold,
        Theme::Style::Dim,       Theme::Style::Italic,
        Theme::Style::Underline, Theme::Style::Reverse};

    std::vector<std::string> styles;

    for (const Theme::Style style : STYLES)
    {
        const std::string sequence = Theme::style(style);

        CHECK_FALSE(sequence.empty());
        CHECK_CONTAINS(sequence, "\033[");

        styles.push_back(sequence);
    }

    for (std::size_t i = 0; i < styles.size(); ++i)
    {
        for (std::size_t j = i + 1; j < styles.size(); ++j)
            CHECK_NE(styles[i], styles[j]);
    }
}

/******************************************************************************
 * @file test_display.cpp
 * @brief Tests for the presentation layer.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Screen composition is verified through Screen::compose(), which returns the
 * lines that would be drawn without emitting cursor control sequences. That
 * makes the layout testable without a terminal, and covers the FR-008
 * requirement that a redraw must not duplicate output.
 ******************************************************************************/

#include "TestFramework.hpp"

#include <algorithm>

#include "Display.hpp"
#include "Screen.hpp"
#include "StatusBar.hpp"
#include "ThemeManager.hpp"

namespace
{
    /**
     * @brief Counts how many composed lines contain @p needle.
     */
    std::size_t countContaining(const std::vector<std::string> &lines,
                                const std::string &needle)
    {
        return static_cast<std::size_t>(
            std::count_if(
                lines.begin(),
                lines.end(),
                [&needle](const std::string &line)
                { return line.find(needle) != std::string::npos; }));
    }
}

TEST_CASE(UT_080, "Screen composes the documented layout")
{
    Screen screen;

    screen.resize(60, 30);
    screen.setTime("10 : 45 : 30 AM");
    screen.setDate("Monday, 03 August 2026");
    screen.setStatusLines({"Theme : Dark", "Status : Running"});
    screen.setFooterHint("Press Q or Ctrl+C to Exit");

    const std::vector<std::string> lines = screen.compose();

    CHECK_TRUE(lines.size() > 0);

    // Every element from the User Manual layout must appear exactly once.
    CHECK_EQ(countContaining(lines, "10 : 45 : 30 AM"),
             static_cast<std::size_t>(1));
    CHECK_EQ(countContaining(lines, "Monday, 03 August 2026"),
             static_cast<std::size_t>(1));
    CHECK_EQ(countContaining(lines, "Theme : Dark"),
             static_cast<std::size_t>(1));
    CHECK_EQ(countContaining(lines, "Status : Running"),
             static_cast<std::size_t>(1));
    CHECK_EQ(countContaining(lines, "Press Q or Ctrl+C to Exit"),
             static_cast<std::size_t>(1));

    // The banner sits above the clock.
    const std::size_t bannerRow = static_cast<std::size_t>(
        std::distance(
            lines.begin(),
            std::find_if(
                lines.begin(),
                lines.end(),
                [](const std::string &line)
                { return !line.empty(); })));

    const std::size_t timeRow = static_cast<std::size_t>(
        std::distance(
            lines.begin(),
            std::find_if(
                lines.begin(),
                lines.end(),
                [](const std::string &line)
                {
                    return line.find("10 : 45 : 30 AM") != std::string::npos;
                })));

    CHECK_TRUE(bannerRow < timeRow);

    // Rules span the full layout width.
    const std::string horizontal(60, '-');
    CHECK_EQ(countContaining(lines, horizontal), static_cast<std::size_t>(1));
}

TEST_CASE(UT_081, "Repeated composition produces identical output")
{
    Screen screen;

    screen.resize(80, 30);
    screen.setTime("09 : 42 : 16");
    screen.setDate("Monday, 03 August 2026");
    screen.setStatusLines({"Theme : Dark"});

    const std::vector<std::string> first = screen.compose();
    const std::vector<std::string> second = screen.compose();

    // FR-008: a redraw must not accumulate or duplicate content.
    CHECK_EQ(first.size(), second.size());
    CHECK_TRUE(first == second);

    // Updating the time replaces it rather than appending.
    screen.setTime("09 : 42 : 17");

    const std::vector<std::string> third = screen.compose();

    CHECK_EQ(third.size(), first.size());
    CHECK_EQ(countContaining(third, "09 : 42 : 16"),
             static_cast<std::size_t>(0));
    CHECK_EQ(countContaining(third, "09 : 42 : 17"),
             static_cast<std::size_t>(1));
}

TEST_CASE(UT_082, "Screen adapts to the terminal size")
{
    Screen screen;

    // A tall terminal shows the banner.
    screen.resize(80, 40);
    CHECK_TRUE(screen.banner().isVisible());
    CHECK_EQ(screen.width(), 80);
    CHECK_EQ(screen.height(), 40);

    // A short terminal drops the banner to keep the clock visible.
    screen.resize(80, 10);
    CHECK_FALSE(screen.banner().isVisible());

    const std::vector<std::string> compact = screen.compose();
    CHECK_TRUE(compact.size() > 0);

    // An unusably narrow terminal is clamped rather than producing bad output.
    screen.resize(2, 40);
    CHECK_TRUE(screen.width() >= 20);

    // reset() returns the default layout.
    screen.reset();
    CHECK_EQ(screen.width(), 80);
    CHECK_EQ(screen.height(), 24);
    CHECK_TRUE(screen.banner().isVisible());
}

TEST_CASE(UT_083, "StatusBar renders aligned fields in insertion order")
{
    StatusBar status;

    status.setField("Theme", "Dark");
    status.setField("Refresh Rate", "1 Second");
    status.setField("Status", "Running");

    std::vector<std::string> lines = status.lines();

    CHECK_EQ(lines.size(), static_cast<std::size_t>(3));

    // Labels are padded to a common width so the separators line up.
    const std::size_t separator = lines[0].find(" : ");

    CHECK_TRUE(separator != std::string::npos);
    CHECK_EQ(lines[1].find(" : "), separator);
    CHECK_EQ(lines[2].find(" : "), separator);

    CHECK_CONTAINS(lines[0], "Theme");
    CHECK_CONTAINS(lines[0], "Dark");

    // Updating a field replaces its value rather than adding a row.
    status.setField("Status", "Stopped");

    lines = status.lines();

    CHECK_EQ(lines.size(), static_cast<std::size_t>(3));
    CHECK_CONTAINS(lines[2], "Stopped");
    CHECK_EQ(status.field("Status"), std::string("Stopped"));

    // A transient message is appended after the fields.
    status.setMessage("Theme file missing");

    lines = status.lines();

    CHECK_EQ(lines.size(), static_cast<std::size_t>(4));
    CHECK_EQ(lines[3], std::string("Theme file missing"));

    status.clear();

    CHECK_EQ(status.lines().size(), static_cast<std::size_t>(0));
    CHECK_EQ(status.field("Theme"), std::string(""));
}

TEST_CASE(UT_084, "Screen omits colour codes when the theme disables colour")
{
    ThemeManager theme;
    Screen screen;

    theme.setColorEnabled(false);
    screen.attach(theme);

    screen.resize(60, 30);
    screen.setTime("09 : 42 : 16");
    screen.setDate("Monday, 03 August 2026");

    TestFramework::OutputCapture capture;
    screen.draw();

    const std::string drawn = capture.text();

    // Cursor control is still emitted, but no colour sequences.
    CHECK_CONTAINS(drawn, "09 : 42 : 16");
    CHECK_NOT_CONTAINS(drawn, "\033[32m");
    CHECK_NOT_CONTAINS(drawn, "\033[36m");
    CHECK_NOT_CONTAINS(drawn, "\033[0m");
}

TEST_CASE(UT_085, "Screen emits colour codes when the theme enables colour")
{
    ThemeManager theme;
    Screen screen;

    CHECK_TRUE(theme.loadTheme("Dark"));
    theme.setColorEnabled(true);
    screen.attach(theme);

    screen.resize(60, 30);
    screen.setTime("09 : 42 : 16");
    screen.setDate("Monday, 03 August 2026");

    TestFramework::OutputCapture capture;
    screen.draw();

    const std::string drawn = capture.text();

    CHECK_CONTAINS(drawn, "09 : 42 : 16");
    CHECK_CONTAINS(drawn, "\033[0m");

    // The dark theme draws the time in bright green.
    CHECK_CONTAINS(drawn, "\033[92m");
}

TEST_CASE(UT_086, "Display forwards formatted text to the layout")
{
    Display display;

    // renderClock and renderDate store text without drawing it.
    display.renderClock("09 : 42 : 16");
    display.renderDate("Monday, 03 August 2026");
    display.setStatusField("Theme", "Dark");
    display.setMessage("Ready");

    const std::vector<std::string> lines = display.screen().compose();

    CHECK_EQ(countContaining(lines, "09 : 42 : 16"),
             static_cast<std::size_t>(1));
    CHECK_EQ(countContaining(lines, "Monday, 03 August 2026"),
             static_cast<std::size_t>(1));

    CHECK_EQ(display.statusBar().field("Theme"), std::string("Dark"));
    CHECK_EQ(display.statusBar().message(), std::string("Ready"));

    // shutdown() before initialize() must be safe.
    display.shutdown();
}

TEST_CASE(TC_082, "The status bar can be hidden and shown again")
{
    StatusBar status;

    status.setField("Theme", "Dark");
    status.setField("Mode", "Clock");
    status.setMessage("Ready");

    CHECK_TRUE(status.isVisible());
    CHECK_EQ(status.lines().size(), static_cast<std::size_t>(3));

    /*
    Hiding must suppress the rows without discarding them. Clearing the
    fields instead would make hiding a destructive operation, and showing the
    bar again would produce an empty one.
    */
    status.setVisible(false);

    CHECK_FALSE(status.isVisible());
    CHECK_TRUE(status.lines().empty());

    // The fields survived being hidden.
    CHECK_EQ(status.field("Theme"), std::string("Dark"));
    CHECK_EQ(status.message(), std::string("Ready"));

    status.setVisible(true);

    CHECK_TRUE(status.isVisible());
    CHECK_EQ(status.lines().size(), static_cast<std::size_t>(3));

    // A field with no name is not a field, and must not create a blank row.
    status.setField("", "ignored");
    status.setField("   ", "ignored");

    CHECK_EQ(status.lines().size(), static_cast<std::size_t>(3));

    // An unknown field reads as empty rather than inventing a value.
    CHECK_EQ(status.field("NotAField"), std::string(""));
}

TEST_CASE(TC_083, "StatusBar::show writes its rows, and nothing when hidden")
{
    StatusBar status;

    status.setField("Theme", "Blue");
    status.setField("Status", "Running");

    std::string drawn;

    {
        TestFramework::OutputCapture capture;

        status.show();

        drawn = capture.text();
    }

    CHECK_CONTAINS(drawn, "Theme");
    CHECK_CONTAINS(drawn, "Blue");
    CHECK_CONTAINS(drawn, "Running");

    // Labels are padded to a common width so the colons form a column.
    CHECK_CONTAINS(drawn, "Theme  : Blue");

    std::string hidden;

    {
        TestFramework::OutputCapture capture;

        status.setVisible(false);
        status.show();

        hidden = capture.text();
    }

    // Hidden means nothing is emitted at all, not a blank line.
    CHECK_EQ(hidden, std::string(""));
}

TEST_CASE(TC_084, "Display's render aliases all draw the same frame")
{
    /*
    render(), update() and renderScreen() are documented as equivalent (API
    Documentation, section 4.4). They are one-line forwards, which is exactly
    why they are worth asserting: an alias that quietly stopped forwarding
    would leave a caller with a frame that never redraws, and nothing else in
    the suite calls them.
    */
    Display display;

    display.renderClock("11 : 05 : 00");
    display.renderDate("Tuesday, 04 August 2026");
    display.setStatusField("Theme", "Green");

    std::string viaRenderScreen;
    std::string viaRender;
    std::string viaUpdate;

    {
        TestFramework::OutputCapture capture;
        display.renderScreen();
        viaRenderScreen = capture.text();
    }

    {
        TestFramework::OutputCapture capture;
        display.render();
        viaRender = capture.text();
    }

    {
        TestFramework::OutputCapture capture;
        display.update();
        viaUpdate = capture.text();
    }

    CHECK_FALSE(viaRenderScreen.empty());
    CHECK_CONTAINS(viaRenderScreen, "11 : 05 : 00");

    // Identical output, not merely similar.
    CHECK_EQ(viaRender, viaRenderScreen);
    CHECK_EQ(viaUpdate, viaRenderScreen);
}

TEST_CASE(TC_085, "Display::clear and refresh reach the console")
{
    Display display;

    std::string cleared;

    {
        TestFramework::OutputCapture capture;

        display.clear();

        cleared = capture.text();
    }

    // 2J erases the screen; without the flush it would sit in the buffer.
    CHECK_CONTAINS(cleared, "\033[2J");

    // refresh() on its own emits nothing new; it flushes what is pending.
    {
        TestFramework::OutputCapture capture;

        display.refresh();

        CHECK_EQ(capture.text(), std::string(""));
    }

    // readKey() without an initialised console reports no key rather than
    // blocking or reading from a terminal that was never prepared.
    CHECK_EQ(display.readKey(), -1);
}

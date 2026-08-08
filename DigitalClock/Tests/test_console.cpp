/******************************************************************************
 * @file test_console.cpp
 * @brief Direct tests for the Console presentation component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Covers TC-061 to TC-065, closing KI-009.
 *
 * Console was previously exercised only through Display and Application. That
 * left its own behaviour unasserted, because the parts worth testing are
 * exactly the parts that depend on being attached to a terminal: colour
 * detection, raw input mode, and reading a keystroke.
 *
 * These tests open a pseudo-terminal and point the process at it, so isatty()
 * is true and the real terminal paths execute rather than the redirected-output
 * fallbacks. That is the only way to reach them; a test with piped stdio
 * measures the fallback and reports success.
 *
 * The whole file is POSIX-only. On Windows the equivalent paths are covered by
 * the CI smoke step, which runs the application with --once on a real console.
 ******************************************************************************/

#include "TestFramework.hpp"

#include "Console.hpp"

#ifndef _WIN32

#include <cstdlib>
#include <string>
#include <termios.h>
#include <unistd.h>

#include "PseudoTerminal.hpp"

using TestSupport::PseudoTerminal;

TEST_CASE(TC_061, "Verify the console detects a terminal and emits colour")
{
    // Console probes for colour at construction, so the pty must exist first.
    PseudoTerminal terminal;

    if (!terminal.isReady())
    {
        FAIL_TEST("could not open a pseudo-terminal");
        return;
    }

    // NO_COLOR is honoured, and would otherwise mask the result.
    const char *noColor = std::getenv("NO_COLOR");

    if (noColor != nullptr)
        unsetenv("NO_COLOR");

    {
        Console console;

        // Attached to a terminal, colour is appropriate. This is the branch a
        // piped-stdio test can never reach.
        CHECK_TRUE(console.supportsColor());
    }

    if (noColor != nullptr)
        setenv("NO_COLOR", noColor, 1);
}

TEST_CASE(TC_062, "Verify NO_COLOR suppresses colour even on a terminal")
{
    PseudoTerminal terminal;

    if (!terminal.isReady())
    {
        FAIL_TEST("could not open a pseudo-terminal");
        return;
    }

    const char *previous = std::getenv("NO_COLOR");
    const std::string saved = (previous != nullptr) ? previous : "";

    setenv("NO_COLOR", "1", 1);

    {
        Console console;

        // The convention is honoured regardless of terminal capability.
        CHECK_FALSE(console.supportsColor());
    }

    if (previous != nullptr)
        setenv("NO_COLOR", saved.c_str(), 1);
    else
        unsetenv("NO_COLOR");
}

TEST_CASE(TC_063, "Verify console output emits the expected escape sequences")
{
    PseudoTerminal terminal;

    if (!terminal.isReady())
    {
        FAIL_TEST("could not open a pseudo-terminal");
        return;
    }

    {
        Console console;

        console.clear();
        console.home();
        console.hideCursor();
        console.setCursor(5, 12);
        console.print("hello");
        console.printLine(" world");
        console.showCursor();
        console.refresh();
    }

    const std::string emitted = terminal.drain();

    // 2J clears, H homes, ?25l and ?25h control the cursor.
    CHECK_CONTAINS(emitted, "\033[2J");
    CHECK_CONTAINS(emitted, "\033[H");
    CHECK_CONTAINS(emitted, "\033[?25l");
    CHECK_CONTAINS(emitted, "\033[?25h");

    // Cursor addressing is one-based row;column.
    CHECK_CONTAINS(emitted, "\033[5;12H");

    CHECK_CONTAINS(emitted, "hello");
    CHECK_CONTAINS(emitted, "world");
}

TEST_CASE(TC_064, "Verify cursor positions are clamped to the visible area")
{
    PseudoTerminal terminal;

    if (!terminal.isReady())
    {
        FAIL_TEST("could not open a pseudo-terminal");
        return;
    }

    {
        Console console;

        // Row and column are one-based; zero and negative values would
        // produce an escape sequence the terminal interprets unpredictably.
        console.setCursor(0, 0);
        console.setCursor(-5, -5);
        console.setCursor(1, 1);

        // Nothing reaches the terminal until the stream is flushed, and this
        // Console is never initialize()d, so its destructor does not do it.
        console.refresh();
    }

    const std::string emitted = terminal.drain();

    CHECK_CONTAINS(emitted, "\033[1;1H");
    CHECK_NOT_CONTAINS(emitted, "\033[0;0H");
    CHECK_NOT_CONTAINS(emitted, "\033[-5;-5H");
}

TEST_CASE(TC_065, "Verify raw input reads keystrokes and restores the terminal")
{
    PseudoTerminal terminal;

    if (!terminal.isReady())
    {
        FAIL_TEST("could not open a pseudo-terminal");
        return;
    }

    // The mode the terminal is in before the console takes it over.
    termios before{};
    const bool haveBefore = (tcgetattr(STDIN_FILENO, &before) == 0);

    CHECK_TRUE(haveBefore);

    {
        Console console;

        // Before initialize() there is no raw mode, so nothing is readable.
        CHECK_EQ(console.readKey(), -1);

        console.initialize();
        CHECK_TRUE(console.isInitialized());

        // Canonical mode and echo must both be off, or keystrokes would wait
        // for Enter and be echoed over the rendered frame.
        termios raw{};
        CHECK_TRUE(tcgetattr(STDIN_FILENO, &raw) == 0);
        CHECK_EQ(static_cast<int>(raw.c_lflag & ICANON), 0);
        CHECK_EQ(static_cast<int>(raw.c_lflag & ECHO), 0);

        // An empty terminal returns immediately rather than blocking, which
        // is what lets the refresh loop poll for keys.
        CHECK_EQ(console.readKey(), -1);

        terminal.sendKeys("q");
        CHECK_EQ(console.readKey(), static_cast<int>('q'));

        terminal.sendKeys("M");
        CHECK_EQ(console.readKey(), static_cast<int>('M'));

        // Once drained, polling is empty again.
        CHECK_EQ(console.readKey(), -1);

        console.shutdown();
        CHECK_FALSE(console.isInitialized());
    }

    // The terminal must be handed back in the state it was found in. A
    // failure here would leave the user's shell without echo.
    termios after{};
    CHECK_TRUE(tcgetattr(STDIN_FILENO, &after) == 0);

    CHECK_EQ(static_cast<int>(after.c_lflag & ICANON),
             static_cast<int>(before.c_lflag & ICANON));
    CHECK_EQ(static_cast<int>(after.c_lflag & ECHO),
             static_cast<int>(before.c_lflag & ECHO));
}

TEST_CASE(UT_130, "Console reports terminal dimensions")
{
    PseudoTerminal terminal;

    if (!terminal.isReady())
    {
        FAIL_TEST("could not open a pseudo-terminal");
        return;
    }

    Console console;

    // openpty gives a terminal with a real size, so these come from ioctl
    // rather than the fallback constants.
    CHECK_TRUE(console.width() > 0);
    CHECK_TRUE(console.height() > 0);

    // Repeated shutdown without initialize() is safe.
    console.shutdown();
    console.shutdown();

    CHECK_FALSE(console.isInitialized());
}

#else // _WIN32

TEST_CASE(UT_131, "Console tests are POSIX-only")
{
    /*
    The pseudo-terminal harness above has no Windows equivalent that is worth
    maintaining here. The Windows console paths are exercised by the CI smoke
    step, which runs the application with --once on a real console.
    */
    Console console;

    console.shutdown();

    CHECK_FALSE(console.isInitialized());
}

#endif // _WIN32

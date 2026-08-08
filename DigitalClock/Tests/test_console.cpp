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
#include <fcntl.h>
#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <vector>

/*
openpty() is declared in different headers depending on the C library: glibc
puts it in <pty.h>, while macOS and the BSDs put it in <util.h>. There is no
portable spelling, so the platform has to be tested for.
*/
#if defined(__linux__)
#include <pty.h>
#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || \
    defined(__NetBSD__)
#include <util.h>
#else
#include <pty.h>
#endif

namespace
{
    /**
     * @class PseudoTerminal
     * @brief Redirects stdin and stdout to a pty for the object's lifetime.
     *
     * Console reads the real file descriptors rather than the C++ streams --
     * it calls isatty(), ioctl() and tcsetattr() directly -- so capturing
     * std::cout is not enough. The descriptors themselves have to be replaced,
     * and put back afterwards, or every later test would write into a closed
     * terminal.
     */
    class PseudoTerminal
    {
    public:
        PseudoTerminal()
            : master(-1), slave(-1), savedIn(-1), savedOut(-1), ready(false)
        {
            /*
            Flush before redirecting. std::cout writes to whatever descriptor 1
            points at when the buffer drains, not when the text was written, so
            anything the test framework has buffered would otherwise be emptied
            into the pty and vanish from the report.
            */
            std::cout.flush();

            if (openpty(&master, &slave, nullptr, nullptr, nullptr) != 0)
                return;

            savedIn = dup(STDIN_FILENO);
            savedOut = dup(STDOUT_FILENO);

            if (savedIn < 0 || savedOut < 0)
                return;

            if (dup2(slave, STDIN_FILENO) < 0 || dup2(slave, STDOUT_FILENO) < 0)
                return;

            ready = true;
        }

        ~PseudoTerminal()
        {
            // Drain anything the test wrote while the pty was still attached,
            // for the same reason.
            std::cout.flush();

            if (savedIn >= 0)
            {
                dup2(savedIn, STDIN_FILENO);
                close(savedIn);
            }

            if (savedOut >= 0)
            {
                dup2(savedOut, STDOUT_FILENO);
                close(savedOut);
            }

            if (slave >= 0)
                close(slave);

            if (master >= 0)
                close(master);
        }

        PseudoTerminal(const PseudoTerminal &) = delete;
        PseudoTerminal &operator=(const PseudoTerminal &) = delete;

        /**
         * @brief Reports whether the terminal was set up successfully.
         */
        bool isReady() const { return ready; }

        /**
         * @brief Writes bytes the application will read as keystrokes.
         */
        void sendKeys(const std::string &keys) const
        {
            const ssize_t written =
                ::write(master, keys.data(), keys.size());

            (void)written;

            // Give the terminal line discipline a moment to deliver them.
            usleep(20000);
        }

        /**
         * @brief Reads whatever the application has written to the terminal.
         */
        std::string drain() const
        {
            std::string collected;
            char buffer[4096];

            // Non-blocking, so an empty terminal returns rather than hanging.
            const int flags = fcntl(master, F_GETFL, 0);
            fcntl(master, F_SETFL, flags | O_NONBLOCK);

            for (int attempt = 0; attempt < 20; ++attempt)
            {
                const ssize_t count = ::read(master, buffer, sizeof(buffer));

                if (count > 0)
                {
                    collected.append(buffer, static_cast<std::size_t>(count));
                    continue;
                }

                if (!collected.empty())
                    break;

                usleep(5000);
            }

            fcntl(master, F_SETFL, flags);

            return collected;
        }

    private:
        int master;
        int slave;
        int savedIn;
        int savedOut;
        bool ready;
    };
}

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

/******************************************************************************
 * @file test_lifecycle.cpp
 * @brief Tests that drive the real refresh loop.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Covers TC-066 to TC-072.
 *
 * Everything else in the suite calls Application's methods directly, which
 * leaves the loop that actually runs the program -- run(), waitForNextFrame()
 * and handleKey() -- unexecuted. That was over half of Application.cpp, and it
 * is not incidental code: it is what runs for every second the application is
 * on screen, and it is where DEF-007 hid. That defect was invisible to unit
 * tests because poll() and minutesUntil() were each correct on their own; only
 * their interaction across frames was wrong.
 *
 * So these tests attach the process to a pseudo-terminal, start run(), and
 * type at it from another thread, exactly as a user would. The assertions are
 * made against what was drawn on the terminal and the state the components
 * were left in.
 *
 * The whole file is POSIX-only, for the reasons given in PseudoTerminal.hpp.
 ******************************************************************************/

#include "TestFramework.hpp"

#include "Application.hpp"

#ifndef _WIN32

#include <chrono>
#include <csignal>
#include <cstdint>
#include <string>
#include <thread>

#include "PseudoTerminal.hpp"

using TestSupport::PseudoTerminal;

namespace
{
    /**
     * @brief Sleeps for a number of milliseconds.
     */
    void pause(int milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    /**
     * @brief Asks the loop to quit, repeatedly.
     *
     * One 'q' is enough in practice. The repeats are insurance: a test that
     * lost its only quit key would not fail, it would hang the whole suite,
     * and a hung suite in CI is far more expensive to diagnose than a failed
     * assertion. run() returns on the first one; the rest are harmless.
     */
    void quit(const PseudoTerminal &terminal)
    {
        for (int attempt = 0; attempt < 3; ++attempt)
        {
            terminal.sendKeys("q");
            pause(50);
        }
    }

    /**
     * @class Typist
     * @brief Runs a script of keystrokes while run() blocks the main thread.
     *
     * Joined before the terminal is torn down, so the script can never write
     * into a destroyed object.
     */
    class Typist
    {
    public:
        template <typename Script>
        explicit Typist(Script script) : worker(script)
        {
        }

        ~Typist() { join(); }

        Typist(const Typist &) = delete;
        Typist &operator=(const Typist &) = delete;

        void join()
        {
            if (worker.joinable())
                worker.join();
        }

    private:
        std::thread worker;
    };

    /** A refresh fast enough that a test is not spent waiting on it. */
    const char *const FAST_LOOP =
        "RefreshInterval=50\n"
        "Theme=Dark\n"
        "Logging=Disabled\n"
        "Alarms=false\n";

    /**
     * @brief Renders a two-digit number, for building HH:MM.
     */
    std::string twoDigits(int value)
    {
        const std::string text = std::to_string(value);

        return (text.size() < 2) ? ("0" + text) : text;
    }
}

TEST_CASE(TC_066, "Verify the refresh loop draws frames and exits on Q")
{
    PseudoTerminal terminal;

    if (!terminal.isReady())
    {
        FAIL_TEST("could not open a pseudo-terminal");
        return;
    }

    const std::string path =
        TestFramework::writeTempFile("loop-basic.ini", FAST_LOOP);

    Application application;

    CHECK_TRUE(application.initialize(path));

    terminal.startCapture();

    Typist typist(
        [&terminal]()
        {
            // Long enough for several frames at a 50 ms interval.
            pause(200);
            quit(terminal);
        });

    const int status = application.run();

    typist.join();

    const std::string drawn = terminal.stopCapture();

    CHECK_EQ(status, Application::EXIT_OK);
    CHECK_FALSE(application.isRunning());

    // initialize() draws nothing, so every one of these came from the loop.
    CHECK_CONTAINS(drawn, "Theme");
    CHECK_CONTAINS(drawn, "Refresh Rate");
    CHECK_CONTAINS(drawn, "Press Q or Ctrl+C to Exit");

    // The screen is redrawn in place rather than cleared and repainted, which
    // is what keeps it from flickering: the cursor goes home each frame.
    CHECK_CONTAINS(drawn, "\033[H");
}

TEST_CASE(TC_067, "Verify M cycles through every mode in the running loop")
{
    PseudoTerminal terminal;

    if (!terminal.isReady())
    {
        FAIL_TEST("could not open a pseudo-terminal");
        return;
    }

    // The world clock is only in the cycle when zones are configured, so the
    // fourth press has somewhere to go.
    const std::string path = TestFramework::writeTempFile(
        "loop-modes.ini",
        "RefreshInterval=50\n"
        "Logging=Disabled\n"
        "Alarms=false\n"
        "TimeZones=UTC | Coordinated, +05:30 | Kolkata\n");

    Application application;

    CHECK_TRUE(application.initialize(path));
    CHECK_TRUE(application.mode() == Application::Mode::Clock);

    terminal.startCapture();

    Typist typist(
        [&terminal]()
        {
            pause(120);

            // Clock -> Stopwatch -> Timer -> World -> Clock.
            for (int press = 0; press < 4; ++press)
            {
                terminal.sendKeys("m");
                pause(120);
            }

            quit(terminal);
        });

    application.run();

    typist.join();

    const std::string drawn = terminal.stopCapture();

    // Four presses from Clock arrive back at Clock.
    CHECK_TRUE(application.mode() == Application::Mode::Clock);

    // Each mode drew its own readout on the way round.
    CHECK_CONTAINS(drawn, "Stopwatch - Stopped");
    CHECK_CONTAINS(drawn, "Timer - Ready");
    CHECK_CONTAINS(drawn, "Kolkata");

    // The footer follows the mode, so the visible keys match what works.
    CHECK_CONTAINS(drawn, "[L] Lap");
    CHECK_CONTAINS(drawn, "[Space] Start/Pause");
}

TEST_CASE(TC_068, "Verify stopwatch keys work through the running loop")
{
    PseudoTerminal terminal;

    if (!terminal.isReady())
    {
        FAIL_TEST("could not open a pseudo-terminal");
        return;
    }

    const std::string path =
        TestFramework::writeTempFile("loop-stopwatch.ini", FAST_LOOP);

    Application application;

    CHECK_TRUE(application.initialize(path));

    terminal.startCapture();

    Typist typist(
        [&terminal]()
        {
            pause(120);

            terminal.sendKeys("m");   // Clock -> Stopwatch
            pause(120);

            terminal.sendKeys(" ");   // start
            pause(150);

            terminal.sendKeys("l");   // lap
            pause(120);

            terminal.sendKeys(" ");   // stop
            pause(120);

            terminal.sendKeys("r");   // reset
            pause(120);

            quit(terminal);
        });

    application.run();

    typist.join();

    const std::string drawn = terminal.stopCapture();

    // The run happened: these lines can only have been drawn while it did.
    CHECK_CONTAINS(drawn, "Stopwatch - Running");
    CHECK_CONTAINS(drawn, "Lap 1");
    CHECK_CONTAINS(drawn, "Stopwatch - Stopped");

    // And the reset that followed left it genuinely empty.
    Stopwatch &stopwatch = application.stopwatch();

    CHECK_FALSE(stopwatch.isRunning());
    CHECK_EQ(stopwatch.lapCount(), static_cast<std::size_t>(0));
    CHECK_EQ(stopwatch.elapsed(Application::monotonicNow()),
             static_cast<std::int64_t>(0));
}

TEST_CASE(TC_069, "Verify the countdown expires and is acknowledged in the loop")
{
    PseudoTerminal terminal;

    if (!terminal.isReady())
    {
        FAIL_TEST("could not open a pseudo-terminal");
        return;
    }

    // One second, so the expiry arrives inside the test rather than after it.
    const std::string path = TestFramework::writeTempFile(
        "loop-timer.ini",
        "RefreshInterval=50\n"
        "Logging=Disabled\n"
        "Alarms=false\n"
        "AlarmBell=false\n"
        "TimerDuration=00:01\n");

    Application application;

    CHECK_TRUE(application.initialize(path));

    CHECK_EQ(application.timer().duration(), static_cast<std::int64_t>(1000));

    terminal.startCapture();

    Typist typist(
        [&terminal]()
        {
            pause(120);

            terminal.sendKeys("m");   // Clock -> Stopwatch
            pause(100);
            terminal.sendKeys("m");   // Stopwatch -> Timer
            pause(100);

            terminal.sendKeys(" ");   // start the countdown
            pause(1400);              // let it run out

            terminal.sendKeys("d");   // acknowledge the alert
            pause(150);

            terminal.sendKeys("r");   // reset for another run
            pause(150);

            quit(terminal);
        });

    application.run();

    typist.join();

    const std::string drawn = terminal.stopCapture();

    CHECK_CONTAINS(drawn, "Timer - Running");

    // Expiry raised the alert panel, which is the whole point of the timer.
    CHECK_CONTAINS(drawn, "Countdown finished");
    CHECK_CONTAINS(drawn, "[D] Dismiss");

    CountdownTimer &countdown = application.timer();

    // Acknowledged, then reset, so it is ready to run again.
    CHECK_FALSE(countdown.isFinished());
    CHECK_FALSE(countdown.isRunning());
    CHECK_EQ(countdown.remaining(Application::monotonicNow()),
             countdown.duration());
}

TEST_CASE(TC_070, "Verify a termination signal stops the refresh loop")
{
    PseudoTerminal terminal;

    if (!terminal.isReady())
    {
        FAIL_TEST("could not open a pseudo-terminal");
        return;
    }

    const std::string path =
        TestFramework::writeTempFile("loop-signal.ini", FAST_LOOP);

    Application application;

    CHECK_TRUE(application.initialize(path));

    // initialize() installs the handlers and clears any earlier request.
    CHECK_FALSE(Application::signalReceived());

    terminal.startCapture();

    Typist typist(
        [&terminal]()
        {
            pause(150);

            // What Ctrl+C delivers. The loop must notice it mid-sleep, which
            // is why it sleeps in slices rather than for the whole interval.
            std::raise(SIGINT);

            // If the signal were somehow missed, this stops the suite hanging.
            pause(400);
            quit(terminal);
        });

    const int status = application.run();

    typist.join();

    terminal.stopCapture();

    CHECK_EQ(status, Application::EXIT_OK);
    CHECK_TRUE(Application::signalReceived());
    CHECK_FALSE(application.isRunning());

    // Leave the flag clear for whatever runs next.
    Application::installSignalHandlers();

    CHECK_FALSE(Application::signalReceived());
}

TEST_CASE(TC_071, "Verify an alarm fires in the loop and can be snoozed")
{
    PseudoTerminal terminal;

    if (!terminal.isReady())
    {
        FAIL_TEST("could not open a pseudo-terminal");
        return;
    }

    Clock now;
    now.update();

    /*
    The alarm is set for the current minute so it fires on the first frame. If
    that minute is about to end, wait for the next one: otherwise the minute
    could roll between writing the file and the first poll, and the test would
    fail for a reason that has nothing to do with the code.
    */
    if (now.getSecond() >= 58)
    {
        const int startedAt = now.getMinute();

        for (int attempt = 0; attempt < 30 && now.getMinute() == startedAt;
             ++attempt)
        {
            pause(100);
            now.update();
        }
    }

    const std::string alarmFile = TestFramework::writeTempFile(
        "loop-alarms.ini",
        "Alarm1=" + twoDigits(now.getHour()) + ":" +
            twoDigits(now.getMinute()) + " | Wake Up | Daily\n");

    const std::string path = TestFramework::writeTempFile(
        "loop-alarm-snooze.ini",
        "RefreshInterval=50\n"
        "Logging=Disabled\n"
        "Alarms=true\n"
        "AlarmBell=false\n"
        "SnoozeMinutes=9\n"
        "AlarmFile=" + alarmFile + "\n");

    Application application;

    CHECK_TRUE(application.initialize(path));
    CHECK_EQ(application.alarms().count(), static_cast<std::size_t>(1));

    terminal.startCapture();

    Typist typist(
        [&terminal]()
        {
            pause(200);

            terminal.sendKeys("s");   // snooze
            pause(150);

            quit(terminal);
        });

    application.run();

    typist.join();

    const std::string drawn = terminal.stopCapture();

    // The alert panel was raised, with the alarm's own label on it.
    CHECK_CONTAINS(drawn, "ALARM");
    CHECK_CONTAINS(drawn, "Wake Up");
    CHECK_CONTAINS(drawn, "[S] Snooze 9m");
    CHECK_CONTAINS(drawn, "Next Alarm");

    // Snoozing stopped the ringing and took the panel down.
    CHECK_FALSE(application.alarms().isRinging());
    CHECK_FALSE(application.notifier().isActive());

    // The alarm is still armed, now for its snooze time.
    CHECK_TRUE(application.alarms().at(0).isSnoozed());
}

TEST_CASE(TC_072, "Verify a ringing alarm can be dismissed in the loop")
{
    PseudoTerminal terminal;

    if (!terminal.isReady())
    {
        FAIL_TEST("could not open a pseudo-terminal");
        return;
    }

    Clock now;
    now.update();

    if (now.getSecond() >= 58)
    {
        const int startedAt = now.getMinute();

        for (int attempt = 0; attempt < 30 && now.getMinute() == startedAt;
             ++attempt)
        {
            pause(100);
            now.update();
        }
    }

    const std::string alarmFile = TestFramework::writeTempFile(
        "loop-alarms-dismiss.ini",
        "Alarm1=" + twoDigits(now.getHour()) + ":" +
            twoDigits(now.getMinute()) + " | Stand Up | Daily\n");

    const std::string path = TestFramework::writeTempFile(
        "loop-alarm-dismiss.ini",
        "RefreshInterval=50\n"
        "Logging=Disabled\n"
        "Alarms=true\n"
        "AlarmBell=false\n"
        "AlarmFile=" + alarmFile + "\n");

    Application application;

    CHECK_TRUE(application.initialize(path));

    terminal.startCapture();

    Typist typist(
        [&terminal]()
        {
            pause(200);

            terminal.sendKeys("d");   // dismiss
            pause(150);

            quit(terminal);
        });

    application.run();

    typist.join();

    const std::string drawn = terminal.stopCapture();

    CHECK_CONTAINS(drawn, "Stand Up");

    CHECK_FALSE(application.alarms().isRinging());
    CHECK_FALSE(application.notifier().isActive());

    // Dismissing a repeating alarm clears the snooze without disarming it, so
    // it still rings tomorrow.
    CHECK_FALSE(application.alarms().at(0).isSnoozed());
    CHECK_TRUE(application.alarms().at(0).isEnabled());
}

#else // _WIN32

TEST_CASE(UT_132, "Refresh loop tests are POSIX-only")
{
    /*
    Driving the loop needs a pseudo-terminal, which has no Windows equivalent
    worth maintaining here. The Windows loop is exercised by the CI smoke step,
    which runs the application with --once on a real console.
    */
    Application application;

    CHECK_EQ(application.run(), Application::EXIT_STARTUP_FAILED);
}

#endif // _WIN32

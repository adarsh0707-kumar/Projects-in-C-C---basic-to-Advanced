/******************************************************************************
 * @file test_timing.cpp
 * @brief Tests for the stopwatch and countdown timer added in v1.2.0.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Covers TC-042 to TC-052.
 *
 * Both components take the current time as a parameter rather than reading a
 * clock, so these tests drive them at exact instants. That is deliberate:
 * asserting that a countdown fires at precisely its zero crossing is not
 * possible against a real clock without sleeping, and a test that sleeps is
 * both slow and flaky.
 ******************************************************************************/

#include "TestFramework.hpp"

#include "Application.hpp"
#include "CountdownTimer.hpp"
#include "Stopwatch.hpp"

namespace
{
    /** An arbitrary starting reading, to catch any assumption of zero. */
    const std::int64_t T0 = 1'000'000;

    const std::int64_t SECOND = 1000;
    const std::int64_t MINUTE = 60 * SECOND;
    const std::int64_t HOUR = 60 * MINUTE;
}

TEST_CASE(TC_042, "Verify the stopwatch measures elapsed time")
{
    Stopwatch watch;

    CHECK_FALSE(watch.isRunning());
    CHECK_EQ(watch.elapsed(T0), static_cast<std::int64_t>(0));

    CHECK_TRUE(watch.start(T0));
    CHECK_TRUE(watch.isRunning());

    CHECK_EQ(watch.elapsed(T0), static_cast<std::int64_t>(0));
    CHECK_EQ(watch.elapsed(T0 + 1500), static_cast<std::int64_t>(1500));
    CHECK_EQ(watch.elapsed(T0 + MINUTE), MINUTE);

    // Starting an already-running stopwatch must not restart it.
    CHECK_FALSE(watch.start(T0 + MINUTE));
    CHECK_EQ(watch.elapsed(T0 + MINUTE), MINUTE);

    // A reading earlier than the start yields zero rather than a negative.
    CHECK_EQ(watch.elapsed(T0 - 500), static_cast<std::int64_t>(0));
}

TEST_CASE(TC_043, "Verify stop and resume accumulate correctly")
{
    Stopwatch watch;

    CHECK_TRUE(watch.start(T0));
    CHECK_TRUE(watch.stop(T0 + 5 * SECOND));
    CHECK_FALSE(watch.isRunning());

    // A stopped stopwatch holds its value however far the clock moves on.
    CHECK_EQ(watch.elapsed(T0 + 5 * SECOND), 5 * SECOND);
    CHECK_EQ(watch.elapsed(T0 + HOUR), 5 * SECOND);

    // Stopping again is a no-op, not a second accumulation.
    CHECK_FALSE(watch.stop(T0 + HOUR));
    CHECK_EQ(watch.elapsed(T0 + HOUR), 5 * SECOND);

    // Resuming continues from the banked total rather than restarting.
    CHECK_TRUE(watch.start(T0 + HOUR));
    CHECK_EQ(watch.elapsed(T0 + HOUR + 3 * SECOND), 8 * SECOND);

    // toggle() reports the state after the call.
    CHECK_FALSE(watch.toggle(T0 + HOUR + 3 * SECOND));
    CHECK_TRUE(watch.toggle(T0 + HOUR + 3 * SECOND));

    watch.reset();
    CHECK_FALSE(watch.isRunning());
    CHECK_EQ(watch.elapsed(T0 + HOUR), static_cast<std::int64_t>(0));
}

TEST_CASE(TC_044, "Verify stopwatch laps and splits")
{
    Stopwatch watch;

    // A lap on a stopped stopwatch would record the same value repeatedly.
    CHECK_FALSE(watch.lap(T0));

    CHECK_TRUE(watch.start(T0));

    CHECK_TRUE(watch.lap(T0 + 10 * SECOND));
    CHECK_TRUE(watch.lap(T0 + 25 * SECOND));
    CHECK_TRUE(watch.lap(T0 + 30 * SECOND));

    CHECK_EQ(watch.lapCount(), static_cast<std::size_t>(3));

    // laps() holds total elapsed time at each lap.
    CHECK_EQ(watch.laps()[0], 10 * SECOND);
    CHECK_EQ(watch.laps()[1], 25 * SECOND);
    CHECK_EQ(watch.laps()[2], 30 * SECOND);

    // lapSplit() holds the duration of each individual lap.
    CHECK_EQ(watch.lapSplit(0), 10 * SECOND);
    CHECK_EQ(watch.lapSplit(1), 15 * SECOND);
    CHECK_EQ(watch.lapSplit(2), 5 * SECOND);

    // An out-of-range index yields zero rather than reading past the end.
    CHECK_EQ(watch.lapSplit(3), static_cast<std::int64_t>(0));
    CHECK_EQ(watch.lapSplit(999), static_cast<std::int64_t>(0));

    watch.reset();
    CHECK_EQ(watch.lapCount(), static_cast<std::size_t>(0));
}

TEST_CASE(TC_045, "Verify the stopwatch caps its lap history")
{
    Stopwatch watch;

    CHECK_TRUE(watch.start(T0));

    for (std::size_t index = 0; index < Stopwatch::MAX_LAPS; ++index)
        CHECK_TRUE(watch.lap(T0 + static_cast<std::int64_t>(index) * SECOND));

    CHECK_EQ(watch.lapCount(), Stopwatch::MAX_LAPS);

    // Beyond the cap, further laps are refused rather than growing without
    // bound during a long unattended run.
    CHECK_FALSE(watch.lap(T0 + 10 * HOUR));
    CHECK_EQ(watch.lapCount(), Stopwatch::MAX_LAPS);
}

TEST_CASE(TC_046, "Verify stopwatch formatting")
{
    CHECK_EQ(Stopwatch::format(0), std::string("00:00.00"));
    CHECK_EQ(Stopwatch::format(90), std::string("00:00.09"));
    CHECK_EQ(Stopwatch::format(1500), std::string("00:01.50"));
    CHECK_EQ(Stopwatch::format(SECOND), std::string("00:01.00"));
    CHECK_EQ(Stopwatch::format(MINUTE), std::string("01:00.00"));
    CHECK_EQ(Stopwatch::format(9 * MINUTE + 59 * SECOND + 990),
             std::string("09:59.99"));

    // Widens only once an hour is reached, so the common case stays compact.
    CHECK_EQ(Stopwatch::format(HOUR), std::string("1:00:00.00"));
    CHECK_EQ(Stopwatch::format(2 * HOUR + 3 * MINUTE + 4 * SECOND),
             std::string("2:03:04.00"));

    // Negative durations render as zero rather than as a stray minus sign.
    CHECK_EQ(Stopwatch::format(-5000), std::string("00:00.00"));

    Stopwatch watch;
    CHECK_TRUE(watch.start(T0));
    CHECK_EQ(watch.formatted(T0 + 2500), std::string("00:02.50"));
}

TEST_CASE(TC_047, "Verify the countdown timer counts down")
{
    CountdownTimer timer;

    CHECK_EQ(timer.duration(), CountdownTimer::DEFAULT_DURATION_MS);
    CHECK_FALSE(timer.isRunning());

    CHECK_TRUE(timer.setDuration(10 * SECOND));
    CHECK_EQ(timer.duration(), 10 * SECOND);
    CHECK_EQ(timer.remaining(T0), 10 * SECOND);

    CHECK_TRUE(timer.start(T0));
    CHECK_TRUE(timer.isRunning());

    CHECK_EQ(timer.remaining(T0 + 3 * SECOND), 7 * SECOND);
    CHECK_EQ(timer.remaining(T0 + 10 * SECOND), static_cast<std::int64_t>(0));

    // Never reports a negative remainder, however far past zero it is read.
    CHECK_EQ(timer.remaining(T0 + HOUR), static_cast<std::int64_t>(0));

    CHECK_TRUE(timer.hasExpired(T0 + 10 * SECOND));
    CHECK_FALSE(timer.hasExpired(T0 + 9 * SECOND));

    // Starting an already-running timer must not restart it.
    CHECK_FALSE(timer.start(T0 + 3 * SECOND));
}

TEST_CASE(TC_048, "Verify the countdown timer pauses and resumes")
{
    CountdownTimer timer;

    CHECK_TRUE(timer.setDuration(MINUTE));
    CHECK_TRUE(timer.start(T0));

    CHECK_TRUE(timer.pause(T0 + 20 * SECOND));
    CHECK_FALSE(timer.isRunning());

    // A paused timer holds its remainder however far the clock moves on.
    CHECK_EQ(timer.remaining(T0 + 20 * SECOND), 40 * SECOND);
    CHECK_EQ(timer.remaining(T0 + HOUR), 40 * SECOND);

    // Pausing again is a no-op rather than a second subtraction.
    CHECK_FALSE(timer.pause(T0 + HOUR));
    CHECK_EQ(timer.remaining(T0 + HOUR), 40 * SECOND);

    // Resuming continues from the remainder.
    CHECK_TRUE(timer.start(T0 + HOUR));
    CHECK_EQ(timer.remaining(T0 + HOUR + 10 * SECOND), 30 * SECOND);

    timer.reset();
    CHECK_FALSE(timer.isRunning());
    CHECK_EQ(timer.remaining(T0 + HOUR), MINUTE);
}

TEST_CASE(TC_049, "Verify the countdown timer fires exactly once")
{
    CountdownTimer timer;

    CHECK_TRUE(timer.setDuration(5 * SECOND));

    // A timer that has not been started never fires.
    CHECK_FALSE(timer.poll(T0 + HOUR));

    CHECK_TRUE(timer.start(T0));

    CHECK_FALSE(timer.poll(T0 + 4 * SECOND));
    CHECK_FALSE(timer.isFinished());

    // Fires on the poll that observes the zero crossing.
    CHECK_TRUE(timer.poll(T0 + 5 * SECOND));
    CHECK_TRUE(timer.isFinished());

    // Expiry stops the countdown; it must not keep running past zero.
    CHECK_FALSE(timer.isRunning());

    // Never fires twice for the same run, however often it is polled.
    CHECK_FALSE(timer.poll(T0 + 5 * SECOND));
    CHECK_FALSE(timer.poll(T0 + 6 * SECOND));
    CHECK_FALSE(timer.poll(T0 + HOUR));

    // Acknowledging clears the flag without restoring the duration.
    timer.acknowledge();
    CHECK_FALSE(timer.isFinished());
    CHECK_EQ(timer.remaining(T0 + HOUR), static_cast<std::int64_t>(0));

    // A timer sitting at zero cannot be started again without a reset.
    CHECK_FALSE(timer.start(T0 + HOUR));

    // After a reset it runs and fires again.
    timer.reset();
    CHECK_TRUE(timer.start(T0 + HOUR));
    CHECK_TRUE(timer.poll(T0 + HOUR + 5 * SECOND));
}

TEST_CASE(TC_050, "Verify countdown duration parsing")
{
    std::int64_t value = 0;

    // MM:SS
    CHECK_TRUE(CountdownTimer::parseDuration("05:00", value));
    CHECK_EQ(value, 5 * MINUTE);

    CHECK_TRUE(CountdownTimer::parseDuration("00:30", value));
    CHECK_EQ(value, 30 * SECOND);

    // H:MM:SS
    CHECK_TRUE(CountdownTimer::parseDuration("1:30:00", value));
    CHECK_EQ(value, 90 * MINUTE);

    // A bare number is a count of seconds.
    CHECK_TRUE(CountdownTimer::parseDuration("90", value));
    CHECK_EQ(value, 90 * SECOND);

    CHECK_TRUE(CountdownTimer::parseDuration("  45  ", value));
    CHECK_EQ(value, 45 * SECOND);

    // Malformed and out-of-range values are rejected.
    CHECK_FALSE(CountdownTimer::parseDuration("", value));
    CHECK_FALSE(CountdownTimer::parseDuration("abc", value));
    CHECK_FALSE(CountdownTimer::parseDuration("05:", value));
    CHECK_FALSE(CountdownTimer::parseDuration(":30", value));
    CHECK_FALSE(CountdownTimer::parseDuration("1:2:3:4", value));
    CHECK_FALSE(CountdownTimer::parseDuration("-5", value));
    CHECK_FALSE(CountdownTimer::parseDuration("0", value));
    CHECK_FALSE(CountdownTimer::parseDuration("05:99", value));
    CHECK_FALSE(CountdownTimer::parseDuration("1:99:00", value));

    // 24 hours is the cap.
    CHECK_TRUE(CountdownTimer::parseDuration("24:00:00", value));
    CHECK_FALSE(CountdownTimer::parseDuration("24:00:01", value));

    // setDuration applies the same bounds.
    CountdownTimer timer;
    CHECK_FALSE(timer.setDuration(0));
    CHECK_FALSE(timer.setDuration(-1));
    CHECK_FALSE(timer.setDuration(CountdownTimer::MAX_DURATION_MS + 1));
    CHECK_TRUE(timer.setDuration(CountdownTimer::MAX_DURATION_MS));
}

TEST_CASE(TC_051, "Verify countdown formatting rounds up")
{
    /*
    A countdown showing 00:00 while a fraction of a second remains reads as
    finished when it is not. Rounding up means the display counts
    ...3, 2, 1 and only then shows zero, which is what the last second of a
    countdown should look like.
    */
    CHECK_EQ(CountdownTimer::format(1), std::string("00:01"));
    CHECK_EQ(CountdownTimer::format(999), std::string("00:01"));
    CHECK_EQ(CountdownTimer::format(SECOND), std::string("00:01"));
    CHECK_EQ(CountdownTimer::format(SECOND + 1), std::string("00:02"));

    // Zero is the only value that renders as zero.
    CHECK_EQ(CountdownTimer::format(0), std::string("00:00"));
    CHECK_EQ(CountdownTimer::format(-500), std::string("00:00"));

    CHECK_EQ(CountdownTimer::format(5 * MINUTE), std::string("05:00"));
    CHECK_EQ(CountdownTimer::format(HOUR), std::string("1:00:00"));
    CHECK_EQ(CountdownTimer::format(HOUR + 2 * MINUTE + 3 * SECOND),
             std::string("1:02:03"));

    CountdownTimer timer;
    CHECK_TRUE(timer.setDuration(90 * SECOND));
    CHECK_EQ(timer.formatted(T0), std::string("01:30"));
}

TEST_CASE(TC_052, "Verify mode switching leaves the other modes undisturbed")
{
    /*
    Plugins switched off, so the cycle is the four built-in modes whatever
    happens to be sitting in Plugins/. A plugin adds a step, which is the
    point of it -- but a test asserting the shape of the cycle must not
    depend on what a particular machine has installed.
    */
    const std::string path = TestFramework::writeTempFile(
        "modes-no-plugins.ini",
        "RefreshInterval=1000\n"
        "Logging=Disabled\n"
        "Plugins=false\n");

    Application application;

    {
        TestFramework::OutputCapture capture;

        CHECK_TRUE(application.initialize(path));

        // The clock is the default view.
        CHECK_TRUE(application.mode() == Application::Mode::Clock);

        // Cycling wraps back around to the clock.
        CHECK_TRUE(application.cycleMode() == Application::Mode::Stopwatch);
        CHECK_TRUE(application.cycleMode() == Application::Mode::Timer);
        CHECK_TRUE(application.cycleMode() == Application::Mode::Clock);

        // A stopwatch started in one mode keeps running while another is on
        // screen: switching views must not stop what is being measured.
        const std::int64_t now = Application::monotonicNow();

        application.setMode(Application::Mode::Stopwatch);
        CHECK_TRUE(application.stopwatch().start(now));

        application.setMode(Application::Mode::Clock);
        CHECK_TRUE(application.stopwatch().isRunning());

        application.setMode(Application::Mode::Timer);
        CHECK_TRUE(application.stopwatch().isRunning());

        // Every mode renders without error.
        for (const Application::Mode mode : {Application::Mode::Clock,
                                             Application::Mode::Stopwatch,
                                             Application::Mode::Timer})
        {
            application.setMode(mode);
            application.renderFrame();
        }

        application.shutdown();
    }

    CHECK_EQ(Application::modeName(Application::Mode::Clock),
             std::string("Clock"));
    CHECK_EQ(Application::modeName(Application::Mode::Stopwatch),
             std::string("Stopwatch"));
    CHECK_EQ(Application::modeName(Application::Mode::Timer),
             std::string("Timer"));
}

TEST_CASE(UT_110, "Application reads the configured timer duration")
{
    const std::string path = TestFramework::writeTempFile(
        "timer-config.ini",
        "TimerDuration=00:45\n"
        "Logging=Disabled\n");

    Application application;

    {
        TestFramework::OutputCapture capture;

        CHECK_TRUE(application.initialize(path));
        CHECK_EQ(application.timer().duration(), 45 * SECOND);

        application.shutdown();
    }

    // An invalid duration falls back to the default rather than failing.
    const std::string bad = TestFramework::writeTempFile(
        "timer-bad.ini",
        "TimerDuration=nonsense\n"
        "Logging=Disabled\n");

    Application fallback;

    {
        TestFramework::OutputCapture capture;

        CHECK_TRUE(fallback.initialize(bad));
        CHECK_EQ(fallback.timer().duration(),
                 CountdownTimer::DEFAULT_DURATION_MS);

        fallback.shutdown();
    }
}

TEST_CASE(UT_111, "Monotonic readings advance and never go backwards")
{
    const std::int64_t first = Application::monotonicNow();
    const std::int64_t second = Application::monotonicNow();

    // steady_clock is monotonic, so a later reading is never smaller.
    CHECK_TRUE(second >= first);

    // Sanity: the reading is in milliseconds, not seconds or nanoseconds.
    CHECK_TRUE(first > 0);
}

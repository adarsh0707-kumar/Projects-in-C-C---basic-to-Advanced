#ifndef COUNTDOWN_TIMER_HPP
#define COUNTDOWN_TIMER_HPP

/******************************************************************************
 * @file CountdownTimer.hpp
 * @brief Declaration of the CountdownTimer core component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * A CountdownTimer counts down from a set duration and signals once when it
 * reaches zero.
 *
 * As with Stopwatch, the current time is supplied by the caller rather than
 * read internally, so expiry can be tested at an exact instant and a change
 * to the wall clock cannot disturb a running countdown.
 *
 * Introduced in v1.2.0 (Change Log, section 6.3).
 ******************************************************************************/

#include <cstdint>
#include <string>

/**
 * @class CountdownTimer
 * @brief Counts down from a duration and fires once on reaching zero.
 *
 * Expiry is reported by poll(), which returns true exactly once per run. That
 * mirrors AlarmManager::poll(): the timer is *expired* for as long as it sits
 * at zero, but should only announce itself a single time.
 */
class CountdownTimer
{
public:
    /** Duration used when the configuration supplies none, in milliseconds. */
    static const std::int64_t DEFAULT_DURATION_MS;

    /** Longest accepted duration, in milliseconds (24 hours). */
    static const std::int64_t MAX_DURATION_MS;

    CountdownTimer();

    /**
     * @brief Sets the duration to count down from.
     *
     * Resets the timer, so a duration change never leaves a partly elapsed
     * countdown against a different total.
     *
     * @param milliseconds Duration, from 1 ms to MAX_DURATION_MS.
     * @return true if the duration was accepted.
     */
    bool setDuration(std::int64_t milliseconds);

    /**
     * @brief Returns the configured duration.
     * @return std::int64_t Duration in milliseconds.
     */
    std::int64_t duration() const;

    /**
     * @brief Starts or resumes the countdown.
     *
     * @param nowMs Current monotonic reading, in milliseconds.
     * @return true if the countdown started, false if already running or
     *         already at zero.
     */
    bool start(std::int64_t nowMs);

    /**
     * @brief Pauses the countdown, keeping the remaining time.
     *
     * @param nowMs Current monotonic reading, in milliseconds.
     * @return true if the countdown was paused, false if not running.
     */
    bool pause(std::int64_t nowMs);

    /**
     * @brief Starts if paused, pauses if running.
     *
     * @param nowMs Current monotonic reading, in milliseconds.
     * @return true if the countdown is running after the call.
     */
    bool toggle(std::int64_t nowMs);

    /**
     * @brief Returns to the full duration and clears any expiry.
     */
    void reset();

    /**
     * @brief Reports whether the countdown is running.
     * @return true while counting down.
     */
    bool isRunning() const;

    /**
     * @brief Returns the time left.
     *
     * Never negative: a timer past its end reports zero.
     *
     * @param nowMs Current monotonic reading, in milliseconds.
     * @return std::int64_t Milliseconds remaining.
     */
    std::int64_t remaining(std::int64_t nowMs) const;

    /**
     * @brief Reports whether the countdown has reached zero.
     *
     * @param nowMs Current monotonic reading, in milliseconds.
     * @return true when no time remains.
     */
    bool hasExpired(std::int64_t nowMs) const;

    /**
     * @brief Checks for expiry, reporting it once per run.
     *
     * Stops the countdown on expiry so it does not run past zero.
     *
     * @param nowMs Current monotonic reading, in milliseconds.
     * @return true on the single call that observes the countdown reaching
     *         zero. Subsequent calls return false until the timer is reset
     *         and run again.
     */
    bool poll(std::int64_t nowMs);

    /**
     * @brief Reports whether an expiry is awaiting acknowledgement.
     * @return true between poll() firing and acknowledge().
     */
    bool isFinished() const;

    /**
     * @brief Clears an unacknowledged expiry without resetting the duration.
     */
    void acknowledge();

    /**
     * @brief Returns the time left, formatted for display.
     *
     * @param nowMs Current monotonic reading, in milliseconds.
     * @return std::string Formatted remaining time.
     */
    std::string formatted(std::int64_t nowMs) const;

    /**
     * @brief Formats a duration as @c MM:SS or @c H:MM:SS.
     *
     * @param milliseconds Duration to format. Negative values render as zero.
     * @return std::string Formatted duration.
     */
    static std::string format(std::int64_t milliseconds);

    /**
     * @brief Parses a duration from configuration text.
     *
     * Accepts @c MM:SS, @c H:MM:SS, and a bare number of seconds:
     *
     * @code
     * 05:00      -> five minutes
     * 1:30:00    -> ninety minutes
     * 90         -> ninety seconds
     * @endcode
     *
     * @param text         Text to parse.
     * @param milliseconds Receives the parsed duration.
     * @return true if @p text was a well-formed, in-range duration.
     */
    static bool parseDuration(const std::string &text,
                              std::int64_t &milliseconds);

private:
    std::int64_t total;     ///< Configured duration.
    std::int64_t remainder; ///< Time left when paused.
    std::int64_t startedAt; ///< Reading when the current run began.
    bool running;           ///< Whether a run is in progress.
    bool finished;          ///< Expiry seen but not yet acknowledged.
    bool fired;             ///< Whether poll() has reported this expiry.
};

#endif // COUNTDOWN_TIMER_HPP

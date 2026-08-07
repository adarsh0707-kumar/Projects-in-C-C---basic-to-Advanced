#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

/******************************************************************************
 * @file Stopwatch.hpp
 * @brief Declaration of the Stopwatch core component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * A Stopwatch measures elapsed time, with start, stop, lap and reset.
 *
 * Every method that needs the current time takes it as a parameter rather
 * than reading a clock itself. That keeps the class free of hidden state and
 * makes lap and rollover behaviour testable at exact instants, which reading
 * a clock internally would not allow. The caller supplies a *monotonic*
 * reading -- Application uses std::chrono::steady_clock -- so a change to the
 * wall clock cannot corrupt a running measurement.
 *
 * Introduced in v1.2.0 (Change Log, section 6.3).
 ******************************************************************************/

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

/**
 * @class Stopwatch
 * @brief Measures elapsed time with lap support.
 *
 * Typical use:
 * @code
 * stopwatch.start(now);
 * ...
 * stopwatch.lap(now);
 * std::string text = stopwatch.formatted(now);
 * @endcode
 */
class Stopwatch
{
public:
    /** Maximum number of laps retained; older laps are discarded. */
    static const std::size_t MAX_LAPS;

    Stopwatch();

    /**
     * @brief Starts or resumes measuring.
     *
     * Time already accumulated is kept, so start after stop resumes rather
     * than restarting. Use reset() to return to zero.
     *
     * @param nowMs Current monotonic reading, in milliseconds.
     * @return true if the stopwatch was started, false if already running.
     */
    bool start(std::int64_t nowMs);

    /**
     * @brief Stops measuring, keeping the elapsed time.
     *
     * @param nowMs Current monotonic reading, in milliseconds.
     * @return true if the stopwatch was stopped, false if already stopped.
     */
    bool stop(std::int64_t nowMs);

    /**
     * @brief Starts if stopped, stops if running.
     *
     * @param nowMs Current monotonic reading, in milliseconds.
     * @return true if the stopwatch is running after the call.
     */
    bool toggle(std::int64_t nowMs);

    /**
     * @brief Returns to zero and discards all laps.
     *
     * Stops the stopwatch if it was running.
     */
    void reset();

    /**
     * @brief Records a lap at the current elapsed time.
     *
     * @param nowMs Current monotonic reading, in milliseconds.
     * @return true if a lap was recorded, false when the stopwatch is not
     *         running or MAX_LAPS would be exceeded.
     */
    bool lap(std::int64_t nowMs);

    /**
     * @brief Reports whether the stopwatch is measuring.
     * @return true while running.
     */
    bool isRunning() const;

    /**
     * @brief Returns the total elapsed time.
     *
     * @param nowMs Current monotonic reading, in milliseconds.
     * @return std::int64_t Elapsed milliseconds.
     */
    std::int64_t elapsed(std::int64_t nowMs) const;

    /**
     * @brief Returns the recorded lap times.
     *
     * Each entry is the total elapsed time at which that lap was taken, not
     * the split from the previous lap. Use lapSplit() for splits.
     *
     * @return const std::vector<std::int64_t>& Lap times in milliseconds.
     */
    const std::vector<std::int64_t> &laps() const;

    /**
     * @brief Returns the number of recorded laps.
     * @return std::size_t Lap count.
     */
    std::size_t lapCount() const;

    /**
     * @brief Returns the duration of a single lap.
     *
     * @param index Zero-based lap index.
     * @return std::int64_t Milliseconds between this lap and the previous
     *         one, or 0 when @p index is out of range.
     */
    std::int64_t lapSplit(std::size_t index) const;

    /**
     * @brief Returns the elapsed time formatted for display.
     *
     * @param nowMs Current monotonic reading, in milliseconds.
     * @return std::string Formatted elapsed time.
     */
    std::string formatted(std::int64_t nowMs) const;

    /**
     * @brief Formats a duration for display.
     *
     * Renders as @c MM:SS.cc, widening to @c H:MM:SS.cc once the duration
     * reaches an hour so the common case stays compact.
     *
     * @param milliseconds Duration to format. Negative values render as zero.
     * @return std::string Formatted duration.
     */
    static std::string format(std::int64_t milliseconds);

private:
    std::int64_t accumulated;        ///< Milliseconds banked by earlier runs.
    std::int64_t startedAt;          ///< Reading when the current run began.
    bool running;                    ///< Whether a run is in progress.
    std::vector<std::int64_t> lapTimes; ///< Elapsed time at each lap.
};

#endif // STOPWATCH_HPP

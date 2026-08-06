#ifndef CLOCK_HPP
#define CLOCK_HPP

/******************************************************************************
 * @file Clock.hpp
 * @brief Declaration of the Clock core component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Clock reads the current time from the operating system and exposes it
 * as individual fields and as formatted text (FR-001). Formatting decisions
 * that depend on user configuration belong to TimeFormatter; Clock only
 * offers the two canonical representations.
 *
 * Reference: API Documentation, section 2.2.
 ******************************************************************************/

#include <ctime>
#include <string>

/**
 * @class Clock
 * @brief Holds a snapshot of the current system time.
 *
 * The stored time changes only when update() is called, so a single render
 * pass always draws a consistent time even if it queries the fields
 * separately.
 */
class Clock
{
public:
    /**
     * @brief Constructs the clock and takes an initial time snapshot.
     */
    Clock();

    /**
     * @brief Refreshes the snapshot from the system clock.
     *
     * @return true if the system time was read successfully. On failure the
     *         previous snapshot is retained so the display keeps showing the
     *         last known good time (FR-001 exception handling).
     */
    bool update();

    /**
     * @brief Returns the time in 24-hour @c HH:MM:SS form.
     * @return std::string Formatted time, for example "20:45:30".
     */
    std::string getCurrentTime() const;

    /**
     * @brief Returns the hour in 24-hour form.
     * @return int Hour in the range 0-23.
     */
    int getHour() const;

    /**
     * @brief Returns the hour in 12-hour form.
     *
     * Midnight and noon both report 12, as required by FR-004.
     *
     * @return int Hour in the range 1-12.
     */
    int getHour12() const;

    /**
     * @brief Returns the minute.
     * @return int Minute in the range 0-59.
     */
    int getMinute() const;

    /**
     * @brief Returns the second.
     * @return int Second in the range 0-59 (60 during a leap second).
     */
    int getSecond() const;

    /**
     * @brief Reports whether the stored time is in the afternoon.
     * @return true for 12:00:00 onwards, false before noon.
     */
    bool isPM() const;

    /**
     * @brief Returns "AM" or "PM" for the stored time.
     * @return std::string Meridiem indicator.
     */
    std::string getMeridiem() const;

    /**
     * @brief Returns the time in 24-hour @c HH:MM:SS form.
     * @return std::string Formatted time, for example "20:45:30".
     */
    std::string getTime24() const;

    /**
     * @brief Returns the time in 12-hour form with an AM/PM suffix.
     * @return std::string Formatted time, for example "08:45:30 PM".
     */
    std::string getTime12() const;

    /**
     * @brief Overrides the stored time.
     *
     * Provided so that formatting behaviour can be verified deterministically
     * at boundary times such as midnight and noon (TC-005, TC-006). Values
     * outside their valid ranges are rejected.
     *
     * @param hour   Hour in the range 0-23.
     * @param minute Minute in the range 0-59.
     * @param second Second in the range 0-60.
     * @return true if the values were accepted.
     */
    bool setTime(int hour, int minute, int second);

    /**
     * @brief Reports whether the last update() succeeded.
     * @return true when the snapshot reflects a successful system read.
     */
    bool isValid() const;

private:
    int hour;   ///< 0-23.
    int minute; ///< 0-59.
    int second; ///< 0-60.
    bool valid; ///< Whether the last system read succeeded.
};

#endif // CLOCK_HPP

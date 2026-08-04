#ifndef CLOCK_HPP
#define CLOCK_HPP

/******************************************************************************
 * @file Clock.hpp
 * @brief Declaration of the Clock class.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Clock class retrieves and stores the current system time.
 * It provides both 24-hour and 12-hour formatted time along with
 * individual access to hours, minutes, and seconds.
 ******************************************************************************/

#include <string>

/**
 * @class Clock
 * @brief Represents the current system time.
 *
 * The class updates its internal values from the operating system
 * and provides multiple methods for retrieving formatted or
 * individual time components.
 */
class Clock
{
public:
    /**
     * @brief Constructs a Clock object.
     */
    Clock();

    /**
     * @brief Updates the current system time.
     */
    void update();

    /**
     * @brief Returns the current hour in 24-hour format.
     * @return Current hour.
     */
    int getHour24() const;

    /**
     * @brief Returns the current hour in 12-hour format.
     * @return Current hour.
     */
    int getHour12() const;

    /**
     * @brief Returns the current minute.
     * @return Current minute.
     */
    int getMinute() const;

    /**
     * @brief Returns the current second.
     * @return Current second.
     */
    int getSecond() const;

    /**
     * @brief Determines whether the current time is PM.
     * @return true if PM; otherwise false.
     */
    bool isPM() const;

    /**
     * @brief Returns the current time in 24-hour format.
     * @return Formatted time string.
     */
    std::string getTime24() const;

    /**
     * @brief Returns the current time in 12-hour format.
     * @return Formatted time string.
     */
    std::string getTime12() const;

private:
    /// Current hour.
    int hour;

    /// Current minute.
    int minute;

    /// Current second.
    int second;
};

#endif // CLOCK_HPP
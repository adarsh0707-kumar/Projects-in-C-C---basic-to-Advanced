#ifndef DATE_HPP
#define DATE_HPP

/******************************************************************************
 * @file Date.hpp
 * @brief Declaration of the Date class.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Date class retrieves and stores the current system date.
 * It provides formatted date strings along with individual access
 * to the day, month, year, and weekday.
 ******************************************************************************/

#include <string>

/**
 * @class Date
 * @brief Represents the current system date.
 *
 * The class updates its internal values from the operating system
 * and provides multiple formatted representations of the date.
 */
class Date
{
public:
    /**
     * @brief Constructs a Date object.
     */
    Date();

    /**
     * @brief Updates the current system date.
     */
    void update();

    /**
     * @brief Returns the current day.
     * @return Day of the month.
     */
    int getDay() const;

    /**
     * @brief Returns the current month.
     * @return Month value.
     */
    int getMonth() const;

    /**
     * @brief Returns the current year.
     * @return Current year.
     */
    int getYear() const;

    /**
     * @brief Returns the current weekday name.
     * @return Weekday string.
     */
    std::string getWeekday() const;

    /**
     * @brief Returns the current month name.
     * @return Month name.
     */
    std::string getMonthName() const;

    /**
     * @brief Returns the date in numeric format.
     * @return Formatted numeric date.
     */
    std::string getDateNumeric() const;

    /**
     * @brief Returns the date in long format.
     * @return Formatted long date.
     */
    std::string getDateLong() const;

private:
    /// Current day of the month.
    int day;

    /// Current month.
    int month;

    /// Current year.
    int year;

    /// Current weekday.
    int weekday;
};

#endif // DATE_HPP
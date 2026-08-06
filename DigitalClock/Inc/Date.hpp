#ifndef DATE_HPP
#define DATE_HPP

/******************************************************************************
 * @file Date.hpp
 * @brief Declaration of the Date core component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Date component reads the current calendar date from the operating
 * system and exposes it as individual fields and as formatted text (FR-002).
 *
 * Reference: API Documentation, section 2.3.
 ******************************************************************************/

#include <ctime>
#include <string>

/**
 * @class Date
 * @brief Holds a snapshot of the current system date.
 *
 * As with Clock, the stored value changes only when update() is called so
 * that a render pass sees a consistent date.
 */
class Date
{
public:
    /**
     * @brief Constructs the object and takes an initial date snapshot.
     */
    Date();

    /**
     * @brief Refreshes the snapshot from the system clock.
     *
     * @return true if the system date was read successfully. On failure the
     *         previous snapshot is retained.
     */
    bool update();

    /**
     * @brief Returns the date in long form.
     * @return std::string For example "Monday, 03 August 2026".
     */
    std::string getCurrentDate() const;

    /**
     * @brief Returns the day of the month.
     * @return int Day in the range 1-31.
     */
    int getDay() const;

    /**
     * @brief Returns the month.
     * @return int Month in the range 1-12.
     */
    int getMonth() const;

    /**
     * @brief Returns the year.
     * @return int Four-digit year.
     */
    int getYear() const;

    /**
     * @brief Returns the day of the week as a number.
     * @return int 0 for Sunday through 6 for Saturday.
     */
    int getWeekday() const;

    /**
     * @brief Returns the name of the day of the week.
     * @return std::string For example "Monday".
     */
    std::string getWeekdayName() const;

    /**
     * @brief Returns the name of the month.
     * @return std::string For example "August".
     */
    std::string getMonthName() const;

    /**
     * @brief Returns the date in long form.
     * @return std::string For example "Monday, 03 August 2026".
     */
    std::string getDateLong() const;

    /**
     * @brief Returns the date as @c DD-MM-YYYY.
     * @return std::string For example "03-08-2026".
     */
    std::string getDateDayFirst() const;

    /**
     * @brief Returns the date as @c MM-DD-YYYY.
     * @return std::string For example "08-03-2026".
     */
    std::string getDateMonthFirst() const;

    /**
     * @brief Returns the date as @c YYYY-MM-DD.
     * @return std::string For example "2026-08-03".
     */
    std::string getDateIso() const;

    /**
     * @brief Overrides the stored date.
     *
     * The weekday is derived from the supplied values. Provided so that date
     * formatting can be verified deterministically (TC-007, TC-008).
     *
     * @param year  Four-digit year, 1900 or later.
     * @param month Month in the range 1-12.
     * @param day   Day valid for @p month in @p year.
     * @return true if the values form a real calendar date.
     */
    bool setDate(int year, int month, int day);

    /**
     * @brief Reports whether the last update() succeeded.
     * @return true when the snapshot reflects a successful system read.
     */
    bool isValid() const;

    /**
     * @brief Reports whether a year is a leap year.
     * @param year Four-digit year.
     * @return true for leap years.
     */
    static bool isLeapYear(int year);

    /**
     * @brief Returns the number of days in a month.
     * @param year  Four-digit year, used to resolve February.
     * @param month Month in the range 1-12.
     * @return int Day count, or 0 when @p month is out of range.
     */
    static int daysInMonth(int year, int month);

private:
    int day;     ///< 1-31.
    int month;   ///< 1-12.
    int year;    ///< Four-digit year.
    int weekday; ///< 0 (Sunday) - 6 (Saturday).
    bool valid;  ///< Whether the last system read succeeded.
};

#endif // DATE_HPP

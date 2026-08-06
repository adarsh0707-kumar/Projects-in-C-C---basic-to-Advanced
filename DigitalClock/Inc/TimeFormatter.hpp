#ifndef TIME_FORMATTER_HPP
#define TIME_FORMATTER_HPP

/******************************************************************************
 * @file TimeFormatter.hpp
 * @brief Declaration of the TimeFormatter core component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The TimeFormatter turns Clock and Date snapshots into the text the user
 * sees, applying the format selected in the configuration file (FR-004 and
 * FR-005 for time, FR-002 for the date).
 *
 * Reference: API Documentation, section 2.4.
 ******************************************************************************/

#include <string>

#include "Clock.hpp"
#include "Date.hpp"

/**
 * @class TimeFormatter
 * @brief Applies the configured time and date formats.
 *
 * Keeping formatting here means Clock and Date stay free of presentation
 * concerns, and a format change affects exactly one class.
 */
class TimeFormatter
{
public:
    /**
     * @enum TimeFormat
     * @brief Supported clock formats.
     */
    enum class TimeFormat
    {
        Hour24, ///< 20:45:30 (FR-005).
        Hour12  ///< 08:45:30 PM (FR-004).
    };

    /**
     * @enum DateFormat
     * @brief Supported date formats.
     */
    enum class DateFormat
    {
        Long,      ///< Monday, 03 August 2026.
        DayFirst,  ///< 03-08-2026.
        MonthFirst,///< 08-03-2026.
        Iso        ///< 2026-08-03.
    };

    /**
     * @brief Constructs a formatter using the 24-hour and long date formats.
     */
    TimeFormatter();

    /**
     * @brief Formats a time using the configured format.
     * @param clock Time snapshot to format.
     * @return std::string For example "20:45:30" or "08:45:30 PM".
     */
    std::string formatTime(const Clock &clock) const;

    /**
     * @brief Formats a time with spaced separators for the main display.
     *
     * This is the wide form shown in the User Manual's screen layout, for
     * example "10 : 45 : 30 AM".
     *
     * @param clock Time snapshot to format.
     * @return std::string Spaced time.
     */
    std::string formatTimeWide(const Clock &clock) const;

    /**
     * @brief Formats a date using the configured format.
     * @param date Date snapshot to format.
     * @return std::string For example "Monday, 03 August 2026".
     */
    std::string formatDate(const Date &date) const;

    /**
     * @brief Formats a date and time together.
     * @param clock Time snapshot to format.
     * @param date  Date snapshot to format.
     * @return std::string Date and time separated by " - ".
     */
    std::string formatDateTime(const Clock &clock, const Date &date) const;

    /**
     * @brief Selects the clock format.
     * @param format Format to apply to subsequent calls.
     */
    void setTimeFormat(TimeFormat format);

    /**
     * @brief Selects the date format.
     * @param format Format to apply to subsequent calls.
     */
    void setDateFormat(DateFormat format);

    /**
     * @brief Returns the active clock format.
     * @return TimeFormat Current setting.
     */
    TimeFormat timeFormat() const;

    /**
     * @brief Returns the active date format.
     * @return DateFormat Current setting.
     */
    DateFormat dateFormat() const;

    /**
     * @brief Converts a configuration value to a clock format.
     *
     * Accepts "12" and "24", and the spellings "12H"/"24H" and
     * "12-HOUR"/"24-HOUR". Unrecognised values yield @p fallback.
     *
     * @param text     Configuration value.
     * @param fallback Returned when @p text is not recognised.
     * @return TimeFormat Parsed format.
     */
    static TimeFormat timeFormatFromString(
        const std::string &text,
        TimeFormat fallback = TimeFormat::Hour24);

    /**
     * @brief Converts a configuration value to a date format.
     *
     * Accepts "LONG", "DD-MM-YYYY", "MM-DD-YYYY" and "YYYY-MM-DD", with
     * either dashes or slashes. Unrecognised values yield @p fallback.
     *
     * @param text     Configuration value.
     * @param fallback Returned when @p text is not recognised.
     * @return DateFormat Parsed format.
     */
    static DateFormat dateFormatFromString(
        const std::string &text,
        DateFormat fallback = DateFormat::Long);

private:
    TimeFormat currentTimeFormat; ///< Active clock format.
    DateFormat currentDateFormat; ///< Active date format.
};

#endif // TIME_FORMATTER_HPP

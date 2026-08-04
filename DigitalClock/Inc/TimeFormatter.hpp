#pragma once

/******************************************************************************
 * @file TimeFormatter.hpp
 * @brief Declaration and implementation of the TimeFormatter class.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The TimeFormatter class provides utility functions for formatting
 * time and date values into human-readable strings. It supports both
 * 12-hour and 24-hour time formats, as well as multiple date formats
 * commonly used by the Digital Clock application.
 *
 * Supported Time Formats:
 *  - 24-Hour (HH:MM:SS)
 *  - 12-Hour (HH:MM:SS AM/PM)
 *
 * Supported Date Formats:
 *  - SHORT (DD/MM/YYYY)
 *  - ISO (YYYY-MM-DD)
 *  - LONG (Weekday, DD Month YYYY)
 ******************************************************************************/

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "Clock.hpp"
#include "Date.hpp"

/**
 * @class TimeFormatter
 * @brief Formats Clock and Date objects into readable strings.
 *
 * The TimeFormatter class converts time and date information into
 * different display formats. It is intended to simplify presentation
 * logic by keeping formatting separate from the Clock and Date classes.
 */
class TimeFormatter
{
public:
    /**
     * @brief Formats the current time.
     *
     * Generates either a 24-hour or 12-hour formatted time string
     * using the values stored in the supplied Clock object.
     *
     * Supported formats:
     * - HH:MM:SS
     * - HH:MM:SS AM/PM
     *
     * @param clock Reference to the Clock object.
     * @param is24Hour true for 24-hour format; false for 12-hour format.
     * @return std::string Formatted time string.
     */
    std::string formatTime(const Clock &clock, bool is24Hour)
    {
        int h = clock.getHour();
        int m = clock.getMinute();
        int s = clock.getSecond();

        std::ostringstream oss;

        if (!is24Hour)
        {
            std::string ampm = (h >= 12) ? "PM" : "AM";

            h = (h % 12 == 0) ? 12 : h % 12;

            oss << std::setfill('0') << std::setw(2) << h << ":"
                << std::setfill('0') << std::setw(2) << m << ":"
                << std::setfill('0') << std::setw(2) << s
                << " " << ampm;
        }
        else
        {
            oss << std::setfill('0') << std::setw(2) << h << ":"
                << std::setfill('0') << std::setw(2) << m << ":"
                << std::setfill('0') << std::setw(2) << s;
        }

        return oss.str();
    }

    /**
     * @brief Formats the current date.
     *
     * Converts the supplied Date object into one of the supported
     * output formats.
     *
     * Supported formats:
     * - SHORT : DD/MM/YYYY
     * - ISO   : YYYY-MM-DD
     * - LONG  : Weekday, DD Month YYYY
     *
     * @param date Reference to the Date object.
     * @param format Desired output format.
     * @return std::string Formatted date string.
     */
    std::string formatDate(const Date &date, const std::string &format)
    {
        if (format == "SHORT")
        {
            return std::to_string(date.getDay()) + "/" +
                   std::to_string(date.getMonth()) + "/" +
                   std::to_string(date.getYear());
        }
        else if (format == "ISO")
        {
            return std::to_string(date.getYear()) + "-" +
                   std::to_string(date.getMonth()) + "-" +
                   std::to_string(date.getDay());
        }
        else
        {
            /**
             * Month names used for LONG date formatting.
             */
            static const std::string months[] =
                {
                    "January",
                    "February",
                    "March",
                    "April",
                    "May",
                    "June",
                    "July",
                    "August",
                    "September",
                    "October",
                    "November",
                    "December"};

            return date.getWeekday() + ", " +
                   std::to_string(date.getDay()) + " " +
                   months[date.getMonth() - 1] + " " +
                   std::to_string(date.getYear());
        }
    }
};
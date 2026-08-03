#pragma once

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "Clock.hpp"
#include "Date.hpp"

class TimeFormatter
{
public:
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
                << std::setfill('0') << std::setw(2) << s << " " << ampm;
        }
        else
        {
            oss << std::setfill('0') << std::setw(2) << h << ":"
                << std::setfill('0') << std::setw(2) << m << ":"
                << std::setfill('0') << std::setw(2) << s;
        }
        return oss.str();
    }

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
        { // LONG
            static const std::string months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
            return date.getWeekday() + ", " + std::to_string(date.getDay()) + " " +
                   months[date.getMonth() - 1] + " " + std::to_string(date.getYear());
        }
    }
};
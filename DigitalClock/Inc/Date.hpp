#pragma once

#include <ctime>
#include <chrono>
#include <string>

class Date
{
private:
    int day;
    int month;
    int year;
    std::string weekday;

public:
    Date() : day(0), month(0), year(0) {}

    bool update()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t tt = std::chrono::system_clock::to_time_t(now);
        std::tm *tm = std::localtime(&tt);
        if (tm)
        {
            day = tm->tm_mday;
            month = tm->tm_mon + 1;
            year = tm->tm_year + 1900;
            static const std::string days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
            weekday = days[tm->tm_wday];
            return true;
        }
        return false;
    }

    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    std::string getWeekday() const { return weekday; }
};
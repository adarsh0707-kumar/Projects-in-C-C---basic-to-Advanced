#include "Date.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace
{
    const char *WEEKDAYS[] =
        {
            "Sunday",
            "Monday",
            "Tuesday",
            "Wednesday",
            "Thursday",
            "Friday",
            "Saturday"};

    const char *MONTHS[] =
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
}

Date::Date()
{
    update();
}

void Date::update()
{
    auto now = std::chrono::system_clock::now();

    std::time_t current =
        std::chrono::system_clock::to_time_t(now);

    std::tm localTime = *std::localtime(&current);

    day = localTime.tm_mday;
    month = localTime.tm_mon + 1;
    year = localTime.tm_year + 1900;
    weekday = localTime.tm_wday;
}

int Date::getDay() const
{
    return day;
}

int Date::getMonth() const
{
    return month;
}

int Date::getYear() const
{
    return year;
}

std::string Date::getWeekday() const
{
    return WEEKDAYS[weekday];
}

std::string Date::getMonthName() const
{
    return MONTHS[month - 1];
}

std::string Date::getDateNumeric() const
{
    std::ostringstream out;

    out << std::setw(2)
        << std::setfill('0')
        << day
        << "/"
        << std::setw(2)
        << month
        << "/"
        << year;

    return out.str();
}

std::string Date::getDateLong() const
{
    std::ostringstream out;

    out << getWeekday()
        << ", "
        << day
        << " "
        << getMonthName()
        << " "
        << year;

    return out.str();
}
#include "Date.hpp"

#include <chrono>

#include "Utility.hpp"

namespace
{
    const char *const WEEKDAY_NAMES[] = {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday"};

    const char *const MONTH_NAMES[] = {
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

    /**
     * Sakamoto's algorithm: returns 0 (Sunday) - 6 (Saturday) for a date in
     * the proleptic Gregorian calendar.
     */
    int weekdayFor(int year, int month, int day)
    {
        static const int OFFSETS[] = {
            0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

        int adjusted = year;

        if (month < 3)
            adjusted -= 1;

        return (adjusted +
                adjusted / 4 -
                adjusted / 100 +
                adjusted / 400 +
                OFFSETS[month - 1] +
                day) %
               7;
    }
}

Date::Date()
    : day(1),
      month(1),
      year(1970),
      weekday(4),
      valid(false)
{
    update();
}

bool Date::update()
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t raw = std::chrono::system_clock::to_time_t(now);

    std::tm local{};

#ifdef _WIN32
    const bool converted = (localtime_s(&local, &raw) == 0);
#else
    const bool converted = (localtime_r(&raw, &local) != nullptr);
#endif

    if (!converted)
    {
        valid = false;
        return false;
    }

    day = local.tm_mday;
    month = local.tm_mon + 1;
    year = local.tm_year + 1900;
    weekday = local.tm_wday;
    valid = true;

    return true;
}

std::string Date::getCurrentDate() const
{
    return getDateLong();
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

int Date::getWeekday() const
{
    return weekday;
}

std::string Date::getWeekdayName() const
{
    if (weekday < 0 || weekday > 6)
        return "";

    return WEEKDAY_NAMES[weekday];
}

std::string Date::getMonthName() const
{
    if (month < 1 || month > 12)
        return "";

    return MONTH_NAMES[month - 1];
}

std::string Date::getDateLong() const
{
    return getWeekdayName() + ", " +
           Utility::padZero(day) + " " +
           getMonthName() + " " +
           std::to_string(year);
}

std::string Date::getDateDayFirst() const
{
    return Utility::padZero(day) + "-" +
           Utility::padZero(month) + "-" +
           std::to_string(year);
}

std::string Date::getDateMonthFirst() const
{
    return Utility::padZero(month) + "-" +
           Utility::padZero(day) + "-" +
           std::to_string(year);
}

std::string Date::getDateIso() const
{
    return std::to_string(year) + "-" +
           Utility::padZero(month) + "-" +
           Utility::padZero(day);
}

bool Date::isLeapYear(int year)
{
    if (year % 400 == 0)
        return true;

    if (year % 100 == 0)
        return false;

    return year % 4 == 0;
}

int Date::daysInMonth(int year, int month)
{
    static const int DAYS[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month < 1 || month > 12)
        return 0;

    if (month == 2 && isLeapYear(year))
        return 29;

    return DAYS[month - 1];
}

bool Date::setDate(int newYear, int newMonth, int newDay)
{
    if (newYear < 1900)
        return false;

    if (newMonth < 1 || newMonth > 12)
        return false;

    if (newDay < 1 || newDay > daysInMonth(newYear, newMonth))
        return false;

    year = newYear;
    month = newMonth;
    day = newDay;
    weekday = weekdayFor(newYear, newMonth, newDay);
    valid = true;

    return true;
}

bool Date::isValid() const
{
    return valid;
}

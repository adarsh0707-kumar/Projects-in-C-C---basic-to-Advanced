#include "TimeFormatter.hpp"

#include <algorithm>

#include "Utility.hpp"

TimeFormatter::TimeFormatter()
    : currentTimeFormat(TimeFormat::Hour24),
      currentDateFormat(DateFormat::Long)
{
}

std::string TimeFormatter::formatTime(const Clock &clock) const
{
    if (currentTimeFormat == TimeFormat::Hour12)
        return clock.getTime12();

    return clock.getTime24();
}

std::string TimeFormatter::formatTimeWide(const Clock &clock) const
{
    const int hour = (currentTimeFormat == TimeFormat::Hour12)
                         ? clock.getHour12()
                         : clock.getHour();

    std::string wide =
        Utility::padZero(hour) + " : " +
        Utility::padZero(clock.getMinute()) + " : " +
        Utility::padZero(clock.getSecond());

    if (currentTimeFormat == TimeFormat::Hour12)
        wide += " " + clock.getMeridiem();

    return wide;
}

std::string TimeFormatter::formatDate(const Date &date) const
{
    switch (currentDateFormat)
    {
    case DateFormat::DayFirst:
        return date.getDateDayFirst();

    case DateFormat::MonthFirst:
        return date.getDateMonthFirst();

    case DateFormat::Iso:
        return date.getDateIso();

    case DateFormat::Long:
        break;
    }

    return date.getDateLong();
}

std::string TimeFormatter::formatDateTime(const Clock &clock,
                                          const Date &date) const
{
    return formatDate(date) + " - " + formatTime(clock);
}

void TimeFormatter::setTimeFormat(TimeFormat format)
{
    currentTimeFormat = format;
}

void TimeFormatter::setDateFormat(DateFormat format)
{
    currentDateFormat = format;
}

TimeFormatter::TimeFormat TimeFormatter::timeFormat() const
{
    return currentTimeFormat;
}

TimeFormatter::DateFormat TimeFormatter::dateFormat() const
{
    return currentDateFormat;
}

TimeFormatter::TimeFormat TimeFormatter::timeFormatFromString(
    const std::string &text,
    TimeFormat fallback)
{
    const std::string value = Utility::toUpper(Utility::trim(text));

    if (value == "12" || value == "12H" || value == "12-HOUR" ||
        value == "12HOUR" || value == "AMPM")
    {
        return TimeFormat::Hour12;
    }

    if (value == "24" || value == "24H" || value == "24-HOUR" ||
        value == "24HOUR" || value == "MILITARY")
    {
        return TimeFormat::Hour24;
    }

    return fallback;
}

TimeFormatter::DateFormat TimeFormatter::dateFormatFromString(
    const std::string &text,
    DateFormat fallback)
{
    std::string value = Utility::toUpper(Utility::trim(text));

    // Treat slashes and dashes as equivalent separators.
    std::replace(value.begin(), value.end(), '/', '-');

    if (value == "LONG" || value == "FULL")
        return DateFormat::Long;

    if (value == "DD-MM-YYYY" || value == "DDMMYYYY")
        return DateFormat::DayFirst;

    if (value == "MM-DD-YYYY" || value == "MMDDYYYY")
        return DateFormat::MonthFirst;

    if (value == "YYYY-MM-DD" || value == "YYYYMMDD" || value == "ISO")
        return DateFormat::Iso;

    return fallback;
}

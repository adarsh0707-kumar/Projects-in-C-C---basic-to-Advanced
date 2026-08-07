#include "Alarm.hpp"

#include <sstream>
#include <vector>

#include "Utility.hpp"

namespace
{
    /** Minutes in a full day, used for wrap-around arithmetic. */
    const int MINUTES_PER_DAY = 24 * 60;

    /** Weekday abbreviations, indexed by Date::getWeekday(). */
    const char *const DAY_NAMES[] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    /**
     * @brief Splits text on a delimiter, trimming each field.
     */
    std::vector<std::string> split(const std::string &text, char delimiter)
    {
        std::vector<std::string> fields;
        std::istringstream stream(text);
        std::string field;

        while (std::getline(stream, field, delimiter))
            fields.push_back(Utility::trim(field));

        return fields;
    }

    /**
     * @brief Converts a day name to its mask bit.
     *
     * Accepts full names and three-letter abbreviations, case-insensitively.
     *
     * @param name Day name.
     * @param bit  Receives the matching bit.
     * @return true if @p name named a weekday.
     */
    bool dayBitFromName(const std::string &name, std::uint8_t &bit)
    {
        const std::string value = Utility::toUpper(Utility::trim(name));

        if (value == "SUN" || value == "SUNDAY")    { bit = Alarm::SUNDAY;    return true; }
        if (value == "MON" || value == "MONDAY")    { bit = Alarm::MONDAY;    return true; }
        if (value == "TUE" || value == "TUESDAY")   { bit = Alarm::TUESDAY;   return true; }
        if (value == "WED" || value == "WEDNESDAY") { bit = Alarm::WEDNESDAY; return true; }
        if (value == "THU" || value == "THURSDAY")  { bit = Alarm::THURSDAY;  return true; }
        if (value == "FRI" || value == "FRIDAY")    { bit = Alarm::FRIDAY;    return true; }
        if (value == "SAT" || value == "SATURDAY")  { bit = Alarm::SATURDAY;  return true; }

        return false;
    }
}

Alarm::Alarm()
    : hour(0),
      minute(0),
      repeat(Repeat::Once),
      dayMask(ALL_DAYS),
      enabled(false),
      snoozed(false),
      snoozeHour(0),
      snoozeMinute(0)
{
}

Alarm::Alarm(int alarmHour,
             int alarmMinute,
             const std::string &alarmLabel,
             Repeat alarmRepeat)
    : hour(0),
      minute(0),
      label(alarmLabel),
      repeat(alarmRepeat),
      dayMask(maskFor(alarmRepeat)),
      enabled(true),
      snoozed(false),
      snoozeHour(0),
      snoozeMinute(0)
{
    setTime(alarmHour, alarmMinute);
}

std::uint8_t Alarm::maskFor(Repeat repeat)
{
    switch (repeat)
    {
    case Repeat::Daily:
        return ALL_DAYS;

    case Repeat::Weekdays:
        return WEEKDAYS;

    case Repeat::Weekends:
        return WEEKENDS;

    case Repeat::Once:
        // A one-shot alarm may land on any day; it disarms after ringing.
        return ALL_DAYS;

    case Repeat::Custom:
        break;
    }

    return ALL_DAYS;
}

bool Alarm::setTime(int newHour, int newMinute)
{
    if (newHour < 0 || newHour > 23)
        return false;

    if (newMinute < 0 || newMinute > 59)
        return false;

    hour = newHour;
    minute = newMinute;

    return true;
}

int Alarm::getHour() const
{
    return hour;
}

int Alarm::getMinute() const
{
    return minute;
}

void Alarm::setLabel(const std::string &newLabel)
{
    label = newLabel;
}

std::string Alarm::getLabel() const
{
    return label;
}

void Alarm::setRepeat(Repeat newRepeat)
{
    repeat = newRepeat;

    // Named modes define their own day set; Custom keeps whatever is there.
    if (newRepeat != Repeat::Custom)
        dayMask = maskFor(newRepeat);
}

Alarm::Repeat Alarm::getRepeat() const
{
    return repeat;
}

void Alarm::setDayMask(std::uint8_t mask)
{
    dayMask = static_cast<std::uint8_t>(mask & ALL_DAYS);

    // Recognise a mask that happens to match a named mode, so that
    // repeatString() reports the friendlier name.
    if (dayMask == ALL_DAYS)
        repeat = Repeat::Daily;
    else if (dayMask == WEEKDAYS)
        repeat = Repeat::Weekdays;
    else if (dayMask == WEEKENDS)
        repeat = Repeat::Weekends;
    else
        repeat = Repeat::Custom;
}

std::uint8_t Alarm::getDayMask() const
{
    return dayMask;
}

bool Alarm::appliesOn(int weekday) const
{
    if (weekday < 0 || weekday > 6)
        return false;

    const std::uint8_t bit =
        static_cast<std::uint8_t>(1 << static_cast<unsigned>(weekday));

    return (dayMask & bit) != 0;
}

void Alarm::setEnabled(bool isEnabled)
{
    enabled = isEnabled;

    if (!enabled)
        clearSnooze();
}

bool Alarm::isEnabled() const
{
    return enabled;
}

int Alarm::getEffectiveHour() const
{
    return snoozed ? snoozeHour : hour;
}

int Alarm::getEffectiveMinute() const
{
    return snoozed ? snoozeMinute : minute;
}

bool Alarm::isDueAt(const Clock &clock, const Date &date) const
{
    if (!enabled)
        return false;

    /*
    A snoozed alarm rings at its snooze time regardless of the day rule: the
    user has already been woken, and a snooze that crosses midnight should
    still ring.
    */
    if (snoozed)
    {
        return clock.getHour() == snoozeHour &&
               clock.getMinute() == snoozeMinute;
    }

    if (!appliesOn(date.getWeekday()))
        return false;

    return clock.getHour() == hour && clock.getMinute() == minute;
}

bool Alarm::snooze(int fromHour, int fromMinute, int minutes)
{
    if (minutes <= 0)
        return false;

    if (fromHour < 0 || fromHour > 23 || fromMinute < 0 || fromMinute > 59)
        return false;

    const int target =
        ((fromHour * 60 + fromMinute) + minutes) % MINUTES_PER_DAY;

    snoozeHour = target / 60;
    snoozeMinute = target % 60;
    snoozed = true;

    return true;
}

void Alarm::clearSnooze()
{
    snoozed = false;
    snoozeHour = 0;
    snoozeMinute = 0;
}

bool Alarm::isSnoozed() const
{
    return snoozed;
}

int Alarm::minutesUntil(const Clock &clock,
                        const Date &date,
                        bool excludeNow) const
{
    if (!enabled)
        return -1;

    const int now = clock.getHour() * 60 + clock.getMinute();

    if (snoozed)
    {
        const int target = snoozeHour * 60 + snoozeMinute;
        return ((target - now) + MINUTES_PER_DAY) % MINUTES_PER_DAY;
    }

    if (dayMask == 0)
        return -1;

    const int target = hour * 60 + minute;

    // Walk forward up to a week to find the next day the rule allows.
    for (int offset = 0; offset < 7; ++offset)
    {
        const int weekday = (date.getWeekday() + offset) % 7;

        if (!appliesOn(weekday))
            continue;

        if (offset == 0)
        {
            // Today still counts if the time has not yet passed. When the
            // caller has excluded the current minute, an alarm due exactly
            // now belongs to the next occurrence instead.
            if (target > now || (target == now && !excludeNow))
                return target - now;

            continue;
        }

        return offset * MINUTES_PER_DAY - now + target;
    }

    // The rule allows only today, and today's time has already passed.
    return MINUTES_PER_DAY * 7 - now + target;
}

std::string Alarm::timeString() const
{
    return Utility::padZero(hour) + ":" + Utility::padZero(minute);
}

std::string Alarm::dayAbbreviation(int weekday)
{
    if (weekday < 0 || weekday > 6)
        return "";

    return DAY_NAMES[weekday];
}

std::string Alarm::repeatString() const
{
    switch (repeat)
    {
    case Repeat::Once:
        return "Once";

    case Repeat::Daily:
        return "Daily";

    case Repeat::Weekdays:
        return "Weekdays";

    case Repeat::Weekends:
        return "Weekends";

    case Repeat::Custom:
        break;
    }

    std::string days;

    for (int weekday = 0; weekday < 7; ++weekday)
    {
        if (!appliesOn(weekday))
            continue;

        if (!days.empty())
            days += ",";

        days += dayAbbreviation(weekday);
    }

    return days.empty() ? "Never" : days;
}

std::string Alarm::describe() const
{
    std::string text = timeString() + " " + repeatString();

    if (!label.empty())
        text += " - " + label;

    if (snoozed)
    {
        text += " (snoozed to " +
                Utility::padZero(snoozeHour) + ":" +
                Utility::padZero(snoozeMinute) + ")";
    }

    if (!enabled)
        text += " [disabled]";

    return text;
}

bool Alarm::repeatFromString(const std::string &text,
                             Repeat &repeat,
                             std::uint8_t &dayMask)
{
    const std::string value = Utility::toUpper(Utility::trim(text));

    if (value.empty())
        return false;

    if (value == "ONCE" || value == "ONE" || value == "SINGLE")
    {
        repeat = Repeat::Once;
        dayMask = maskFor(Repeat::Once);
        return true;
    }

    if (value == "DAILY" || value == "EVERYDAY" || value == "EVERY DAY" ||
        value == "ALL")
    {
        repeat = Repeat::Daily;
        dayMask = maskFor(Repeat::Daily);
        return true;
    }

    if (value == "WEEKDAYS" || value == "WEEKDAY")
    {
        repeat = Repeat::Weekdays;
        dayMask = maskFor(Repeat::Weekdays);
        return true;
    }

    if (value == "WEEKENDS" || value == "WEEKEND")
    {
        repeat = Repeat::Weekends;
        dayMask = maskFor(Repeat::Weekends);
        return true;
    }

    // Otherwise treat it as a comma-separated day list.
    std::uint8_t mask = 0;

    for (const std::string &day : split(value, ','))
    {
        if (day.empty())
            continue;

        std::uint8_t bit = 0;

        if (!dayBitFromName(day, bit))
            return false;

        mask = static_cast<std::uint8_t>(mask | bit);
    }

    if (mask == 0)
        return false;

    dayMask = mask;

    if (mask == ALL_DAYS)
        repeat = Repeat::Daily;
    else if (mask == WEEKDAYS)
        repeat = Repeat::Weekdays;
    else if (mask == WEEKENDS)
        repeat = Repeat::Weekends;
    else
        repeat = Repeat::Custom;

    return true;
}

bool Alarm::parse(const std::string &text, Alarm &alarm)
{
    std::string source = Utility::trim(text);

    if (source.empty())
        return false;

    // A leading '!' keeps the alarm in the file but disarmed.
    bool startEnabled = true;

    if (source.front() == '!')
    {
        startEnabled = false;
        source = Utility::trim(source.substr(1));
    }

    const std::vector<std::string> fields = split(source, '|');

    if (fields.empty() || fields[0].empty())
        return false;

    // Field 1: HH:MM
    const std::vector<std::string> parts = split(fields[0], ':');

    if (parts.size() != 2)
        return false;

    if (parts[0].empty() || parts[1].empty())
        return false;

    const int parsedHour = Utility::toInt(parts[0], -1);
    const int parsedMinute = Utility::toInt(parts[1], -1);

    Alarm parsed;

    if (!parsed.setTime(parsedHour, parsedMinute))
        return false;

    // Field 2: label (optional)
    if (fields.size() > 1)
        parsed.setLabel(fields[1]);

    // Field 3: recurrence (optional, defaults to Once)
    if (fields.size() > 2 && !fields[2].empty())
    {
        Repeat parsedRepeat = Repeat::Once;
        std::uint8_t parsedMask = ALL_DAYS;

        if (!repeatFromString(fields[2], parsedRepeat, parsedMask))
            return false;

        parsed.repeat = parsedRepeat;
        parsed.dayMask = parsedMask;
    }

    parsed.setEnabled(startEnabled);

    alarm = parsed;

    return true;
}

std::string Alarm::toConfigString() const
{
    std::string text;

    if (!enabled)
        text += "!";

    text += timeString();
    text += " | " + label;
    text += " | " + repeatString();

    return text;
}

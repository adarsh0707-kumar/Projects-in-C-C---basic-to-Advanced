#include "AlarmManager.hpp"

#include "ConfigurationManager.hpp"
#include "Utility.hpp"

namespace
{
    /**
     * @brief Encodes a date as YYYYMMDD, for comparing firing days.
     */
    int dateKey(const Date &date)
    {
        return date.getYear() * 10000 + date.getMonth() * 100 + date.getDay();
    }
}

const int AlarmManager::MAX_ALARMS = 64;
const int AlarmManager::DEFAULT_SNOOZE_MINUTES = 5;

AlarmManager::AlarmManager()
    : activeIndex(-1),
      snoozeDelay(DEFAULT_SNOOZE_MINUTES),
      invalidEntries(0)
{
}

bool AlarmManager::load(const std::string &path)
{
    clear();

    invalidEntries = 0;

    if (!resources.exists(path))
        return false;

    ConfigurationManager file;

    if (!file.load(resources.resolve(path)))
        return false;

    for (int index = 1; index <= MAX_ALARMS; ++index)
    {
        const std::string key = "Alarm" + std::to_string(index);

        if (!file.hasValue(key))
            continue;

        const std::string value = file.getValue(key);

        if (Utility::trim(value).empty())
            continue;

        Alarm alarm;

        if (!Alarm::parse(value, alarm))
        {
            // One malformed entry must not discard the rest of the file.
            ++invalidEntries;
            continue;
        }

        add(alarm);
    }

    return true;
}

void AlarmManager::add(const Alarm &alarm)
{
    Entry entry;
    entry.alarm = alarm;
    entry.lastFiredDate = 0;
    entry.lastFiredMinute = -1;

    entries.push_back(entry);
}

void AlarmManager::clear()
{
    entries.clear();
    activeIndex = -1;
}

std::size_t AlarmManager::count() const
{
    return entries.size();
}

std::size_t AlarmManager::invalidCount() const
{
    return invalidEntries;
}

Alarm &AlarmManager::at(std::size_t index)
{
    return entries.at(index).alarm;
}

const Alarm &AlarmManager::at(std::size_t index) const
{
    return entries.at(index).alarm;
}

std::vector<Alarm> AlarmManager::all() const
{
    std::vector<Alarm> copies;
    copies.reserve(entries.size());

    for (const Entry &entry : entries)
        copies.push_back(entry.alarm);

    return copies;
}

bool AlarmManager::poll(const Clock &clock, const Date &date)
{
    // Only one alarm rings at a time; the rest wait their turn.
    if (activeIndex >= 0)
        return false;

    const int today = dateKey(date);
    const int nowMinute = clock.getHour() * 60 + clock.getMinute();

    for (std::size_t index = 0; index < entries.size(); ++index)
    {
        Entry &entry = entries[index];

        if (!entry.alarm.isDueAt(clock, date))
            continue;

        /*
        An alarm reports itself due for the whole of its minute. Recording the
        date and minute of each firing keeps it to one ring, while still
        allowing the same alarm to fire again tomorrow at the same time.
        */
        if (entry.lastFiredDate == today &&
            entry.lastFiredMinute == nowMinute)
        {
            continue;
        }

        entry.lastFiredDate = today;
        entry.lastFiredMinute = nowMinute;

        activeIndex = static_cast<int>(index);

        return true;
    }

    return false;
}

Alarm *AlarmManager::active()
{
    if (activeIndex < 0)
        return nullptr;

    return &entries[static_cast<std::size_t>(activeIndex)].alarm;
}

const Alarm *AlarmManager::active() const
{
    if (activeIndex < 0)
        return nullptr;

    return &entries[static_cast<std::size_t>(activeIndex)].alarm;
}

bool AlarmManager::isRinging() const
{
    return activeIndex >= 0;
}

bool AlarmManager::snoozeActive(const Clock &clock)
{
    Alarm *alarm = active();

    if (alarm == nullptr)
        return false;

    if (!alarm->snooze(clock.getHour(), clock.getMinute(), snoozeDelay))
        return false;

    activeIndex = -1;

    return true;
}

bool AlarmManager::dismissActive()
{
    Alarm *alarm = active();

    if (alarm == nullptr)
        return false;

    alarm->clearSnooze();

    // A one-shot alarm has now served its purpose.
    if (alarm->getRepeat() == Alarm::Repeat::Once)
        alarm->setEnabled(false);

    activeIndex = -1;

    return true;
}

void AlarmManager::setSnoozeMinutes(int minutes)
{
    if (minutes < 1)
        return;

    snoozeDelay = minutes;
}

int AlarmManager::snoozeMinutes() const
{
    return snoozeDelay;
}

int AlarmManager::nextIndex(const Clock &clock, const Date &date) const
{
    int best = -1;
    int bestMinutes = -1;

    const int today = dateKey(date);
    const int nowMinute = clock.getHour() * 60 + clock.getMinute();

    for (std::size_t index = 0; index < entries.size(); ++index)
    {
        /*
        An alarm that already rang this minute will not ring again until its
        next occurrence, so the countdown must skip the current minute rather
        than reporting it as due now.
        */
        const bool alreadyFired =
            entries[index].lastFiredDate == today &&
            entries[index].lastFiredMinute == nowMinute;

        const int minutes =
            entries[index].alarm.minutesUntil(clock, date, alreadyFired);

        if (minutes < 0)
            continue;

        if (best < 0 || minutes < bestMinutes)
        {
            best = static_cast<int>(index);
            bestMinutes = minutes;
        }
    }

    return best;
}

std::string AlarmManager::describeDuration(int minutes)
{
    if (minutes <= 0)
        return "now";

    const int days = minutes / (24 * 60);
    const int hours = (minutes % (24 * 60)) / 60;
    const int remainder = minutes % 60;

    std::string text;

    if (days > 0)
        text += std::to_string(days) + "d ";

    if (hours > 0)
        text += std::to_string(hours) + "h ";

    // Suppress a trailing "0m" only when a larger unit already carries meaning.
    if (remainder > 0 || text.empty())
        text += std::to_string(remainder) + "m";

    return Utility::trim(text);
}

std::string AlarmManager::nextSummary(const Clock &clock,
                                      const Date &date) const
{
    const int index = nextIndex(clock, date);

    if (index < 0)
        return "";

    const Entry &entry = entries[static_cast<std::size_t>(index)];
    const Alarm &alarm = entry.alarm;

    const bool alreadyFired =
        entry.lastFiredDate == dateKey(date) &&
        entry.lastFiredMinute == clock.getHour() * 60 + clock.getMinute();

    std::string summary =
        Utility::padZero(alarm.getEffectiveHour()) + ":" +
        Utility::padZero(alarm.getEffectiveMinute());

    if (!alarm.getLabel().empty())
        summary += " " + alarm.getLabel();

    const int minutes = alarm.minutesUntil(clock, date, alreadyFired);

    // "in now" does not read as English; an alarm due this minute is "due now".
    if (minutes <= 0)
        summary += " (due now)";
    else
        summary += " (in " + describeDuration(minutes) + ")";

    return summary;
}

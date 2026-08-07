#ifndef ALARM_MANAGER_HPP
#define ALARM_MANAGER_HPP

/******************************************************************************
 * @file AlarmManager.hpp
 * @brief Declaration of the AlarmManager service.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The AlarmManager owns the set of configured alarms and decides when one
 * should ring. It is the component that turns "this alarm is due" into "this
 * alarm has just fired", which requires remembering what has already fired:
 * an alarm reports itself due for a whole minute, but must ring only once.
 *
 * Introduced in v1.1.0 (Change Log, section 6.3).
 ******************************************************************************/

#include <cstddef>
#include <string>
#include <vector>

#include "Alarm.hpp"
#include "Clock.hpp"
#include "Date.hpp"
#include "ResourceManager.hpp"

/**
 * @class AlarmManager
 * @brief Loads, stores and fires alarms.
 *
 * Typical use per frame:
 * @code
 * if (alarms.poll(clock, date))
 *     notifier.notify(*alarms.active());
 * @endcode
 */
class AlarmManager
{
public:
    /** Highest alarm index scanned when loading a file. */
    static const int MAX_ALARMS;

    /** Snooze delay used when none is configured, in minutes. */
    static const int DEFAULT_SNOOZE_MINUTES;

    AlarmManager();

    /**
     * @brief Loads alarms from a configuration file.
     *
     * The file uses numbered keys, each holding one alarm in the form
     * accepted by Alarm::parse():
     *
     * @code{.ini}
     * Alarm1=07:30 | Wake up | Weekdays
     * Alarm2=13:00 | Lunch   | Daily
     * @endcode
     *
     * Indices need not be contiguous; every index from 1 to MAX_ALARMS is
     * examined. Entries that fail to parse are skipped and counted, so one bad
     * line does not discard the rest of the file.
     *
     * @param path Path of the alarm file.
     * @return true if the file was read. A false return means the file was
     *         absent or unreadable, which is not an error: the application
     *         simply runs with no alarms configured.
     */
    bool load(const std::string &path);

    /**
     * @brief Adds an alarm.
     * @param alarm Alarm to add.
     */
    void add(const Alarm &alarm);

    /**
     * @brief Removes every alarm and clears the active one.
     */
    void clear();

    /**
     * @brief Returns the number of configured alarms.
     * @return std::size_t Alarm count.
     */
    std::size_t count() const;

    /**
     * @brief Returns the number of entries skipped by the last load().
     * @return std::size_t Count of malformed entries.
     */
    std::size_t invalidCount() const;

    /**
     * @brief Returns an alarm by index.
     * @param index Zero-based index, less than count().
     * @return Alarm& The alarm.
     */
    Alarm &at(std::size_t index);

    /**
     * @brief Returns an alarm by index.
     * @param index Zero-based index, less than count().
     * @return const Alarm& The alarm.
     */
    const Alarm &at(std::size_t index) const;

    /**
     * @brief Returns a copy of every configured alarm.
     * @return std::vector<Alarm> The alarms, in configuration order.
     */
    std::vector<Alarm> all() const;

    /**
     * @brief Checks whether any alarm has just become due.
     *
     * An alarm fires at most once per calendar minute: the manager records the
     * date and minute of each firing and will not fire the same alarm again
     * until one of them changes. Only one alarm rings at a time; if a second
     * becomes due while the first is unacknowledged, the first keeps the
     * display and the second fires once it is dismissed.
     *
     * @param clock Current time.
     * @param date  Current date.
     * @return true if an alarm started ringing on this call.
     */
    bool poll(const Clock &clock, const Date &date);

    /**
     * @brief Returns the alarm that is currently ringing.
     * @return Alarm* The ringing alarm, or nullptr when none is.
     */
    Alarm *active();

    /**
     * @brief Returns the alarm that is currently ringing.
     * @return const Alarm* The ringing alarm, or nullptr when none is.
     */
    const Alarm *active() const;

    /**
     * @brief Reports whether an alarm is currently ringing.
     * @return true when an alarm awaits acknowledgement.
     */
    bool isRinging() const;

    /**
     * @brief Postpones the ringing alarm.
     *
     * @param clock Current time, which the snooze is measured from.
     * @return true if an alarm was ringing and has been snoozed.
     */
    bool snoozeActive(const Clock &clock);

    /**
     * @brief Acknowledges the ringing alarm.
     *
     * A one-shot alarm disarms itself; a recurring alarm stays armed for its
     * next occurrence.
     *
     * @return true if an alarm was ringing and has been dismissed.
     */
    bool dismissActive();

    /**
     * @brief Sets the snooze delay.
     * @param minutes Delay in minutes. Values below 1 are ignored.
     */
    void setSnoozeMinutes(int minutes);

    /**
     * @brief Returns the snooze delay.
     * @return int Delay in minutes.
     */
    int snoozeMinutes() const;

    /**
     * @brief Returns a one-line summary of the next alarm due.
     *
     * @param clock Current time.
     * @param date  Current date.
     * @return std::string Summary such as "07:30 Wake up (in 2h 15m)", or an
     *         empty string when no alarm is armed.
     */
    std::string nextSummary(const Clock &clock, const Date &date) const;

    /**
     * @brief Returns the index of the next alarm due.
     *
     * @param clock Current time.
     * @param date  Current date.
     * @return int Zero-based index, or -1 when no alarm is armed.
     */
    int nextIndex(const Clock &clock, const Date &date) const;

    /**
     * @brief Renders a duration in minutes as compact text.
     * @param minutes Duration in minutes.
     * @return std::string Text such as "2h 15m", "45m" or "now".
     */
    static std::string describeDuration(int minutes);

private:
    /**
     * @struct Entry
     * @brief An alarm plus the bookkeeping that stops it re-firing.
     */
    struct Entry
    {
        Alarm alarm;         ///< The configured alarm.
        int lastFiredDate;   ///< Date of the last firing, as YYYYMMDD.
        int lastFiredMinute; ///< Minute of day of the last firing, or -1.
    };

    std::vector<Entry> entries; ///< Configured alarms, in file order.
    int activeIndex;            ///< Index of the ringing alarm, or -1.
    int snoozeDelay;            ///< Snooze delay in minutes.
    std::size_t invalidEntries; ///< Malformed entries from the last load().
    ResourceManager resources;  ///< Locates the alarm file.
};

#endif // ALARM_MANAGER_HPP

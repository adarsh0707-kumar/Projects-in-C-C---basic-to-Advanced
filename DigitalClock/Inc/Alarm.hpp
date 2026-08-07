#ifndef ALARM_HPP
#define ALARM_HPP

/******************************************************************************
 * @file Alarm.hpp
 * @brief Declaration of the Alarm core component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * An Alarm is a time of day, a label, and a rule describing which days it
 * applies to. It knows whether it is due at a given moment and holds its own
 * snooze state, but it does not decide what to do about it: firing and
 * notification belong to AlarmManager and Notifier respectively.
 *
 * Introduced in v1.1.0 (Change Log, section 6.3).
 ******************************************************************************/

#include <cstdint>
#include <string>

#include "Clock.hpp"
#include "Date.hpp"

/**
 * @class Alarm
 * @brief A single alarm: when it rings, what it says, and how often.
 *
 * Recurrence is stored as a seven-bit day mask, one bit per weekday starting
 * at Sunday. The named repeat modes are conveniences that expand to a mask,
 * which keeps the due-check a single bit test regardless of how the alarm was
 * written in the configuration file.
 */
class Alarm
{
public:
    /**
     * @enum Repeat
     * @brief How often an alarm recurs.
     */
    enum class Repeat
    {
        Once,     ///< Rings at the next occurrence, then disarms itself.
        Daily,    ///< Every day.
        Weekdays, ///< Monday through Friday.
        Weekends, ///< Saturday and Sunday.
        Custom    ///< An explicit set of days, held in the day mask.
    };

    /** Day mask bit for each weekday, matching Date::getWeekday(). */
    enum DayBit : std::uint8_t
    {
        SUNDAY    = 1 << 0,
        MONDAY    = 1 << 1,
        TUESDAY   = 1 << 2,
        WEDNESDAY = 1 << 3,
        THURSDAY  = 1 << 4,
        FRIDAY    = 1 << 5,
        SATURDAY  = 1 << 6,

        ALL_DAYS  = 0x7F,
        WEEKDAYS  = MONDAY | TUESDAY | WEDNESDAY | THURSDAY | FRIDAY,
        WEEKENDS  = SATURDAY | SUNDAY
    };

    /**
     * @brief Constructs a disabled alarm at 00:00 with no label.
     */
    Alarm();

    /**
     * @brief Constructs an alarm.
     *
     * @param hour   Hour in the range 0-23.
     * @param minute Minute in the range 0-59.
     * @param label  Text shown when the alarm rings.
     * @param repeat Recurrence rule.
     */
    Alarm(int hour,
          int minute,
          const std::string &label,
          Repeat repeat = Repeat::Once);

    /**
     * @brief Sets the time at which the alarm rings.
     *
     * @param hour   Hour in the range 0-23.
     * @param minute Minute in the range 0-59.
     * @return true if the values were accepted.
     */
    bool setTime(int hour, int minute);

    /**
     * @brief Returns the hour at which the alarm rings.
     * @return int Hour in the range 0-23.
     */
    int getHour() const;

    /**
     * @brief Returns the minute at which the alarm rings.
     * @return int Minute in the range 0-59.
     */
    int getMinute() const;

    /**
     * @brief Sets the label shown when the alarm rings.
     * @param label Text to show. May be empty.
     */
    void setLabel(const std::string &label);

    /**
     * @brief Returns the label.
     * @return std::string Label text.
     */
    std::string getLabel() const;

    /**
     * @brief Sets the recurrence rule.
     *
     * Named modes overwrite the day mask. Repeat::Custom leaves the mask
     * alone, so set the mask first when building a custom rule.
     *
     * @param repeat Rule to apply.
     */
    void setRepeat(Repeat repeat);

    /**
     * @brief Returns the recurrence rule.
     * @return Repeat Current rule.
     */
    Repeat getRepeat() const;

    /**
     * @brief Sets the days on which the alarm applies.
     *
     * Also sets the repeat mode to Custom unless the mask matches one of the
     * named modes exactly.
     *
     * @param mask Bitwise OR of DayBit values.
     */
    void setDayMask(std::uint8_t mask);

    /**
     * @brief Returns the day mask.
     * @return std::uint8_t Bitwise OR of DayBit values.
     */
    std::uint8_t getDayMask() const;

    /**
     * @brief Reports whether the alarm applies on a given weekday.
     * @param weekday 0 for Sunday through 6 for Saturday.
     * @return true if the alarm may ring that day.
     */
    bool appliesOn(int weekday) const;

    /**
     * @brief Arms or disarms the alarm.
     * @param enabled true to arm.
     */
    void setEnabled(bool enabled);

    /**
     * @brief Reports whether the alarm is armed.
     * @return true when armed.
     */
    bool isEnabled() const;

    /**
     * @brief Reports whether the alarm should ring at the given moment.
     *
     * An alarm is due when it is armed, the weekday matches, and the current
     * time equals its time (or its snooze time). It reports due for the whole
     * of the matching minute; AlarmManager is responsible for firing it only
     * once, since Alarm itself holds no notion of "already fired".
     *
     * @param clock Current time.
     * @param date  Current date, used for the weekday.
     * @return true when the alarm should ring.
     */
    bool isDueAt(const Clock &clock, const Date &date) const;

    /**
     * @brief Postpones the alarm by a number of minutes.
     *
     * The snooze time is stored as an absolute time of day, so a snooze that
     * crosses midnight wraps correctly.
     *
     * @param fromHour   Hour the snooze starts from.
     * @param fromMinute Minute the snooze starts from.
     * @param minutes    Delay in minutes. Must be positive.
     * @return true if the snooze was applied.
     */
    bool snooze(int fromHour, int fromMinute, int minutes);

    /**
     * @brief Cancels any pending snooze.
     */
    void clearSnooze();

    /**
     * @brief Reports whether the alarm is currently snoozed.
     * @return true when a snooze is pending.
     */
    bool isSnoozed() const;

    /**
     * @brief Returns the hour the alarm will next ring at.
     *
     * This is the snooze hour when snoozed, otherwise the alarm hour.
     *
     * @return int Hour in the range 0-23.
     */
    int getEffectiveHour() const;

    /**
     * @brief Returns the minute the alarm will next ring at.
     * @return int Minute in the range 0-59.
     */
    int getEffectiveMinute() const;

    /**
     * @brief Returns minutes from the given time until the alarm next rings.
     *
     * Used to decide which of several alarms is next. A result of 0 means the
     * alarm is due now.
     *
     * @param clock      Current time.
     * @param date       Current date, used for the weekday.
     * @param excludeNow When true, an alarm whose time equals the current
     *                   minute is treated as already past, so the result is
     *                   its next occurrence rather than zero. AlarmManager
     *                   sets this for an alarm that has already fired this
     *                   minute, so the countdown does not read "due now"
     *                   after the alarm has been acknowledged.
     * @return int Minutes until the alarm rings, or -1 when it never will.
     */
    int minutesUntil(const Clock &clock,
                     const Date &date,
                     bool excludeNow = false) const;

    /**
     * @brief Returns a one-line description, such as "07:30 Daily - Wake up".
     * @return std::string Human-readable summary.
     */
    std::string describe() const;

    /**
     * @brief Returns the time as @c HH:MM.
     * @return std::string Formatted time.
     */
    std::string timeString() const;

    /**
     * @brief Returns the recurrence as text, such as "Weekdays" or "Mon,Fri".
     * @return std::string Human-readable recurrence.
     */
    std::string repeatString() const;

    /**
     * @brief Parses an alarm from its configuration form.
     *
     * The accepted form is three pipe-separated fields, the last two optional:
     *
     * @code
     * 07:30 | Wake up | Daily
     * 13:00 | Lunch
     * 22:15
     * @endcode
     *
     * The recurrence field accepts @c Once, @c Daily, @c Weekdays,
     * @c Weekends, or a comma-separated day list such as @c Mon,Wed,Fri.
     * A leading @c ! disables the alarm without removing it.
     *
     * @param text  Text to parse.
     * @param alarm Receives the parsed alarm on success.
     * @return true if @p text was a well-formed alarm.
     */
    static bool parse(const std::string &text, Alarm &alarm);

    /**
     * @brief Renders the alarm back to its configuration form.
     * @return std::string Text that parse() would accept.
     */
    std::string toConfigString() const;

    /**
     * @brief Converts a repeat name to a Repeat.
     *
     * @param text     Name such as "Daily". Case-insensitive.
     * @param repeat   Receives the parsed mode.
     * @param dayMask  Receives the matching day mask.
     * @return true if @p text was recognised as a named mode or a day list.
     */
    static bool repeatFromString(const std::string &text,
                                 Repeat &repeat,
                                 std::uint8_t &dayMask);

    /**
     * @brief Returns the day mask implied by a named repeat mode.
     * @param repeat Mode to expand.
     * @return std::uint8_t Day mask, or ALL_DAYS for Once.
     */
    static std::uint8_t maskFor(Repeat repeat);

    /**
     * @brief Returns the three-letter abbreviation for a weekday.
     * @param weekday 0 for Sunday through 6 for Saturday.
     * @return std::string Abbreviation such as "Mon", or empty if out of range.
     */
    static std::string dayAbbreviation(int weekday);

private:
    int hour;               ///< 0-23.
    int minute;             ///< 0-59.
    std::string label;      ///< Text shown when ringing.
    Repeat repeat;          ///< Recurrence mode.
    std::uint8_t dayMask;   ///< Days the alarm applies to.
    bool enabled;           ///< Whether the alarm is armed.
    bool snoozed;           ///< Whether a snooze is pending.
    int snoozeHour;         ///< Hour the snooze expires, when snoozed.
    int snoozeMinute;       ///< Minute the snooze expires, when snoozed.
};

#endif // ALARM_HPP

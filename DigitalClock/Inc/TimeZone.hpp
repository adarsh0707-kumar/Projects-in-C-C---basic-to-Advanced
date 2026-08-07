#ifndef TIME_ZONE_HPP
#define TIME_ZONE_HPP

/******************************************************************************
 * @file TimeZone.hpp
 * @brief Declaration of the TimeZone core component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * A TimeZone converts a UTC instant into wall-clock time somewhere else.
 *
 * C++17 has no timezone database -- std::chrono::time_zone arrived in C++20 --
 * so this is built on what C++17 and POSIX actually provide. Two kinds of zone
 * are supported, and they differ in an important way:
 *
 *   Fixed offset   UTC+05:30, -08:00. Arithmetic on a UTC instant. Works
 *                  identically on every platform and is exactly reproducible,
 *                  but has no notion of daylight saving: a fixed -05:00 is
 *                  wrong for New York for the months it observes EDT.
 *
 *   Named zone     Asia/Kolkata, America/New_York. Resolved through the
 *                  platform's own timezone database, so daylight saving is
 *                  handled correctly. Requires that database, which POSIX
 *                  systems have and Windows does not expose under these names.
 *
 * The distinction is deliberately visible in the API rather than hidden, since
 * choosing a fixed offset for a DST-observing region is a correctness decision
 * the user is making, not an implementation detail.
 *
 * Introduced in v1.3.0 (Change Log, section 6.3).
 ******************************************************************************/

#include <ctime>
#include <string>

/**
 * @class TimeZone
 * @brief Converts a UTC instant to wall-clock time in one zone.
 */
class TimeZone
{
public:
    /**
     * @enum Kind
     * @brief How the zone determines its offset.
     */
    enum class Kind
    {
        FixedOffset, ///< A constant offset from UTC; no daylight saving.
        Named        ///< Resolved by the platform's timezone database.
    };

    /** Largest accepted offset from UTC, in minutes (UTC+14:00). */
    static const int MAX_OFFSET_MINUTES;

    /** Smallest accepted offset from UTC, in minutes (UTC-12:00). */
    static const int MIN_OFFSET_MINUTES;

    /**
     * @brief Constructs UTC.
     */
    TimeZone();

    /**
     * @brief Sets the zone to a constant offset from UTC.
     *
     * @param minutes Offset in minutes, from MIN_OFFSET_MINUTES to
     *                MAX_OFFSET_MINUTES.
     * @return true if the offset was accepted.
     */
    bool setFixedOffset(int minutes);

    /**
     * @brief Sets the zone to a name resolved by the platform.
     *
     * The name is not validated here: whether it resolves is a property of
     * the platform's database, and is reported by resolves().
     *
     * @param name Zone name, such as "Asia/Kolkata".
     * @return true if @p name was non-empty.
     */
    bool setNamedZone(const std::string &name);

    /**
     * @brief Sets the label shown next to the time.
     * @param label Display label. When empty, the specification is shown.
     */
    void setLabel(const std::string &label);

    /**
     * @brief Returns the display label.
     * @return std::string Label, or the specification when none was given.
     */
    std::string label() const;

    /**
     * @brief Returns how this zone determines its offset.
     * @return Kind Fixed offset or named.
     */
    Kind kind() const;

    /**
     * @brief Returns the zone name, for a named zone.
     * @return std::string Zone name, empty for a fixed offset.
     */
    std::string name() const;

    /**
     * @brief Converts a UTC instant to wall-clock time in this zone.
     *
     * @param utc Instant to convert.
     * @param out Receives the broken-down local time.
     * @return true on success. A named zone that the platform cannot resolve
     *         yields UTC and returns false, so the caller can report it
     *         rather than silently showing the wrong time.
     */
    bool localTime(std::time_t utc, std::tm &out) const;

    /**
     * @brief Returns this zone's offset from UTC at a given instant.
     *
     * For a named zone the offset depends on the instant, because daylight
     * saving may or may not be in effect.
     *
     * @param utc Instant to evaluate at.
     * @return int Offset in minutes; 0 when the zone cannot be resolved.
     */
    int offsetMinutes(std::time_t utc) const;

    /**
     * @brief Reports whether this zone can be resolved on this platform.
     *
     * Always true for a fixed offset. For a named zone, true when the
     * platform's database recognises the name.
     *
     * @return true when the zone yields a meaningful time.
     */
    bool resolves() const;

    /**
     * @brief Formats the time in this zone.
     *
     * @param utc    Instant to format.
     * @param hour12 true for 12-hour with a meridiem, false for 24-hour.
     * @return std::string Formatted time, such as "14:05" or "02:05 PM".
     */
    std::string formatTime(std::time_t utc, bool hour12) const;

    /**
     * @brief Returns the offset as text, such as "+05:30" or "-08:00".
     * @param utc Instant to evaluate at.
     * @return std::string Formatted offset.
     */
    std::string formatOffset(std::time_t utc) const;

    /**
     * @brief Parses a zone from configuration text.
     *
     * Accepted forms, with an optional trailing label after a pipe:
     *
     * @code
     * UTC
     * UTC+05:30 | Home
     * +05:30
     * -08:00 | Pacific
     * Asia/Kolkata | Home
     * @endcode
     *
     * A specification containing '/' is treated as a named zone; anything
     * else is parsed as an offset.
     *
     * @param text Text to parse.
     * @param zone Receives the parsed zone on success.
     * @return true if @p text was a well-formed specification.
     */
    static bool parse(const std::string &text, TimeZone &zone);

    /**
     * @brief Parses an offset specification into minutes.
     *
     * Accepts @c UTC, @c UTC+HH:MM, @c +HH:MM, @c -HH:MM and @c +HH.
     *
     * @param text    Text to parse.
     * @param minutes Receives the offset.
     * @return true if @p text was a well-formed offset.
     */
    static bool parseOffset(const std::string &text, int &minutes);

private:
    Kind zoneKind;         ///< Fixed offset or named.
    int fixedMinutes;      ///< Offset, for a fixed-offset zone.
    std::string zoneName;  ///< Zone name, for a named zone.
    std::string zoneLabel; ///< Display label.
    std::string spec;      ///< Original specification, used as a fallback label.
};

#endif // TIME_ZONE_HPP

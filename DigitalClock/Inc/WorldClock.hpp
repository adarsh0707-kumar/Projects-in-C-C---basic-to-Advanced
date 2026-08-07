#ifndef WORLD_CLOCK_HPP
#define WORLD_CLOCK_HPP

/******************************************************************************
 * @file WorldClock.hpp
 * @brief Declaration of the WorldClock core component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * A WorldClock holds the set of additional time zones configured for display
 * and renders them as aligned rows.
 *
 * Introduced in v1.3.0 (Change Log, section 6.3).
 ******************************************************************************/

#include <cstddef>
#include <ctime>
#include <string>
#include <vector>

#include "TimeZone.hpp"

/**
 * @class WorldClock
 * @brief The configured set of additional time zones.
 */
class WorldClock
{
public:
    /** Largest number of zones accepted from configuration. */
    static const std::size_t MAX_ZONES;

    WorldClock();

    /**
     * @brief Loads zones from a comma-separated configuration value.
     *
     * @code
     * TimeZones=UTC, Asia/Kolkata | Home, -08:00 | Pacific
     * @endcode
     *
     * Entries that fail to parse are skipped and counted rather than
     * discarding the rest, matching how alarms are loaded.
     *
     * @param value Configuration value.
     * @return true if at least one zone was loaded.
     */
    bool load(const std::string &value);

    /**
     * @brief Adds a zone.
     * @param zone Zone to add.
     * @return true if it was added, false once MAX_ZONES is reached.
     */
    bool add(const TimeZone &zone);

    /**
     * @brief Removes every zone.
     */
    void clear();

    /**
     * @brief Returns the number of configured zones.
     * @return std::size_t Zone count.
     */
    std::size_t count() const;

    /**
     * @brief Returns the number of entries the last load() could not parse.
     * @return std::size_t Malformed entry count.
     */
    std::size_t invalidCount() const;

    /**
     * @brief Returns the number of zones the platform cannot resolve.
     * @return std::size_t Unresolvable zone count.
     */
    std::size_t unresolvedCount() const;

    /**
     * @brief Returns a zone by index.
     * @param index Zero-based index, less than count().
     * @return const TimeZone& The zone.
     */
    const TimeZone &at(std::size_t index) const;

    /**
     * @brief Renders the zones as aligned rows.
     *
     * Labels are padded to a common width so the times line up. A zone the
     * platform cannot resolve is marked, rather than quietly showing UTC
     * under someone else's label.
     *
     * @param utc    Instant to render.
     * @param hour12 true for 12-hour times.
     * @return std::vector<std::string> One row per zone.
     */
    std::vector<std::string> rows(std::time_t utc, bool hour12) const;

private:
    std::vector<TimeZone> zones; ///< Configured zones, in file order.
    std::size_t invalidEntries;  ///< Malformed entries from the last load().
};

#endif // WORLD_CLOCK_HPP

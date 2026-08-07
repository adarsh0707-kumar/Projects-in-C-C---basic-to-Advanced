#include "WorldClock.hpp"

#include <algorithm>
#include <sstream>

#include "Utility.hpp"

const std::size_t WorldClock::MAX_ZONES = 8;

WorldClock::WorldClock()
    : invalidEntries(0)
{
}

bool WorldClock::load(const std::string &value)
{
    clear();

    std::istringstream stream(value);
    std::string entry;

    while (std::getline(stream, entry, ','))
    {
        const std::string trimmed = Utility::trim(entry);

        if (trimmed.empty())
            continue;

        TimeZone zone;

        if (!TimeZone::parse(trimmed, zone))
        {
            // One bad entry must not discard the rest of the list.
            ++invalidEntries;
            continue;
        }

        if (!add(zone))
            break;
    }

    return !zones.empty();
}

bool WorldClock::add(const TimeZone &zone)
{
    if (zones.size() >= MAX_ZONES)
        return false;

    zones.push_back(zone);

    return true;
}

void WorldClock::clear()
{
    zones.clear();
    invalidEntries = 0;
}

std::size_t WorldClock::count() const
{
    return zones.size();
}

std::size_t WorldClock::invalidCount() const
{
    return invalidEntries;
}

std::size_t WorldClock::unresolvedCount() const
{
    return static_cast<std::size_t>(
        std::count_if(
            zones.begin(),
            zones.end(),
            [](const TimeZone &zone)
            { return !zone.resolves(); }));
}

const TimeZone &WorldClock::at(std::size_t index) const
{
    return zones.at(index);
}

std::vector<std::string> WorldClock::rows(std::time_t utc, bool hour12) const
{
    std::vector<std::string> result;

    if (zones.empty())
        return result;

    // Pad labels to a common width so the times form a column.
    std::size_t labelWidth = 0;

    for (const TimeZone &zone : zones)
        labelWidth = std::max(labelWidth, zone.label().size());

    for (const TimeZone &zone : zones)
    {
        std::string row = zone.label();
        row.append(labelWidth - zone.label().size(), ' ');

        row += "  " + zone.formatTime(utc, hour12);
        row += "  (UTC" + zone.formatOffset(utc) + ")";

        /*
        A named zone the platform cannot resolve falls back to UTC. Saying so
        is the whole point: showing UTC under a "Tokyo" label would be a
        wrong answer presented as a right one.
        */
        if (!zone.resolves())
            row += "  [unavailable on this platform]";

        result.push_back(row);
    }

    return result;
}

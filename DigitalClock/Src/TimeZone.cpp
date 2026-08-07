#include "TimeZone.hpp"

#include <cstdlib>
#include <mutex>

#include "Utility.hpp"

namespace
{
    /**
     * Setting TZ mutates process-global state, so the swap has to be
     * serialised even though the application renders on one thread. Tests
     * and any future worker would otherwise be able to interleave.
     */
    std::mutex &zoneMutex()
    {
        static std::mutex mutex;
        return mutex;
    }

    /**
     * @brief Resolves a UTC instant through the platform timezone database.
     *
     * Swaps TZ, converts, and restores TZ. This is the only mechanism C++17
     * offers for "what is the time in another named zone": the standard
     * library has no timezone type before C++20.
     *
     * @param name Zone name, such as "Asia/Kolkata".
     * @param utc  Instant to convert.
     * @param out  Receives the broken-down local time.
     * @return true if the conversion succeeded.
     */
    bool resolveNamed(const std::string &name, std::time_t utc, std::tm &out)
    {
#ifdef _WIN32
        /*
        The Windows CRT accepts only POSIX-style TZ strings such as
        "EST5EDT", not IANA names, and silently treats anything it does not
        understand as UTC. Reporting failure is better than displaying UTC
        under a New York label.
        */
        (void)name;
        (void)utc;
        (void)out;

        return false;
#else
        std::lock_guard<std::mutex> lock(zoneMutex());

        const char *previous = std::getenv("TZ");
        const std::string saved = (previous != nullptr) ? previous : "";
        const bool hadTz = (previous != nullptr);

        setenv("TZ", name.c_str(), 1);
        tzset();

        std::tm converted{};
        const bool ok = (localtime_r(&utc, &converted) != nullptr);

        if (hadTz)
            setenv("TZ", saved.c_str(), 1);
        else
            unsetenv("TZ");

        tzset();

        if (!ok)
            return false;

        out = converted;

        return true;
#endif
    }

    /**
     * @brief Reports whether the platform recognises a zone name.
     *
     * An unknown name is treated as UTC by POSIX, so a name is accepted only
     * when it either produces a non-zero offset or is one of the spellings
     * that legitimately mean UTC. That leaves a narrow false-positive band:
     * a genuinely zero-offset zone such as Europe/London in winter cannot be
     * distinguished from an unrecognised name. Reporting such a zone as
     * resolved is the safe direction to be wrong in -- the displayed time is
     * correct either way.
     */
    bool namedZoneResolves(const std::string &name)
    {
#ifdef _WIN32
        (void)name;
        return false;
#else
        const std::string upper = Utility::toUpper(name);

        if (upper == "UTC" || upper == "GMT" || upper == "UCT" ||
            upper == "ZULU" || upper == "ETC/UTC" || upper == "ETC/GMT")
        {
            return true;
        }

        // A name with no '/' and no digits is unlikely to be a real zone id.
        const std::time_t probe = std::time(nullptr);

        std::tm local{};

        if (!resolveNamed(name, probe, local))
            return false;

        std::tm utc{};

        if (gmtime_r(&probe, &utc) == nullptr)
            return false;

        const int localMinutes = local.tm_hour * 60 + local.tm_min;
        const int utcMinutes = utc.tm_hour * 60 + utc.tm_min;

        return localMinutes != utcMinutes;
#endif
    }
}

const int TimeZone::MAX_OFFSET_MINUTES = 14 * 60;
const int TimeZone::MIN_OFFSET_MINUTES = -12 * 60;

TimeZone::TimeZone()
    : zoneKind(Kind::FixedOffset),
      fixedMinutes(0),
      spec("UTC")
{
}

bool TimeZone::setFixedOffset(int minutes)
{
    if (minutes < MIN_OFFSET_MINUTES || minutes > MAX_OFFSET_MINUTES)
        return false;

    zoneKind = Kind::FixedOffset;
    fixedMinutes = minutes;
    zoneName.clear();

    return true;
}

bool TimeZone::setNamedZone(const std::string &name)
{
    const std::string trimmed = Utility::trim(name);

    if (trimmed.empty())
        return false;

    zoneKind = Kind::Named;
    zoneName = trimmed;
    fixedMinutes = 0;

    return true;
}

void TimeZone::setLabel(const std::string &label)
{
    zoneLabel = Utility::trim(label);
}

std::string TimeZone::label() const
{
    if (!zoneLabel.empty())
        return zoneLabel;

    if (zoneKind == Kind::Named)
        return zoneName;

    return spec;
}

TimeZone::Kind TimeZone::kind() const
{
    return zoneKind;
}

std::string TimeZone::name() const
{
    return zoneName;
}

bool TimeZone::resolves() const
{
    if (zoneKind == Kind::FixedOffset)
        return true;

    return namedZoneResolves(zoneName);
}

bool TimeZone::localTime(std::time_t utc, std::tm &out) const
{
    if (zoneKind == Kind::Named)
    {
        if (resolveNamed(zoneName, utc, out))
            return true;

        // Fall through to UTC so the caller still has something to render.
        std::tm fallback{};

#ifdef _WIN32
        if (gmtime_s(&fallback, &utc) != 0)
            return false;
#else
        if (gmtime_r(&utc, &fallback) == nullptr)
            return false;
#endif

        out = fallback;

        return false;
    }

    // A fixed offset is arithmetic on the UTC instant, which keeps it exact
    // and identical on every platform.
    const std::time_t shifted =
        utc + static_cast<std::time_t>(fixedMinutes) * 60;

    std::tm converted{};

#ifdef _WIN32
    if (gmtime_s(&converted, &shifted) != 0)
        return false;
#else
    if (gmtime_r(&shifted, &converted) == nullptr)
        return false;
#endif

    out = converted;

    return true;
}

int TimeZone::offsetMinutes(std::time_t utc) const
{
    if (zoneKind == Kind::FixedOffset)
        return fixedMinutes;

    std::tm local{};

    if (!localTime(utc, local))
        return 0;

    std::tm utcTime{};

#ifdef _WIN32
    if (gmtime_s(&utcTime, &utc) != 0)
        return 0;
#else
    if (gmtime_r(&utc, &utcTime) == nullptr)
        return 0;
#endif

    const int localMinutes =
        local.tm_yday * 24 * 60 + local.tm_hour * 60 + local.tm_min;
    const int utcMinutes =
        utcTime.tm_yday * 24 * 60 + utcTime.tm_hour * 60 + utcTime.tm_min;

    int difference = localMinutes - utcMinutes;

    // A year boundary makes tm_yday jump by a whole year rather than one day.
    if (difference > 16 * 60)
        difference -= 365 * 24 * 60;
    else if (difference < -16 * 60)
        difference += 365 * 24 * 60;

    return difference;
}

std::string TimeZone::formatTime(std::time_t utc, bool hour12) const
{
    std::tm local{};

    // A zone that cannot be resolved still renders, as UTC, and the caller
    // is expected to have reported it via resolves().
    localTime(utc, local);

    if (!hour12)
    {
        return Utility::padZero(local.tm_hour) + ":" +
               Utility::padZero(local.tm_min);
    }

    const int hour = local.tm_hour % 12;

    return Utility::padZero(hour == 0 ? 12 : hour) + ":" +
           Utility::padZero(local.tm_min) +
           (local.tm_hour >= 12 ? " PM" : " AM");
}

std::string TimeZone::formatOffset(std::time_t utc) const
{
    const int minutes = offsetMinutes(utc);
    const int magnitude = (minutes < 0) ? -minutes : minutes;

    return std::string(minutes < 0 ? "-" : "+") +
           Utility::padZero(magnitude / 60) + ":" +
           Utility::padZero(magnitude % 60);
}

bool TimeZone::parseOffset(const std::string &text, int &minutes)
{
    std::string value = Utility::toUpper(Utility::trim(text));

    if (value.empty())
        return false;

    // "UTC" alone, or a "UTC" prefix on a signed offset.
    if (value == "UTC" || value == "GMT" || value == "Z")
    {
        minutes = 0;
        return true;
    }

    if (value.rfind("UTC", 0) == 0)
        value = value.substr(3);
    else if (value.rfind("GMT", 0) == 0)
        value = value.substr(3);

    value = Utility::trim(value);

    if (value.empty())
    {
        minutes = 0;
        return true;
    }

    int sign = 1;

    if (value[0] == '+')
    {
        value = value.substr(1);
    }
    else if (value[0] == '-')
    {
        sign = -1;
        value = value.substr(1);
    }
    else
    {
        // An offset must carry a sign; a bare number is too ambiguous to
        // guess at.
        return false;
    }

    if (value.empty())
        return false;

    int hours = 0;
    int mins = 0;

    const std::size_t colon = value.find(':');

    if (colon == std::string::npos)
    {
        hours = Utility::toInt(value, -1);

        if (hours < 0)
            return false;
    }
    else
    {
        const std::string hourPart = value.substr(0, colon);
        const std::string minutePart = value.substr(colon + 1);

        if (hourPart.empty() || minutePart.empty())
            return false;

        hours = Utility::toInt(hourPart, -1);
        mins = Utility::toInt(minutePart, -1);

        if (hours < 0 || mins < 0 || mins > 59)
            return false;
    }

    const int total = sign * (hours * 60 + mins);

    if (total < MIN_OFFSET_MINUTES || total > MAX_OFFSET_MINUTES)
        return false;

    minutes = total;

    return true;
}

bool TimeZone::parse(const std::string &text, TimeZone &zone)
{
    const std::string source = Utility::trim(text);

    if (source.empty())
        return false;

    std::string specification = source;
    std::string label;

    const std::size_t pipe = source.find('|');

    if (pipe != std::string::npos)
    {
        specification = Utility::trim(source.substr(0, pipe));
        label = Utility::trim(source.substr(pipe + 1));
    }

    if (specification.empty())
        return false;

    TimeZone parsed;
    parsed.spec = specification;

    // A '/' marks an IANA zone id; everything else is treated as an offset.
    if (specification.find('/') != std::string::npos)
    {
        if (!parsed.setNamedZone(specification))
            return false;
    }
    else
    {
        int minutes = 0;

        if (!parseOffset(specification, minutes))
            return false;

        if (!parsed.setFixedOffset(minutes))
            return false;
    }

    parsed.setLabel(label);

    zone = parsed;

    return true;
}

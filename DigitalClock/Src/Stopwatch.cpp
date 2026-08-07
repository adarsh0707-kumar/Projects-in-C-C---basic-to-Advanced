#include "Stopwatch.hpp"

#include "Utility.hpp"

namespace
{
    const std::int64_t MS_PER_SECOND = 1000;
    const std::int64_t MS_PER_MINUTE = 60 * MS_PER_SECOND;
    const std::int64_t MS_PER_HOUR = 60 * MS_PER_MINUTE;
}

const std::size_t Stopwatch::MAX_LAPS = 99;

Stopwatch::Stopwatch()
    : accumulated(0),
      startedAt(0),
      running(false)
{
}

bool Stopwatch::start(std::int64_t nowMs)
{
    if (running)
        return false;

    startedAt = nowMs;
    running = true;

    return true;
}

bool Stopwatch::stop(std::int64_t nowMs)
{
    if (!running)
        return false;

    accumulated += nowMs - startedAt;
    running = false;

    return true;
}

bool Stopwatch::toggle(std::int64_t nowMs)
{
    if (running)
        stop(nowMs);
    else
        start(nowMs);

    return running;
}

void Stopwatch::reset()
{
    accumulated = 0;
    startedAt = 0;
    running = false;

    lapTimes.clear();
}

bool Stopwatch::lap(std::int64_t nowMs)
{
    // A lap on a stopped stopwatch would record the same value repeatedly.
    if (!running)
        return false;

    if (lapTimes.size() >= MAX_LAPS)
        return false;

    lapTimes.push_back(elapsed(nowMs));

    return true;
}

bool Stopwatch::isRunning() const
{
    return running;
}

std::int64_t Stopwatch::elapsed(std::int64_t nowMs) const
{
    if (!running)
        return accumulated;

    const std::int64_t total = accumulated + (nowMs - startedAt);

    // A caller passing a reading earlier than the start would otherwise
    // produce a negative elapsed time.
    return (total < 0) ? 0 : total;
}

const std::vector<std::int64_t> &Stopwatch::laps() const
{
    return lapTimes;
}

std::size_t Stopwatch::lapCount() const
{
    return lapTimes.size();
}

std::int64_t Stopwatch::lapSplit(std::size_t index) const
{
    if (index >= lapTimes.size())
        return 0;

    if (index == 0)
        return lapTimes[0];

    return lapTimes[index] - lapTimes[index - 1];
}

std::string Stopwatch::formatted(std::int64_t nowMs) const
{
    return format(elapsed(nowMs));
}

std::string Stopwatch::format(std::int64_t milliseconds)
{
    std::int64_t value = (milliseconds < 0) ? 0 : milliseconds;

    const std::int64_t hours = value / MS_PER_HOUR;
    value -= hours * MS_PER_HOUR;

    const std::int64_t minutes = value / MS_PER_MINUTE;
    value -= minutes * MS_PER_MINUTE;

    const std::int64_t seconds = value / MS_PER_SECOND;
    value -= seconds * MS_PER_SECOND;

    // Hundredths rather than milliseconds: a millisecond digit changes far
    // too fast to read, and the display refreshes far more slowly than that.
    const std::int64_t hundredths = value / 10;

    std::string text;

    if (hours > 0)
        text += std::to_string(hours) + ":";

    text += Utility::padZero(static_cast<int>(minutes)) + ":" +
            Utility::padZero(static_cast<int>(seconds)) + "." +
            Utility::padZero(static_cast<int>(hundredths));

    return text;
}

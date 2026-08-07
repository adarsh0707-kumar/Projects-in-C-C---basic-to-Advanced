#include "CountdownTimer.hpp"

#include <vector>

#include "Utility.hpp"

namespace
{
    const std::int64_t MS_PER_SECOND = 1000;
    const std::int64_t MS_PER_MINUTE = 60 * MS_PER_SECOND;
    const std::int64_t MS_PER_HOUR = 60 * MS_PER_MINUTE;
}

const std::int64_t CountdownTimer::DEFAULT_DURATION_MS = 5 * MS_PER_MINUTE;
const std::int64_t CountdownTimer::MAX_DURATION_MS = 24 * MS_PER_HOUR;

CountdownTimer::CountdownTimer()
    : total(DEFAULT_DURATION_MS),
      remainder(DEFAULT_DURATION_MS),
      startedAt(0),
      running(false),
      finished(false),
      fired(false)
{
}

bool CountdownTimer::setDuration(std::int64_t milliseconds)
{
    if (milliseconds <= 0 || milliseconds > MAX_DURATION_MS)
        return false;

    total = milliseconds;

    // Changing the duration mid-run would leave the remainder measured
    // against a total it no longer belongs to.
    reset();

    return true;
}

std::int64_t CountdownTimer::duration() const
{
    return total;
}

bool CountdownTimer::start(std::int64_t nowMs)
{
    if (running)
        return false;

    // Nothing to count down from.
    if (remainder <= 0)
        return false;

    startedAt = nowMs;
    running = true;

    return true;
}

bool CountdownTimer::pause(std::int64_t nowMs)
{
    if (!running)
        return false;

    remainder = remaining(nowMs);
    running = false;

    return true;
}

bool CountdownTimer::toggle(std::int64_t nowMs)
{
    if (running)
        pause(nowMs);
    else
        start(nowMs);

    return running;
}

void CountdownTimer::reset()
{
    remainder = total;
    startedAt = 0;
    running = false;
    finished = false;
    fired = false;
}

bool CountdownTimer::isRunning() const
{
    return running;
}

std::int64_t CountdownTimer::remaining(std::int64_t nowMs) const
{
    if (!running)
        return remainder;

    const std::int64_t left = remainder - (nowMs - startedAt);

    return (left < 0) ? 0 : left;
}

bool CountdownTimer::hasExpired(std::int64_t nowMs) const
{
    return remaining(nowMs) <= 0;
}

bool CountdownTimer::poll(std::int64_t nowMs)
{
    if (!running)
        return false;

    if (!hasExpired(nowMs))
        return false;

    // Bank the expiry and stop, so the countdown does not continue past zero.
    remainder = 0;
    running = false;
    finished = true;

    if (fired)
        return false;

    fired = true;

    return true;
}

bool CountdownTimer::isFinished() const
{
    return finished;
}

void CountdownTimer::acknowledge()
{
    finished = false;
}

std::string CountdownTimer::formatted(std::int64_t nowMs) const
{
    return format(remaining(nowMs));
}

std::string CountdownTimer::format(std::int64_t milliseconds)
{
    std::int64_t value = (milliseconds < 0) ? 0 : milliseconds;

    /*
    Round up to the next whole second. A countdown showing 00:00 while a
    fraction of a second remains reads as finished when it is not; counting
    5, 4, 3, 2, 1 and then firing is what a user expects.
    */
    value = (value + MS_PER_SECOND - 1) / MS_PER_SECOND * MS_PER_SECOND;

    const std::int64_t hours = value / MS_PER_HOUR;
    value -= hours * MS_PER_HOUR;

    const std::int64_t minutes = value / MS_PER_MINUTE;
    value -= minutes * MS_PER_MINUTE;

    const std::int64_t seconds = value / MS_PER_SECOND;

    std::string text;

    if (hours > 0)
        text += std::to_string(hours) + ":";

    text += Utility::padZero(static_cast<int>(minutes)) + ":" +
            Utility::padZero(static_cast<int>(seconds));

    return text;
}

bool CountdownTimer::parseDuration(const std::string &text,
                                   std::int64_t &milliseconds)
{
    const std::string value = Utility::trim(text);

    if (value.empty())
        return false;

    // Split on ':' into up to three fields.
    std::vector<std::string> fields;
    std::string field;

    for (const char character : value)
    {
        if (character == ':')
        {
            fields.push_back(field);
            field.clear();
            continue;
        }

        field += character;
    }

    fields.push_back(field);

    if (fields.size() > 3)
        return false;

    std::vector<std::int64_t> parts;

    for (const std::string &entry : fields)
    {
        const std::string trimmed = Utility::trim(entry);

        if (trimmed.empty())
            return false;

        const int parsed = Utility::toInt(trimmed, -1);

        if (parsed < 0)
            return false;

        parts.push_back(parsed);
    }

    std::int64_t result = 0;

    if (parts.size() == 1)
    {
        // A bare number is a count of seconds.
        result = parts[0] * MS_PER_SECOND;
    }
    else if (parts.size() == 2)
    {
        if (parts[1] > 59)
            return false;

        result = parts[0] * MS_PER_MINUTE + parts[1] * MS_PER_SECOND;
    }
    else
    {
        if (parts[1] > 59 || parts[2] > 59)
            return false;

        result = parts[0] * MS_PER_HOUR +
                 parts[1] * MS_PER_MINUTE +
                 parts[2] * MS_PER_SECOND;
    }

    if (result <= 0 || result > MAX_DURATION_MS)
        return false;

    milliseconds = result;

    return true;
}

#include "Clock.hpp"

#include <chrono>

#include "Utility.hpp"

Clock::Clock()
    : hour(0),
      minute(0),
      second(0),
      valid(false)
{
    update();
}

bool Clock::update()
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t raw = std::chrono::system_clock::to_time_t(now);

    std::tm local{};

#ifdef _WIN32
    const bool converted = (localtime_s(&local, &raw) == 0);
#else
    const bool converted = (localtime_r(&raw, &local) != nullptr);
#endif

    if (!converted)
    {
        // Keep the previous snapshot rather than showing a bogus time.
        valid = false;
        return false;
    }

    hour = local.tm_hour;
    minute = local.tm_min;
    second = local.tm_sec;
    valid = true;

    return true;
}

std::string Clock::getCurrentTime() const
{
    return getTime24();
}

int Clock::getHour() const
{
    return hour;
}

int Clock::getHour12() const
{
    const int converted = hour % 12;

    return (converted == 0) ? 12 : converted;
}

int Clock::getMinute() const
{
    return minute;
}

int Clock::getSecond() const
{
    return second;
}

bool Clock::isPM() const
{
    return hour >= 12;
}

std::string Clock::getMeridiem() const
{
    return isPM() ? "PM" : "AM";
}

std::string Clock::getTime24() const
{
    return Utility::padZero(hour) + ":" +
           Utility::padZero(minute) + ":" +
           Utility::padZero(second);
}

std::string Clock::getTime12() const
{
    return Utility::padZero(getHour12()) + ":" +
           Utility::padZero(minute) + ":" +
           Utility::padZero(second) + " " +
           getMeridiem();
}

bool Clock::setTime(int newHour, int newMinute, int newSecond)
{
    if (newHour < 0 || newHour > 23)
        return false;

    if (newMinute < 0 || newMinute > 59)
        return false;

    // 60 is permitted so that leap seconds reported by the OS are accepted.
    if (newSecond < 0 || newSecond > 60)
        return false;

    hour = newHour;
    minute = newMinute;
    second = newSecond;
    valid = true;

    return true;
}

bool Clock::isValid() const
{
    return valid;
}

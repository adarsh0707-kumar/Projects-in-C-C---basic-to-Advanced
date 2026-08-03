#include "Clock.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

Clock::Clock()
{
    update();
}

void Clock::update()
{
    auto now = std::chrono::system_clock::now();
    std::time_t current = std::chrono::system_clock::to_time_t(now);

    std::tm localTime = *std::localtime(&current);

    hour = localTime.tm_hour;
    minute = localTime.tm_min;
    second = localTime.tm_sec;
}

int Clock::getHour24() const
{
    return hour;
}

int Clock::getHour12() const
{
    int h = hour % 12;
    return (h == 0) ? 12 : h;
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

std::string Clock::getTime24() const
{
    std::ostringstream out;

    out << std::setw(2) << std::setfill('0') << hour << ":"
        << std::setw(2) << minute << ":"
        << std::setw(2) << second;

    return out.str();
}

std::string Clock::getTime12() const
{
    std::ostringstream out;

    out << std::setw(2) << std::setfill('0') << getHour12() << ":"
        << std::setw(2) << minute << ":"
        << std::setw(2) << second
        << (isPM() ? " PM" : " AM");

    return out.str();
}
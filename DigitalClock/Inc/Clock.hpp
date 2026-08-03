#pragma once

#include <ctime>
#include <chrono>

class Clock
{
private:
    int hour;
    int minute;
    int second;

public:
    Clock() : hour(0), minute(0), second(0) {}

    bool update()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t tt = std::chrono::system_clock::to_time_t(now);
        std::tm *tm = std::localtime(&tt);
        if (tm)
        {
            hour = tm->tm_hour;
            minute = tm->tm_min;
            second = tm->tm_sec;
            return true;
        }
        return false;
    }

    int getHour() const { return hour; }
    int getMinute() const { return minute; }
    int getSecond() const { return second; }
};
#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <string>

class Clock
{
public:
    Clock();

    void update();

    int getHour24() const;
    int getHour12() const;
    int getMinute() const;
    int getSecond() const;

    bool isPM() const;

    std::string getTime24() const;
    std::string getTime12() const;

private:
    int hour;
    int minute;
    int second;
};

#endif // CLOCK_HPP
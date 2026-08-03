#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "Clock.hpp"
#include "Date.hpp"

class Display
{
public:
    Display() = default;

    void clearScreen() const;

    void printHeader() const;

    void printClock(const Clock &clock) const;

    void printDate(const Date &date) const;

    void printFooter() const;

    void render(const Clock &clock,
                const Date &date) const;
};

#endif // DISPLAY_HPP
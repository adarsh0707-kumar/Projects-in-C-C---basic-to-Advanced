#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "Clock.hpp"
#include "Date.hpp"
#include "Theme.hpp"
#include "Banner.hpp"
#include "Screen.hpp"

class Display
{
public:
    Display() = default;

    void clearScreen() const;
    void printHeader() const;
    void printClock(const Clock &) const;
    void printDate(const Date &) const;
    void printFooter() const;

    void render(const Clock &, const Date &) const;

private:
    Theme theme;
    Banner banner;
    Screen screen;
};

#endif
#ifndef DISPLAY_HPP
#define DISPLAY_HPP

/*
------------------------------------------------------------
Display Module
------------------------------------------------------------

Purpose
-------
Responsible for rendering everything shown on the console.

Responsibilities
----------------
• Clear screen
• Print application header
• Print current date
• Print current time
• Print footer
• Coordinate Banner, Theme and Screen modules

Dependencies
------------
Banner
Theme
Screen
Clock
Date

Author
------
Adarsh Kumar
------------------------------------------------------------
*/

#include "Banner.hpp"
#include "Clock.hpp"
#include "Date.hpp"
#include "Screen.hpp"
#include "Theme.hpp"

class Display
{
public:
    /*
    --------------------------------------------------------
    Constructor
    --------------------------------------------------------
    */
    Display() = default;

    /*
    --------------------------------------------------------
    Clear console screen.
    --------------------------------------------------------
    */
    void clearScreen() const;

    /*
    --------------------------------------------------------
    Display application header.
    --------------------------------------------------------
    */
    void printHeader() const;

    /*
    --------------------------------------------------------
    Display current date.
    --------------------------------------------------------
    */
    void printDate(const Date &date) const;

    /*
    --------------------------------------------------------
    Display current time.
    --------------------------------------------------------
    */
    void printClock(const Clock &clock) const;

    /*
    --------------------------------------------------------
    Display footer.
    --------------------------------------------------------
    */
    void printFooter() const;

    /*
    --------------------------------------------------------
    Render complete UI.
    --------------------------------------------------------
    */
    void render(const Clock &clock,
                const Date &date) const;

private:
    Banner banner;
    Theme theme;
    Screen screen;
};

#endif
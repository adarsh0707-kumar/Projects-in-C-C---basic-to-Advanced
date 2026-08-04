#include "Display.hpp"

#include <iostream>

#include "Version.hpp"

/*
------------------------------------------------------------
Clear screen.

Delegates responsibility to Screen module.
------------------------------------------------------------
*/
void Display::clearScreen() const
{
    screen.clear();
    screen.home();
    screen.hideCursor();
}

/*
------------------------------------------------------------
Display header.

Header consists of:

• Banner
• Version
• Author

Theme colors are applied here.
------------------------------------------------------------
*/
void Display::printHeader() const
{
    std::cout
        << theme.style(Theme::Style::Bold)
        << theme.foreground(Theme::Color::Cyan);

    std::cout
        << banner.loadBanner();

    std::cout
        << theme.reset();

    std::cout
        << "\nVersion : "
        << Version::VERSION
        << '\n';

    std::cout
        << "Author  : "
        << Version::AUTHOR
        << "\n\n";
}

/*
------------------------------------------------------------
Display today's date.
------------------------------------------------------------
*/
void Display::printDate(const Date &date) const
{
    std::cout
        << theme.foreground(Theme::Color::Yellow);

    std::cout
        << "Date : "
        << date.getDateLong()
        << '\n';

    std::cout
        << theme.reset();
}

/*
------------------------------------------------------------
Display current time.

Shows both 24-hour
and 12-hour formats.
------------------------------------------------------------
*/
void Display::printClock(const Clock &clock) const
{
    std::cout
        << theme.foreground(Theme::Color::Green);

    std::cout
        << "Time : "
        << clock.getTime24()
        << '\n';

    std::cout
        << "12H  : "
        << clock.getTime12()
        << '\n';

    std::cout
        << theme.reset();
}

/*
------------------------------------------------------------
Display footer.
------------------------------------------------------------
*/
void Display::printFooter() const
{
    std::cout
        << "\n------------------------------------------------------------\n";

    std::cout
        << "Press Ctrl+C to Exit\n";
}

/*
------------------------------------------------------------
Render complete application UI.

Rendering order

1. Clear screen
2. Header
3. Date
4. Clock
5. Footer
------------------------------------------------------------
*/
void Display::render(const Clock &clock,
                     const Date &date) const
{
    clearScreen();

    printHeader();

    printDate(date);

    std::cout << '\n';

    printClock(clock);

    printFooter();
}
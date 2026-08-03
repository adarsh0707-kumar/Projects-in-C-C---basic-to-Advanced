#include "Display.hpp"

#include <cstdlib>
#include <iostream>

#include "Version.hpp"

void Display::clearScreen() const
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void Display::printHeader() const
{
    std::cout
        << "+==========================================+\n"
        << "|          DIGITAL CLOCK SYSTEM            |\n"
        << "+==========================================+\n\n";

    std::cout
        << "Version : "
        << Version::VERSION
        << "\n";

    std::cout
        << "Author  : "
        << Version::AUTHOR
        << "\n\n";
}

void Display::printDate(const Date &date) const
{
    std::cout
        << "Date : "
        << date.getDateLong()
        << "\n";
}

void Display::printClock(const Clock &clock) const
{
    std::cout
        << "Time : "
        << clock.getTime24()
        << "\n";

    std::cout
        << "12H  : "
        << clock.getTime12()
        << "\n";
}

void Display::printFooter() const
{
    std::cout
        << "\n--------------------------------------------\n";

    std::cout
        << "Press Ctrl+C to Exit\n";
}

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
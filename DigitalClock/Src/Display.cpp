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
        << theme.style(Theme::Style::Bold)
        << theme.foreground(Theme::Color::Cyan);

    std::cout
        << banner.loadBanner("Resources/banner.txt");

    std::cout
        << theme.reset();

    std::cout << "\nVersion : "
              << Version::VERSION;

    std::cout << "\nAuthor  : "
              << Version::AUTHOR
              << "\n\n";
}

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
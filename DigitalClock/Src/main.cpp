#include <chrono>
#include <iostream>
#include <thread>

#include "Clock.hpp"
#include "Date.hpp"
#include "Version.hpp"

int main()
{
    Clock clock;
    Date date;

    while (true)
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        clock.update();
        date.update();

        std::cout << "========================================\n";
        std::cout << "        " << Version::NAME << "\n";
        std::cout << "========================================\n\n";

        std::cout << "Version : " << Version::VERSION << '\n';
        std::cout << "Author  : " << Version::AUTHOR << "\n\n";

        std::cout << "Date : " << date.getDateLong() << '\n';
        std::cout << "Time : " << clock.getTime24() << '\n';
        std::cout << "12H  : " << clock.getTime12() << '\n';

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
#include <iostream>

#include "Clock.hpp"
#include "Version.hpp"

int main()
{
    Clock clock;

    std::cout << "=====================================\n";
    std::cout << "      " << Version::NAME << '\n';
    std::cout << "=====================================\n\n";

    std::cout << "Version : " << Version::VERSION << '\n';
    std::cout << "Author  : " << Version::AUTHOR << "\n\n";

    std::cout << "24-Hour Time : " << clock.getTime24() << '\n';
    std::cout << "12-Hour Time : " << clock.getTime12() << '\n';

    return 0;
}
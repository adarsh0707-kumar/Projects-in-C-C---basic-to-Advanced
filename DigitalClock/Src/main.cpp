#include <iostream>
#include "Version.hpp"

int main()
{
    std::cout << "=====================================\n";
    std::cout << "      " << Version::NAME << '\n';
    std::cout << "=====================================\n\n";

    std::cout << "Version : " << Version::VERSION << '\n';
    std::cout << "Author  : " << Version::AUTHOR << '\n';

    std::cout << "\nSprint 2.1 completed successfully.\n";

    return 0;
}
#include "Screen.hpp"

#include <iostream>

#ifdef _WIN32
#include <cstdlib>
#endif

void Screen::clear() const
{
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[H";
    std::cout.flush();
#endif
}

void Screen::moveCursorHome() const
{
    std::cout << "\033[H";
}
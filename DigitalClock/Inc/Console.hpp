#pragma once

#include <string>
#include <iostream>
#include <cstdlib>
#include "Utility.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

class Console
{
public:
    void initialize()
    {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
#endif
        clear();
        hideCursor();
    }

    void clear()
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void hideCursor()
    {
        std::cout << "\033[?25l";
    }

    void showCursor()
    {
        std::cout << "\033[?25h";
    }

    void print(const std::string &text)
    {
        std::cout << text;
    }

    void printCentered(const std::string &text, int width = 80)
    {
        int pad = (width - static_cast<int>(text.length())) / 2;
        if (pad > 0)
            std::cout << std::string(pad, ' ');
        std::cout << text << std::endl;
    }

    void setColor(const std::string &color)
    {
        // Basic ANSI color mapping
        if (color == "Default" || color == "White")
            std::cout << "\033[0m"; // Reset or White
        else if (color == "Black")
            std::cout << "\033[30m";
        else if (color == "Red")
            std::cout << "\033[31m";
        else if (color == "Green")
            std::cout << "\033[32m";
        else if (color == "Yellow")
            std::cout << "\033[33m";
        else if (color == "Blue")
            std::cout << "\033[34m";
        else if (color == "Cyan")
            std::cout << "\033[36m";
        else if (color == "DarkGray")
            std::cout << "\033[90m";
        else if (color == "DarkBlue")
            std::cout << "\033[94m"; // Bright blue
    }
};
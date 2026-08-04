#include "Screen.hpp"

#include <iostream>

/*
------------------------------------------------------------
Clear console screen.

ANSI Escape Sequence

2J  -> Clear entire screen
H   -> Move cursor to home
------------------------------------------------------------
*/
void Screen::clear() const
{
    std::cout << "\033[2J\033[H";
}

/*
------------------------------------------------------------
Move cursor to home position.
------------------------------------------------------------
*/
void Screen::home() const
{
    std::cout << "\033[H";
}

/*
------------------------------------------------------------
Hide console cursor.

ANSI:
?25l
------------------------------------------------------------
*/
void Screen::hideCursor() const
{
    std::cout << "\033[?25l";
}

/*
------------------------------------------------------------
Show console cursor.

ANSI:
?25h
------------------------------------------------------------
*/
void Screen::showCursor() const
{
    std::cout << "\033[?25h";
}

/*
------------------------------------------------------------
Immediately flush console output.
------------------------------------------------------------
*/
void Screen::flush() const
{
    std::cout.flush();
}
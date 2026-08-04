#ifndef SCREEN_HPP
#define SCREEN_HPP

/*
------------------------------------------------------------
Screen Manager
------------------------------------------------------------

Purpose
-------
Provides low-level console control.

Responsibilities
----------------
• Clear console
• Move cursor
• Hide cursor
• Show cursor
• Flush console output

This class hides all ANSI escape sequences from the rest
of the application.

Author
------
Adarsh Kumar
------------------------------------------------------------
*/

class Screen
{
public:
    /*
    --------------------------------------------------------
    Constructor
    --------------------------------------------------------
    */
    Screen() = default;

    /*
    --------------------------------------------------------
    Clear the entire console screen.
    --------------------------------------------------------
    */
    void clear() const;

    /*
    --------------------------------------------------------
    Move cursor to the top-left corner.
    --------------------------------------------------------
    */
    void home() const;

    /*
    --------------------------------------------------------
    Hide the console cursor.
    Useful while refreshing the screen continuously.
    --------------------------------------------------------
    */
    void hideCursor() const;

    /*
    --------------------------------------------------------
    Show the console cursor.
    Call this before application exits.
    --------------------------------------------------------
    */
    void showCursor() const;

    /*
    --------------------------------------------------------
    Flush pending console output.
    --------------------------------------------------------
    */
    void flush() const;
};

#endif
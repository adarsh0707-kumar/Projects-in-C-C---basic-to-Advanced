#ifndef THEME_HPP
#define THEME_HPP

/*
------------------------------------------------------------
Theme Module
------------------------------------------------------------

Purpose
-------
Provides ANSI escape sequences for console formatting.

Responsibilities
----------------
• Foreground colors
• Background colors
• Text styles
• Reset formatting

Supported Styles
----------------
✓ Normal
✓ Bold
✓ Underline

Supported Colors
----------------
✓ Black
✓ Red
✓ Green
✓ Yellow
✓ Blue
✓ Magenta
✓ Cyan
✓ White
✓ Default

Author
------
Adarsh Kumar
------------------------------------------------------------
*/

#include <string>

class Theme
{
public:
    /*
    --------------------------------------------------------
    Available foreground/background colors.
    --------------------------------------------------------
    */
    enum class Color
    {
        Default,
        Black,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White
    };

    /*
    --------------------------------------------------------
    Available text styles.
    --------------------------------------------------------
    */
    enum class Style
    {
        Normal,
        Bold,
        Underline
    };

    Theme() = default;

    /*
    --------------------------------------------------------
    Return ANSI foreground color sequence.
    --------------------------------------------------------
    */
    std::string foreground(Color color) const;

    /*
    --------------------------------------------------------
    Return ANSI background color sequence.
    --------------------------------------------------------
    */
    std::string background(Color color) const;

    /*
    --------------------------------------------------------
    Return ANSI text style.
    --------------------------------------------------------
    */
    std::string style(Style style) const;

    /*
    --------------------------------------------------------
    Reset all console formatting.
    --------------------------------------------------------
    */
    std::string reset() const;
};

#endif
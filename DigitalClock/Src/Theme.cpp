#include "Theme.hpp"

/*
------------------------------------------------------------
Return ANSI foreground color.
------------------------------------------------------------
*/
std::string Theme::foreground(Color color) const
{
    switch (color)
    {
    case Color::Black:
        return "\033[30m";

    case Color::Red:
        return "\033[31m";

    case Color::Green:
        return "\033[32m";

    case Color::Yellow:
        return "\033[33m";

    case Color::Blue:
        return "\033[34m";

    case Color::Magenta:
        return "\033[35m";

    case Color::Cyan:
        return "\033[36m";

    case Color::White:
        return "\033[37m";

    default:
        return "\033[39m";
    }
}

/*
------------------------------------------------------------
Return ANSI background color.
------------------------------------------------------------
*/
std::string Theme::background(Color color) const
{
    switch (color)
    {
    case Color::Black:
        return "\033[40m";

    case Color::Red:
        return "\033[41m";

    case Color::Green:
        return "\033[42m";

    case Color::Yellow:
        return "\033[43m";

    case Color::Blue:
        return "\033[44m";

    case Color::Magenta:
        return "\033[45m";

    case Color::Cyan:
        return "\033[46m";

    case Color::White:
        return "\033[47m";

    default:
        return "\033[49m";
    }
}

/*
------------------------------------------------------------
Return ANSI text style.
------------------------------------------------------------
*/
std::string Theme::style(Style style) const
{
    switch (style)
    {
    case Style::Bold:
        return "\033[1m";

    case Style::Underline:
        return "\033[4m";

    default:
        return "\033[0m";
    }
}

/*
------------------------------------------------------------
Reset console formatting.
------------------------------------------------------------
*/
std::string Theme::reset() const
{
    return "\033[0m";
}
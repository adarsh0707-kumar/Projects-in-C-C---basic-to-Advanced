#include "Theme.hpp"

#include "Utility.hpp"

namespace
{
    /**
     * @brief Looks up an already upper-cased, trimmed colour name.
     *
     * One table backs both colorFromName() and parseColor(), so a name they
     * disagree about cannot exist.
     *
     * @param value Upper-cased, trimmed name.
     * @param color Receives the colour, untouched when not recognised.
     * @return true if @p value names a colour.
     */
    bool lookupColor(const std::string &value, Theme::Color &color)
    {
        using Color = Theme::Color;

        if (value.empty())
            return false;

        if (value == "DEFAULT")       { color = Color::Default;       return true; }
        if (value == "BLACK")         { color = Color::Black;         return true; }
        if (value == "RED")           { color = Color::Red;           return true; }
        if (value == "GREEN")         { color = Color::Green;         return true; }
        if (value == "YELLOW")        { color = Color::Yellow;        return true; }
        if (value == "BLUE")          { color = Color::Blue;          return true; }
        if (value == "MAGENTA")       { color = Color::Magenta;       return true; }
        if (value == "CYAN")          { color = Color::Cyan;          return true; }
        if (value == "WHITE")         { color = Color::White;         return true; }

        if (value == "BRIGHTBLACK")   { color = Color::BrightBlack;   return true; }
        if (value == "BRIGHTRED")     { color = Color::BrightRed;     return true; }
        if (value == "BRIGHTGREEN")   { color = Color::BrightGreen;   return true; }
        if (value == "BRIGHTYELLOW")  { color = Color::BrightYellow;  return true; }
        if (value == "BRIGHTBLUE")    { color = Color::BrightBlue;    return true; }
        if (value == "BRIGHTMAGENTA") { color = Color::BrightMagenta; return true; }
        if (value == "BRIGHTCYAN")    { color = Color::BrightCyan;    return true; }
        if (value == "BRIGHTWHITE")   { color = Color::BrightWhite;   return true; }

        /*
        Spellings used by the bundled theme files and by Config/themes.ini.
        The "dark" variants map onto the standard (non-bright) colours and the
        grey variants onto bright black, which is how terminals render them.
        */
        if (value == "DARKBLUE")      { color = Color::Blue;          return true; }
        if (value == "DARKRED")       { color = Color::Red;           return true; }
        if (value == "DARKGREEN")     { color = Color::Green;         return true; }

        if (value == "DARKGRAY" || value == "DARKGREY" ||
            value == "GRAY" || value == "GREY")
        {
            color = Color::BrightBlack;
            return true;
        }

        return false;
    }
}

namespace Theme
{
    std::string foreground(Color color)
    {
        switch (color)
        {
        case Color::Black:         return "\033[30m";
        case Color::Red:           return "\033[31m";
        case Color::Green:         return "\033[32m";
        case Color::Yellow:        return "\033[33m";
        case Color::Blue:          return "\033[34m";
        case Color::Magenta:       return "\033[35m";
        case Color::Cyan:          return "\033[36m";
        case Color::White:         return "\033[37m";
        case Color::BrightBlack:   return "\033[90m";
        case Color::BrightRed:     return "\033[91m";
        case Color::BrightGreen:   return "\033[92m";
        case Color::BrightYellow:  return "\033[93m";
        case Color::BrightBlue:    return "\033[94m";
        case Color::BrightMagenta: return "\033[95m";
        case Color::BrightCyan:    return "\033[96m";
        case Color::BrightWhite:   return "\033[97m";
        case Color::Default:       break;
        }

        return "\033[39m";
    }

    std::string background(Color color)
    {
        switch (color)
        {
        case Color::Black:         return "\033[40m";
        case Color::Red:           return "\033[41m";
        case Color::Green:         return "\033[42m";
        case Color::Yellow:        return "\033[43m";
        case Color::Blue:          return "\033[44m";
        case Color::Magenta:       return "\033[45m";
        case Color::Cyan:          return "\033[46m";
        case Color::White:         return "\033[47m";
        case Color::BrightBlack:   return "\033[100m";
        case Color::BrightRed:     return "\033[101m";
        case Color::BrightGreen:   return "\033[102m";
        case Color::BrightYellow:  return "\033[103m";
        case Color::BrightBlue:    return "\033[104m";
        case Color::BrightMagenta: return "\033[105m";
        case Color::BrightCyan:    return "\033[106m";
        case Color::BrightWhite:   return "\033[107m";
        case Color::Default:       break;
        }

        return "\033[49m";
    }

    std::string style(Style style)
    {
        switch (style)
        {
        case Style::Bold:      return "\033[1m";
        case Style::Dim:       return "\033[2m";
        case Style::Italic:    return "\033[3m";
        case Style::Underline: return "\033[4m";
        case Style::Reverse:   return "\033[7m";
        case Style::Normal:    break;
        }

        return "\033[0m";
    }

    std::string reset()
    {
        return "\033[0m";
    }

    bool parseColor(const std::string &name, Color &color)
    {
        return lookupColor(Utility::toUpper(Utility::trim(name)), color);
    }

    bool parseStyle(const std::string &name, Style &style)
    {
        const std::string value = Utility::toUpper(Utility::trim(name));

        if (value.empty())
            return false;

        if (value == "NORMAL" || value == "NONE")
        {
            style = Style::Normal;
            return true;
        }

        if (value == "BOLD")      { style = Style::Bold;      return true; }
        if (value == "DIM")       { style = Style::Dim;       return true; }
        if (value == "ITALIC")    { style = Style::Italic;    return true; }
        if (value == "UNDERLINE") { style = Style::Underline; return true; }
        if (value == "REVERSE")   { style = Style::Reverse;   return true; }

        return false;
    }

    std::string styleName(Style style)
    {
        switch (style)
        {
        case Style::Normal:    return "Normal";
        case Style::Bold:      return "Bold";
        case Style::Dim:       return "Dim";
        case Style::Italic:    return "Italic";
        case Style::Underline: return "Underline";
        case Style::Reverse:   return "Reverse";
        }

        return "Normal";
    }

    Color colorFromName(const std::string &name, Color fallback)
    {
        Color parsed = fallback;

        if (!lookupColor(Utility::toUpper(Utility::trim(name)), parsed))
            return fallback;

        return parsed;
    }

    std::string colorName(Color color)
    {
        switch (color)
        {
        case Color::Default:       return "Default";
        case Color::Black:         return "Black";
        case Color::Red:           return "Red";
        case Color::Green:         return "Green";
        case Color::Yellow:        return "Yellow";
        case Color::Blue:          return "Blue";
        case Color::Magenta:       return "Magenta";
        case Color::Cyan:          return "Cyan";
        case Color::White:         return "White";
        case Color::BrightBlack:   return "BrightBlack";
        case Color::BrightRed:     return "BrightRed";
        case Color::BrightGreen:   return "BrightGreen";
        case Color::BrightYellow:  return "BrightYellow";
        case Color::BrightBlue:    return "BrightBlue";
        case Color::BrightMagenta: return "BrightMagenta";
        case Color::BrightCyan:    return "BrightCyan";
        case Color::BrightWhite:   return "BrightWhite";
        }

        return "Default";
    }
}

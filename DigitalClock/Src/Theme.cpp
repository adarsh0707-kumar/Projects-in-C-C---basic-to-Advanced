#include "Theme.hpp"

#include "Utility.hpp"

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

    Color colorFromName(const std::string &name, Color fallback)
    {
        const std::string value = Utility::toUpper(Utility::trim(name));

        if (value.empty())
            return fallback;

        if (value == "DEFAULT")       return Color::Default;
        if (value == "BLACK")         return Color::Black;
        if (value == "RED")           return Color::Red;
        if (value == "GREEN")         return Color::Green;
        if (value == "YELLOW")        return Color::Yellow;
        if (value == "BLUE")          return Color::Blue;
        if (value == "MAGENTA")       return Color::Magenta;
        if (value == "CYAN")          return Color::Cyan;
        if (value == "WHITE")         return Color::White;

        if (value == "BRIGHTBLACK")   return Color::BrightBlack;
        if (value == "BRIGHTRED")     return Color::BrightRed;
        if (value == "BRIGHTGREEN")   return Color::BrightGreen;
        if (value == "BRIGHTYELLOW")  return Color::BrightYellow;
        if (value == "BRIGHTBLUE")    return Color::BrightBlue;
        if (value == "BRIGHTMAGENTA") return Color::BrightMagenta;
        if (value == "BRIGHTCYAN")    return Color::BrightCyan;
        if (value == "BRIGHTWHITE")   return Color::BrightWhite;

        /*
        Spellings used by the bundled theme files and by Config/themes.ini.
        The "dark" variants map onto the standard (non-bright) colours and the
        grey variants onto bright black, which is how terminals render them.
        */
        if (value == "DARKBLUE")   return Color::Blue;
        if (value == "DARKRED")    return Color::Red;
        if (value == "DARKGREEN")  return Color::Green;
        if (value == "DARKGRAY" || value == "DARKGREY")
            return Color::BrightBlack;
        if (value == "GRAY" || value == "GREY")
            return Color::BrightBlack;

        return fallback;
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

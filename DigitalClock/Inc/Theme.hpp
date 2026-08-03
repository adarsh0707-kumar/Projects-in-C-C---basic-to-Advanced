#ifndef THEME_HPP
#define THEME_HPP

#include <string>

class Theme
{
public:
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

    enum class Style
    {
        Normal,
        Bold,
        Underline
    };

    Theme() = default;

    std::string foreground(Color color) const;
    std::string background(Color color) const;
    std::string style(Style style) const;
    std::string reset() const;
};

#endif // THEME_HPP
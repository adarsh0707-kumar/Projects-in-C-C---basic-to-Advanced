#ifndef THEME_HPP
#define THEME_HPP

/******************************************************************************
 * @file Theme.hpp
 * @brief ANSI colour and style primitives.
 * @author Adarsh Kumar
 * @date 2026
 *
 * This header provides the low-level colour vocabulary used by ThemeManager
 * and the presentation layer. It knows only how to turn a colour or style into
 * an ANSI escape sequence; deciding which colour a screen element should use
 * is the responsibility of ThemeManager.
 ******************************************************************************/

#include <string>

/**
 * @namespace Theme
 * @brief ANSI colour and style vocabulary.
 */
namespace Theme
{
    /**
     * @enum Color
     * @brief Terminal colours available to themes.
     */
    enum class Color
    {
        Default,       ///< Terminal's own foreground/background.
        Black,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,
        BrightBlack,   ///< Commonly rendered as dark grey.
        BrightRed,
        BrightGreen,
        BrightYellow,
        BrightBlue,
        BrightMagenta,
        BrightCyan,
        BrightWhite
    };

    /**
     * @enum Style
     * @brief Text attributes available to themes.
     */
    enum class Style
    {
        Normal,    ///< Clears all attributes.
        Bold,
        Dim,
        Italic,
        Underline,
        Reverse
    };

    /**
     * @brief Returns the escape sequence that sets a foreground colour.
     * @param color Colour to select.
     * @return std::string ANSI sequence.
     */
    std::string foreground(Color color);

    /**
     * @brief Returns the escape sequence that sets a background colour.
     * @param color Colour to select.
     * @return std::string ANSI sequence.
     */
    std::string background(Color color);

    /**
     * @brief Returns the escape sequence that sets a text attribute.
     * @param style Attribute to select.
     * @return std::string ANSI sequence.
     */
    std::string style(Style style);

    /**
     * @brief Returns the escape sequence that clears all colours and styles.
     * @return std::string ANSI reset sequence.
     */
    std::string reset();

    /**
     * @brief Converts a colour name to a Color.
     *
     * Matching is case-insensitive and tolerates the spellings used in the
     * bundled theme files, including "DarkBlue", "DarkGray" and "Grey".
     * Unrecognised names yield @p fallback.
     *
     * @param name     Colour name.
     * @param fallback Returned when @p name is not recognised.
     * @return Color Parsed colour.
     */
    Color colorFromName(const std::string &name,
                        Color fallback = Color::Default);

    /**
     * @brief Converts a Color back to its canonical name.
     * @param color Colour to name.
     * @return std::string Canonical name, such as "Cyan".
     */
    std::string colorName(Color color);

    /**
     * @brief Parses a colour name, reporting whether it was recognised.
     *
     * colorFromName() cannot distinguish "the name was not recognised" from
     * "the name resolved to the fallback", which a theme file parser needs to
     * know: a token that is not a colour may still be a valid style.
     *
     * @param name  Colour name.
     * @param color Receives the parsed colour, untouched on failure.
     * @return true if @p name names a colour.
     */
    bool parseColor(const std::string &name, Color &color);

    /**
     * @brief Parses a style name.
     *
     * Matching is case-insensitive. "Normal" and "None" both clear styling.
     *
     * @param name  Style name, such as "Bold".
     * @param style Receives the parsed style, untouched on failure.
     * @return true if @p name names a style.
     */
    bool parseStyle(const std::string &name, Style &style);

    /**
     * @brief Converts a Style back to its canonical name.
     * @param style Style to name.
     * @return std::string Canonical name, such as "Bold".
     */
    std::string styleName(Style style);
}

#endif // THEME_HPP

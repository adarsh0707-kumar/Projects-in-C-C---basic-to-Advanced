#ifndef THEME_HPP
#define THEME_HPP

/******************************************************************************
 * @file Theme.hpp
 * @brief Declaration of the Theme class.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Theme module provides ANSI escape sequences used to format
 * console output. It supports foreground colors, background colors,
 * text styles, and resetting terminal formatting to the default state.
 *
 * Responsibilities:
 *  - Apply foreground colors
 *  - Apply background colors
 *  - Apply text styles
 *  - Reset console formatting
 ******************************************************************************/

#include <string>

/**
 * @class Theme
 * @brief Provides ANSI escape sequences for console formatting.
 *
 * The Theme class generates ANSI escape codes that allow the
 * application to display colored and formatted text in supported
 * terminal environments. It abstracts ANSI codes behind simple
 * member functions, making console formatting easy to use.
 */
class Theme
{
public:
    /**
     * @enum Color
     * @brief Defines the supported console colors.
     */
    enum class Color
    {
        Default, /**< Use the terminal's default color. */
        Black,   /**< Black color. */
        Red,     /**< Red color. */
        Green,   /**< Green color. */
        Yellow,  /**< Yellow color. */
        Blue,    /**< Blue color. */
        Magenta, /**< Magenta color. */
        Cyan,    /**< Cyan color. */
        White    /**< White color. */
    };

    /**
     * @enum Style
     * @brief Defines the supported text styles.
     */
    enum class Style
    {
        Normal,   /**< Normal text style. */
        Bold,     /**< Bold text style. */
        Underline /**< Underlined text style. */
    };

    /**
     * @brief Constructs a Theme object.
     *
     * No explicit initialization is required because the class
     * only generates ANSI escape sequences.
     */
    Theme() = default;

    /**
     * @brief Returns the ANSI foreground color sequence.
     *
     * Generates the ANSI escape sequence corresponding to the
     * specified foreground color.
     *
     * @param color Desired foreground color.
     * @return std::string ANSI escape sequence.
     */
    std::string foreground(Color color) const;

    /**
     * @brief Returns the ANSI background color sequence.
     *
     * Generates the ANSI escape sequence corresponding to the
     * specified background color.
     *
     * @param color Desired background color.
     * @return std::string ANSI escape sequence.
     */
    std::string background(Color color) const;

    /**
     * @brief Returns the ANSI text style sequence.
     *
     * Generates the ANSI escape sequence corresponding to the
     * specified text style.
     *
     * @param style Desired text style.
     * @return std::string ANSI escape sequence.
     */
    std::string style(Style style) const;

    /**
     * @brief Resets all console formatting.
     *
     * Returns the ANSI escape sequence that restores the terminal
     * to its default foreground color, background color, and text style.
     *
     * @return std::string ANSI reset escape sequence.
     */
    std::string reset() const;
};

#endif // THEME_HPP
#ifndef THEME_MANAGER_HPP
#define THEME_MANAGER_HPP

/******************************************************************************
 * @file ThemeManager.hpp
 * @brief Declaration of the ThemeManager service.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The ThemeManager decides which colour each screen element is drawn in. It
 * loads a named theme from Resources/themes and falls back to the built-in
 * default when the requested theme is missing or unreadable (TC-013, TC-025).
 *
 * Reference: API Documentation, section 3.3.
 ******************************************************************************/

#include <map>
#include <string>
#include <vector>

#include "ResourceManager.hpp"
#include "Theme.hpp"

/**
 * @class ThemeManager
 * @brief Loads theme definitions and maps screen elements to colours.
 *
 * A theme file is a flat key/value list naming one colour per element, with
 * optional text styles after it:
 *
 * @code{.ini}
 * HEADER=Cyan Bold
 * TIME=Green
 * DATE=Yellow Italic
 * STATUS=White
 * FOOTER=White Dim
 * @endcode
 *
 * Order does not matter, so @c "Bold Cyan" is the same as @c "Cyan Bold", and
 * styles may be separated by spaces or commas.
 */
class ThemeManager
{
public:
    /**
     * @enum Element
     * @brief Screen elements that can be coloured independently.
     */
    enum class Element
    {
        Header, ///< Banner and title block.
        Time,   ///< The clock itself.
        Date,   ///< The date line.
        Status, ///< Status bar rows.
        Footer, ///< Exit hint and separators.
        Accent, ///< Separator rules and highlights.
        Alert,  ///< Alarm notification panel.
        Error   ///< Error and warning text.
    };

    /**
     * @brief Constructs a manager holding the built-in default theme.
     */
    ThemeManager();

    /**
     * @brief Loads a theme by name.
     *
     * The name is case-insensitive and is resolved to
     * @c Resources/themes/<name>.theme. When the file cannot be found or
     * contains no usable colours, the built-in default theme is applied and
     * the call reports failure so the caller can log a warning.
     *
     * @param themeName Theme to load, such as "Dark".
     * @return true if the named theme was loaded, false if the default was
     *         substituted.
     */
    bool loadTheme(const std::string &themeName);

    /**
     * @brief Marks the loaded theme as active.
     *
     * @return true when colours will be emitted, false when colour output is
     *         disabled and the theme therefore has no visible effect.
     */
    bool applyTheme();

    /**
     * @brief Returns the name of the active theme.
     * @return std::string Theme name, such as "Dark".
     */
    std::string currentTheme() const;

    /**
     * @brief Returns the escape sequence that starts drawing @p element.
     *
     * Returns an empty string when colour output is disabled, which keeps
     * redirected output free of escape sequences.
     *
     * @param element Element about to be drawn.
     * @return std::string ANSI sequence, or empty.
     */
    std::string colorFor(Element element) const;

    /**
     * @brief Returns the escape sequence that clears all attributes.
     * @return std::string ANSI reset, or empty when colour is disabled.
     */
    std::string reset() const;

    /**
     * @brief Returns the colour configured for @p element.
     * @param element Element to query.
     * @return Theme::Color Configured colour.
     */
    Theme::Color color(Element element) const;

    /**
     * @brief Returns the text styles configured for @p element.
     * @param element Element to query.
     * @return std::vector<Theme::Style> Styles, empty when none were named.
     */
    std::vector<Theme::Style> styles(Element element) const;

    /**
     * @brief Describes an element's appearance, for display and diagnostics.
     * @param element Element to describe.
     * @return std::string Text such as "BrightGreen Bold".
     */
    std::string describe(Element element) const;

    /**
     * @brief Enables or disables all colour output.
     *
     * The Console disables colour when standard output is not a terminal.
     *
     * @param enabled true to emit escape sequences.
     */
    void setColorEnabled(bool enabled);

    /**
     * @brief Reports whether colour output is enabled.
     * @return true when escape sequences are emitted.
     */
    bool isColorEnabled() const;

    /**
     * @brief Reports whether applyTheme() has been called.
     * @return true once the theme has been activated.
     */
    bool isApplied() const;

    /**
     * @brief Restores the built-in default theme.
     */
    void applyDefaultTheme();

    /**
     * @brief Counts tokens in the loaded theme that named no colour or style.
     *
     * A misspelt colour would otherwise be indistinguishable from a theme
     * that simply did not mention that element.
     *
     * @return std::size_t Number of unrecognised tokens.
     */
    std::size_t unknownTokenCount() const;

    /**
     * @brief Lists the themes shipped with the application.
     * @return std::vector<std::string> Theme names.
     */
    static std::vector<std::string> availableThemes();

    /**
     * @brief Loads the theme after the active one, wrapping around.
     *
     * Lets the user cycle themes at runtime rather than editing the
     * configuration and restarting. A theme that is not in the shipped list --
     * the built-in default, or a user's own file -- starts the cycle from the
     * beginning.
     *
     * @return std::string Name of the theme now active.
     */
    std::string cycleTheme();

    /**
     * @brief Builds the resource path for a theme name.
     * @param themeName Theme name, such as "Dark".
     * @return std::string Path such as @c Resources/themes/dark.theme.
     */
    static std::string themePath(const std::string &themeName);

private:
    /**
     * @brief Maps a theme-file key to the element it colours.
     *
     * @param key     Key from the theme file.
     * @param element Receives the matching element.
     * @return true if @p key names a known element.
     */
    static bool elementFromKey(const std::string &key, Element &element);

    /**
     * @brief Parses a theme value into a colour and any styles.
     *
     * Tokens are separated by spaces or commas and may appear in any order,
     * so "Bold Cyan" and "Cyan, Bold" are equivalent. Unrecognised tokens are
     * counted rather than silently dropped.
     *
     * @param value    Value from the theme file, such as "BrightGreen Bold".
     * @param color    Receives the colour; left alone when none was named.
     * @param elementStyles Receives the styles, cleared first.
     * @return std::size_t Number of tokens that named neither.
     */
    static std::size_t parseAppearance(
        const std::string &value,
        Theme::Color &color,
        std::vector<Theme::Style> &elementStyles);

    std::map<Element, Theme::Color> colors; ///< Element to colour mapping.

    /// Element to text-style mapping. Absent or empty means unstyled.
    std::map<Element, std::vector<Theme::Style>> elementStyles;

    std::string themeName;                  ///< Name of the active theme.
    bool colorEnabled;                      ///< Whether colour is emitted.
    bool applied;                           ///< Whether applyTheme() ran.
    std::size_t unknownTokens;              ///< Unrecognised tokens in the file.
    ResourceManager resources;              ///< Locates theme files.
};

#endif // THEME_MANAGER_HPP

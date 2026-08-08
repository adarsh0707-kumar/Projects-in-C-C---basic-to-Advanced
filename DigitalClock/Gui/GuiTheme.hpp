#ifndef GUI_THEME_HPP
#define GUI_THEME_HPP

/******************************************************************************
 * @file GuiTheme.hpp
 * @brief Bridges the console ThemeManager to Qt colours and stylesheets.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The graphical interface reads the same theme files from Resources/themes as
 * the console one. That is the point: a theme is a property of the
 * application, not of one of its front ends, and duplicating the definitions
 * would let the two drift apart.
 *
 * What the theme files do not define is a background, because the console
 * inherits the terminal's. A window has to pick one, so it is derived from
 * the theme rather than added to the format -- see prefersDarkBackground().
 ******************************************************************************/

#include <QColor>
#include <QFont>
#include <QString>

#include "Theme.hpp"
#include "ThemeManager.hpp"

/**
 * @namespace GuiTheme
 * @brief Translation between the theme vocabulary and Qt.
 */
namespace GuiTheme
{
    /**
     * @brief Converts a theme colour to the RGB a terminal would render.
     *
     * Uses the widely-implemented xterm palette, so a theme looks the same in
     * the window as it does in a terminal.
     *
     * @param color Theme colour.
     * @param onDark Whether it will be drawn on a dark background, which
     *               decides what @c Default resolves to.
     * @return QColor Corresponding colour.
     */
    QColor toColor(Theme::Color color, bool onDark);

    /**
     * @brief Reports whether a theme is intended for a dark background.
     *
     * Decided from the theme's own Status colour: a theme that draws its
     * status text in a dark colour must expect a light background behind it,
     * and vice versa. This keeps the answer in the theme file rather than in
     * a hardcoded list of theme names that a user's own theme could not join.
     *
     * @param theme Loaded theme.
     * @return true when the window should use a dark background.
     */
    bool prefersDarkBackground(const ThemeManager &theme);

    /**
     * @brief Returns the window background for a theme.
     * @param theme Loaded theme.
     * @return QColor Background colour.
     */
    QColor background(const ThemeManager &theme);

    /**
     * @brief Builds the stylesheet for the whole window.
     * @param theme Loaded theme.
     * @return QString Qt stylesheet.
     */
    QString styleSheet(const ThemeManager &theme);

    /**
     * @brief Returns the colour for one screen element.
     * @param theme   Loaded theme.
     * @param element Element to colour.
     * @return QColor Element colour.
     */
    QColor colorFor(const ThemeManager &theme, ThemeManager::Element element);

    /**
     * @brief Applies a theme element's text styles to a font.
     *
     * Bold, italic and underline carry over from the theme file. Dim has no
     * font equivalent and is applied as a colour adjustment by the caller;
     * reverse has no sensible meaning here and is ignored.
     *
     * @param theme   Loaded theme.
     * @param element Element whose styles apply.
     * @param font    Font to modify.
     */
    void applyStyles(const ThemeManager &theme,
                     ThemeManager::Element element,
                     QFont &font);

    /**
     * @brief Reports whether an element is dimmed by its theme.
     * @param theme   Loaded theme.
     * @param element Element to query.
     * @return true when the theme names Dim for this element.
     */
    bool isDimmed(const ThemeManager &theme, ThemeManager::Element element);
}

#endif // GUI_THEME_HPP

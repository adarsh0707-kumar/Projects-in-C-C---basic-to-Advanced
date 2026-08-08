#include "GuiTheme.hpp"

#include <vector>

namespace
{
    /**
     * @brief Returns the xterm RGB for a theme colour.
     *
     * These are the values a terminal emulator uses for the sixteen ANSI
     * colours. Matching them means a theme reads the same in the window as in
     * a terminal, which is the whole reason the two share theme files.
     */
    QColor paletteColor(Theme::Color color)
    {
        switch (color)
        {
        case Theme::Color::Black:         return QColor(0x00, 0x00, 0x00);
        case Theme::Color::Red:           return QColor(0xCD, 0x00, 0x00);
        case Theme::Color::Green:         return QColor(0x00, 0xCD, 0x00);
        case Theme::Color::Yellow:        return QColor(0xCD, 0xCD, 0x00);
        case Theme::Color::Blue:          return QColor(0x00, 0x00, 0xEE);
        case Theme::Color::Magenta:       return QColor(0xCD, 0x00, 0xCD);
        case Theme::Color::Cyan:          return QColor(0x00, 0xCD, 0xCD);
        case Theme::Color::White:         return QColor(0xE5, 0xE5, 0xE5);
        case Theme::Color::BrightBlack:   return QColor(0x7F, 0x7F, 0x7F);
        case Theme::Color::BrightRed:     return QColor(0xFF, 0x00, 0x00);
        case Theme::Color::BrightGreen:   return QColor(0x00, 0xFF, 0x00);
        case Theme::Color::BrightYellow:  return QColor(0xFF, 0xFF, 0x00);
        case Theme::Color::BrightBlue:    return QColor(0x5C, 0x5C, 0xFF);
        case Theme::Color::BrightMagenta: return QColor(0xFF, 0x00, 0xFF);
        case Theme::Color::BrightCyan:    return QColor(0x00, 0xFF, 0xFF);
        case Theme::Color::BrightWhite:   return QColor(0xFF, 0xFF, 0xFF);
        case Theme::Color::Default:       break;
        }

        // Default has no palette entry; the caller decides from the
        // background, which is what a terminal does too.
        return QColor();
    }

    /**
     * @brief Reports whether a colour is light enough to sit on a dark field.
     *
     * Uses perceived luminance rather than a plain average, because the eye
     * is far more sensitive to green than to blue and an average would call
     * pure blue "light".
     */
    bool isLight(const QColor &color)
    {
        const double luminance = (0.299 * color.redF()) +
                                 (0.587 * color.greenF()) +
                                 (0.114 * color.blueF());

        return luminance > 0.5;
    }
}

QColor GuiTheme::toColor(Theme::Color color, bool onDark)
{
    const QColor mapped = paletteColor(color);

    if (mapped.isValid())
        return mapped;

    // Theme::Color::Default means "the terminal's own foreground".
    return onDark ? QColor(0xE5, 0xE5, 0xE5) : QColor(0x1A, 0x1A, 0x1A);
}

bool GuiTheme::prefersDarkBackground(const ThemeManager &theme)
{
    /*
    Ask the theme rather than its name. A theme that draws status text in a
    dark colour must be expecting a light background behind it; one that uses
    white expects a dark one. This way a user's own theme file gets the right
    background without having to be added to a list somewhere in the code.
    */
    return isLight(paletteColor(theme.color(ThemeManager::Element::Status)));
}

QColor GuiTheme::background(const ThemeManager &theme)
{
    // Not pure black or white: a little off the extreme is easier to look at
    // for something that stays on screen all day.
    return prefersDarkBackground(theme) ? QColor(0x12, 0x14, 0x18)
                                        : QColor(0xF4, 0xF4, 0xF2);
}

QColor GuiTheme::colorFor(const ThemeManager &theme,
                          ThemeManager::Element element)
{
    const bool onDark = prefersDarkBackground(theme);

    QColor color = toColor(theme.color(element), onDark);

    if (isDimmed(theme, element))
    {
        /*
        Dim has no font equivalent, so it is applied where a terminal applies
        it: to the colour. Blending towards the background rather than simply
        darkening keeps it dimmer on a light theme too, where darkening would
        make it stand out more.
        */
        const QColor field = background(theme);

        color = QColor(
            (color.red() + field.red()) / 2,
            (color.green() + field.green()) / 2,
            (color.blue() + field.blue()) / 2);
    }

    return color;
}

bool GuiTheme::isDimmed(const ThemeManager &theme,
                        ThemeManager::Element element)
{
    for (const Theme::Style style : theme.styles(element))
    {
        if (style == Theme::Style::Dim)
            return true;
    }

    return false;
}

void GuiTheme::applyStyles(const ThemeManager &theme,
                           ThemeManager::Element element,
                           QFont &font)
{
    for (const Theme::Style style : theme.styles(element))
    {
        switch (style)
        {
        case Theme::Style::Bold:
            font.setBold(true);
            break;

        case Theme::Style::Italic:
            font.setItalic(true);
            break;

        case Theme::Style::Underline:
            font.setUnderline(true);
            break;

        case Theme::Style::Normal:
            font.setBold(false);
            font.setItalic(false);
            font.setUnderline(false);
            break;

        // Dim is handled as a colour adjustment in colorFor(); Reverse means
        // swapping foreground and background, which is a terminal idea with
        // no useful equivalent for an individual label here.
        case Theme::Style::Dim:
        case Theme::Style::Reverse:
            break;
        }
    }
}

QString GuiTheme::styleSheet(const ThemeManager &theme)
{
    const QColor field = background(theme);
    const QColor status = colorFor(theme, ThemeManager::Element::Status);
    const QColor accent = colorFor(theme, ThemeManager::Element::Accent);
    const QColor footer = colorFor(theme, ThemeManager::Element::Footer);

    const bool onDark = prefersDarkBackground(theme);

    // Controls need a surface distinct from the window without introducing a
    // colour the theme never named, so the background is nudged rather than
    // replaced.
    const QColor surface = onDark ? field.lighter(160) : field.darker(108);
    const QColor border = onDark ? field.lighter(220) : field.darker(120);

    return QString(
               "QMainWindow, QWidget { background-color: %1; color: %2; }"
               "QMenuBar { background-color: %3; color: %2; }"
               "QMenuBar::item { background: transparent; padding: 4px 9px; }"
               "QMenuBar::item:selected { background-color: %4; color: %1; }"
               "QMenu { background-color: %3; color: %2; border: 1px solid %5; }"
               "QMenu::item:selected { background-color: %4; color: %1; }"
               "QPushButton {"
               "  background-color: %3; color: %2;"
               "  border: 1px solid %5; border-radius: 4px;"
               "  padding: 6px 14px; }"
               "QPushButton:hover { border-color: %4; }"
               "QPushButton:pressed { background-color: %4; color: %1; }"
               "QPushButton:disabled { color: %6; border-color: %6; }"
               "QListWidget, QTableWidget, QLineEdit {"
               "  background-color: %3; color: %2;"
               "  border: 1px solid %5; }"
               "QHeaderView::section {"
               "  background-color: %3; color: %6;"
               "  border: 0px; padding: 4px; }"
               "QStatusBar { color: %6; }")
        .arg(field.name(),
             status.name(),
             surface.name(),
             accent.name(),
             border.name(),
             footer.name());
}

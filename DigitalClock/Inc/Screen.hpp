#ifndef SCREEN_HPP
#define SCREEN_HPP

/******************************************************************************
 * @file Screen.hpp
 * @brief Declaration of the Screen presentation component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Screen composes the complete console layout from the banner, the clock,
 * the date and the status rows. It is the only component that decides where
 * things sit on screen.
 *
 * Redraws are flicker-free: rather than clearing the screen and repainting,
 * draw() returns the cursor home and overwrites each line, erasing the rest of
 * each line as it goes. This satisfies FR-008 (no duplicated output) without
 * the flashing a full clear would cause (FR-003).
 *
 * Reference: API Documentation, section 4.5.
 ******************************************************************************/

#include <string>
#include <vector>

#include "Banner.hpp"
#include "Console.hpp"
#include "ThemeManager.hpp"

/**
 * @class Screen
 * @brief Lays out and draws the complete application screen.
 */
class Screen
{
public:
    /**
     * @brief Constructs a screen with the default 80x24 layout.
     */
    Screen();

    /**
     * @brief Directs output to a Console.
     *
     * Until a console is attached, draw() writes to standard output.
     *
     * @param console Console to draw through. Must outlive this Screen.
     */
    void attach(Console &console);

    /**
     * @brief Supplies the theme used to colour the layout.
     *
     * @param theme Theme manager to consult. Must outlive this Screen.
     */
    void attach(ThemeManager &theme);

    /**
     * @brief Draws the composed screen.
     */
    void draw();

    /**
     * @brief Updates the layout dimensions.
     *
     * When the terminal is too short for the full layout the banner is hidden
     * automatically so the clock stays visible.
     *
     * @param width  New width in columns.
     * @param height New height in rows.
     */
    void resize(int width, int height);

    /**
     * @brief Restores the default layout and clears the drawn content.
     */
    void reset();

    /**
     * @brief Sets the formatted time to display.
     * @param time Text produced by TimeFormatter.
     */
    void setTime(const std::string &time);

    /**
     * @brief Sets the formatted date to display.
     * @param date Text produced by TimeFormatter.
     */
    void setDate(const std::string &date);

    /**
     * @brief Sets the status rows shown beneath the clock.
     * @param lines Rows produced by StatusBar.
     */
    void setStatusLines(const std::vector<std::string> &lines);

    /**
     * @brief Sets the hint shown on the final row.
     * @param hint Text such as "Press Q or Ctrl+C to Exit".
     */
    void setFooterHint(const std::string &hint);

    /**
     * @brief Sets informational rows shown beneath the date.
     *
     * Used by the world clock for its zone list. Drawn above any alert
     * panel, so a ringing alarm still sits closest to the eye.
     *
     * @param lines Rows to draw. An empty vector removes them.
     */
    void setInfoLines(const std::vector<std::string> &lines);

    /**
     * @brief Sets the alert panel shown beneath the date.
     *
     * Pass an empty vector to remove it. The panel is drawn in the theme's
     * alert colour so a ringing alarm is unmistakable.
     *
     * @param lines Panel lines produced by Notifier.
     */
    void setNotification(const std::vector<std::string> &lines);

    /**
     * @brief Reports whether an alert panel is currently part of the layout.
     * @return true when a panel will be drawn.
     */
    bool hasNotification() const;

    /**
     * @brief Composes the screen without drawing it.
     *
     * Exposed so that layout can be verified in tests without a terminal.
     *
     * @return std::vector<std::string> The lines that draw() would emit,
     *         without colour codes or cursor control.
     */
    std::vector<std::string> compose() const;

    /**
     * @brief Returns the banner component so it can be loaded or hidden.
     * @return Banner& The owned banner.
     */
    Banner &banner();

    /**
     * @brief Returns the layout width.
     * @return int Width in columns.
     */
    int width() const;

    /**
     * @brief Returns the layout height.
     * @return int Height in rows.
     */
    int height() const;

private:
    /**
     * @brief Returns a horizontal rule spanning the layout width.
     * @param character Character to repeat.
     * @return std::string Rule of the current width.
     */
    std::string rule(char character) const;

    Banner bannerArt;                     ///< Header artwork.
    std::string timeText;                 ///< Formatted time.
    std::string dateText;                 ///< Formatted date.
    std::vector<std::string> statusLines; ///< Status rows.
    std::vector<std::string> infoLines;   ///< Informational rows, if any.
    std::vector<std::string> alertLines;  ///< Alarm alert panel, if any.
    std::string footerHint;               ///< Exit hint.

    int layoutWidth;  ///< Layout width in columns.
    int layoutHeight; ///< Layout height in rows.

    Console *console;    ///< Output target, or null for standard output.
    ThemeManager *theme; ///< Colour source, or null for uncoloured output.
};

#endif // SCREEN_HPP

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

/******************************************************************************
 * @file Display.hpp
 * @brief Declaration of the Display presentation component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Display coordinates the presentation layer. It receives already
 * formatted text from TimeFormatter, hands it to Screen for layout, and draws
 * the result through Console. It performs no formatting of its own, which
 * keeps presentation and business logic apart.
 *
 * Reference: API Documentation, sections 2.5 and 4.4.
 ******************************************************************************/

#include <string>
#include <vector>

#include "Console.hpp"
#include "Screen.hpp"
#include "StatusBar.hpp"
#include "ThemeManager.hpp"

/**
 * @class Display
 * @brief Owns the console, screen layout and status bar.
 *
 * Typical use per frame:
 * @code
 * display.renderClock(formatter.formatTimeWide(clock));
 * display.renderDate(formatter.formatDate(date));
 * display.renderScreen();
 * @endcode
 */
class Display
{
public:
    Display();

    /**
     * @brief Restores the terminal if shutdown() was not called.
     */
    ~Display();

    Display(const Display &) = delete;
    Display &operator=(const Display &) = delete;

    /**
     * @brief Prepares the console and wires the theme into the layout.
     *
     * Also disables colour when the terminal cannot render it, so redirected
     * output stays free of escape sequences.
     *
     * @param theme Theme manager supplying element colours. Must outlive this
     *              Display.
     */
    void initialize(ThemeManager &theme);

    /**
     * @brief Stores the formatted time for the next frame.
     * @param time Formatted time text.
     */
    void renderClock(const std::string &time);

    /**
     * @brief Stores the formatted date for the next frame.
     * @param date Formatted date text.
     */
    void renderDate(const std::string &date);

    /**
     * @brief Composes and draws the complete screen.
     *
     * Re-reads the terminal size first so the layout follows a resized
     * window.
     */
    void renderScreen();

    /**
     * @brief Draws the screen. Equivalent to renderScreen().
     */
    void render();

    /**
     * @brief Redraws the screen. Equivalent to renderScreen().
     */
    void update();

    /**
     * @brief Flushes pending output to the terminal.
     */
    void refresh();

    /**
     * @brief Clears the terminal.
     */
    void clear();

    /**
     * @brief Sets the transient status message.
     * @param text Message to show, or an empty string to remove it.
     */
    void setMessage(const std::string &text);

    /**
     * @brief Adds or updates a named status field.
     * @param name  Field label, such as "Theme".
     * @param value Field value, such as "Dark".
     */
    void setStatusField(const std::string &name, const std::string &value);

    /**
     * @brief Shows an alert panel beneath the clock.
     *
     * @param lines Panel lines, typically produced by Notifier. An empty
     *              vector removes the panel.
     */
    void showNotification(const std::vector<std::string> &lines);

    /**
     * @brief Removes the alert panel.
     */
    void clearNotification();

    /**
     * @brief Loads banner artwork from a resource file.
     * @param fileName Resource path.
     * @return true if the artwork was loaded, false if the default was kept.
     */
    bool loadBanner(const std::string &fileName);

    /**
     * @brief Restores the terminal to its pre-run state.
     *
     * Safe to call more than once.
     */
    void shutdown();

    /**
     * @brief Reads a keystroke without blocking.
     * @return int Character read, or -1 when no key is waiting.
     */
    int readKey();

    /**
     * @brief Returns the owned console.
     * @return Console& The console used for output.
     */
    Console &console();

    /**
     * @brief Returns the owned screen layout.
     * @return Screen& The screen used for composition.
     */
    Screen &screen();

    /**
     * @brief Returns the owned status bar.
     * @return StatusBar& The status bar shown beneath the clock.
     */
    StatusBar &statusBar();

private:
    Console terminal;  ///< Terminal abstraction.
    Screen layout;     ///< Layout composer, owns the banner.
    StatusBar status;  ///< Runtime information rows.
    bool initialized;  ///< Whether initialize() has run.
};

#endif // DISPLAY_HPP

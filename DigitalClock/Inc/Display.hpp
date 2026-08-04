#ifndef DISPLAY_HPP
#define DISPLAY_HPP

/******************************************************************************
 * @file Display.hpp
 * @brief Declaration of the Display class.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Display module is responsible for rendering the complete user
 * interface of the Digital Clock application. It coordinates multiple
 * modules to display formatted information on the console.
 *
 * Responsibilities:
 *  - Clear the console screen
 *  - Display application header
 *  - Display current date
 *  - Display current time
 *  - Display footer
 *  - Coordinate Banner, Theme, and Screen modules
 ******************************************************************************/

#include "Banner.hpp"
#include "Clock.hpp"
#include "Date.hpp"
#include "Screen.hpp"
#include "Theme.hpp"

/**
 * @class Display
 * @brief Handles rendering of the Digital Clock user interface.
 *
 * The Display class combines information from the Clock and Date
 * modules with visual resources provided by the Banner, Theme,
 * and Screen classes to render the complete console interface.
 */
class Display
{
public:
    /**
     * @brief Constructs a Display object.
     *
     * Initializes the display manager. The associated Banner,
     * Theme, and Screen objects are default constructed.
     */
    Display() = default;

    /**
     * @brief Clears the console screen.
     *
     * Removes all previously displayed content in preparation
     * for rendering the next screen.
     */
    void clearScreen() const;

    /**
     * @brief Displays the application header.
     *
     * Prints the application banner or logo at the top of
     * the console window.
     */
    void printHeader() const;

    /**
     * @brief Displays the current date.
     *
     * Formats and prints the supplied date information.
     *
     * @param date Reference to the Date object.
     */
    void printDate(const Date &date) const;

    /**
     * @brief Displays the current time.
     *
     * Formats and prints the supplied time information.
     *
     * @param clock Reference to the Clock object.
     */
    void printClock(const Clock &clock) const;

    /**
     * @brief Displays the application footer.
     *
     * Prints footer information such as status text or
     * additional application details.
     */
    void printFooter() const;

    /**
     * @brief Renders the complete user interface.
     *
     * Coordinates all rendering operations including clearing
     * the screen, printing the header, date, time, and footer.
     *
     * @param clock Reference to the Clock object.
     * @param date Reference to the Date object.
     */
    void render(const Clock &clock,
                const Date &date) const;

private:
    /**
     * @brief Provides ASCII banner resources.
     */
    Banner banner;

    /**
     * @brief Provides console color and text formatting.
     */
    Theme theme;

    /**
     * @brief Manages low-level console operations.
     */
    Screen screen;
};

#endif // DISPLAY_HPP
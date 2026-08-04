#ifndef SCREEN_HPP
#define SCREEN_HPP

/******************************************************************************
 * @file Screen.hpp
 * @brief Declaration of the Screen class.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Screen module provides low-level console manipulation functions
 * used throughout the Digital Clock application. It encapsulates ANSI
 * escape sequences behind a simple interface, allowing other modules
 * to perform screen operations without dealing directly with terminal
 * control codes.
 *
 * Responsibilities:
 *  - Clear the console screen
 *  - Move the cursor to the home position
 *  - Hide the console cursor
 *  - Show the console cursor
 *  - Flush pending console output
 ******************************************************************************/

/**
 * @class Screen
 * @brief Provides console screen management utilities.
 *
 * The Screen class abstracts terminal-specific operations such as
 * clearing the display, controlling cursor visibility, positioning
 * the cursor, and flushing output. This keeps ANSI escape sequences
 * isolated from the rest of the application.
 */
class Screen
{
public:
    /**
     * @brief Constructs a Screen object.
     *
     * No explicit initialization is required because this class
     * provides utility functions for console manipulation.
     */
    Screen() = default;

    /**
     * @brief Clears the entire console screen.
     *
     * Removes all visible content from the terminal and prepares
     * it for rendering a new screen.
     */
    void clear() const;

    /**
     * @brief Moves the cursor to the home position.
     *
     * Positions the cursor at the top-left corner of the console.
     */
    void home() const;

    /**
     * @brief Hides the console cursor.
     *
     * Prevents the cursor from being displayed while the application
     * continuously refreshes the screen, providing a cleaner interface.
     */
    void hideCursor() const;

    /**
     * @brief Shows the console cursor.
     *
     * Restores cursor visibility before the application exits.
     */
    void showCursor() const;

    /**
     * @brief Flushes pending console output.
     *
     * Ensures that all buffered output is immediately written
     * to the terminal.
     */
    void flush() const;
};

#endif // SCREEN_HPP
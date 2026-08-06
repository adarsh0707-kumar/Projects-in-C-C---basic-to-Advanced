#ifndef CONSOLE_HPP
#define CONSOLE_HPP

/******************************************************************************
 * @file Console.hpp
 * @brief Declaration of the Console presentation component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Console owns every direct interaction with the terminal: clearing the
 * screen, positioning the cursor, writing text and reading keystrokes. No
 * other component emits escape sequences of its own, which keeps terminal
 * handling in one place and makes a future GUI port tractable.
 *
 * Reference: API Documentation, section 4.3.
 ******************************************************************************/

#include <string>

/**
 * @class Console
 * @brief Terminal abstraction for the presentation layer.
 *
 * initialize() takes ownership of the terminal state (cursor visibility and,
 * on POSIX, canonical input mode) and shutdown() restores it. The destructor
 * calls shutdown() so the terminal is repaired even on an unusual exit path.
 */
class Console
{
public:
    Console();

    /**
     * @brief Restores the terminal if shutdown() was not called.
     */
    ~Console();

    Console(const Console &) = delete;
    Console &operator=(const Console &) = delete;

    /**
     * @brief Prepares the terminal for rendering.
     *
     * Hides the cursor and, when standard input is a terminal, switches to
     * non-canonical unbuffered input so that keystrokes can be polled without
     * waiting for Enter.
     */
    void initialize();

    /**
     * @brief Clears the screen and moves the cursor to the top-left corner.
     */
    void clear();

    /**
     * @brief Flushes buffered output to the terminal.
     */
    void refresh();

    /**
     * @brief Moves the cursor to a one-based screen position.
     *
     * @param row    Row, counting from 1 at the top.
     * @param column Column, counting from 1 at the left.
     */
    void setCursor(int row, int column);

    /**
     * @brief Writes text without appending a newline.
     * @param text Text to write.
     */
    void print(const std::string &text);

    /**
     * @brief Writes text followed by a newline.
     * @param text Text to write.
     */
    void printLine(const std::string &text);

    /**
     * @brief Restores the terminal to the state it had before initialize().
     *
     * Shows the cursor, restores canonical input and flushes output. Safe to
     * call more than once.
     */
    void shutdown();

    /**
     * @brief Hides the text cursor.
     */
    void hideCursor();

    /**
     * @brief Shows the text cursor.
     */
    void showCursor();

    /**
     * @brief Moves the cursor to the top-left corner without clearing.
     */
    void home();

    /**
     * @brief Reads one keystroke if the user has pressed a key.
     *
     * Never blocks. Returns immediately when no key is waiting or when
     * standard input is not a terminal.
     *
     * @return int The character read, or -1 when no key is available.
     */
    int readKey();

    /**
     * @brief Reports whether the terminal can render colour.
     *
     * False when standard output is redirected to a file or a pipe, so that
     * captured output contains no escape sequences.
     *
     * @return true when colour output is appropriate.
     */
    bool supportsColor() const;

    /**
     * @brief Returns the terminal width in columns.
     * @return int Column count, defaulting to 80 when it cannot be detected.
     */
    int width() const;

    /**
     * @brief Returns the terminal height in rows.
     * @return int Row count, defaulting to 24 when it cannot be detected.
     */
    int height() const;

    /**
     * @brief Reports whether initialize() has run without a later shutdown().
     * @return true while the console owns the terminal state.
     */
    bool isInitialized() const;

private:
    /**
     * @brief Switches standard input to unbuffered, non-echoing mode.
     */
    void enableRawInput();

    /**
     * @brief Restores the input mode saved by enableRawInput().
     */
    void restoreInput();

    bool initialized;    ///< Whether the terminal state is currently owned.
    bool rawInputActive; ///< Whether the input mode was changed.
    bool colorCapable;   ///< Cached result of the colour capability probe.
};

#endif // CONSOLE_HPP

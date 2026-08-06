#ifndef STATUS_BAR_HPP
#define STATUS_BAR_HPP

/******************************************************************************
 * @file StatusBar.hpp
 * @brief Declaration of the StatusBar presentation component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The StatusBar reports runtime information below the clock: the active
 * theme, the refresh interval, the application state and any transient
 * message such as a warning about a missing resource.
 *
 * Reference: API Documentation, section 4.7.
 ******************************************************************************/

#include <string>
#include <utility>
#include <vector>

/**
 * @class StatusBar
 * @brief Builds the rows of runtime information shown under the clock.
 *
 * Fields are rendered in the order they were first set, so the layout stays
 * stable between frames.
 */
class StatusBar
{
public:
    StatusBar();

    /**
     * @brief Sets the transient message row.
     *
     * The message is rendered last, after the named fields. An empty message
     * removes the row.
     *
     * @param message Text to show.
     */
    void setMessage(const std::string &message);

    /**
     * @brief Returns the current transient message.
     * @return std::string Message text, empty when none is set.
     */
    std::string message() const;

    /**
     * @brief Adds or updates a named field.
     *
     * @param name  Field label, such as "Theme".
     * @param value Field value, such as "Dark".
     */
    void setField(const std::string &name, const std::string &value);

    /**
     * @brief Returns the value of a named field.
     *
     * @param name Field label.
     * @return std::string Field value, empty when the field is not set.
     */
    std::string field(const std::string &name) const;

    /**
     * @brief Returns the rows to draw.
     *
     * Each row is formatted as <tt>Label : Value</tt> with labels padded to a
     * common width so the values line up.
     *
     * @return std::vector<std::string> Rows, empty when the bar is hidden.
     */
    std::vector<std::string> lines() const;

    /**
     * @brief Writes the status rows to standard output.
     */
    void show();

    /**
     * @brief Removes every field and the transient message.
     */
    void clear();

    /**
     * @brief Makes the status bar visible.
     */
    void setVisible(bool visible);

    /**
     * @brief Reports whether the status bar should be drawn.
     * @return true when visible.
     */
    bool isVisible() const;

private:
    /** Field labels paired with their values, in insertion order. */
    std::vector<std::pair<std::string, std::string>> fields;

    std::string transientMessage; ///< Optional trailing message row.
    bool visible;                 ///< Whether the bar is drawn.
};

#endif // STATUS_BAR_HPP

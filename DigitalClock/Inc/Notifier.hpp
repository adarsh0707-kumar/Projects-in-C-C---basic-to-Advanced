#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

/******************************************************************************
 * @file Notifier.hpp
 * @brief Declaration of the Notifier presentation component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Notifier turns a ringing alarm into something the user can see and
 * hear. It composes the alert panel and emits the terminal bell, but holds no
 * alarm logic of its own: it is told what to announce and when to stop.
 *
 * Introduced in v1.1.0 (Change Log, section 6.3).
 ******************************************************************************/

#include <string>
#include <vector>

#include "Alarm.hpp"

/**
 * @class Notifier
 * @brief Composes the alert panel shown while an alarm is ringing.
 *
 * The panel is returned as plain text lines rather than drawn directly, so
 * that Screen keeps sole responsibility for layout and colour, and so the
 * panel can be asserted in tests without a terminal.
 */
class Notifier
{
public:
    Notifier();

    /**
     * @brief Announces a ringing alarm.
     *
     * Emits the terminal bell once if the bell is enabled, and makes the
     * alert panel active.
     *
     * @param alarm         Alarm that is ringing.
     * @param snoozeMinutes Snooze delay to advertise in the key hint.
     */
    void notify(const Alarm &alarm, int snoozeMinutes);

    /**
     * @brief Re-emits the terminal bell while an alarm is still ringing.
     *
     * Called once per refresh so an unattended alarm keeps signalling. Does
     * nothing when no alert is active or the bell is disabled.
     */
    void pulse();

    /**
     * @brief Dismisses the alert panel.
     */
    void clear();

    /**
     * @brief Reports whether an alert is being shown.
     * @return true while the panel is active.
     */
    bool isActive() const;

    /**
     * @brief Returns the alert panel as text lines.
     *
     * @param width Width of the surrounding layout, in columns.
     * @return std::vector<std::string> Panel lines, empty when inactive.
     */
    std::vector<std::string> lines(int width) const;

    /**
     * @brief Returns the label of the alarm being announced.
     * @return std::string Alarm label, empty when inactive or unlabelled.
     */
    std::string label() const;

    /**
     * @brief Returns the time of the alarm being announced, as @c HH:MM.
     * @return std::string Formatted time, empty when inactive.
     */
    std::string time() const;

    /**
     * @brief Enables or disables the terminal bell.
     * @param enabled true to emit the bell.
     */
    void setBellEnabled(bool enabled);

    /**
     * @brief Reports whether the terminal bell is enabled.
     * @return true when the bell will be emitted.
     */
    bool bellEnabled() const;

    /**
     * @brief Returns how many times the bell has been emitted.
     *
     * Exposed so that bell behaviour can be asserted without listening to a
     * terminal.
     *
     * @return int Bell count since construction.
     */
    int bellCount() const;

private:
    /**
     * @brief Emits the terminal bell if enabled, and counts it.
     */
    void ring();

    bool active;        ///< Whether the panel is being shown.
    std::string alarmTime;  ///< Time of the ringing alarm, as HH:MM.
    std::string alarmLabel; ///< Label of the ringing alarm.
    std::string hint;       ///< Key hint line.
    bool bell;          ///< Whether the terminal bell is enabled.
    int bells;          ///< Number of bells emitted.
};

#endif // NOTIFIER_HPP

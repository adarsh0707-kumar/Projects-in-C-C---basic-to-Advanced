#ifndef UTILITY_HPP
#define UTILITY_HPP

/******************************************************************************
 * @file Utility.hpp
 * @brief Declaration of the Utility helper class.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Utility class provides small, stateless helper functions that are
 * shared across every layer of the Digital Clock System. Grouping them here
 * avoids duplicating string and filesystem helpers throughout the project.
 *
 * Reference: API Documentation, section 3.6.
 ******************************************************************************/

#include <string>

/**
 * @class Utility
 * @brief Stateless helper functions shared by all components.
 *
 * All members are static; the class is never instantiated. It exists as a
 * namespace-like grouping so that helpers are discoverable from a single
 * well-known header.
 */
class Utility
{
public:
    Utility() = delete;

    /**
     * @brief Removes leading and trailing whitespace.
     *
     * Whitespace is defined as space, tab, carriage return and newline.
     *
     * @param text Text to trim.
     * @return std::string Trimmed copy. Empty if @p text is all whitespace.
     */
    static std::string trim(const std::string &text);

    /**
     * @brief Converts text to upper case.
     *
     * @param text Text to convert.
     * @return std::string Upper-case copy.
     */
    static std::string toUpper(const std::string &text);

    /**
     * @brief Converts text to lower case.
     *
     * @param text Text to convert.
     * @return std::string Lower-case copy.
     */
    static std::string toLower(const std::string &text);

    /**
     * @brief Checks whether a readable file exists at @p path.
     *
     * @param path Path to test.
     * @return true if the file exists and can be opened for reading.
     */
    static bool fileExists(const std::string &path);

    /**
     * @brief Returns the current date and time as text.
     *
     * The format is <tt>YYYY-MM-DD HH:MM:SS</tt> in local time.
     *
     * @return std::string Formatted timestamp.
     */
    static std::string currentDateTime();

    /**
     * @brief Left-pads an integer with zeros to the requested width.
     *
     * @param value Value to render.
     * @param width Minimum number of characters.
     * @return std::string Zero-padded representation.
     */
    static std::string padZero(int value, int width = 2);

    /**
     * @brief Centres text inside a field of the given width.
     *
     * If @p text is longer than @p width the text is returned unchanged.
     *
     * @param text  Text to centre.
     * @param width Total field width.
     * @return std::string Text prefixed with the required padding.
     */
    static std::string center(const std::string &text, int width);

    /**
     * @brief Converts common textual boolean spellings to a bool.
     *
     * Recognises (case-insensitively) "true", "yes", "on", "enabled" and "1"
     * as true, and "false", "no", "off", "disabled" and "0" as false.
     *
     * @param text         Text to interpret.
     * @param defaultValue Returned when @p text matches nothing.
     * @return bool Interpreted value.
     */
    static bool toBool(const std::string &text, bool defaultValue = false);

    /**
     * @brief Converts text to an integer without throwing.
     *
     * @param text         Text to parse.
     * @param defaultValue Returned when @p text is not a valid integer.
     * @return int Parsed value, or @p defaultValue on failure.
     */
    static int toInt(const std::string &text, int defaultValue = 0);

    /**
     * @brief Reads an environment variable portably.
     *
     * MSVC deprecates @c std::getenv and warns (C4996) on every use, offering
     * @c _dupenv_s instead. Wrapping the difference here keeps that warning
     * out of the Windows build without defining @c _CRT_SECURE_NO_WARNINGS,
     * which would also silence the genuine cases the diagnostic exists for.
     *
     * Reports whether the variable is @e set rather than whether it has a
     * value, because the two differ: the NO_COLOR convention treats a
     * variable set to an empty string as switching colour off.
     *
     * @param name  Variable name.
     * @param value Receives the value; set to empty when the variable is not.
     * @return true if the variable is present in the environment.
     */
    static bool environment(const std::string &name, std::string &value);

    /**
     * @brief Reports whether an environment variable is set.
     * @param name Variable name.
     * @return true if the variable is present, whatever its value.
     */
    static bool hasEnvironment(const std::string &name);
};

#endif // UTILITY_HPP

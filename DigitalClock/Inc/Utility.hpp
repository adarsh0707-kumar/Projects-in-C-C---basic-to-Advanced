#pragma once

/******************************************************************************
 * @file Utility.hpp
 * @brief Collection of utility helper functions.
 * @author Adarsh Kumar
 * @date 2026
 *
 * This header provides commonly used utility functions for the
 * Digital Clock application. These inline functions simplify
 * string manipulation, file handling, timing operations, and
 * timestamp generation while avoiding the need for separate
 * implementation files.
 *
 * Features:
 *  - String trimming
 *  - String case conversion
 *  - Thread sleeping
 *  - File existence checking
 *  - Current timestamp generation
 ******************************************************************************/

#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cctype>

/**
 * @namespace Utility
 * @brief Contains reusable helper functions.
 *
 * The Utility namespace groups together frequently used helper
 * functions that support various modules throughout the application.
 */
namespace Utility
{

    /**
     * @brief Removes leading and trailing whitespace.
     *
     * Whitespace characters including spaces, tabs, carriage returns,
     * and newlines are removed from both ends of the string.
     *
     * @param str Input string.
     * @return std::string Trimmed string.
     */
    inline std::string trim(const std::string &str)
    {
        size_t first = str.find_first_not_of(" \t\n\r");

        if (first == std::string::npos)
            return "";

        size_t last = str.find_last_not_of(" \t\n\r");

        return str.substr(first, (last - first + 1));
    }

    /**
     * @brief Converts a string to uppercase.
     *
     * Every alphabetic character in the string is converted
     * to its uppercase equivalent.
     *
     * @param str Input string.
     * @return std::string Uppercase string.
     */
    inline std::string toUpper(const std::string &str)
    {
        std::string result = str;

        std::transform(result.begin(),
                       result.end(),
                       result.begin(),
                       ::toupper);

        return result;
    }

    /**
     * @brief Converts a string to lowercase.
     *
     * Every alphabetic character in the string is converted
     * to its lowercase equivalent.
     *
     * @param str Input string.
     * @return std::string Lowercase string.
     */
    inline std::string toLower(const std::string &str)
    {
        std::string result = str;

        std::transform(result.begin(),
                       result.end(),
                       result.begin(),
                       ::tolower);

        return result;
    }

    /**
     * @brief Suspends execution for a specified duration.
     *
     * Pauses the current thread for the given number of
     * milliseconds.
     *
     * @param milliseconds Sleep duration in milliseconds.
     */
    inline void sleep(int milliseconds)
    {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(milliseconds));
    }

    /**
     * @brief Checks whether a file exists.
     *
     * Attempts to open the specified file and determines
     * whether it is accessible.
     *
     * @param path Path to the file.
     * @return true if the file exists.
     * @return false otherwise.
     */
    inline bool fileExists(const std::string &path)
    {
        std::ifstream f(path);

        return f.good();
    }

    /**
     * @brief Returns the current local timestamp.
     *
     * The timestamp is formatted using the pattern:
     *
     * YYYY-MM-DD HH:MM:SS
     *
     * Example:
     * 2026-08-04 21:15:32
     *
     * @return std::string Current timestamp.
     */
    inline std::string currentTimestamp()
    {
        auto now = std::chrono::system_clock::now();

        std::time_t tt =
            std::chrono::system_clock::to_time_t(now);

        std::tm *tm =
            std::localtime(&tt);

        char buffer[80];

        strftime(buffer,
                 sizeof(buffer),
                 "%Y-%m-%d %H:%M:%S",
                 tm);

        return std::string(buffer);
    }

} // namespace Utility
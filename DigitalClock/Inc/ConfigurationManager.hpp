#ifndef CONFIGURATION_MANAGER_HPP
#define CONFIGURATION_MANAGER_HPP

/******************************************************************************
 * @file ConfigurationManager.hpp
 * @brief Declaration of the ConfigurationManager service.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The ConfigurationManager loads user settings from an INI-style file so that
 * application behaviour can be customised without recompiling (FR-006).
 * A missing or malformed file is never fatal: callers fall back to the
 * defaults supplied at each call site (TC-010, TC-011, TC-021).
 *
 * Reference: API Documentation, section 3.2.
 ******************************************************************************/

#include <climits>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @class ConfigurationManager
 * @brief Reads, queries and writes application settings.
 *
 * The supported file format is a small INI dialect:
 *
 * @code{.ini}
 * # comment
 * ; also a comment
 * [Section]
 * Key=Value
 * @endcode
 *
 * Keys are matched case-insensitively, so @c Theme, @c THEME and @c theme all
 * refer to the same setting. Keys inside a section are additionally reachable
 * as @c Section.Key.
 */
class ConfigurationManager
{
public:
    ConfigurationManager();

    /**
     * @brief Loads settings from a configuration file.
     *
     * Existing settings are discarded first. Blank lines, comments and lines
     * without a '=' separator are skipped rather than treated as errors, so a
     * partially corrupt file still yields every setting it does contain.
     *
     * @param filePath Path of the configuration file.
     * @return true if the file was opened and parsed, false if it could not be
     *         opened. On false the object is left empty and every getter
     *         returns its supplied default.
     */
    bool load(const std::string &filePath);

    /**
     * @brief Returns a setting as text.
     *
     * @param key Setting name (case-insensitive).
     * @return std::string The stored value, or an empty string when absent.
     */
    std::string getValue(const std::string &key) const;

    /**
     * @brief Returns a setting as text, with a fallback.
     *
     * @param key          Setting name (case-insensitive).
     * @param defaultValue Returned when the key is absent or empty.
     * @return std::string The stored value, or @p defaultValue.
     */
    std::string getValue(const std::string &key,
                         const std::string &defaultValue) const;

    /**
     * @brief Returns a setting as an integer.
     *
     * Values that are absent, non-numeric, or outside [@p minimum, @p maximum]
     * yield @p defaultValue, which satisfies the validation required by
     * TC-022.
     *
     * @param key          Setting name (case-insensitive).
     * @param defaultValue Returned when the value is missing or invalid.
     * @param minimum      Smallest accepted value.
     * @param maximum      Largest accepted value.
     * @return int Validated value.
     */
    int getInt(const std::string &key,
               int defaultValue = 0,
               int minimum = INT_MIN,
               int maximum = INT_MAX) const;

    /**
     * @brief Returns a setting as a boolean.
     *
     * Accepts true/yes/on/enabled/1 and false/no/off/disabled/0.
     *
     * @param key          Setting name (case-insensitive).
     * @param defaultValue Returned when the value is missing or unrecognised.
     * @return bool Interpreted value.
     */
    bool getBool(const std::string &key, bool defaultValue = false) const;

    /**
     * @brief Adds or replaces a setting.
     *
     * @param key   Setting name. Must not be empty.
     * @param value New value.
     * @return true if the setting was stored, false if @p key was empty.
     */
    bool setValue(const std::string &key, const std::string &value);

    /**
     * @brief Reports whether a setting is present.
     *
     * @param key Setting name (case-insensitive).
     * @return true if the key exists.
     */
    bool hasValue(const std::string &key) const;

    /**
     * @brief Writes the current settings back to the loaded file.
     *
     * Settings are written in the order they were first seen. Comments from
     * the original file are not preserved.
     *
     * @return true on success, false when no file has been loaded or the file
     *         could not be written.
     */
    bool save() const;

    /**
     * @brief Writes the current settings to an explicit path.
     *
     * @param filePath Destination path.
     * @return true on success.
     */
    bool saveAs(const std::string &filePath) const;

    /**
     * @brief Removes every loaded setting.
     */
    void clear();

    /**
     * @brief Returns the number of stored settings.
     * @return std::size_t Setting count.
     */
    std::size_t size() const;

    /**
     * @brief Returns the path passed to the last successful load().
     * @return std::string File path, or an empty string if never loaded.
     */
    const std::string &filePath() const;

    /**
     * @brief Lists loaded keys that are not in @p recognised.
     *
     * An unrecognised key is almost always a typo, and without this it is
     * indistinguishable from a setting deliberately left out: the value is
     * read by nobody and the default applies silently. @c Them=Light looks
     * exactly like a working configuration until the theme does not change.
     *
     * Comparison is case-insensitive, matching lookups. Keys are returned in
     * the order they appeared in the file, spelled as the file spelled them,
     * so a warning can quote what the user actually typed.
     *
     * @param recognised Keys the application knows how to use.
     * @return std::vector<std::string> Unrecognised keys, in file order.
     */
    std::vector<std::string> unknownKeys(
        const std::vector<std::string> &recognised) const;

private:
    /**
     * @brief Normalises a key for case-insensitive lookup.
     * @param key Key as written by the caller.
     * @return std::string Upper-case, trimmed key.
     */
    static std::string normalize(const std::string &key);

    /** Normalised key to value. */
    std::unordered_map<std::string, std::string> settings;

    /** Normalised key to the spelling used in the file, for save(). */
    std::unordered_map<std::string, std::string> originalKeys;

    /** Normalised keys in first-seen order, so save() is stable. */
    std::vector<std::string> insertionOrder;

    /** Path given to the last successful load(). */
    std::string loadedPath;
};

#endif // CONFIGURATION_MANAGER_HPP

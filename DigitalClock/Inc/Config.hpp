#ifndef CONFIG_HPP
#define CONFIG_HPP

/******************************************************************************
 * @file Config.hpp
 * @brief Declaration of the Config class.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Config class loads application settings from a configuration
 * file and provides methods to retrieve configuration values in
 * different formats.
 ******************************************************************************/

#include <string>
#include <unordered_map>

/**
 * @class Config
 * @brief Manages application configuration settings.
 *
 * Configuration values are stored as key-value pairs and can be
 * accessed as strings or integers.
 */
class Config
{
public:
    /**
     * @brief Constructs a Config object.
     */
    Config();

    /**
     * @brief Loads configuration values from a file.
     *
     * @param filename Configuration file path.
     * @return true if loading succeeds.
     * @return false otherwise.
     */
    bool load(const std::string &filename);

    /**
     * @brief Returns a string configuration value.
     *
     * @param key Configuration key.
     * @param defaultValue Value returned if the key is not found.
     * @return std::string Configuration value.
     */
    std::string getString(const std::string &key,
                          const std::string &defaultValue = "") const;

    /**
     * @brief Returns an integer configuration value.
     *
     * @param key Configuration key.
     * @param defaultValue Value returned if the key is not found.
     * @return int Configuration value.
     */
    int getInt(const std::string &key,
               int defaultValue = 0) const;

private:
    /**
     * @brief Stores configuration key-value pairs.
     */
    std::unordered_map<std::string, std::string> settings;
};

#endif // CONFIG_HPP
#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

/******************************************************************************
 * @file ResourceManager.hpp
 * @brief Declaration of the ResourceManager class.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The ResourceManager class provides a centralized interface for
 * locating and accessing application resources. It simplifies file
 * management by supplying standard resource paths and utility
 * functions for loading text-based files.
 *
 * Responsibilities:
 *  - Verify resource availability
 *  - Load text resources
 *  - Provide standard resource file paths
 *  - Centralize resource management
 ******************************************************************************/

#include <string>

/**
 * @class ResourceManager
 * @brief Manages application resource files.
 *
 * The ResourceManager class provides helper functions for checking
 * whether resource files exist, reading text resources, and returning
 * predefined paths for commonly used application resources such as
 * banners, configuration files, and log files.
 */
class ResourceManager
{
public:
    /**
     * @brief Constructs a ResourceManager object.
     *
     * No explicit initialization is required because this class
     * provides utility functions for resource management.
     */
    ResourceManager() = default;

    /**
     * @brief Checks whether a resource exists.
     *
     * Determines if the specified file or resource is available.
     *
     * @param path Path to the resource.
     * @return true if the resource exists.
     * @return false otherwise.
     */
    bool exists(const std::string &path) const;

    /**
     * @brief Loads a text resource.
     *
     * Reads the complete contents of a text file and returns
     * it as a string.
     *
     * @param path Path to the text resource.
     * @return std::string Contents of the file.
     */
    std::string loadText(const std::string &path) const;

    /**
     * @brief Returns the default banner resource path.
     *
     * @return std::string Banner file path.
     */
    std::string bannerPath() const;

    /**
     * @brief Returns the default configuration file path.
     *
     * @return std::string Configuration file path.
     */
    std::string configPath() const;

    /**
     * @brief Returns the default log file path.
     *
     * @return std::string Log file path.
     */
    std::string logPath() const;
};

#endif // RESOURCE_MANAGER_HPP
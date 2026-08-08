#ifndef VERSION_HPP
#define VERSION_HPP

/******************************************************************************
 * @file Version.hpp
 * @brief Application version information.
 * @author Adarsh Kumar
 * @date 2026
 *
 * This header defines the version information and metadata for the
 * Digital Clock application. The values declared here can be used
 * throughout the project to display version details, author
 * information, and application name in logs, startup banners,
 * and documentation.
 *
 * Version Format:
 * MAJOR.MINOR.PATCH
 *
 * Current Version:
 * 1.0.0
 ******************************************************************************/

#include <string>

/**
 * @namespace Version
 * @brief Contains application version and metadata.
 *
 * The Version namespace provides centralized constants describing
 * the application, including version numbers, application name,
 * author information, and a formatted version string.
 */
namespace Version
{
    /**
     * @brief Major version number.
     *
     * Incremented for significant releases containing
     * major architectural or functional changes.
     */
    inline constexpr int MAJOR = 1;

    /**
     * @brief Minor version number.
     *
     * Incremented when new features are introduced while
     * maintaining backward compatibility.
     */
    inline constexpr int MINOR = 4;

    /**
     * @brief Patch version number.
     *
     * Incremented for bug fixes, maintenance updates,
     * and minor improvements.
     */
    inline constexpr int PATCH = 0;

    /**
     * @brief Official application name.
     */
    inline const std::string NAME = "Digital Clock System";

    /**
     * @brief Application author.
     */
    inline const std::string AUTHOR = "Adarsh Kumar";

    /**
     * @brief Complete application version string.
     *
     * The version string is generated using the MAJOR,
     * MINOR, and PATCH version components.
     *
     * Format:
     * MAJOR.MINOR.PATCH
     *
     * Example:
     * 1.0.0
     */
    inline const std::string VERSION =
        std::to_string(MAJOR) + "." +
        std::to_string(MINOR) + "." +
        std::to_string(PATCH);
}

#endif // VERSION_HPP
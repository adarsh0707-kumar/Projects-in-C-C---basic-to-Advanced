#ifndef BANNER_HPP
#define BANNER_HPP

/******************************************************************************
 * @file Banner.hpp
 * @brief Declaration of the Banner class.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Banner module is responsible for loading ASCII art resources
 * used throughout the Digital Clock application. It separates the
 * presentation resources from the application logic, making it easy
 * to customize the user interface without modifying the source code.
 *
 * Resources:
 *  - Resources/logo.txt
 *  - Resources/banner.txt
 ******************************************************************************/

#include <string>

/**
 * @class Banner
 * @brief Loads ASCII art used by the application.
 *
 * The Banner class reads text-based resources from disk and returns
 * their contents as strings. These resources are displayed during
 * application startup and while refreshing the screen.
 */
class Banner
{
public:
    /**
     * @brief Constructs a Banner object.
     *
     * No initialization is required because the class only
     * loads text files when requested.
     */
    Banner() = default;

    /**
     * @brief Loads the startup logo.
     *
     * Reads the contents of the logo resource file.
     * If the file cannot be opened, a default logo is returned.
     *
     * @return std::string Complete logo text.
     */
    std::string loadLogo() const;

    /**
     * @brief Loads the application banner.
     *
     * Reads the banner displayed during screen refreshes.
     * If the file cannot be opened, a default banner is returned.
     *
     * @return std::string Complete banner text.
     */
    std::string loadBanner() const;

private:
    /**
     * @brief Reads a text file.
     *
     * Opens the specified file and returns its complete contents.
     *
     * @param filename Path of the file to read.
     * @return std::string Entire file contents.
     */
    std::string readFile(const std::string &filename) const;
};

#endif // BANNER_HPP
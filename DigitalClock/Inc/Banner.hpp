#ifndef BANNER_HPP
#define BANNER_HPP

/******************************************************************************
 * @file Banner.hpp
 * @brief Declaration of the Banner presentation component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Banner loads the ASCII art shown at the top of the screen. Keeping the
 * artwork in Resources rather than in the source lets it be changed without
 * recompiling. A missing banner file is not an error: the application falls
 * back to a built-in banner and carries on (TC-017).
 *
 * Reference: API Documentation, section 4.6.
 ******************************************************************************/

#include <string>
#include <vector>

#include "ResourceManager.hpp"

/**
 * @class Banner
 * @brief Loads and supplies the application banner artwork.
 */
class Banner
{
public:
    /**
     * @brief Constructs a banner holding the built-in default artwork.
     */
    Banner();

    /**
     * @brief Loads banner artwork from a resource file.
     *
     * When the file cannot be found the built-in banner is retained so the
     * screen still has a header.
     *
     * @param fileName Resource path, such as @c Resources/banner.txt.
     * @return true if the file was loaded, false if the default was kept.
     */
    bool load(const std::string &fileName);

    /**
     * @brief Makes the banner visible.
     */
    void show();

    /**
     * @brief Hides the banner without discarding it.
     *
     * Used when the terminal is too short to show the full layout.
     */
    void hide();

    /**
     * @brief Reports whether the banner should be drawn.
     * @return true when visible.
     */
    bool isVisible() const;

    /**
     * @brief Returns the banner as a single string.
     * @return std::string Banner text, or empty when hidden.
     */
    std::string content() const;

    /**
     * @brief Returns the banner split into individual lines.
     *
     * Trailing blank lines are removed so the layout spacing is controlled by
     * Screen rather than by the resource file.
     *
     * @return std::vector<std::string> Banner lines, empty when hidden.
     */
    std::vector<std::string> lines() const;

    /**
     * @brief Returns the number of lines the banner occupies.
     * @return std::size_t Line count, 0 when hidden.
     */
    std::size_t lineCount() const;

    /**
     * @brief Restores the built-in banner artwork.
     */
    void reset();

private:
    std::string text;          ///< Current banner artwork.
    bool visible;              ///< Whether the banner is drawn.
    ResourceManager resources; ///< Locates banner files.
};

#endif // BANNER_HPP

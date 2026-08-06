#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

/******************************************************************************
 * @file ResourceManager.hpp
 * @brief Declaration of the ResourceManager service.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The ResourceManager loads external text assets such as banners, logos and
 * theme definitions. A missing asset yields empty content rather than an
 * error, so the application degrades gracefully (TC-017, TC-023).
 *
 * Reference: API Documentation, section 3.5.
 ******************************************************************************/

#include <string>
#include <vector>

/**
 * @class ResourceManager
 * @brief Locates and reads text resources from disk.
 *
 * Relative paths are resolved against a list of search directories, which
 * lets the application find its assets whether it is launched from the
 * project root or from the Build directory.
 */
class ResourceManager
{
public:
    /**
     * @brief Constructs a manager with the default search paths.
     *
     * The defaults are the current directory, the parent directory and the
     * grandparent directory, covering launches from the project root, from
     * @c Build/ and from a nested build tree.
     */
    ResourceManager();

    /**
     * @brief Loads a resource into the internal buffer.
     *
     * @param fileName Absolute path, or a path relative to a search directory.
     * @return true if the resource was found and read. On false the buffer is
     *         cleared and getContent() returns an empty string.
     */
    bool load(const std::string &fileName);

    /**
     * @brief Returns the content of the most recent successful load().
     * @return std::string Resource text, or empty when nothing is loaded.
     */
    std::string getContent() const;

    /**
     * @brief Reports whether a resource can be found.
     *
     * @param fileName Absolute path, or a path relative to a search directory.
     * @return true if a readable file exists.
     */
    bool exists(const std::string &fileName) const;

    /**
     * @brief Resolves a resource name to a usable path.
     *
     * @param fileName Absolute path, or a path relative to a search directory.
     * @return std::string The first path that exists, or an empty string when
     *         the resource cannot be found anywhere.
     */
    std::string resolve(const std::string &fileName) const;

    /**
     * @brief Reads a resource without disturbing the internal buffer.
     *
     * @param fileName Absolute path, or a path relative to a search directory.
     * @return std::string File content, or empty when it cannot be read.
     */
    std::string read(const std::string &fileName) const;

    /**
     * @brief Adds a directory to the front of the search path.
     * @param directory Directory to search before the existing entries.
     */
    void addSearchPath(const std::string &directory);

    /**
     * @brief Discards the currently loaded content.
     */
    void clear();

    /**
     * @brief Returns the path that satisfied the last successful load().
     * @return std::string Resolved path, or empty when nothing is loaded.
     */
    const std::string &loadedPath() const;

private:
    std::vector<std::string> searchPaths; ///< Directories tried, in order.
    std::string content;                  ///< Buffer filled by load().
    std::string resolvedPath;             ///< Path that load() actually used.
};

#endif // RESOURCE_MANAGER_HPP

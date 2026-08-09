#ifndef PLUGIN_MANAGER_HPP
#define PLUGIN_MANAGER_HPP

/******************************************************************************
 * @file PluginManager.hpp
 * @brief Finds, loads and owns plugin modes.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Loading arbitrary code is the one thing here that can take the application
 * down without a bug of its own, so the rule throughout is that a bad plugin
 * is refused and logged, never trusted:
 *
 *   - the file must export the documented symbol, by name;
 *   - it must report the ABI this build was compiled against;
 *   - it must supply a name and a render function.
 *
 * A plugin failing any of those is skipped and the application carries on
 * without it. Nothing about the clock depends on a plugin existing.
 *
 * Reference: Change Log, KI-006.
 ******************************************************************************/

#include <cstddef>
#include <string>
#include <vector>

#include "Plugin.h"

/**
 * @class PluginManager
 * @brief Owns the loaded libraries and the plugins they supplied.
 */
class PluginManager
{
public:
    /** Most plugins that will be loaded, so a full directory cannot stall
     *  startup indefinitely. */
    static const std::size_t MAX_PLUGINS;

    PluginManager();

    /**
     * @brief Unloads everything still loaded.
     */
    ~PluginManager();

    PluginManager(const PluginManager &) = delete;
    PluginManager &operator=(const PluginManager &) = delete;

    /**
     * @brief Loads every plugin in a directory.
     *
     * A missing directory is not an error: it is the normal state for an
     * installation with no plugins.
     *
     * @param directory Directory to scan, such as @c Plugins.
     * @param host      Services to lend each plugin; must outlive this.
     * @return std::size_t Number of plugins successfully loaded.
     */
    std::size_t loadDirectory(const std::string &directory,
                              const DigitalClockHost &host);

    /**
     * @brief Loads a single plugin library.
     * @param path Shared library to load.
     * @param host Services to lend it.
     * @return true if it was loaded and accepted.
     */
    bool loadPlugin(const std::string &path, const DigitalClockHost &host);

    /**
     * @brief Number of plugins currently loaded.
     */
    std::size_t count() const;

    /**
     * @brief Returns a loaded plugin.
     * @param index Zero-based, less than count().
     * @return const DigitalClockPlugin* The plugin, or null if out of range.
     */
    const DigitalClockPlugin *at(std::size_t index) const;

    /**
     * @brief Returns a plugin's display name, or an empty string.
     */
    std::string nameAt(std::size_t index) const;

    /**
     * @brief Calls shutdown() on every plugin and closes the libraries.
     *
     * Safe to call more than once.
     */
    void unloadAll();

    /**
     * @brief Messages describing plugins that were refused, and why.
     *
     * Kept rather than only logged, so the reason survives a run with
     * logging switched off and can be asserted in a test.
     */
    const std::vector<std::string> &rejections() const;

    /**
     * @brief Reports whether this build can load plugins at all.
     *
     * False where the platform's dynamic loader is unavailable, in which
     * case loadDirectory() does nothing and says so.
     */
    static bool isSupported();

    /**
     * @brief The file extension shared libraries use on this platform.
     * @return std::string @c .so, @c .dylib or @c .dll.
     */
    static std::string librarySuffix();

private:
    struct Loaded
    {
        void *handle;                    ///< Platform library handle.
        const DigitalClockPlugin *plugin; ///< What it exported.
        std::string path;                ///< Where it came from.
    };

    std::vector<Loaded> loaded;
    std::vector<std::string> refused;
};

#endif // PLUGIN_MANAGER_HPP

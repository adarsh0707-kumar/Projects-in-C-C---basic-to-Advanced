#include "PluginManager.hpp"

#include <algorithm>
#include <filesystem>

#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace
{
    /**
     * @brief Opens a shared library, or returns null.
     */
    void *openLibrary(const std::string &path)
    {
#if defined(_WIN32)
        /*
        Without this, a LoadLibrary that fails can raise a modal error
        dialog -- "bad image", a missing dependency -- and on a machine
        with nobody to dismiss it the process simply stops. That is not
        hypothetical: the Windows CI job hung indefinitely on the test
        that deliberately tries to load a text file, while every other
        platform finished in seconds.

        The previous mode is restored, because this is process-wide and
        the application has no business changing it permanently.
        */
        const UINT previousMode =
            SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);

        void *handle = static_cast<void *>(LoadLibraryA(path.c_str()));

        SetErrorMode(previousMode);

        return handle;
#else
        /*
        RTLD_LOCAL so a plugin's symbols do not join the global namespace,
        where two plugins defining the same name would silently resolve to
        whichever loaded first.
        */
        return dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif
    }

    /**
     * @brief Looks up a symbol, or returns null.
     */
    void *findSymbol(void *handle, const char *name)
    {
#if defined(_WIN32)
        return reinterpret_cast<void *>(
            GetProcAddress(static_cast<HMODULE>(handle), name));
#else
        return dlsym(handle, name);
#endif
    }

    /**
     * @brief Closes a library.
     */
    void closeLibrary(void *handle)
    {
#if defined(_WIN32)
        FreeLibrary(static_cast<HMODULE>(handle));
#else
        dlclose(handle);
#endif
    }

    /**
     * @brief Describes why the last open or lookup failed.
     */
    std::string lastError()
    {
#if defined(_WIN32)
        return "the library could not be loaded";
#else
        const char *message = dlerror();

        return (message != nullptr) ? message : "unknown error";
#endif
    }
}

const std::size_t PluginManager::MAX_PLUGINS = 16;

PluginManager::PluginManager() = default;

PluginManager::~PluginManager()
{
    unloadAll();
}

bool PluginManager::isSupported()
{
#if defined(_WIN32) || defined(__unix__) || defined(__APPLE__)
    return true;
#else
    return false;
#endif
}

std::string PluginManager::librarySuffix()
{
#if defined(_WIN32)
    return ".dll";
#elif defined(__APPLE__)
    return ".dylib";
#else
    return ".so";
#endif
}

std::size_t PluginManager::loadDirectory(const std::string &directory,
                                         const DigitalClockHost &host)
{
    if (!isSupported())
    {
        refused.push_back(
            "plugins are not supported on this platform");

        return 0;
    }

    std::error_code code;

    if (!std::filesystem::is_directory(directory, code))
    {
        // Not an error: an installation with no plugins has no directory.
        return 0;
    }

    // Sorted, so the order plugins appear in the mode cycle is the same on
    // every machine rather than whatever the filesystem returns.
    std::vector<std::string> candidates;

    for (const auto &entry :
         std::filesystem::directory_iterator(directory, code))
    {
        if (!entry.is_regular_file(code))
            continue;

        if (entry.path().extension().string() != librarySuffix())
            continue;

        candidates.push_back(entry.path().string());
    }

    std::sort(candidates.begin(), candidates.end());

    std::size_t added = 0;

    for (const std::string &path : candidates)
    {
        if (loaded.size() >= MAX_PLUGINS)
        {
            refused.push_back(
                "too many plugins; '" + path + "' and any after it "
                "were not loaded");
            break;
        }

        if (loadPlugin(path, host))
            ++added;
    }

    return added;
}

bool PluginManager::loadPlugin(const std::string &path,
                               const DigitalClockHost &host)
{
    if (!isSupported())
        return false;

    void *handle = openLibrary(path);

    if (handle == nullptr)
    {
        refused.push_back("'" + path + "': " + lastError());
        return false;
    }

    void *symbol = findSymbol(handle, DIGITALCLOCK_PLUGIN_INIT_SYMBOL);

    if (symbol == nullptr)
    {
        /*
        A shared library that happens to be in the directory but is not a
        plugin lands here. Refusing by name is the point: the alternative is
        guessing at an entry point and calling something arbitrary.
        */
        refused.push_back(
            "'" + path + "': no " +
            std::string(DIGITALCLOCK_PLUGIN_INIT_SYMBOL) + " symbol");

        closeLibrary(handle);

        return false;
    }

    DigitalClockPluginInit initialise =
        reinterpret_cast<DigitalClockPluginInit>(symbol);

    const DigitalClockPlugin *plugin = initialise(&host);

    if (plugin == nullptr)
    {
        // A plugin may decline: an unmet requirement of its own is its
        // business, and saying no is better than half-working.
        refused.push_back("'" + path + "': declined to initialise");
        closeLibrary(handle);
        return false;
    }

    if (plugin->abiVersion != DIGITALCLOCK_PLUGIN_ABI)
    {
        refused.push_back(
            "'" + path + "': built for plugin ABI " +
            std::to_string(plugin->abiVersion) + ", this build expects " +
            std::to_string(DIGITALCLOCK_PLUGIN_ABI));

        closeLibrary(handle);

        return false;
    }

    if (plugin->name == nullptr || plugin->name[0] == '\0' ||
        plugin->render == nullptr)
    {
        refused.push_back(
            "'" + path + "': a plugin must supply a name and a render "
            "function");

        closeLibrary(handle);

        return false;
    }

    Loaded entry;
    entry.handle = handle;
    entry.plugin = plugin;
    entry.path = path;

    loaded.push_back(entry);

    return true;
}

std::size_t PluginManager::count() const
{
    return loaded.size();
}

const DigitalClockPlugin *PluginManager::at(std::size_t index) const
{
    if (index >= loaded.size())
        return nullptr;

    return loaded[index].plugin;
}

std::string PluginManager::nameAt(std::size_t index) const
{
    const DigitalClockPlugin *plugin = at(index);

    if (plugin == nullptr || plugin->name == nullptr)
        return "";

    std::string name(plugin->name);

    if (name.size() > DIGITALCLOCK_PLUGIN_NAME_MAX)
        name.resize(DIGITALCLOCK_PLUGIN_NAME_MAX);

    return name;
}

void PluginManager::unloadAll()
{
    /*
    Shut every plugin down before closing any library. A plugin's shutdown()
    lives in its own library, so closing as we go would be fine -- but a
    plugin that kept a pointer into another would not be, and the ordering
    costs nothing.
    */
    for (const Loaded &entry : loaded)
    {
        if (entry.plugin != nullptr && entry.plugin->shutdown != nullptr)
            entry.plugin->shutdown();
    }

    for (const Loaded &entry : loaded)
    {
        if (entry.handle != nullptr)
            closeLibrary(entry.handle);
    }

    loaded.clear();
}

const std::vector<std::string> &PluginManager::rejections() const
{
    return refused;
}

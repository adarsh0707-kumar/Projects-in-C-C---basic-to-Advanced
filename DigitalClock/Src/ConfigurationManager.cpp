#include "ConfigurationManager.hpp"

#include <filesystem>
#include <fstream>

#include "Utility.hpp"

ConfigurationManager::ConfigurationManager() = default;

std::string ConfigurationManager::normalize(const std::string &key)
{
    return Utility::toUpper(Utility::trim(key));
}

bool ConfigurationManager::load(const std::string &filePath)
{
    clear();

    std::ifstream file(filePath);

    if (!file)
        return false;

    loadedPath = filePath;

    std::string section;
    std::string line;

    while (std::getline(file, line))
    {
        line = Utility::trim(line);

        if (line.empty())
            continue;

        if (line[0] == '#' || line[0] == ';')
            continue;

        // [Section] header.
        if (line.front() == '[' && line.back() == ']')
        {
            section = Utility::trim(line.substr(1, line.size() - 2));
            continue;
        }

        const std::size_t separator = line.find('=');

        // A line without '=' is malformed; skip it rather than fail the load.
        if (separator == std::string::npos)
            continue;

        const std::string key = Utility::trim(line.substr(0, separator));
        const std::string value = Utility::trim(line.substr(separator + 1));

        if (key.empty())
            continue;

        setValue(key, value);

        // Sectioned keys are also reachable as "Section.Key".
        if (!section.empty())
            setValue(section + "." + key, value);
    }

    return true;
}

std::string ConfigurationManager::getValue(const std::string &key) const
{
    const auto entry = settings.find(normalize(key));

    if (entry == settings.end())
        return "";

    return entry->second;
}

std::string ConfigurationManager::getValue(
    const std::string &key,
    const std::string &defaultValue) const
{
    const auto entry = settings.find(normalize(key));

    if (entry == settings.end() || entry->second.empty())
        return defaultValue;

    return entry->second;
}

int ConfigurationManager::getInt(const std::string &key,
                                 int defaultValue,
                                 int minimum,
                                 int maximum) const
{
    const auto entry = settings.find(normalize(key));

    if (entry == settings.end())
        return defaultValue;

    const int parsed = Utility::toInt(entry->second, defaultValue);

    // Out-of-range values are rejected in favour of the default (TC-022).
    if (parsed < minimum || parsed > maximum)
        return defaultValue;

    return parsed;
}

bool ConfigurationManager::getBool(const std::string &key,
                                   bool defaultValue) const
{
    const auto entry = settings.find(normalize(key));

    if (entry == settings.end())
        return defaultValue;

    return Utility::toBool(entry->second, defaultValue);
}

bool ConfigurationManager::setValue(const std::string &key,
                                    const std::string &value)
{
    const std::string normalized = normalize(key);

    if (normalized.empty())
        return false;

    if (settings.find(normalized) == settings.end())
    {
        insertionOrder.push_back(normalized);
        originalKeys[normalized] = Utility::trim(key);
    }

    settings[normalized] = value;

    return true;
}

bool ConfigurationManager::hasValue(const std::string &key) const
{
    return settings.find(normalize(key)) != settings.end();
}

bool ConfigurationManager::save() const
{
    if (loadedPath.empty())
        return false;

    return saveAs(loadedPath);
}

bool ConfigurationManager::saveAs(const std::string &filePath) const
{
    if (filePath.empty())
        return false;

    const std::filesystem::path path(filePath);

    if (path.has_parent_path())
    {
        std::error_code code;
        std::filesystem::create_directories(path.parent_path(), code);
    }

    std::ofstream file(filePath, std::ios::trunc);

    if (!file)
        return false;

    file << "# Digital Clock Configuration\n"
         << "# Written by ConfigurationManager on "
         << Utility::currentDateTime()
         << "\n\n";

    for (const std::string &normalized : insertionOrder)
    {
        // Composite "Section.Key" aliases are derived on load, not stored
        // settings in their own right, so they are not written back.
        if (normalized.find('.') != std::string::npos)
            continue;

        const auto value = settings.find(normalized);

        if (value == settings.end())
            continue;

        const auto original = originalKeys.find(normalized);

        const std::string name =
            (original != originalKeys.end()) ? original->second : normalized;

        file << name << "=" << value->second << '\n';
    }

    return file.good();
}

void ConfigurationManager::clear()
{
    settings.clear();
    originalKeys.clear();
    insertionOrder.clear();
    loadedPath.clear();
}

std::size_t ConfigurationManager::size() const
{
    return settings.size();
}

const std::string &ConfigurationManager::filePath() const
{
    return loadedPath;
}

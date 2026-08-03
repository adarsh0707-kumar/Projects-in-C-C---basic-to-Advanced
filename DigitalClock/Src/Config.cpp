#include "Config.hpp"

#include <fstream>
#include <sstream>

Config::Config() = default;

bool Config::load(const std::string &filename)
{
    settings.clear();

    std::ifstream file(filename);

    if (!file)
        return false;

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        if (line[0] == '#')
            continue;

        std::size_t pos = line.find('=');

        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        settings[key] = value;
    }

    return true;
}

std::string Config::getString(
    const std::string &key,
    const std::string &defaultValue) const
{
    auto it = settings.find(key);

    if (it == settings.end())
        return defaultValue;

    return it->second;
}

int Config::getInt(
    const std::string &key,
    int defaultValue) const
{
    auto it = settings.find(key);

    if (it == settings.end())
        return defaultValue;

    try
    {
        return std::stoi(it->second);
    }
    catch (...)
    {
        return defaultValue;
    }
}
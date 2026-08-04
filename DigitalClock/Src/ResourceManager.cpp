#include "ResourceManager.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>

bool ResourceManager::exists(const std::string &path) const
{
    return std::filesystem::exists(path);
}

std::string ResourceManager::loadText(const std::string &path) const
{
    std::ifstream file(path);

    if (!file)
        return "";

    std::ostringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}

std::string ResourceManager::bannerPath() const
{
    return "Resources/banner.txt";
}

std::string ResourceManager::configPath() const
{
    return "Resources/config.ini";
}

std::string ResourceManager::logPath() const
{
    return "Logs/application.log";
}
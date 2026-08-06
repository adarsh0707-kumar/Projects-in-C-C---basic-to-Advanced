#include "ResourceManager.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>

ResourceManager::ResourceManager()
    : searchPaths{".", "..", "../.."}
{
}

std::string ResourceManager::resolve(const std::string &fileName) const
{
    if (fileName.empty())
        return "";

    const std::filesystem::path requested(fileName);

    if (requested.is_absolute())
    {
        std::error_code code;

        if (std::filesystem::is_regular_file(requested, code))
            return fileName;

        return "";
    }

    for (const std::string &base : searchPaths)
    {
        const std::filesystem::path candidate =
            std::filesystem::path(base) / requested;

        std::error_code code;

        if (std::filesystem::is_regular_file(candidate, code))
            return candidate.string();
    }

    return "";
}

bool ResourceManager::exists(const std::string &fileName) const
{
    return !resolve(fileName).empty();
}

std::string ResourceManager::read(const std::string &fileName) const
{
    const std::string path = resolve(fileName);

    if (path.empty())
        return "";

    std::ifstream file(path);

    if (!file)
        return "";

    std::ostringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

bool ResourceManager::load(const std::string &fileName)
{
    clear();

    const std::string path = resolve(fileName);

    if (path.empty())
        return false;

    std::ifstream file(path);

    if (!file)
        return false;

    std::ostringstream buffer;
    buffer << file.rdbuf();

    content = buffer.str();
    resolvedPath = path;

    return true;
}

std::string ResourceManager::getContent() const
{
    return content;
}

void ResourceManager::addSearchPath(const std::string &directory)
{
    if (directory.empty())
        return;

    searchPaths.insert(searchPaths.begin(), directory);
}

void ResourceManager::clear()
{
    content.clear();
    resolvedPath.clear();
}

const std::string &ResourceManager::loadedPath() const
{
    return resolvedPath;
}

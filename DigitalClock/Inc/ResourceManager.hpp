#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <string>

class ResourceManager
{
public:
    ResourceManager() = default;

    bool exists(const std::string &path) const;

    std::string loadText(const std::string &path) const;

    std::string bannerPath() const;

    std::string configPath() const;

    std::string logPath() const;
};

#endif // RESOURCE_MANAGER_HPP
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <unordered_map>

class Config
{
public:
    Config();

    bool load(const std::string &filename);

    std::string getString(const std::string &key,
                          const std::string &defaultValue = "") const;

    int getInt(const std::string &key,
               int defaultValue = 0) const;

private:
    std::unordered_map<std::string, std::string> settings;
};

#endif // CONFIG_HPP
#pragma once

#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include "Utility.hpp"

class Config
{
private:
    std::map<std::string, std::string> settings;

    void trimPair(std::string &key, std::string &value)
    {
        key = Utility::trim(key);
        value = Utility::trim(value);
    }

public:
    bool load(const std::string &filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
            return false;

        std::string line;
        while (std::getline(file, line))
        {
            line = Utility::trim(line);
            if (line.empty() || line[0] == '#' || line[0] == ';')
                continue;

            size_t eqPos = line.find('=');
            if (eqPos == std::string::npos)
                continue;

            std::string key = line.substr(0, eqPos);
            std::string value = line.substr(eqPos + 1);
            trimPair(key, value);
            settings[key] = value;
        }
        return true;
    }

    std::string getValue(const std::string &key, const std::string &defaultValue = "") const
    {
        auto it = settings.find(key);
        if (it != settings.end())
            return it->second;
        return defaultValue;
    }

    int getInt(const std::string &key, int defaultValue = 0) const
    {
        std::string val = getValue(key);
        if (val.empty())
            return defaultValue;
        try
        {
            return std::stoi(val);
        }
        catch (...)
        {
            return defaultValue;
        }
    }

    bool getBool(const std::string &key, bool defaultValue = false) const
    {
        std::string val = Utility::toLower(getValue(key));
        if (val == "true" || val == "1" || val == "yes" || val == "enabled")
            return true;
        if (val == "false" || val == "0" || val == "no" || val == "disabled")
            return false;
        return defaultValue;
    }
};
#pragma once

#include <string>
#include <map>
#include <iostream>
#include "Utility.hpp"

class Theme
{
private:
    std::map<std::string, std::string> colors;

public:
    bool load(const std::string &themeName, const std::string& /*themesFilePath*/)
    {
        // For simplicity, we mock the color parsing. In a real app, we'd parse themes.ini.
        // Hardcoded themes for demonstration.
        if (Utility::toLower(themeName) == "dark")
        {
            colors = {
                {"Foreground", "White"}, {"Background", "Black"}, {"Header", "Cyan"}, {"Clock", "Green"}, {"Date", "Yellow"}, {"Status", "White"}, {"Error", "Red"}};
        }
        else if (Utility::toLower(themeName) == "light")
        {
            colors = {
                {"Foreground", "Black"}, {"Background", "White"}, {"Header", "Blue"}, {"Clock", "Red"}, {"Date", "DarkBlue"}, {"Status", "DarkGray"}, {"Error", "Red"}};
        }
        else
        {
            return false; // Theme not found
        }
        return true;
    }

    std::string getColor(const std::string &key) const
    {
        auto it = colors.find(key);
        if (it != colors.end())
            return it->second;
        return "Default";
    }
};
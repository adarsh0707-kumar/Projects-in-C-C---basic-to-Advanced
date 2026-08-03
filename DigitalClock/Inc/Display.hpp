#pragma once

#include <string>
#include "Console.hpp"
#include "TimeFormatter.hpp"
#include "Clock.hpp"
#include "Date.hpp"
#include "Theme.hpp"
#include "Config.hpp"

class Display
{
private:
    Console console;
    TimeFormatter formatter;
    Theme theme;
    Config &config;

public:
    Display(Config &cfg) : config(cfg) {}

    void initialize(const std::string &themeName)
    {
        console.initialize();
        if (!theme.load(themeName, "Config/themes.ini"))
        {
            theme.load("Dark", "Config/themes.ini"); // Fallback
        }
        setColor("Header");
        console.clear();
        console.setColor("Default");
    }

    void setColor(const std::string &element)
    {
        console.setColor(theme.getColor(element));
    }

    void resetColor()
    {
        console.setColor("Default");
    }

    void render(const Clock &clock, const Date &date)
    {
        console.clear();

        // Header
        setColor("Header");
        console.printCentered("=========================================");
        console.printCentered("          DIGITAL CLOCK SYSTEM          ");
        console.printCentered("=========================================");
        resetColor();

        console.print("\n\n");

        // Clock
        setColor("Clock");
        std::string timeStr = formatter.formatTime(clock, config.getBool("TimeFormat", true));
        console.printCentered("             " + timeStr);
        resetColor();

        console.print("\n\n");

        // Date
        setColor("Date");
        std::string dateStr = formatter.formatDate(date, config.getValue("DateFormat", "LONG"));
        console.printCentered("             " + dateStr);
        resetColor();

        console.print("\n\n");

        // Status
        setColor("Status");
        console.printCentered("-----------------------------------------");
        console.printCentered(" Theme : " + config.getValue("Theme", "Dark"));
        console.printCentered(" Status: Running");
        console.printCentered(" Refresh: " + std::to_string(config.getInt("RefreshInterval", 1000)) + " ms");
        console.printCentered("-----------------------------------------");
        resetColor();

        console.print("\n Press 'q' or Ctrl+C to exit.\n");
    }

    void setMessage(const std::string &text)
    {
        console.clear();
        setColor("Status");
        console.printCentered(text);
        resetColor();
    }
};
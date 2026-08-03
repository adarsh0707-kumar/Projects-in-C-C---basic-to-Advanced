#include <iostream>
#include <string>
#include <csignal>
#include <atomic>

#include "Clock.hpp"
#include "Date.hpp"
#include "TimeFormatter.hpp"
#include "Display.hpp"
#include "Config.hpp"
#include "Logger.hpp"
#include "Console.hpp"
#include "Utility.hpp"

std::atomic<bool> running{true};

void signalHandler(int)
{
    running = false;
}

int main()
{
    // Setup graceful shutdown
    signal(SIGINT, signalHandler);

    // Initialize Config
    Config config;
    if (!config.load("Config/config.ini"))
    {
        std::cerr << "WARNING: config.ini not found. Using defaults." << std::endl;
    }

    // Initialize Logger
    Logger logger;
    if (config.getBool("Logging", true))
    {
        logger.initialize("Logs/application.log");
    }
    logger.info("Digital Clock System starting...");

    // Initialize Core Modules
    Clock clock;
    Date date;
    Display display(config);

    // Load Theme
    std::string themeName = config.getValue("Theme", "Dark");
    display.initialize(themeName);
    logger.info("Theme loaded: " + themeName);

    int refreshRate = config.getInt("RefreshInterval", 1000);
    if (refreshRate < 100)
        refreshRate = 1000; // Sanity check

    logger.info("Refresh Rate: " + std::to_string(refreshRate) + " ms");

    // Main Loop
    display.setMessage("Digital Clock System Initialized. Starting...");
    Utility::sleep(1000); // Brief pause to show initialization message

    while (running)
    {
        // Update data
        if (!clock.update())
        {
            logger.warning("Failed to update Clock.");
        }
        if (!date.update())
        {
            logger.warning("Failed to update Date.");
        }

        // Render
        display.render(clock, date);

        // Wait
        Utility::sleep(refreshRate);
    }

    // Shutdown
    logger.info("Shutdown requested. Cleaning up...");
    display.setMessage("Shutting down... Goodbye.");
    Utility::sleep(500);
    Console().showCursor();

    return 0;
}
#include <chrono>
#include <iostream>
#include <thread>

#include "Clock.hpp"
#include "Config.hpp"
#include "Date.hpp"
#include "Display.hpp"
#include "Logger.hpp"
#include "Version.hpp"

int main()
{
    Logger logger;

    if (!logger.open())
    {
        std::cerr << "Unable to open log file.\n";
        return 1;
    }

    logger.log(Logger::Level::INFO,
               "Logger initialized successfully.");

    Config config;

    if (!config.load("Resources/config.ini"))
    {
        logger.log(Logger::Level::ERROR,
                   "Failed to load configuration.");

        std::cerr << "Failed to load configuration.\n";
        return 1;
    }

    logger.log(
        Logger::Level::INFO,
        std::string(Version::NAME) +
            " v" +
            Version::VERSION +
            " starting.");

    logger.log(Logger::Level::INFO,
               "Configuration loaded successfully.");

    logger.log(
        Logger::Level::INFO,
        "Theme: " +
            config.getString("THEME", "Default"));

    logger.log(
        Logger::Level::INFO,
        "Refresh Interval: " +
            std::to_string(
                config.getInt("REFRESH_INTERVAL", 1000)) +
            " ms");

    Clock clock;
    Date date;
    Display display;

    while (true)
    {
        clock.update();
        date.update();

        display.render(clock, date);

        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                config.getInt("REFRESH_INTERVAL", 1000)));
    }

    // This code is currently unreachable.
    logger.log(Logger::Level::INFO,
               "Shutdown requested.");

    logger.log(Logger::Level::INFO,
               "Releasing resources.");

    logger.log(Logger::Level::INFO,
               "Logger shutdown complete.");

    return 0;
}
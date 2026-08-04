#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

#include "Clock.hpp"
#include "Config.hpp"
#include "Date.hpp"
#include "Display.hpp"
#include "Logger.hpp"
#include "Version.hpp"

std::atomic<bool> running(true);

void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        running = false;
    }
}

int main()
{
    std::signal(SIGINT, signalHandler);

    Logger logger;
    Screen screen;

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

    while (running)
    {
        clock.update();
        date.update();

        display.render(clock, date);

        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                config.getInt("REFRESH_INTERVAL", 1000)));
    }

    screen.showCursor();

    logger.log(Logger::Level::INFO,
               "Shutdown requested.");

    logger.log(Logger::Level::INFO,
               "Releasing resources.");

    logger.log(Logger::Level::INFO,
               "Logger shutdown complete.");

    return 0;
}
/******************************************************************************
 * @file main.cpp
 * @brief Entry point for the Digital Clock application.
 * @author Adarsh Kumar
 * @date 2026
 *
 * This file initializes the application components, loads configuration,
 * configures the logging system, creates the clock display, and runs the
 * main update loop until the application receives a termination signal.
 ******************************************************************************/

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

/**
 * @brief Indicates whether the application should continue running.
 *
 * This atomic flag is modified by the signal handler when the user
 * presses Ctrl+C (SIGINT). The main loop continuously checks this flag.
 */
std::atomic<bool> running(true);

/**
 * @brief Handles operating system signals.
 *
 * Currently only the SIGINT signal (Ctrl+C) is processed.
 * Receiving SIGINT causes the main application loop to terminate
 * gracefully.
 *
 * @param signal Signal number received from the operating system.
 */
void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        running = false;
    }
}

/**
 * @brief Application entry point.
 *
 * Initializes all application modules including:
 * - Signal handler
 * - Screen management
 * - Logger
 * - Configuration
 * - Clock
 * - Date
 * - Display
 *
 * After successful initialization, the application continuously
 * updates and renders the digital clock until a shutdown signal
 * is received.
 *
 * @return int
 * @retval 0 Application exited successfully.
 * @retval 1 Initialization failed.
 */
int main()
{
    /**
     * Register Ctrl+C (SIGINT) handler for graceful shutdown.
     */
    std::signal(SIGINT, signalHandler);

    /**
     * Creates the terminal screen manager.
     * The constructor usually hides the cursor and prepares
     * the console for rendering.
     */
    Screen screen;

    /*
    ------------------------------------------------------------------------
    Initialize Logger
    ------------------------------------------------------------------------
    */

    Logger logger;

    /**
     * NOTE:
     * The 'config' object is used below before it is created.
     * This should be corrected by constructing Config before
     * accessing any configuration values.
     */
    if (!logger.open(
            config.getString(
                "LOG_FILE",
                "Logs/application.log")))
    {
        std::cerr
            << "Unable to open log file.\n";

        return 1;
    }

    /*
    ------------------------------------------------------------------------
    Configure Logger
    ------------------------------------------------------------------------
    */

    /**
     * Enable or disable console logging based on the configuration.
     */
    logger.setConsoleOutput(

        config.getString(
            "CONSOLE_LOG",
            "false") == "true"

    );

    /**
     * Read the minimum log level from the configuration.
     */
    std::string level =
        config.getString(
            "LOG_LEVEL",
            "INFO");

    /**
     * Configure the logger's minimum severity level.
     */
    if (level == "DEBUG")
    {
        logger.setMinimumLevel(
            Logger::Level::DEBUG);
    }
    else if (level == "WARNING")
    {
        logger.setMinimumLevel(
            Logger::Level::WARNING);
    }
    else if (level == "ERROR")
    {
        logger.setMinimumLevel(
            Logger::Level::ERROR);
    }
    else
    {
        logger.setMinimumLevel(
            Logger::Level::INFO);
    }

    /**
     * Record successful logger initialization.
     */
    logger.log(
        Logger::Level::INFO,
        "Logger initialized successfully.");

    /*
    ------------------------------------------------------------------------
    Load Configuration
    ------------------------------------------------------------------------
    */

    /**
     * Configuration manager responsible for reading values
     * from the configuration file.
     */
    Config config;

    /**
     * Load application configuration.
     */
    if (!config.load("Resources/config.ini"))
    {
        std::cerr
            << "Failed to load configuration.\n";

        return 1;
    }

    /**
     * Log application startup information.
     */
    logger.log(
        Logger::Level::INFO,
        std::string(Version::NAME) +
            " v" +
            Version::VERSION +
            " starting.");

    logger.log(
        Logger::Level::INFO,
        "Configuration loaded successfully.");

    /**
     * Log selected application theme.
     */
    logger.log(
        Logger::Level::INFO,
        "Theme: " +
            config.getString("THEME", "Default"));

    /**
     * Log configured refresh interval.
     */
    logger.log(
        Logger::Level::INFO,
        "Refresh Interval: " +
            std::to_string(
                config.getInt(
                    "REFRESH_INTERVAL",
                    1000)) +
            " ms");

    /**
     * Create the application modules.
     */
    Clock clock;
    Date date;
    Display display;

    /**
     * Main application loop.
     *
     * The loop continues until the running flag becomes false.
     * Each iteration performs the following steps:
     * 1. Update the clock.
     * 2. Update the date.
     * 3. Render the display.
     * 4. Sleep for the configured refresh interval.
     */
    while (running)
    {
        /**
         * Update current system time.
         */
        clock.update();

        /**
         * Update current system date.
         */
        date.update();

        /**
         * Render the latest clock and date information.
         */
        display.render(clock, date);

        /**
         * Delay before refreshing the display again.
         */
        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                config.getInt(
                    "REFRESH_INTERVAL",
                    1000)));
    }

    /**
     * Restore terminal cursor visibility before exiting.
     */
    screen.showCursor();

    /**
     * Record graceful shutdown.
     */
    logger.log(
        Logger::Level::INFO,
        "Shutdown requested.");

    logger.log(
        Logger::Level::INFO,
        "Releasing resources.");

    logger.log(
        Logger::Level::INFO,
        "Logger shutdown complete.");

    return 0;
}
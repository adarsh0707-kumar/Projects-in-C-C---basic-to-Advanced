#ifndef LOGGER_HPP
#define LOGGER_HPP

/******************************************************************************
 * @file Logger.hpp
 * @brief Declaration of the Logger class.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Logger module provides a centralized logging system for the
 * Digital Clock application. It supports formatted log messages,
 * multiple severity levels, optional console output, and thread-safe
 * logging operations.
 *
 * Responsibilities:
 *  - Create and manage log files
 *  - Write formatted log entries
 *  - Generate timestamps
 *  - Support multiple logging levels
 *  - Provide thread-safe logging
 *  - Enable optional console output
 *  - Maintain logging sessions
 ******************************************************************************/

#include <fstream>
#include <mutex>
#include <string>

/**
 * @class Logger
 * @brief Provides logging services for the application.
 *
 * The Logger class records application events into a log file
 * while optionally displaying them on the console. It supports
 * multiple log levels and ensures thread-safe access using a mutex.
 */
class Logger
{
public:
    /**
     * @enum Level
     * @brief Defines the severity level of log messages.
     */
    enum class Level
    {
        DEBUG,   /**< Detailed debugging information. */
        INFO,    /**< General application information. */
        WARNING, /**< Warning messages for unexpected situations. */
        ERROR    /**< Error messages indicating failures. */
    };

    /**
     * @brief Constructs a Logger object.
     *
     * Initializes the logger with default settings.
     */
    Logger();

    /**
     * @brief Destroys the Logger object.
     *
     * Ensures that the log file is properly closed and
     * any allocated resources are released.
     */
    ~Logger();

    /**
     * @brief Opens the log file.
     *
     * Creates or opens the specified log file for writing.
     *
     * @param filename Path to the log file.
     * @return true if the file was opened successfully.
     * @return false if the file could not be opened.
     */
    bool open(const std::string &filename);

    /**
     * @brief Closes the log file.
     *
     * Safely closes the currently opened log file.
     */
    void close();

    /**
     * @brief Writes a formatted log entry.
     *
     * Records a log message with the specified severity level.
     * The message is written to the log file and optionally
     * displayed on the console.
     *
     * @param level Severity level of the log message.
     * @param message Message to be logged.
     */
    void log(Level level,
             const std::string &message);

    /**
     * @brief Enables or disables console logging.
     *
     * @param enabled true to display log messages on the console;
     *                false to disable console output.
     */
    void setConsoleOutput(bool enabled);

    /**
     * @brief Sets the minimum logging level.
     *
     * Messages below the specified level are ignored.
     *
     * @param level Minimum severity level to record.
     */
    void setMinimumLevel(Level level);

private:
    /**
     * @brief Output file stream used for logging.
     */
    std::ofstream file;

    /**
     * @brief Mutex used to ensure thread-safe logging.
     */
    std::mutex mutex;

    /**
     * @brief Indicates whether console output is enabled.
     */
    bool consoleOutput;

    /**
     * @brief Minimum severity level required for logging.
     */
    Level minimumLevel;

    /**
     * @brief Generates the current timestamp.
     *
     * The timestamp is formatted as:
     * YYYY-MM-DD HH:MM:SS
     *
     * @return Formatted timestamp string.
     */
    std::string timestamp() const;

    /**
     * @brief Converts a log level into a readable string.
     *
     * @param level Logging level.
     * @return String representation of the logging level.
     */
    std::string levelToString(Level level) const;

    /**
     * @brief Determines whether a message should be logged.
     *
     * Compares the message severity against the configured
     * minimum logging level.
     *
     * @param level Severity level of the message.
     * @return true if the message should be logged.
     * @return false otherwise.
     */
    bool shouldLog(Level level) const;

    /**
     * @brief Writes a new logging session header.
     *
     * Adds a separator and session information at the
     * beginning of a new logging session.
     */
    void writeSessionHeader();
};

#endif // LOGGER_HPP
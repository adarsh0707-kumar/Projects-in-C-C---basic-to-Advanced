#ifndef LOGGER_HPP
#define LOGGER_HPP

/******************************************************************************
 * @file Logger.hpp
 * @brief Declaration of the Logger service.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Logger records application events, warnings and errors so that runtime
 * behaviour can be traced after the fact. Logging is a support service: when
 * the log file cannot be opened the application continues to run with logging
 * silently disabled (see NFR-009 and TC-024).
 *
 * Reference: API Documentation, section 3.4.
 ******************************************************************************/

#include <fstream>
#include <mutex>
#include <string>

/**
 * @class Logger
 * @brief Thread-safe, file-backed event log with severity filtering.
 *
 * Entries are written as:
 * @code
 * [YYYY-MM-DD HH:MM:SS] [LEVEL] message
 * @endcode
 *
 * Every entry is flushed immediately so that a crash cannot lose the tail of
 * the log.
 */
class Logger
{
public:
    /**
     * @enum Level
     * @brief Severity of a log entry, ordered from least to most severe.
     */
    enum class Level
    {
        DEBUG = 0,  ///< Diagnostic detail useful only while developing.
        INFO = 1,   ///< Normal lifecycle events.
        WARNING = 2, ///< Recoverable problems; the application continues.
        ERROR = 3   ///< Failures that prevented an operation from completing.
    };

    Logger();

    /**
     * @brief Closes the log file if it is still open.
     */
    ~Logger();

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    /**
     * @brief Opens the log file and writes a session header.
     *
     * The file is opened in append mode so that history is preserved across
     * runs. Any parent directory named in @p fileName is created first.
     *
     * @param fileName Path of the log file.
     * @return true if logging is available, false if the file could not be
     *         opened. A false return is not fatal: the caller should continue
     *         and simply run without logging.
     */
    bool initialize(const std::string &fileName);

    /**
     * @brief Records an informational event.
     * @param message Text to record.
     */
    void info(const std::string &message);

    /**
     * @brief Records a recoverable problem.
     * @param message Text to record.
     */
    void warning(const std::string &message);

    /**
     * @brief Records a failure.
     * @param message Text to record.
     */
    void error(const std::string &message);

    /**
     * @brief Records developer-level diagnostic detail.
     * @param message Text to record.
     */
    void debug(const std::string &message);

    /**
     * @brief Records a message at an explicit severity.
     *
     * @param level   Severity of the entry.
     * @param message Text to record.
     */
    void log(Level level, const std::string &message);

    /**
     * @brief Flushes and closes the log file.
     *
     * Safe to call more than once and safe to call when the log was never
     * opened.
     */
    void close();

    /**
     * @brief Mirrors log entries to standard output.
     *
     * Disabled by default because console output would corrupt the clock
     * rendering.
     *
     * @param enabled true to echo entries to the console.
     */
    void setConsoleOutput(bool enabled);

    /**
     * @brief Discards entries below the given severity.
     * @param level Lowest severity that will be recorded.
     */
    void setMinimumLevel(Level level);

    /**
     * @brief Reports whether the log file is currently open.
     * @return true when entries are being written to disk.
     */
    bool isOpen() const;

    /**
     * @brief Converts a severity to its textual name.
     * @param level Severity to convert.
     * @return std::string One of DEBUG, INFO, WARNING or ERROR.
     */
    static std::string levelToString(Level level);

    /**
     * @brief Converts a severity name to a Level.
     *
     * Matching is case-insensitive; unrecognised names yield @p defaultLevel.
     *
     * @param text         Severity name.
     * @param defaultLevel Returned when @p text is not recognised.
     * @return Level Parsed severity.
     */
    static Level levelFromString(const std::string &text,
                                 Level defaultLevel = Level::INFO);

private:
    /**
     * @brief Writes the banner that separates one run from the previous one.
     */
    void writeSessionHeader();

    std::ofstream file;   ///< Destination log file.
    mutable std::mutex mutex; ///< Serialises writes from multiple threads.
    bool consoleOutput;   ///< Whether entries are echoed to stdout.
    Level minimumLevel;   ///< Lowest severity that is recorded.
};

#endif // LOGGER_HPP

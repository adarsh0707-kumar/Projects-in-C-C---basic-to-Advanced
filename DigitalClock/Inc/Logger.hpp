#ifndef LOGGER_HPP
#define LOGGER_HPP

/*
============================================================
Logger Module
============================================================

Author
------
Adarsh Kumar

Purpose
-------
Provides a centralized logging system for the Digital Clock
application.

Responsibilities
----------------
✓ Create and open log files
✓ Write formatted log entries
✓ Generate timestamps
✓ Support multiple log levels
✓ Thread-safe logging
✓ Optional console output
✓ Session management

Features
--------
• DEBUG
• INFO
• WARNING
• ERROR

Example
-------
Logger logger;

logger.open("Logs/application.log");

logger.log(
    Logger::Level::INFO,
    "Application started."
);

============================================================
*/

#include <fstream>
#include <mutex>
#include <string>

class Logger
{
public:
    /*
    --------------------------------------------------------
    Log Severity Levels
    --------------------------------------------------------

    DEBUG
        Detailed debugging information.

    INFO
        General application information.

    WARNING
        Non-critical issues.

    ERROR
        Critical failures.
    --------------------------------------------------------
    */
    enum class Level
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    /*
    --------------------------------------------------------
    Constructor

    Initializes default logger configuration.

    Default Settings
    ----------------
    Console Output : Enabled

    Minimum Level  : DEBUG
    --------------------------------------------------------
    */
    Logger();

    /*
    --------------------------------------------------------
    Destructor

    Ensures the log file is properly closed.

    Automatically flushes remaining buffered data.
    --------------------------------------------------------
    */
    ~Logger();

    /*
    --------------------------------------------------------
    Open Log File

    Parameters
    ----------
    filename

        Path to the log file.

    Returns
    -------
    true

        Successfully opened.

    false

        Failed to open.

    Notes
    -----
    Opens file in append mode.

    A new logging session header is automatically written.
    --------------------------------------------------------
    */
    bool open(const std::string &filename);

    /*
    --------------------------------------------------------
    Close Log File

    Flushes pending output and closes the file.

    Safe to call multiple times.
    --------------------------------------------------------
    */
    void close();

    /*
    --------------------------------------------------------
    Write Log Entry

    Parameters
    ----------
    level

        Log severity.

    message

        Text to be written.

    Behaviour
    ---------
    1. Check minimum log level.
    2. Generate timestamp.
    3. Format log message.
    4. Write to file.
    5. Flush output.
    6. Optionally print to console.

    Thread Safety
    -------------
    Protected using std::mutex.
    --------------------------------------------------------
    */
    void log(Level level,
             const std::string &message);

    /*
    --------------------------------------------------------
    Enable / Disable Console Output

    Parameters
    ----------
    enabled

    true
        Display log messages on terminal.

    false
        Write only to log file.
    --------------------------------------------------------
    */
    void setConsoleOutput(bool enabled);

    /*
    --------------------------------------------------------
    Set Minimum Log Level

    Parameters
    ----------
    level

        Lowest severity level to record.

    Example
    -------

    Minimum Level = WARNING

    DEBUG   -> Ignored

    INFO    -> Ignored

    WARNING -> Logged

    ERROR   -> Logged
    --------------------------------------------------------
    */
    void setMinimumLevel(Level level);

private:
    /*
    --------------------------------------------------------
    Output File Stream

    Stores log entries.
    --------------------------------------------------------
    */
    std::ofstream file;

    /*
    --------------------------------------------------------
    Thread Synchronization

    Prevents simultaneous writes from multiple threads.
    --------------------------------------------------------
    */
    std::mutex mutex;

    /*
    --------------------------------------------------------
    Console Output Flag

    true

        Print logs to terminal.

    false

        File logging only.
    --------------------------------------------------------
    */
    bool consoleOutput;

    /*
    --------------------------------------------------------
    Current Minimum Log Level
    --------------------------------------------------------
    */
    Level minimumLevel;

    /*
    --------------------------------------------------------
    Generate Timestamp

    Returns current local date and time.

    Format

    YYYY-MM-DD HH:MM:SS

    Example

    2026-08-04 20:35:18
    --------------------------------------------------------
    */
    std::string timestamp() const;

    /*
    --------------------------------------------------------
    Convert Log Level To String

    Example

    INFO

    WARNING

    ERROR
    --------------------------------------------------------
    */
    std::string levelToString(Level level) const;

    /*
    --------------------------------------------------------
    Determine Whether Message Should Be Logged

    Returns

    true
        Message is written.

    false
        Message is ignored.
    --------------------------------------------------------
    */
    bool shouldLog(Level level) const;

    /*
    --------------------------------------------------------
    Write Session Header

    Automatically called after opening the log file.

    Example

    =====================================================

    New Logging Session

    Started :
    2026-08-04 20:35:18

    =====================================================
    --------------------------------------------------------
    */
    void writeSessionHeader();
};

#endif // LOGGER_HPP
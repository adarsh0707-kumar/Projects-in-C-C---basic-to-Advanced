/*
============================================================
Logger Module Implementation
============================================================

Author
------
Adarsh Kumar

Description
-----------
Implements the Logger class responsible for writing
formatted log messages to a log file.

Features
--------
✓ Timestamp generation
✓ Thread-safe logging
✓ Session management
✓ Configurable log levels
✓ Optional console output

============================================================
*/

#include "Logger.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

/*
------------------------------------------------------------
Logger Constructor

Purpose
-------
Initializes the logger with default settings.

Default Configuration
---------------------
Console Output : Enabled

Minimum Level  : DEBUG

The log file is NOT opened here.

Call open() before logging messages.
------------------------------------------------------------
*/
Logger::Logger()
    : consoleOutput(true),
      minimumLevel(Level::DEBUG)
{
}

/*
------------------------------------------------------------
Logger Destructor

Purpose
-------
Ensures that the log file is properly closed before
the object is destroyed.

This prevents data loss caused by buffered output.
------------------------------------------------------------
*/
Logger::~Logger()
{
    close();
}

/*
------------------------------------------------------------
Open Log File

Purpose
-------
Opens the specified log file in append mode.

Parameters
----------
filename

    Path of the log file.

Returns
-------
true

    Log file opened successfully.

false

    Failed to open log file.

Behaviour
---------
1. Opens file.
2. Writes session header.
3. Ready for logging.
------------------------------------------------------------
*/
bool Logger::open(const std::string &filename)
{
    file.open(filename, std::ios::app);

    if (!file.is_open())
    {
        return false;
    }

    writeSessionHeader();

    return true;
}

/*
------------------------------------------------------------
Close Log File

Purpose
-------
Flushes buffered log entries and closes the file.

Safe to call multiple times.
------------------------------------------------------------
*/
void Logger::close()
{
    if (file.is_open())
    {
        file.flush();

        file.close();
    }
}

/*
------------------------------------------------------------
Write Log Entry

Purpose
-------
Writes a formatted log entry.

Parameters
----------
level

    Severity level.

message

    Message text.

Behaviour
---------
1. Checks minimum logging level.
2. Creates timestamp.
3. Formats log line.
4. Writes to file.
5. Flushes immediately.
6. Optionally prints to terminal.

Thread Safety
-------------
Protected using std::mutex.
------------------------------------------------------------
*/
void Logger::log(Level level,
                 const std::string &message)
{
    /*
    Ignore messages below the configured
    minimum log level.
    */
    if (!shouldLog(level))
    {
        return;
    }

    /*
    Lock the logger to prevent multiple
    threads writing simultaneously.
    */
    std::lock_guard<std::mutex> lock(mutex);

    /*
    Build formatted log entry.

    Example

    [2026-08-04 21:30:10] [INFO] Application started.
    */
    std::string entry =
        "[" +
        timestamp() +
        "] "
        "[" +
        levelToString(level) +
        "] " + message;

    /*
    Write entry to log file.
    */
    file << entry << '\n';

    /*
    Flush immediately.

    This ensures no data is lost if the
    application exits unexpectedly.
    */
    file.flush();

    /*
    Print to console if enabled.
    */
    if (consoleOutput)
    {
        std::cout
            << entry
            << '\n';
    }
}

/*
------------------------------------------------------------
Enable / Disable Console Output

Purpose
-------
Controls whether log entries are displayed on the
terminal in addition to the log file.

Parameters
----------
enabled

true

    Enable console logging.

false

    Disable console logging.
------------------------------------------------------------
*/
void Logger::setConsoleOutput(bool enabled)
{
    consoleOutput = enabled;
}

/*
------------------------------------------------------------
Set Minimum Log Level

Purpose
-------
Defines the lowest severity level that should
be recorded.

Example
-------

Minimum = WARNING

DEBUG   -> Ignored

INFO    -> Ignored

WARNING -> Logged

ERROR   -> Logged
------------------------------------------------------------
*/
void Logger::setMinimumLevel(Level level)
{
    minimumLevel = level;
}

/*
------------------------------------------------------------
Generate Current Timestamp

Purpose
-------
Returns the current local date and time.

Format
------
YYYY-MM-DD HH:MM:SS

Example
-------
2026-08-04 21:35:18

Returns
-------
A formatted timestamp string.
------------------------------------------------------------
*/
std::string Logger::timestamp() const
{
    /*
    Get current system time.
    */
    auto now = std::chrono::system_clock::now();

    /*
    Convert to time_t.
    */
    std::time_t currentTime =
        std::chrono::system_clock::to_time_t(now);

    /*
    Convert to local time.
    */
    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &currentTime);
#else
    localtime_r(&currentTime, &localTime);
#endif

    /*
    Format timestamp.
    */
    std::ostringstream stream;

    stream << std::put_time(
        &localTime,
        "%Y-%m-%d %H:%M:%S");

    return stream.str();
}

/*
------------------------------------------------------------
Convert Log Level to String

Purpose
-------
Converts the log level enumeration into a readable
text representation.

Parameters
----------
level

    Log severity.

Returns
-------
DEBUG

INFO

WARNING

ERROR
------------------------------------------------------------
*/
std::string Logger::levelToString(Level level) const
{
    switch (level)
    {
    case Level::DEBUG:
        return "DEBUG";

    case Level::INFO:
        return "INFO";

    case Level::WARNING:
        return "WARNING";

    case Level::ERROR:
        return "ERROR";

    default:
        return "UNKNOWN";
    }
}

/*
------------------------------------------------------------
Check Logging Level

Purpose
-------
Determines whether a log message should be written
based on the configured minimum logging level.

Returns
-------
true

    Message should be logged.

false

    Message should be ignored.
------------------------------------------------------------
*/
bool Logger::shouldLog(Level level) const
{
    return static_cast<int>(level) >=
           static_cast<int>(minimumLevel);
}

/*
------------------------------------------------------------
Write Session Header

Purpose
-------
Marks the beginning of every application execution.

This helps separate multiple runs within the same
log file.

Example
-------

=========================================================
New Logging Session
Started : 2026-08-04 21:40:00
=========================================================

------------------------------------------------------------
*/
void Logger::writeSessionHeader()
{
    if (!file.is_open())
    {
        return;
    }

    file << '\n';

    file << "=========================================================\n";

    file << "                 NEW LOGGING SESSION\n";

    file << "=========================================================\n";

    file << "Started : "
         << timestamp()
         << '\n';

    file << "=========================================================\n\n";

    file.flush();
}
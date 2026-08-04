#include "Logger.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

/*
============================================================
Logger Implementation
============================================================
Implements the Logger class for the Digital Clock project.
============================================================
*/

Logger::Logger()
    : consoleOutput(true),
      minimumLevel(Level::DEBUG)
{
}

Logger::~Logger()
{
    close();
}

bool Logger::open(const std::string &filename)
{
    file.open(filename, std::ios::app);

    if (!file.is_open())
        return false;

    writeSessionHeader();
    return true;
}

void Logger::close()
{
    if (file.is_open())
    {
        file.flush();
        file.close();
    }
}

void Logger::log(Level level,
                 const std::string &message)
{
    if (!shouldLog(level))
        return;

    std::lock_guard<std::mutex> lock(mutex);

    std::string entry =
        "[" + timestamp() + "] "
                            "[" +
        levelToString(level) + "] " + message;

    file << entry << '\n';
    file.flush();

    if (consoleOutput)
        std::cout << entry << '\n';
}

void Logger::setConsoleOutput(bool enabled)
{
    consoleOutput = enabled;
}

void Logger::setMinimumLevel(Level level)
{
    minimumLevel = level;
}

std::string Logger::timestamp() const
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::ostringstream out;
    out << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return out.str();
}

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

bool Logger::shouldLog(Level level) const
{
    return static_cast<int>(level) >=
           static_cast<int>(minimumLevel);
}

void Logger::writeSessionHeader()
{
    if (!file.is_open())
        return;

    file << "\n============================================================\n";
    file << "NEW LOGGING SESSION\n";
    file << "Started : " << timestamp() << '\n';
    file << "============================================================\n";
    file.flush();
}
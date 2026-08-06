#include "Logger.hpp"

#include <filesystem>
#include <iostream>

#include "Utility.hpp"

Logger::Logger()
    : consoleOutput(false),
      minimumLevel(Level::INFO)
{
}

Logger::~Logger()
{
    close();
}

bool Logger::initialize(const std::string &fileName)
{
    if (fileName.empty())
        return false;

    std::lock_guard<std::mutex> lock(mutex);

    if (file.is_open())
    {
        file.flush();
        file.close();
    }

    /*
    Create the parent directory when the configured path points somewhere that
    does not exist yet. A failure here is not fatal -- the open below simply
    fails and the application runs without logging (TC-024).
    */
    const std::filesystem::path path(fileName);

    if (path.has_parent_path())
    {
        std::error_code code;
        std::filesystem::create_directories(path.parent_path(), code);
    }

    file.open(fileName, std::ios::app);

    if (!file.is_open())
        return false;

    writeSessionHeader();

    return true;
}

void Logger::close()
{
    std::lock_guard<std::mutex> lock(mutex);

    if (file.is_open())
    {
        file.flush();
        file.close();
    }
}

void Logger::log(Level level, const std::string &message)
{
    if (static_cast<int>(level) < static_cast<int>(minimumLevel))
        return;

    std::lock_guard<std::mutex> lock(mutex);

    const std::string entry =
        "[" + Utility::currentDateTime() + "] " +
        "[" + levelToString(level) + "] " +
        message;

    if (file.is_open())
    {
        file << entry << '\n';
        file.flush();
    }

    if (consoleOutput)
        std::cout << entry << '\n';
}

void Logger::info(const std::string &message)
{
    log(Level::INFO, message);
}

void Logger::warning(const std::string &message)
{
    log(Level::WARNING, message);
}

void Logger::error(const std::string &message)
{
    log(Level::ERROR, message);
}

void Logger::debug(const std::string &message)
{
    log(Level::DEBUG, message);
}

void Logger::setConsoleOutput(bool enabled)
{
    consoleOutput = enabled;
}

void Logger::setMinimumLevel(Level level)
{
    minimumLevel = level;
}

bool Logger::isOpen() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return file.is_open();
}

std::string Logger::levelToString(Level level)
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
    }

    return "UNKNOWN";
}

Logger::Level Logger::levelFromString(const std::string &text,
                                      Level defaultLevel)
{
    const std::string value = Utility::toUpper(Utility::trim(text));

    if (value == "DEBUG")
        return Level::DEBUG;

    if (value == "INFO")
        return Level::INFO;

    if (value == "WARNING" || value == "WARN")
        return Level::WARNING;

    if (value == "ERROR")
        return Level::ERROR;

    return defaultLevel;
}

void Logger::writeSessionHeader()
{
    // Caller already holds the mutex.
    if (!file.is_open())
        return;

    file << '\n'
         << "============================================================\n"
         << "NEW LOGGING SESSION\n"
         << "Started : " << Utility::currentDateTime() << '\n'
         << "============================================================\n";

    file.flush();
}

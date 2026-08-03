#include "Logger.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

Logger::Logger() = default;

Logger::~Logger()
{
    close();
}

bool Logger::open(const std::string &filename)
{
    file.open(filename, std::ios::app);
    return file.is_open();
}

void Logger::close()
{
    if (file.is_open())
    {
        file.close();
    }
}

void Logger::log(Level level, const std::string &message)
{
    if (!file.is_open())
        return;

    file << "[" << getTimestamp() << "] "
         << "[" << levelToString(level) << "] "
         << message << '\n';

    file.flush();

    if (level == Level::ERROR)
    {
        std::cerr << "[ERROR] " << message << '\n';
    }
}

std::string Logger::getTimestamp() const
{
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t time = system_clock::to_time_t(now);

    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif

    std::ostringstream out;

    out << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

    return out.str();
}

std::string Logger::levelToString(Level level) const
{
    switch (level)
    {
    case Level::INFO:
        return "INFO";
    case Level::WARNING:
        return "WARNING";
    case Level::ERROR:
        return "ERROR";
    case Level::DEBUG:
        return "DEBUG";
    }

    return "UNKNOWN";
}
#include "Utility.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace
{
    /** Characters treated as whitespace by Utility::trim(). */
    const char *const WHITESPACE = " \t\n\r\f\v";
}

std::string Utility::trim(const std::string &text)
{
    const std::size_t first = text.find_first_not_of(WHITESPACE);

    if (first == std::string::npos)
        return "";

    const std::size_t last = text.find_last_not_of(WHITESPACE);

    return text.substr(first, last - first + 1);
}

std::string Utility::toUpper(const std::string &text)
{
    std::string result = text;

    std::transform(
        result.begin(),
        result.end(),
        result.begin(),
        [](unsigned char c)
        { return static_cast<char>(std::toupper(c)); });

    return result;
}

std::string Utility::toLower(const std::string &text)
{
    std::string result = text;

    std::transform(
        result.begin(),
        result.end(),
        result.begin(),
        [](unsigned char c)
        { return static_cast<char>(std::tolower(c)); });

    return result;
}

bool Utility::fileExists(const std::string &path)
{
    if (path.empty())
        return false;

    std::ifstream file(path);

    return file.good();
}

std::string Utility::currentDateTime()
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t raw = std::chrono::system_clock::to_time_t(now);

    std::tm local{};

#ifdef _WIN32
    localtime_s(&local, &raw);
#else
    localtime_r(&raw, &local);
#endif

    std::ostringstream out;
    out << std::put_time(&local, "%Y-%m-%d %H:%M:%S");

    return out.str();
}

std::string Utility::padZero(int value, int width)
{
    std::ostringstream out;

    out << std::setw(width)
        << std::setfill('0')
        << value;

    return out.str();
}

std::string Utility::center(const std::string &text, int width)
{
    const int length = static_cast<int>(text.size());

    if (length >= width)
        return text;

    const int padding = (width - length) / 2;

    return std::string(static_cast<std::size_t>(padding), ' ') + text;
}

bool Utility::toBool(const std::string &text, bool defaultValue)
{
    const std::string value = toLower(trim(text));

    if (value == "true" || value == "yes" || value == "on" ||
        value == "enabled" || value == "1")
    {
        return true;
    }

    if (value == "false" || value == "no" || value == "off" ||
        value == "disabled" || value == "0")
    {
        return false;
    }

    return defaultValue;
}

int Utility::toInt(const std::string &text, int defaultValue)
{
    const std::string value = trim(text);

    if (value.empty())
        return defaultValue;

    try
    {
        std::size_t consumed = 0;
        const int parsed = std::stoi(value, &consumed);

        // Reject trailing garbage such as "100abc".
        if (consumed != value.size())
            return defaultValue;

        return parsed;
    }
    catch (...)
    {
        return defaultValue;
    }
}

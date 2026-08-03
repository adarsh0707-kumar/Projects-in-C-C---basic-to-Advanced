#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <string>

class Logger
{
public:
    enum class Level
    {
        INFO,
        WARNING,
        ERROR,
        DEBUG
    };

    Logger();
    ~Logger();

    bool open(const std::string &filename = "Logs/application.log");
    void close();

    void log(Level level, const std::string &message);

private:
    std::ofstream file;

    std::string getTimestamp() const;
    std::string levelToString(Level level) const;
};

#endif // LOGGER_HPP
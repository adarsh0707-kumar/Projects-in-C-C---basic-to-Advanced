#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "Utility.hpp"

class Logger
{
private:
    std::ofstream logFile;
    bool isInitialized = false;

public:
    ~Logger() { close(); }

    bool initialize(const std::string &fileName)
    {
        if (isInitialized)
            return true;
        logFile.open(fileName, std::ios::app);
        if (logFile.is_open())
        {
            isInitialized = true;
            info("Logger initialized successfully.");
            return true;
        }
        return false;
    }

    void info(const std::string &message)
    {
        if (!isInitialized)
            return;
        logFile << "[" << Utility::currentTimestamp() << "] [INFO] " << message << std::endl;
        logFile.flush();
    }

    void warning(const std::string &message)
    {
        if (!isInitialized)
            return;
        logFile << "[" << Utility::currentTimestamp() << "] [WARNING] " << message << std::endl;
        logFile.flush();
    }

    void error(const std::string &message)
    {
        if (!isInitialized)
            return;
        logFile << "[" << Utility::currentTimestamp() << "] [ERROR] " << message << std::endl;
        logFile.flush();
    }

    void close()
    {
        if (isInitialized && logFile.is_open())
        {
            info("Logger shutting down.");
            logFile.close();
            isInitialized = false;
        }
    }
};
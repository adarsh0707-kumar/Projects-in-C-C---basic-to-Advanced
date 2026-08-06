/******************************************************************************
 * @file main.cpp
 * @brief Entry point for the Digital Clock application.
 * @author Adarsh Kumar
 * @date 2026
 *
 * main() is deliberately thin: it parses the few command-line options, hands
 * control to Application, and translates the result into an exit status. The
 * lifecycle itself lives in Application so that it can be tested.
 ******************************************************************************/

#include <iostream>
#include <string>

#include "Application.hpp"
#include "Version.hpp"

namespace
{
    /**
     * @brief Prints the command-line usage summary.
     * @param programName Name the program was invoked as.
     */
    void printUsage(const std::string &programName)
    {
        std::cout
            << Version::NAME << " v" << Version::VERSION << "\n\n"
            << "Usage: " << programName << " [options]\n\n"
            << "Options:\n"
            << "  -c, --config <path>  Configuration file to load\n"
            << "                       (default: "
            << Application::defaultConfigPath() << ")\n"
            << "  -v, --version        Print the version and exit\n"
            << "  -h, --help           Print this help and exit\n\n"
            << "Press Q or Ctrl+C to exit the running clock.\n";
    }
}

/**
 * @brief Application entry point.
 *
 * @param argc Argument count.
 * @param argv Argument values.
 * @return int 0 on a normal shutdown, 1 when startup failed or the arguments
 *         were invalid.
 */
int main(int argc, char *argv[])
{
    const std::string programName = (argc > 0) ? argv[0] : "DigitalClock";

    std::string configPath = Application::defaultConfigPath();

    for (int index = 1; index < argc; ++index)
    {
        const std::string argument = argv[index];

        if (argument == "-h" || argument == "--help")
        {
            printUsage(programName);
            return 0;
        }

        if (argument == "-v" || argument == "--version")
        {
            std::cout << Version::NAME << " v" << Version::VERSION << '\n';
            return 0;
        }

        if (argument == "-c" || argument == "--config")
        {
            if (index + 1 >= argc)
            {
                std::cerr << "Error: " << argument
                          << " requires a file path.\n";
                return 1;
            }

            configPath = argv[++index];
            continue;
        }

        std::cerr << "Error: unknown option '" << argument << "'.\n\n";
        printUsage(programName);

        return 1;
    }

    Application application;

    if (!application.initialize(configPath))
    {
        std::cerr << "Error: the application failed to start. "
                  << "See the log for details.\n";

        return Application::EXIT_STARTUP_FAILED;
    }

    return application.run();
}

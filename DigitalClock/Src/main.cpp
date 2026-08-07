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
            << "  -1, --once           Render a single frame and exit\n"
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

    /*
    --once renders one frame and exits instead of entering the refresh loop.
    It exists so the rendering path can be exercised non-interactively: a CI
    job on a platform with no terminal to attach to still executes console
    initialization, layout and drawing, which a build alone would not.
    */
    bool renderOnce = false;

    for (int index = 1; index < argc; ++index)
    {
        const std::string argument = argv[index];

        if (argument == "-h" || argument == "--help")
        {
            printUsage(programName);
            return 0;
        }

        if (argument == "-1" || argument == "--once")
        {
            renderOnce = true;
            continue;
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

    if (renderOnce)
    {
        application.renderFrame();
        application.shutdown();

        return Application::EXIT_OK;
    }

    return application.run();
}

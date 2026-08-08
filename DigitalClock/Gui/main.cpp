/******************************************************************************
 * @file main.cpp
 * @brief Entry point for the graphical Digital Clock.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Thin on purpose, like the console entry point: it parses arguments, opens
 * the window, and hands over to Qt.
 *
 * --once renders a single frame and exits, which is what lets CI verify the
 * graphical build on a runner with no display. Without it, "the GUI compiles"
 * would be the only claim CI could make about it -- and this project has been
 * built on not making claims it has not earned.
 ******************************************************************************/

#include <iostream>
#include <string>

#include <QApplication>
#include <QTimer>

#include "ClockWindow.hpp"
#include "Version.hpp"

namespace
{
    void printUsage(const std::string &program)
    {
        std::cout
            << Version::NAME << " " << Version::VERSION
            << " (graphical interface)\n\n"
            << "Usage: " << program << " [options]\n\n"
            << "  -c, --config <path>   Configuration file to load\n"
            << "                        (default: Config/config.ini)\n"
            << "  -1, --once            Open, draw one frame, then exit\n"
            << "  -v, --version         Print the version and exit\n"
            << "  -h, --help            Print this message and exit\n\n"
            << "Keys: M next mode, T next theme, F 12/24-hour,\n"
            << "      C reload configuration, Q quit.\n";
    }
}

int main(int argc, char *argv[])
{
    QString configPath = "Config/config.ini";
    bool once = false;

    for (int index = 1; index < argc; ++index)
    {
        const std::string argument = argv[index];

        if (argument == "-h" || argument == "--help")
        {
            printUsage(argv[0]);
            return 0;
        }

        if (argument == "-v" || argument == "--version")
        {
            std::cout << Version::NAME << " v" << Version::VERSION << "\n";
            return 0;
        }

        if (argument == "-1" || argument == "--once")
        {
            once = true;
            continue;
        }

        if (argument == "-c" || argument == "--config")
        {
            if (index + 1 >= argc)
            {
                std::cerr << "Error: " << argument
                          << " requires a file path.\n";
                return 1;
            }

            configPath = QString::fromLocal8Bit(argv[++index]);
            continue;
        }

        std::cerr << "Error: unrecognised option '" << argument << "'.\n\n";
        printUsage(argv[0]);

        return 1;
    }

    QApplication application(argc, argv);

    QApplication::setApplicationName(QString::fromStdString(Version::NAME));
    QApplication::setApplicationVersion(
        QString::fromStdString(Version::VERSION));

    ClockWindow window(configPath);
    window.show();

    if (once)
    {
        /*
        Draw one frame, then quit. Going through the event loop rather than
        returning immediately means the window is actually realised and
        painted, so this exercises the layout rather than only construction.
        */
        window.tick();

        QTimer::singleShot(0, &application, &QCoreApplication::quit);
    }

    return application.exec();
}

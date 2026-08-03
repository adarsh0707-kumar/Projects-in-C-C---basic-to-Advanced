#include <chrono>
#include <thread>
#include <iostream>

#include "Config.hpp"
#include "Clock.hpp"
#include "Date.hpp"
#include "Display.hpp"

int main()
{
    Clock clock;
    Date date;
    Display display;

    Config config;

    if (!config.load("Resources/config.ini"))
    {
        std::cerr << "Failed to load configuration.\n";
        return 1;
    }

    while (true)
    {
        clock.update();
        date.update();

        display.render(clock, date);

        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                config.getInt("REFRESH_INTERVAL", 1000)));
    }

    return 0;
}
#include <chrono>
#include <thread>

#include "Clock.hpp"
#include "Date.hpp"
#include "Display.hpp"

int main()
{
    Clock clock;
    Date date;
    Display display;

    while (true)
    {
        clock.update();
        date.update();

        display.render(clock, date);

        std::this_thread::sleep_for(
            std::chrono::seconds(1));
    }

    return 0;
}
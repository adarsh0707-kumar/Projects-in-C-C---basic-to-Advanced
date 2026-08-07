#include "Display.hpp"

Display::Display()
    : initialized(false)
{
    layout.attach(terminal);
}

Display::~Display()
{
    shutdown();
}

void Display::initialize(ThemeManager &theme)
{
    // Redirected output must not contain escape sequences.
    theme.setColorEnabled(terminal.supportsColor());
    theme.applyTheme();

    layout.attach(theme);

    terminal.initialize();
    terminal.clear();

    layout.resize(terminal.width(), terminal.height());

    initialized = true;
}

void Display::renderClock(const std::string &time)
{
    layout.setTime(time);
}

void Display::renderDate(const std::string &date)
{
    layout.setDate(date);
}

void Display::renderScreen()
{
    // Follow the terminal if the user resized the window between frames.
    layout.resize(terminal.width(), terminal.height());

    layout.setStatusLines(status.lines());
    layout.draw();
}

void Display::render()
{
    renderScreen();
}

void Display::update()
{
    renderScreen();
}

void Display::refresh()
{
    terminal.refresh();
}

void Display::clear()
{
    terminal.clear();
    terminal.refresh();
}

void Display::setMessage(const std::string &text)
{
    status.setMessage(text);
}

void Display::setStatusField(const std::string &name, const std::string &value)
{
    status.setField(name, value);
}

void Display::showNotification(const std::vector<std::string> &lines)
{
    layout.setNotification(lines);
}

void Display::clearNotification()
{
    layout.setNotification({});
}

void Display::showInfoLines(const std::vector<std::string> &lines)
{
    layout.setInfoLines(lines);
}

bool Display::loadBanner(const std::string &fileName)
{
    return layout.banner().load(fileName);
}

void Display::shutdown()
{
    if (!initialized)
        return;

    terminal.shutdown();

    initialized = false;
}

int Display::readKey()
{
    return terminal.readKey();
}

Console &Display::console()
{
    return terminal;
}

Screen &Display::screen()
{
    return layout;
}

StatusBar &Display::statusBar()
{
    return status;
}

#include "Console.hpp"

#include <iostream>
#include <string>

#include "Utility.hpp"

#ifdef _WIN32
#include <conio.h>
#include <io.h>
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace
{
#ifndef _WIN32
    /** Terminal attributes saved by enableRawInput(). */
    termios savedTerminalState{};
#endif

    /** Width reported when the real terminal size is unavailable. */
    const int DEFAULT_WIDTH = 80;

    /** Height reported when the real terminal size is unavailable. */
    const int DEFAULT_HEIGHT = 24;

    /**
     * @brief Reports whether standard output is attached to a terminal.
     */
    bool stdoutIsTerminal()
    {
#ifdef _WIN32
        return _isatty(_fileno(stdout)) != 0;
#else
        return isatty(STDOUT_FILENO) != 0;
#endif
    }

    /**
     * @brief Reports whether standard input is attached to a terminal.
     */
    bool stdinIsTerminal()
    {
#ifdef _WIN32
        return _isatty(_fileno(stdin)) != 0;
#else
        return isatty(STDIN_FILENO) != 0;
#endif
    }

    /**
     * @brief Probes whether colour output is appropriate.
     *
     * Honours the NO_COLOR convention and refuses colour for the "dumb"
     * terminal type or when output is redirected.
     */
    bool detectColorSupport()
    {
        // NO_COLOR is honoured when merely present, even if empty, which is
        // what the convention specifies.
        if (Utility::hasEnvironment("NO_COLOR"))
            return false;

        if (!stdoutIsTerminal())
            return false;

        std::string term;

        if (Utility::environment("TERM", term) && term == "dumb")
            return false;

        return true;
    }
}

Console::Console()
    : initialized(false),
      rawInputActive(false),
      colorCapable(detectColorSupport())
{
}

Console::~Console()
{
    shutdown();
}

void Console::initialize()
{
    if (initialized)
        return;

#ifdef _WIN32
    /*
    Windows 10 and later can interpret ANSI escape sequences, but the console
    mode flag has to be requested explicitly.
    */
    const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    if (output != INVALID_HANDLE_VALUE)
    {
        DWORD mode = 0;

        if (GetConsoleMode(output, &mode))
        {
            SetConsoleMode(
                output,
                mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#endif

    hideCursor();
    enableRawInput();

    initialized = true;

    refresh();
}

void Console::shutdown()
{
    if (!initialized)
        return;

    restoreInput();
    showCursor();

    // Leave the cursor on a fresh line so the shell prompt is not overwritten.
    std::cout << '\n';

    refresh();

    initialized = false;
}

void Console::clear()
{
    // 2J clears the screen, H returns the cursor to the home position.
    std::cout << "\033[2J\033[H";
}

void Console::home()
{
    std::cout << "\033[H";
}

void Console::refresh()
{
    std::cout.flush();
}

void Console::setCursor(int row, int column)
{
    if (row < 1)
        row = 1;

    if (column < 1)
        column = 1;

    std::cout << "\033[" << row << ";" << column << "H";
}

void Console::print(const std::string &text)
{
    std::cout << text;
}

void Console::printLine(const std::string &text)
{
    std::cout << text << '\n';
}

void Console::hideCursor()
{
    std::cout << "\033[?25l";
}

void Console::showCursor()
{
    std::cout << "\033[?25h";
}

bool Console::supportsColor() const
{
    return colorCapable;
}

bool Console::isInitialized() const
{
    return initialized;
}

int Console::width() const
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;

    const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    if (output != INVALID_HANDLE_VALUE &&
        GetConsoleScreenBufferInfo(output, &info))
    {
        return info.srWindow.Right - info.srWindow.Left + 1;
    }
#else
    winsize size{};

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0 && size.ws_col > 0)
        return static_cast<int>(size.ws_col);
#endif

    return DEFAULT_WIDTH;
}

int Console::height() const
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;

    const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    if (output != INVALID_HANDLE_VALUE &&
        GetConsoleScreenBufferInfo(output, &info))
    {
        return info.srWindow.Bottom - info.srWindow.Top + 1;
    }
#else
    winsize size{};

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0 && size.ws_row > 0)
        return static_cast<int>(size.ws_row);
#endif

    return DEFAULT_HEIGHT;
}

void Console::enableRawInput()
{
#ifndef _WIN32
    if (!stdinIsTerminal())
        return;

    if (tcgetattr(STDIN_FILENO, &savedTerminalState) != 0)
        return;

    termios raw = savedTerminalState;

    // Disable line buffering and echo so keystrokes arrive immediately.
    raw.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO));

    // VMIN 0 with VTIME 0 makes read() return at once when nothing is ready.
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == 0)
        rawInputActive = true;
#endif
}

void Console::restoreInput()
{
#ifndef _WIN32
    if (!rawInputActive)
        return;

    tcsetattr(STDIN_FILENO, TCSANOW, &savedTerminalState);

    rawInputActive = false;
#endif
}

int Console::readKey()
{
#ifdef _WIN32
    if (!stdinIsTerminal())
        return -1;

    if (_kbhit() == 0)
        return -1;

    return _getch();
#else
    if (!rawInputActive)
        return -1;

    char character = 0;

    const ssize_t count = read(STDIN_FILENO, &character, 1);

    if (count != 1)
        return -1;

    return static_cast<unsigned char>(character);
#endif
}

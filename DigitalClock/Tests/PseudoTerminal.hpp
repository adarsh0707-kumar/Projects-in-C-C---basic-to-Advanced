#ifndef PSEUDOTERMINAL_HPP
#define PSEUDOTERMINAL_HPP

/******************************************************************************
 * @file PseudoTerminal.hpp
 * @brief A real terminal for tests that need one.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Console and Application read the file descriptors directly -- isatty(),
 * ioctl() and tcsetattr() -- so redirecting std::cout is not enough to reach
 * their terminal paths. A test with piped stdio silently measures the
 * redirected-output fallbacks instead, and reports success.
 *
 * This attaches the process to a pseudo-terminal for the object's lifetime,
 * which makes those paths execute, lets a test type into the application, and
 * lets it read back what was drawn.
 *
 * The whole file is POSIX-only; see test_console.cpp for how Windows is
 * covered instead.
 ******************************************************************************/

#ifndef _WIN32

#include <atomic>
#include <fcntl.h>
#include <iostream>
#include <mutex>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>

/*
openpty() is declared in different headers depending on the C library: glibc
puts it in <pty.h>, while macOS and the BSDs put it in <util.h>. There is no
portable spelling, so the platform has to be tested for.
*/
#if defined(__linux__)
#include <pty.h>
#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || \
    defined(__NetBSD__)
#include <util.h>
#else
#include <pty.h>
#endif

namespace TestSupport
{
    /**
     * @class PseudoTerminal
     * @brief Redirects stdin and stdout to a pty for the object's lifetime.
     *
     * The descriptors are put back on destruction, or every later test would
     * write into a closed terminal.
     */
    class PseudoTerminal
    {
    public:
        PseudoTerminal()
            : master(-1),
              slave(-1),
              savedIn(-1),
              savedOut(-1),
              ready(false),
              capturing(false)
        {
            /*
            Flush before redirecting. std::cout writes to whatever descriptor 1
            points at when the buffer drains, not when the text was written, so
            anything the test framework has buffered would otherwise be emptied
            into the pty and vanish from the report.
            */
            std::cout.flush();

            if (openpty(&master, &slave, nullptr, nullptr, nullptr) != 0)
                return;

            savedIn = dup(STDIN_FILENO);
            savedOut = dup(STDOUT_FILENO);

            if (savedIn < 0 || savedOut < 0)
                return;

            if (dup2(slave, STDIN_FILENO) < 0 || dup2(slave, STDOUT_FILENO) < 0)
                return;

            ready = true;
        }

        ~PseudoTerminal()
        {
            stopCapture();

            // Drain anything the test wrote while the pty was still attached,
            // for the same reason as above.
            std::cout.flush();

            if (savedIn >= 0)
            {
                dup2(savedIn, STDIN_FILENO);
                close(savedIn);
            }

            if (savedOut >= 0)
            {
                dup2(savedOut, STDOUT_FILENO);
                close(savedOut);
            }

            if (slave >= 0)
                close(slave);

            if (master >= 0)
                close(master);
        }

        PseudoTerminal(const PseudoTerminal &) = delete;
        PseudoTerminal &operator=(const PseudoTerminal &) = delete;

        /**
         * @brief Reports whether the terminal was set up successfully.
         */
        bool isReady() const { return ready; }

        /**
         * @brief Writes bytes the application will read as keystrokes.
         *
         * The console is in raw mode while the application is running, so each
         * byte is delivered on its own without waiting for a newline.
         */
        void sendKeys(const std::string &keys) const
        {
            const ssize_t written = ::write(master, keys.data(), keys.size());

            (void)written;

            // Give the terminal line discipline a moment to deliver them.
            usleep(20000);
        }

        /**
         * @brief Reads whatever the application has written to the terminal.
         *
         * Suitable when the writing has already finished. While a refresh loop
         * is running, use startCapture() instead.
         */
        std::string drain() const
        {
            std::string collected;

            for (int attempt = 0; attempt < 20; ++attempt)
            {
                const std::string chunk = readAvailable();

                if (!chunk.empty())
                {
                    collected += chunk;
                    continue;
                }

                if (!collected.empty())
                    break;

                usleep(5000);
            }

            return collected;
        }

        /**
         * @brief Begins draining the terminal on a background thread.
         *
         * A pty holds only a few kilobytes. An application drawing frames into
         * one that nobody is reading blocks on write() once it fills, which
         * would hang the very loop under test rather than measuring it. So the
         * reader has to run while the application does.
         */
        void startCapture()
        {
            if (capturing || !ready)
                return;

            captured.clear();
            capturing = true;

            reader = std::thread(
                [this]()
                {
                    while (capturing.load())
                    {
                        const std::string chunk = readAvailable();

                        if (chunk.empty())
                        {
                            usleep(2000);
                            continue;
                        }

                        std::lock_guard<std::mutex> guard(captureMutex);
                        captured += chunk;
                    }
                });
        }

        /**
         * @brief Stops the background reader and returns everything drawn.
         *
         * Safe to call when no capture is running, and again afterwards.
         *
         * @return std::string Text the application wrote, escape sequences
         *         included.
         */
        std::string stopCapture()
        {
            if (capturing)
            {
                capturing = false;

                if (reader.joinable())
                    reader.join();

                // Whatever landed in the buffer between the last read and the
                // thread noticing it should stop.
                const std::string tail = readAvailable();

                std::lock_guard<std::mutex> guard(captureMutex);
                captured += tail;
            }

            std::lock_guard<std::mutex> guard(captureMutex);

            return captured;
        }

    private:
        /**
         * @brief Reads whatever is waiting, without blocking on an empty pty.
         */
        std::string readAvailable() const
        {
            std::string collected;
            char buffer[4096];

            const int flags = fcntl(master, F_GETFL, 0);
            fcntl(master, F_SETFL, flags | O_NONBLOCK);

            for (;;)
            {
                const ssize_t count = ::read(master, buffer, sizeof(buffer));

                if (count <= 0)
                    break;

                collected.append(buffer, static_cast<std::size_t>(count));
            }

            fcntl(master, F_SETFL, flags);

            return collected;
        }

        int master;
        int slave;
        int savedIn;
        int savedOut;
        bool ready;

        std::atomic<bool> capturing;
        std::thread reader;
        mutable std::mutex captureMutex;
        std::string captured;
    };
}

#endif // !_WIN32

#endif // PSEUDOTERMINAL_HPP

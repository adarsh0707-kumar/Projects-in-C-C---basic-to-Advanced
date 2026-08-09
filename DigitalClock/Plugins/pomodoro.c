/******************************************************************************
 * @file pomodoro.c
 * @brief A Pomodoro mode, supplied as a plugin.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Twenty-five minutes of work, five minutes of break, repeating, with a
 * longer break after every fourth. Space starts and pauses, R resets, N
 * skips to the next interval.
 *
 * This exists to be the answer to "what would a plugin actually do?", which
 * is a fair question to ask before building a loader. A Pomodoro cycle is
 * something people genuinely want from a clock, and just as genuinely does
 * not belong in one: it is a particular working method, with opinions about
 * interval lengths that not everybody shares. A mode is exactly the right
 * size for it, and a plugin is exactly the right place.
 *
 * Written in C, against Plugin.h alone. It links nothing from the
 * application, which is what lets it be built by a different compiler than
 * the one that built the clock.
 *
 * Build (Linux):
 *   cc -std=c11 -shared -fPIC -I../Inc -o pomodoro.so pomodoro.c
 ******************************************************************************/

#include <stdio.h>
#include <string.h>

#include "Plugin.h"

#define MINUTE_MS 60000LL

/** Intervals, in order. The long break arrives after the fourth work turn. */
static const struct
{
    const char *label;
    long long duration;
} INTERVALS[] = {
    {"Work",       25 * MINUTE_MS},
    {"Break",       5 * MINUTE_MS},
    {"Work",       25 * MINUTE_MS},
    {"Break",       5 * MINUTE_MS},
    {"Work",       25 * MINUTE_MS},
    {"Break",       5 * MINUTE_MS},
    {"Work",       25 * MINUTE_MS},
    {"Long break", 15 * MINUTE_MS}
};

#define INTERVAL_COUNT ((int)(sizeof(INTERVALS) / sizeof(INTERVALS[0])))

static const DigitalClockHost *hostServices = NULL;

static int currentInterval = 0;
static int running = 0;
static long long startedAtMs = 0;
static long long bankedMs = 0;   /**< Elapsed before the current pause. */
static int completed = 0;

/**
 * @brief Milliseconds spent in the current interval.
 */
static long long elapsedMs(long long nowMs)
{
    if (!running)
        return bankedMs;

    return bankedMs + (nowMs - startedAtMs);
}

/**
 * @brief Moves to the next interval, resetting the clock for it.
 */
static void advance(void)
{
    currentInterval = (currentInterval + 1) % INTERVAL_COUNT;

    bankedMs = 0;
    running = 0;
    startedAtMs = 0;

    ++completed;

    if (hostServices != NULL && hostServices->log != NULL)
        hostServices->log("Pomodoro: interval finished.");
}

static void pomodoroRender(long long nowMs,
                           char *readout, int readoutSize,
                           char *secondary, int secondarySize)
{
    long long spent = elapsedMs(nowMs);
    const long long total = INTERVALS[currentInterval].duration;

    if (spent > total)
        spent = total;

    long long left = total - spent;

    /*
    Round up, so the readout reaches 00:00 only when the interval actually
    ends -- the same choice CountdownTimer makes, and for the same reason:
    truncating shows 00:00 for the whole final second.
    */
    const long long seconds = (left + 999) / 1000;

    snprintf(readout, (size_t)readoutSize, "%02lld:%02lld",
             seconds / 60, seconds % 60);

    snprintf(secondary, (size_t)secondarySize, "%s - %s   (%d completed)",
             INTERVALS[currentInterval].label,
             running ? "Running" : (spent > 0 ? "Paused" : "Ready"),
             completed);

    /* An interval that has run out advances on its own. */
    if (left <= 0 && running)
        advance();
}

static int pomodoroHandleKey(int key, long long nowMs)
{
    if (key == ' ')
    {
        if (running)
        {
            bankedMs += nowMs - startedAtMs;
            running = 0;
        }
        else
        {
            startedAtMs = nowMs;
            running = 1;
        }

        return 1;
    }

    if (key == 'r' || key == 'R')
    {
        bankedMs = 0;
        running = 0;
        startedAtMs = 0;

        return 1;
    }

    if (key == 'n' || key == 'N')
    {
        advance();
        return 1;
    }

    return 0;
}

static const char *pomodoroFooterHint(void)
{
    return "[Space] Start/Pause  [R] Reset  [N] Next  [M] Mode  [Q] Quit";
}

static void pomodoroShutdown(void)
{
    hostServices = NULL;
}

static const DigitalClockPlugin POMODORO = {
    DIGITALCLOCK_PLUGIN_ABI,
    "Pomodoro",
    "1.0.0",
    pomodoroRender,
    pomodoroHandleKey,
    pomodoroFooterHint,
    pomodoroShutdown
};

DIGITALCLOCK_PLUGIN_EXPORT const DigitalClockPlugin *digitalclock_plugin_init(
    const DigitalClockHost *host)
{
    hostServices = host;

    /*
    Declining is a real option, and the loader handles it. A plugin needing
    something the host does not offer should say no rather than half work.
    */
    if (host == NULL || host->abiVersion != DIGITALCLOCK_PLUGIN_ABI)
        return NULL;

    if (host->log != NULL)
        host->log("Pomodoro plugin initialised.");

    return &POMODORO;
}

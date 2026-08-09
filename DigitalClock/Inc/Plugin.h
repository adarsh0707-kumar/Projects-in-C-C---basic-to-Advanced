#ifndef DIGITALCLOCK_PLUGIN_H
#define DIGITALCLOCK_PLUGIN_H

/******************************************************************************
 * @file Plugin.h
 * @brief The plugin interface: a mode supplied by a shared library.
 * @author Adarsh Kumar
 * @date 2026
 *
 * A plugin adds a *mode* -- a fifth entry in the M cycle alongside Clock,
 * Stopwatch, Timer and World. That is the extension point because it is the
 * one with real pull behind it: a Pomodoro cycle, a countdown to a date, a
 * binary clock. None of those belong in a clock's core, and all of them want
 * exactly what a mode already has: a large readout, a line beneath it, and
 * some keys.
 *
 * Four decisions are worth stating, because each rules something out:
 *
 *   1. This is a C header, and the boundary is a C ABI. C++ has no stable
 *      ABI across compilers or even across versions of one, so a C++
 *      interface would mean a plugin only loads if built with very nearly
 *      the same toolchain as the application. That is not a plugin system;
 *      it is a recompile with extra steps.
 *
 *   2. Buffers are plain char arrays with an explicit size, not
 *      std::string. Passing a std::string across the boundary reintroduces
 *      exactly the coupling point 1 avoids.
 *
 *   3. A plugin never links against the application. It receives whatever
 *      it may call in a host struct, so it has no undefined symbols to
 *      resolve and cannot depend on the host's internals by accident.
 *
 *   4. The ABI is versioned and checked. A plugin built against a different
 *      version is refused with a message rather than called and trusted --
 *      the alternative is a crash inside somebody else's code.
 *
 * Reference: Change Log, KI-006.
 ******************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief The interface version this header describes.
 *
 * Bumped whenever the structs below change shape. The application refuses
 * to load a plugin reporting anything else.
 */
#define DIGITALCLOCK_PLUGIN_ABI 1

/** Longest plugin name the status bar will show. */
#define DIGITALCLOCK_PLUGIN_NAME_MAX 32

/**
 * @struct DigitalClockHost
 * @brief What the application lends to a plugin.
 *
 * Passed once, at initialisation. A plugin that wants nothing may ignore it,
 * but must not keep it beyond shutdown().
 */
typedef struct DigitalClockHost
{
    /** ABI the application was built against. */
    unsigned int abiVersion;

    /**
     * @brief Writes a line to the application's log.
     *
     * Plugins must not print to the terminal themselves: the display is
     * redrawn in place, and anything written directly corrupts the frame.
     */
    void (*log)(const char *message);

    /**
     * @brief Returns a monotonic reading in milliseconds.
     *
     * The same clock the stopwatch and countdown use, so a plugin measuring
     * elapsed time is unaffected by the system clock being adjusted.
     */
    long long (*monotonicNow)(void);
} DigitalClockHost;

/**
 * @struct DigitalClockPlugin
 * @brief What a plugin supplies in return.
 *
 * Every function pointer except render() may be null; the application checks
 * before calling.
 */
typedef struct DigitalClockPlugin
{
    /** ABI this plugin was built against. Must match the application's. */
    unsigned int abiVersion;

    /** Shown in the status bar as the mode name. */
    const char *name;

    /** Free-form, shown in the log at startup. */
    const char *version;

    /**
     * @brief Produces the frame's text.
     *
     * Called once per redraw. Both buffers are pre-terminated as empty, so a
     * plugin that writes nothing simply shows nothing.
     *
     * @param nowMs         Monotonic reading for this frame.
     * @param readout       Large centre readout.
     * @param readoutSize   Bytes available in @p readout.
     * @param secondary     Line beneath it.
     * @param secondarySize Bytes available in @p secondary.
     */
    void (*render)(long long nowMs,
                   char *readout, int readoutSize,
                   char *secondary, int secondarySize);

    /**
     * @brief Offers a keystroke to the plugin.
     *
     * Only keys the application has not already claimed reach here: Q, M, T,
     * F, C, S and D are handled first and never offered.
     *
     * @param key   Character read from the terminal.
     * @param nowMs Monotonic reading.
     * @return Non-zero if the key was consumed and the frame should redraw.
     */
    int (*handleKey)(int key, long long nowMs);

    /**
     * @brief Returns the footer hint for this mode, or null for none.
     */
    const char *(*footerHint)(void);

    /**
     * @brief Called once before the library is closed.
     */
    void (*shutdown)(void);
} DigitalClockPlugin;

/**
 * @brief The one symbol every plugin must export.
 *
 * Named rather than guessed at, so a library that is not a plugin fails to
 * load with a clear message instead of by calling something arbitrary.
 *
 * @param host Services the plugin may use; valid until shutdown().
 * @return The plugin, or null to decline being loaded. The application does
 *         not take ownership: the pointer must stay valid until shutdown().
 */
typedef const DigitalClockPlugin *(*DigitalClockPluginInit)(
    const DigitalClockHost *host);

/** The exported symbol's name, as the loader looks it up. */
#define DIGITALCLOCK_PLUGIN_INIT_SYMBOL "digitalclock_plugin_init"

#ifdef __cplusplus
}
#endif

#endif /* DIGITALCLOCK_PLUGIN_H */

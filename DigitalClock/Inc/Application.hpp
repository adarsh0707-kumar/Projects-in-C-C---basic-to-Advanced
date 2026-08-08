#ifndef APPLICATION_HPP
#define APPLICATION_HPP

/******************************************************************************
 * @file Application.hpp
 * @brief Declaration of the Application orchestrator.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Application owns every component and drives the startup sequence, the
 * refresh loop and the shutdown sequence. Keeping this in a class rather than
 * in main() means the whole lifecycle can be exercised from a test.
 *
 * Startup follows the order given in the API Documentation, section 5.2:
 * logger, configuration, resources, theme, console, core objects, display,
 * then the main loop.
 ******************************************************************************/

#include <string>

#include "AlarmManager.hpp"
#include "Clock.hpp"
#include "CountdownTimer.hpp"
#include "ConfigurationManager.hpp"
#include "Date.hpp"
#include "Display.hpp"
#include "Logger.hpp"
#include "Notifier.hpp"
#include "Stopwatch.hpp"
#include "ResourceManager.hpp"
#include "ThemeManager.hpp"
#include "TimeFormatter.hpp"
#include "WorldClock.hpp"

/**
 * @class Application
 * @brief Owns the component graph and runs the application lifecycle.
 */
class Application
{
public:
    /**
     * @enum Mode
     * @brief What the main readout is showing.
     *
     * The three modes share one screen: whichever is active supplies the
     * large centre readout and the line beneath it. Cycling modes never
     * disturbs the others, so a running stopwatch keeps running while the
     * clock is on screen.
     */
    enum class Mode
    {
        Clock,     ///< Time and date (the default).
        Stopwatch, ///< Elapsed time with laps.
        Timer,     ///< Countdown to zero.
        World      ///< Local time plus the configured zones.
    };

    /** Exit status returned on a normal shutdown. */
    static const int EXIT_OK;

    /** Exit status returned when startup failed. */
    static const int EXIT_STARTUP_FAILED;

    Application();

    /**
     * @brief Shuts down cleanly if run() did not.
     */
    ~Application();

    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    /**
     * @brief Performs the full startup sequence.
     *
     * Every stage degrades gracefully: a missing configuration file, an
     * unknown theme and an unwritable log all produce a warning rather than a
     * failure, in line with FR-006 and TC-010, TC-013 and TC-024.
     *
     * @param configPath Configuration file to load.
     * @return true when the application is ready to run. Only a failure to
     *         prepare the terminal is fatal.
     */
    bool initialize(const std::string &configPath = defaultConfigPath());

    /**
     * @brief Runs the refresh loop until a shutdown is requested.
     *
     * The loop redraws at the configured interval and remains responsive to
     * Q and Ctrl+C throughout, because it sleeps in short slices rather than
     * for the whole interval at once.
     *
     * @return int EXIT_OK on a normal shutdown.
     */
    int run();

    /**
     * @brief Releases resources and restores the terminal (FR-009).
     *
     * Safe to call more than once.
     */
    void shutdown();

    /**
     * @brief Asks the refresh loop to stop at the next opportunity.
     */
    void requestStop();

    /**
     * @brief Reports whether the refresh loop is still running.
     * @return true while the loop should continue.
     */
    bool isRunning() const;

    /**
     * @brief Draws exactly one frame.
     *
     * Separated from run() so a single frame can be rendered and inspected
     * without entering the loop.
     */
    void renderFrame();

    /**
     * @brief Returns the configured refresh interval.
     * @return int Interval in milliseconds.
     */
    int refreshInterval() const;

    /**
     * @brief Returns the active mode.
     * @return Mode Current mode.
     */
    Mode mode() const;

    /**
     * @brief Switches to a mode.
     * @param mode Mode to activate.
     */
    void setMode(Mode mode);

    /**
     * @brief Advances to the next mode, wrapping around.
     * @return Mode The newly active mode.
     */
    Mode cycleMode();

    /**
     * @brief Returns the stopwatch.
     * @return Stopwatch& The stopwatch, running or not.
     */
    Stopwatch &stopwatch();

    /**
     * @brief Returns the countdown timer.
     * @return CountdownTimer& The timer, running or not.
     */
    CountdownTimer &timer();

    /**
     * @brief Returns the configured additional time zones.
     * @return WorldClock& The world clock.
     */
    WorldClock &world();

    /**
     * @brief Returns a monotonic reading for the time-based components.
     *
     * Uses std::chrono::steady_clock, so adjusting the system clock cannot
     * corrupt a running stopwatch or countdown.
     *
     * @return std::int64_t Milliseconds since an unspecified epoch.
     */
    static std::int64_t monotonicNow();

    /**
     * @brief Returns the name of a mode, for display.
     * @param mode Mode to name.
     * @return std::string Name such as "Stopwatch".
     */
    static std::string modeName(Mode mode);

    /**
     * @brief Returns the alarm manager.
     * @return AlarmManager& The configured alarms.
     */
    AlarmManager &alarms();

    /**
     * @brief Returns the notifier.
     * @return Notifier& The alert composer.
     */
    Notifier &notifier();

    /**
     * @brief Snoozes the ringing alarm, if one is ringing.
     * @return true if an alarm was snoozed.
     */
    bool snoozeAlarm();

    /**
     * @brief Dismisses the ringing alarm, if one is ringing.
     * @return true if an alarm was dismissed.
     */
    bool dismissAlarm();

    /**
     * @brief Returns the default configuration file path.
     * @return std::string @c Config/config.ini.
     */
    static std::string defaultConfigPath();

    /**
     * @brief Installs the SIGINT and SIGTERM handlers.
     *
     * Called by initialize(); exposed so a host that manages its own signals
     * can choose not to use it.
     */
    static void installSignalHandlers();

    /**
     * @brief Reports whether a termination signal has been received.
     * @return true once SIGINT or SIGTERM has arrived.
     */
    static bool signalReceived();

private:
    /**
     * @brief Applies logging settings from the configuration.
     */
    void configureLogging();

    /**
     * @brief Applies formatting settings from the configuration.
     */
    void configureFormats();

    /**
     * @brief Loads the configured theme, falling back to the default.
     */
    void configureTheme();

    /**
     * @brief Fills the status bar with the current runtime settings.
     */
    void configureStatusBar();

    /**
     * @brief Loads alarms and applies the alarm-related settings.
     */
    void configureAlarms();

    /**
     * @brief Fires, re-signals and clears alarms for the current frame.
     */
    void updateAlarms();

    /**
     * @brief Checks the countdown timer for expiry.
     * @param nowMs Current monotonic reading.
     */
    void updateTimer(std::int64_t nowMs);

    /**
     * @brief Draws the alert panel and keeps an unacknowledged alert ringing.
     *
     * Separate from updateAlarms() because an alert can be raised by the
     * countdown as well, and must still be shown when alarms are switched off.
     */
    void updateAlert();

    /**
     * @brief Applies the timer settings from the configuration.
     */
    void configureTimer();

    /**
     * @brief Loads the additional time zones from the configuration.
     */
    void configureZones();

    /**
     * @brief Handles a keystroke.
     *
     * @param key Character read from the console.
     * @return true if the key was consumed and the frame should be redrawn
     *         immediately rather than waiting out the interval.
     */
    bool handleKey(int key);

    /**
     * @brief Returns the footer hint for the active mode.
     * @return std::string Key hint text.
     */
    std::string footerHint() const;

    /**
     * @brief Sleeps for the refresh interval while polling for a quit key.
     *
     * @return true if the loop should continue, false if the user asked to
     *         exit.
     */
    bool waitForNextFrame();

    Logger logger;               ///< Runtime event log.
    ConfigurationManager config; ///< User settings.
    ResourceManager resources;   ///< External asset loader.
    ThemeManager theme;          ///< Element colours.
    Clock clock;                 ///< Current time.
    Date date;                   ///< Current date.
    TimeFormatter formatter;     ///< Applies the configured formats.
    AlarmManager alarmManager;   ///< Configured alarms.
    Stopwatch elapsedTimer;      ///< Stopwatch state.
    CountdownTimer countdown;    ///< Countdown timer state.
    WorldClock zones;            ///< Additional time zones.
    Notifier alertNotifier;      ///< Composes the alarm alert panel.
    Display display;             ///< Presentation layer.

    int interval;     ///< Refresh interval in milliseconds.
    bool alarmsEnabled; ///< Whether alarm checking is active.
    bool alertRaised;   ///< Whether this frame raised the alert.
    Mode currentMode;   ///< What the main readout is showing.
    bool running;     ///< Whether the refresh loop should continue.
    bool initialized; ///< Whether initialize() completed.
};

#endif // APPLICATION_HPP

#include "Application.hpp"

#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>

#include "TimeZone.hpp"
#include "Utility.hpp"
#include "Version.hpp"

namespace
{
    /**
     * Set from the signal handler, polled by the refresh loop. Only
     * assignment to an atomic flag is performed here, which is all a signal
     * handler is permitted to do safely.
     */
    std::atomic<bool> terminationRequested(false);

    /** Default refresh interval when the configuration does not supply one. */
    const int DEFAULT_INTERVAL = 1000;

    /** Fastest accepted refresh interval, in milliseconds. */
    const int MINIMUM_INTERVAL = 50;

    /** Slowest accepted refresh interval, in milliseconds (one minute). */
    const int MAXIMUM_INTERVAL = 60000;

    /** Granularity of the sleep loop, so quit keys stay responsive. */
    const int POLL_SLICE_MS = 40;

    /** How long a status message stays on screen before clearing itself. */
    const std::int64_t MESSAGE_DURATION_MS = 3000;

    /**
     * @brief Records a termination request.
     * @param signalNumber Signal that was raised.
     */
    extern "C" void handleSignal(int signalNumber)
    {
        (void)signalNumber;

        terminationRequested = true;
    }

    /**
     * @brief Renders a refresh interval for the status bar.
     * @param milliseconds Interval in milliseconds.
     * @return std::string Human-readable description.
     */
    std::string describeInterval(int milliseconds)
    {
        if (milliseconds == 1000)
            return "1 Second";

        if (milliseconds % 1000 == 0)
            return std::to_string(milliseconds / 1000) + " Seconds";

        return std::to_string(milliseconds) + " ms";
    }
}

const int Application::EXIT_OK = 0;
const int Application::EXIT_STARTUP_FAILED = 1;

Application::Application()
    : interval(DEFAULT_INTERVAL),
      alarmsEnabled(true),
      alertRaised(false),
      currentPlugin(0),
      unknownKeys(0),
      messageUntilMs(0),
      currentMode(Mode::Clock),
      running(false),
      initialized(false)
{
}

Application::~Application()
{
    shutdown();
}

std::string Application::defaultConfigPath()
{
    return "Config/config.ini";
}

void Application::installSignalHandlers()
{
    terminationRequested = false;

    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);
}

bool Application::signalReceived()
{
    return terminationRequested.load();
}

bool Application::initialize(const std::string &path)
{
    installSignalHandlers();

    // Kept so reloadConfiguration() knows which file to re-read.
    configPath = path;

    /*
    Stage 1 - Logger.

    Opened at the default location first so that any problem loading the
    configuration itself can be recorded. configureLogging() reopens it later
    if the configuration names a different file.
    */
    const bool logOpened = logger.initialize("Logs/application.log");

    logger.info(
        std::string(Version::NAME) + " v" + Version::VERSION + " starting.");

    // Stage 2 - Configuration.
    const std::string resolvedConfig = resources.exists(configPath)
                                           ? resources.resolve(configPath)
                                           : configPath;

    if (config.load(resolvedConfig))
    {
        logger.info("Configuration loaded from " + resolvedConfig + ".");
    }
    else
    {
        // A missing configuration is not fatal; defaults apply (TC-010).
        logger.warning(
            "Configuration file '" + configPath +
            "' could not be read. Using default settings.");
    }

    configureLogging();

    if (!logOpened && logger.isOpen())
        logger.warning("Default log file was unavailable at startup.");

    validateConfiguration();

    interval = config.getInt(
        "RefreshInterval",
        DEFAULT_INTERVAL,
        MINIMUM_INTERVAL,
        MAXIMUM_INTERVAL);

    if (config.hasValue("RefreshInterval") &&
        interval != Utility::toInt(config.getValue("RefreshInterval"), interval))
    {
        // The configured value was rejected by validation (TC-022).
        logger.warning(
            "Refresh interval '" + config.getValue("RefreshInterval") +
            "' is invalid. Using " + std::to_string(interval) + " ms.");
    }

    configureFormats();

    // Stage 3 - Resources.
    const std::string bannerPath =
        config.getValue("Banner", "Resources/banner.txt");

    if (!display.loadBanner(bannerPath))
    {
        // A missing banner is not fatal; the built-in artwork is used (TC-017).
        logger.warning(
            "Banner resource '" + bannerPath +
            "' was not found. Using the built-in banner.");
    }
    else
    {
        logger.info("Banner loaded from " + bannerPath + ".");
    }

    // Stage 4 - Theme.
    configureTheme();

    // Stage 4b - Alarms (v1.1.0) and the timer (v1.2.0).
    configureAlarms();
    configureTimer();
    configureZones();
    configurePlugins();

    // Stages 5 and 7 - Console and Display.
    display.initialize(theme);

    if (!display.console().isInitialized())
    {
        logger.error("Console could not be initialized.");
        return false;
    }

    if (!theme.isColorEnabled())
        logger.info("Colour output disabled; terminal does not support it.");

    // Stage 6 - Core objects.
    if (!clock.update())
        logger.error("System time could not be read at startup.");

    if (!date.update())
        logger.error("System date could not be read at startup.");

    configureStatusBar();

    logger.info(
        "Refresh interval: " + std::to_string(interval) + " ms.");

    logger.info("Initialization complete.");

    initialized = true;

    return true;
}

std::vector<std::string> Application::recognisedKeys()
{
    return {
        "TimeFormat",
        "DateFormat",
        "Theme",
        "RefreshInterval",
        "Banner",
        "TimeZones",
        "Alarms",
        "AlarmFile",
        "SnoozeMinutes",
        "AlarmBell",
        "TimerDuration",
        "Logging",
        "LogFile",
        "LogLevel",
        "ConsoleLog",
        "Plugins",
        "PluginDirectory"};
}

void Application::validateConfiguration()
{
    const std::vector<std::string> unrecognised =
        config.unknownKeys(recognisedKeys());

    unknownKeys = unrecognised.size();

    /*
    Worth reporting rather than ignoring. An unrecognised key is almost always
    a typo, and its effect is that the default silently applies: "Them=Light"
    looks like a working configuration right up until the theme does not
    change, with nothing anywhere to say why.
    */
    for (const std::string &key : unrecognised)
    {
        logger.warning(
            "Configuration key '" + key +
            "' is not recognised and has no effect.");
    }
}

std::size_t Application::unknownKeyCount() const
{
    return unknownKeys;
}

void Application::setTransientMessage(const std::string &text,
                                      std::int64_t nowMs)
{
    display.setMessage(text);

    messageUntilMs = nowMs + MESSAGE_DURATION_MS;
}

std::string Application::cycleTheme()
{
    const std::string name = theme.cycleTheme();

    display.setStatusField("Theme", name);
    setTransientMessage("Theme: " + name, monotonicNow());

    logger.info("Theme changed to " + name + ".");

    return name;
}

bool Application::toggleTimeFormat()
{
    const bool wasTwelve =
        formatter.timeFormat() == TimeFormatter::TimeFormat::Hour12;

    formatter.setTimeFormat(
        wasTwelve ? TimeFormatter::TimeFormat::Hour24
                  : TimeFormatter::TimeFormat::Hour12);

    const bool nowTwelve = !wasTwelve;

    setTransientMessage(
        nowTwelve ? "12-hour clock" : "24-hour clock", monotonicNow());

    logger.debug(
        std::string("Clock format switched to ") +
        (nowTwelve ? "12-hour." : "24-hour."));

    return nowTwelve;
}

bool Application::reloadConfiguration()
{
    if (!initialized)
        return false;

    const std::string resolved = resources.exists(configPath)
                                     ? resources.resolve(configPath)
                                     : configPath;

    if (!config.load(resolved))
    {
        logger.warning(
            "Configuration '" + configPath + "' could not be re-read.");

        setTransientMessage("Reload failed: " + configPath, monotonicNow());

        return false;
    }

    configureLogging();
    validateConfiguration();

    interval = config.getInt(
        "RefreshInterval",
        DEFAULT_INTERVAL,
        MINIMUM_INTERVAL,
        MAXIMUM_INTERVAL);

    configureFormats();
    configureTheme();
    configureAlarms();
    configureTimer();
    configureZones();

    /*
    World mode has nothing to show if the zones were removed, and the mode
    cycle would no longer offer a way out of it.
    */
    if (currentMode == Mode::World && zones.count() == 0)
        setMode(Mode::Clock);

    configureStatusBar();

    setTransientMessage("Configuration reloaded", monotonicNow());

    logger.info("Configuration reloaded from " + resolved + ".");

    return true;
}

void Application::configureLogging()
{
    const bool loggingEnabled = config.getBool("Logging", true);

    const std::string logFile =
        config.getValue("LogFile", "Logs/application.log");

    if (!loggingEnabled)
    {
        logger.info("Logging disabled by configuration.");
        logger.close();
        return;
    }

    // Reopen only when the configuration points somewhere else.
    if (logFile != "Logs/application.log" || !logger.isOpen())
    {
        if (!logger.initialize(logFile))
        {
            // Logging is a support service; its absence must not stop the app.
            logger.close();
        }
    }

    logger.setMinimumLevel(
        Logger::levelFromString(config.getValue("LogLevel", "INFO")));

    logger.setConsoleOutput(config.getBool("ConsoleLog", false));
}

void Application::configureFormats()
{
    formatter.setTimeFormat(
        TimeFormatter::timeFormatFromString(
            config.getValue("TimeFormat", "24")));

    formatter.setDateFormat(
        TimeFormatter::dateFormatFromString(
            config.getValue("DateFormat", "LONG")));
}

void Application::configureTheme()
{
    const std::string requested = config.getValue("Theme", "Dark");

    if (theme.loadTheme(requested))
    {
        logger.info("Theme '" + theme.currentTheme() + "' loaded.");
        return;
    }

    // Unknown or unreadable theme: the default is already in place (TC-013).
    logger.warning(
        "Theme '" + requested +
        "' is unavailable. The default theme has been applied.");
}

void Application::configureStatusBar()
{
    display.setStatusField("Theme", theme.currentTheme());
    display.setStatusField("Refresh Rate", describeInterval(interval));
    display.setStatusField("Status", "Running");

    display.setStatusField("Mode", modeName(currentMode));

    if (alarmsEnabled && alarmManager.count() > 0)
        display.setStatusField("Next Alarm", "-");

    /*
    Shown on screen as well as logged, because logging can be switched off in
    the very file the typo is in -- which is exactly when the warning matters.
    */
    if (unknownKeys > 0)
    {
        display.setStatusField(
            "Config",
            std::to_string(unknownKeys) + " unrecognised key(s)");
    }

    display.screen().setFooterHint(footerHint());
}

void Application::configureAlarms()
{
    alarmsEnabled = config.getBool("Alarms", true);

    alertNotifier.setBellEnabled(config.getBool("AlarmBell", true));

    alarmManager.setSnoozeMinutes(
        config.getInt(
            "SnoozeMinutes",
            AlarmManager::DEFAULT_SNOOZE_MINUTES,
            1,
            240));

    if (!alarmsEnabled)
    {
        logger.info("Alarms disabled by configuration.");
        return;
    }

    const std::string alarmFile =
        config.getValue("AlarmFile", "Config/alarms.ini");

    if (!alarmManager.load(alarmFile))
    {
        // No alarm file is a normal state, not a failure.
        logger.info(
            "No alarm file at '" + alarmFile + "'; running without alarms.");
        return;
    }

    logger.info(
        "Loaded " + std::to_string(alarmManager.count()) +
        " alarm(s) from " + alarmFile + ".");

    if (alarmManager.invalidCount() > 0)
    {
        logger.warning(
            std::to_string(alarmManager.invalidCount()) +
            " alarm entr(y/ies) could not be parsed and were skipped.");
    }

    for (std::size_t index = 0; index < alarmManager.count(); ++index)
        logger.debug("Alarm: " + alarmManager.at(index).describe());
}

void Application::configureTimer()
{
    /*
    setDuration() resets the countdown, so applying a reloaded duration to a
    run in progress would move the finish line under the user. The new value
    takes effect at the next reset.
    */
    if (countdown.isRunning())
    {
        logger.info(
            "Timer duration not applied: a countdown is running.");
        return;
    }

    const std::string configured = config.getValue("TimerDuration", "05:00");

    std::int64_t milliseconds = 0;

    if (CountdownTimer::parseDuration(configured, milliseconds))
    {
        countdown.setDuration(milliseconds);
        return;
    }

    logger.warning(
        "Timer duration '" + configured +
        "' is invalid. Using " +
        CountdownTimer::format(CountdownTimer::DEFAULT_DURATION_MS) + ".");

    countdown.setDuration(CountdownTimer::DEFAULT_DURATION_MS);
}

void Application::configureZones()
{
    const std::string configured = config.getValue("TimeZones", "");

    if (Utility::trim(configured).empty())
        return;

    if (!zones.load(configured))
    {
        logger.warning(
            "No usable time zones in '" + configured + "'.");
        return;
    }

    logger.info(
        "Loaded " + std::to_string(zones.count()) + " time zone(s).");

    if (zones.invalidCount() > 0)
    {
        logger.warning(
            std::to_string(zones.invalidCount()) +
            " time zone entr(y/ies) could not be parsed and were skipped.");
    }

    if (zones.unresolvedCount() > 0)
    {
        // Worth a warning rather than a silent UTC: a named zone the platform
        // cannot resolve would otherwise display the wrong time under the
        // right label.
        logger.warning(
            std::to_string(zones.unresolvedCount()) +
            " time zone(s) cannot be resolved on this platform and will "
            "display as UTC.");
    }
}

void Application::updateTimer(std::int64_t nowMs)
{
    if (!countdown.poll(nowMs))
        return;

    logger.info("Countdown timer finished.");

    alertNotifier.notify(
        "TIMER  " + CountdownTimer::format(countdown.duration()),
        "Countdown finished",
        "[D] Dismiss   [R] Reset");

    alertRaised = true;
}

void Application::updateAlarms()
{
    if (!alarmsEnabled)
        return;

    // Fire at most one alarm per frame; poll() handles the once-per-minute rule.
    if (alarmManager.poll(clock, date))
    {
        const Alarm *ringing = alarmManager.active();

        if (ringing != nullptr)
        {
            logger.info("Alarm fired: " + ringing->describe());

            alertNotifier.notify(*ringing, alarmManager.snoozeMinutes());
            display.setStatusField("Status", "ALARM");

            alertRaised = true;
        }
    }

    if (alarmManager.count() > 0)
    {
        const std::string next = alarmManager.nextSummary(clock, date);

        display.setStatusField("Next Alarm", next.empty() ? "none" : next);
    }
}

void Application::updateAlert()
{
    /*
    The alert panel belongs to whoever raised it, not to the alarm subsystem.
    Drawing it from updateAlarms() meant a countdown that finished while
    alarms were switched off was never shown: the timer expired, the bell
    rang once, and the screen said nothing (DEF-009).

    Drawing it here also removes a one-frame lag, because updateTimer() runs
    after updateAlarms() and its alert used to wait for the next frame.
    */
    if (alertNotifier.isActive() && !alertRaised)
    {
        // Keep signalling while the alert waits to be acknowledged. Skipped on
        // the frame it was raised, where notify() has already rung.
        alertNotifier.pulse();
    }

    alertRaised = false;

    display.showNotification(
        alertNotifier.lines(display.screen().width()));
}

bool Application::snoozeAlarm()
{
    if (!alarmManager.isRinging())
        return false;

    const Alarm *ringing = alarmManager.active();
    const std::string description =
        (ringing != nullptr) ? ringing->describe() : "";

    if (!alarmManager.snoozeActive(clock))
        return false;

    alertNotifier.clear();
    display.clearNotification();
    display.setStatusField("Status", "Running");

    logger.info(
        "Alarm snoozed for " + std::to_string(alarmManager.snoozeMinutes()) +
        " minutes: " + description);

    return true;
}

bool Application::dismissAlarm()
{
    if (!alarmManager.isRinging())
        return false;

    const Alarm *ringing = alarmManager.active();
    const std::string description =
        (ringing != nullptr) ? ringing->describe() : "";

    if (!alarmManager.dismissActive())
        return false;

    alertNotifier.clear();
    display.clearNotification();
    display.setStatusField("Status", "Running");

    logger.info("Alarm dismissed: " + description);

    return true;
}

std::int64_t Application::monotonicNow()
{
    const auto now = std::chrono::steady_clock::now().time_since_epoch();

    return std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}

std::string Application::modeName(Mode mode)
{
    switch (mode)
    {
    case Mode::Stopwatch:
        return "Stopwatch";

    case Mode::Timer:
        return "Timer";

    case Mode::World:
        return "World";

    case Mode::Plugin:
        return "Plugin";

    case Mode::Clock:
        break;
    }

    return "Clock";
}

Application::Mode Application::mode() const
{
    return currentMode;
}

void Application::setMode(Mode mode)
{
    currentMode = mode;

    /*
    A plugin's own name, not the word "Plugin": with two loaded, the status
    bar has to say which one is on screen.
    */
    if (mode == Mode::Plugin && pluginManager.count() > 0)
        display.setStatusField("Mode", pluginManager.nameAt(currentPlugin));
    else
        display.setStatusField("Mode", modeName(mode));

    display.screen().setFooterHint(footerHint());
}

Application::Mode Application::cycleMode()
{
    switch (currentMode)
    {
    case Mode::Clock:
        setMode(Mode::Stopwatch);
        break;

    case Mode::Stopwatch:
        setMode(Mode::Timer);
        break;

    case Mode::Timer:
        // Skip the world clock when no extra zones are configured; an empty
        // mode is a dead step in the cycle.
        if (zones.count() > 0)
            setMode(Mode::World);
        else if (pluginManager.count() > 0)
        {
            currentPlugin = 0;
            setMode(Mode::Plugin);
        }
        else
            setMode(Mode::Clock);
        break;

    case Mode::World:
        // Likewise for plugins: none loaded means no plugin step.
        if (pluginManager.count() > 0)
        {
            currentPlugin = 0;
            setMode(Mode::Plugin);
        }
        else
            setMode(Mode::Clock);
        break;

    case Mode::Plugin:
        /*
        Each plugin is its own step in the cycle rather than a submenu, so
        M keeps meaning the same thing however many are installed.
        */
        if (currentPlugin + 1 < pluginManager.count())
        {
            ++currentPlugin;
            setMode(Mode::Plugin);
        }
        else
        {
            currentPlugin = 0;
            setMode(Mode::Clock);
        }
        break;
    }

    return currentMode;
}

Stopwatch &Application::stopwatch()
{
    return elapsedTimer;
}

CountdownTimer &Application::timer()
{
    return countdown;
}

WorldClock &Application::world()
{
    return zones;
}

std::string Application::footerHint() const
{
    /*
    T and C work in every mode, but the stopwatch and timer footers are
    already close to eighty columns, so only the modes with room list them.
    */
    switch (currentMode)
    {
    case Mode::Stopwatch:
        return "[Space] Start/Stop  [L] Lap  [R] Reset  [M] Mode  [T] Theme  "
               "[Q] Quit";

    case Mode::Timer:
        return "[Space] Start/Pause  [R] Reset  [M] Mode  [T] Theme  [Q] Quit";

    case Mode::Plugin:
    {
        const DigitalClockPlugin *plugin = pluginManager.at(currentPlugin);

        if (plugin != nullptr && plugin->footerHint != nullptr)
        {
            const char *hint = plugin->footerHint();

            if (hint != nullptr)
                return hint;
        }

        return "[M] Mode  [T] Theme  [Q] Quit";
    }

    case Mode::World:
    case Mode::Clock:
        break;
    }

    return "[M] Mode  [T] Theme  [F] 12/24  [C] Reload   "
           "Press Q or Ctrl+C to Exit";
}

PluginManager &Application::plugins()
{
    return pluginManager;
}

const DigitalClockPlugin *Application::activePlugin() const
{
    if (currentMode != Mode::Plugin)
        return nullptr;

    return pluginManager.at(currentPlugin);
}

namespace
{
    /*
    The host services lent to every plugin. File-scope because their
    addresses are handed across a C ABI and must outlive any Application:
    a plugin may hold the host pointer until its shutdown().
    */
    Logger *pluginLogger = nullptr;

    extern "C" void pluginLog(const char *message)
    {
        if (pluginLogger != nullptr && message != nullptr)
            pluginLogger->info(std::string("[plugin] ") + message);
    }

    extern "C" long long pluginMonotonicNow()
    {
        return static_cast<long long>(Application::monotonicNow());
    }

    DigitalClockHost pluginHost = {
        DIGITALCLOCK_PLUGIN_ABI, pluginLog, pluginMonotonicNow};
}

void Application::configurePlugins()
{
    if (!config.getBool("Plugins", true))
    {
        logger.info("Plugins disabled by configuration.");
        return;
    }

    if (!PluginManager::isSupported())
    {
        logger.info("Plugins are not supported on this platform.");
        return;
    }

    pluginLogger = &logger;

    const std::string directory = config.getValue("PluginDirectory", "Plugins");

    const std::size_t added =
        pluginManager.loadDirectory(directory, pluginHost);

    if (added > 0)
    {
        logger.info(
            "Loaded " + std::to_string(added) + " plugin(s) from " +
            directory + ".");

        for (std::size_t index = 0; index < pluginManager.count(); ++index)
        {
            const DigitalClockPlugin *plugin = pluginManager.at(index);

            logger.info(
                "Plugin: " + pluginManager.nameAt(index) + " " +
                ((plugin != nullptr && plugin->version != nullptr)
                     ? plugin->version
                     : "?"));
        }
    }

    // Refusals are warnings rather than silence: a plugin that is present
    // but not loaded is exactly the case a user needs told about.
    for (const std::string &reason : pluginManager.rejections())
        logger.warning("Plugin refused: " + reason);
}

AlarmManager &Application::alarms()
{
    return alarmManager;
}

Notifier &Application::notifier()
{
    return alertNotifier;
}

void Application::renderFrame()
{
    const std::int64_t nowMs = monotonicNow();

    clock.update();
    date.update();

    // A status message announces something that just happened, so it clears
    // itself rather than sitting on screen for the rest of the session.
    if (messageUntilMs > 0 && nowMs >= messageUntilMs)
    {
        display.setMessage("");
        messageUntilMs = 0;
    }

    // Alarms and the countdown run regardless of which mode is on screen, so
    // neither is missed while the user is looking at something else.
    updateAlarms();
    updateTimer(nowMs);
    updateAlert();

    switch (currentMode)
    {
    case Mode::Stopwatch:
    {
        display.renderClock(elapsedTimer.formatted(nowMs));

        std::string state = elapsedTimer.isRunning() ? "Running" : "Stopped";

        if (elapsedTimer.lapCount() > 0)
        {
            state += "   Lap " + std::to_string(elapsedTimer.lapCount()) +
                     ": " +
                     Stopwatch::format(
                         elapsedTimer.lapSplit(elapsedTimer.lapCount() - 1));
        }

        display.renderDate("Stopwatch - " + state);
        break;
    }

    case Mode::Timer:
    {
        display.renderClock(countdown.formatted(nowMs));

        std::string state;

        if (countdown.isRunning())
            state = "Running";
        else if (countdown.hasExpired(nowMs))
            state = "Finished";
        else if (countdown.remaining(nowMs) == countdown.duration())
            state = "Ready";
        else
            state = "Paused";

        display.renderDate(
            "Timer - " + state + "   of " +
            CountdownTimer::format(countdown.duration()));
        break;
    }

    case Mode::Plugin:
    {
        const DigitalClockPlugin *plugin = pluginManager.at(currentPlugin);

        /*
        Fixed buffers, pre-terminated. A plugin that writes nothing shows
        nothing rather than whatever the stack held, and one that writes
        too much is truncated rather than trusted with our memory.
        */
        char readout[128] = {0};
        char secondary[192] = {0};

        if (plugin != nullptr && plugin->render != nullptr)
        {
            plugin->render(static_cast<long long>(nowMs),
                           readout, static_cast<int>(sizeof(readout)),
                           secondary, static_cast<int>(sizeof(secondary)));

            readout[sizeof(readout) - 1] = '\0';
            secondary[sizeof(secondary) - 1] = '\0';
        }

        display.renderClock(readout);
        display.renderDate(secondary);
        break;
    }

    case Mode::World:
    {
        display.renderClock(formatter.formatTimeWide(clock));
        display.renderDate(formatter.formatDate(date));
        break;
    }

    case Mode::Clock:
        display.renderClock(formatter.formatTimeWide(clock));
        display.renderDate(formatter.formatDate(date));
        break;
    }

    // The zone rows belong to the world clock only.
    if (currentMode == Mode::World)
    {
        display.showInfoLines(
            zones.rows(
                std::time(nullptr),
                formatter.timeFormat() == TimeFormatter::TimeFormat::Hour12));
    }
    else
    {
        display.showInfoLines({});
    }

    display.renderScreen();
}

bool Application::handleKey(int key)
{
    if (key < 0)
        return false;

    const std::int64_t nowMs = monotonicNow();

    // Acknowledging an alert comes first: whatever the mode, a ringing alarm
    // or a finished countdown is what the user is responding to.
    if (key == 's' || key == 'S')
    {
        if (snoozeAlarm())
            return true;
    }

    if (key == 'd' || key == 'D')
    {
        if (dismissAlarm())
            return true;

        if (countdown.isFinished())
        {
            countdown.acknowledge();
            alertNotifier.clear();
            display.clearNotification();

            logger.info("Countdown acknowledged.");

            return true;
        }
    }

    if (key == 'm' || key == 'M')
    {
        logger.debug("Mode changed to " + modeName(cycleMode()) + ".");
        return true;
    }

    // Theme and reload apply in every mode, so they are handled before the
    // mode-specific keys rather than repeated in each branch.
    if (key == 't' || key == 'T')
    {
        cycleTheme();
        return true;
    }

    if (key == 'c' || key == 'C')
    {
        reloadConfiguration();
        return true;
    }

    if (key == 'f' || key == 'F')
    {
        toggleTimeFormat();
        return true;
    }

    switch (currentMode)
    {
    case Mode::Stopwatch:
        if (key == ' ')
        {
            logger.debug(
                elapsedTimer.toggle(nowMs) ? "Stopwatch started."
                                           : "Stopwatch stopped.");
            return true;
        }

        if (key == 'l' || key == 'L')
            return elapsedTimer.lap(nowMs);

        if (key == 'r' || key == 'R')
        {
            elapsedTimer.reset();
            logger.debug("Stopwatch reset.");
            return true;
        }

        break;

    case Mode::Timer:
        if (key == ' ')
        {
            logger.debug(
                countdown.toggle(nowMs) ? "Timer started." : "Timer paused.");
            return true;
        }

        if (key == 'r' || key == 'R')
        {
            countdown.reset();
            alertNotifier.clear();
            display.clearNotification();

            logger.debug("Timer reset.");

            return true;
        }

        break;

    case Mode::Plugin:
    {
        /*
        Offered last, and only keys the application has not already taken.
        A plugin cannot capture Q, M, T, F, C, S or D, so no plugin can
        leave the user unable to change mode or quit.
        */
        const DigitalClockPlugin *plugin = pluginManager.at(currentPlugin);

        if (plugin != nullptr && plugin->handleKey != nullptr)
            return plugin->handleKey(key, static_cast<long long>(nowMs)) != 0;

        break;
    }

    case Mode::World:
    case Mode::Clock:
        break;
    }

    return false;
}

bool Application::waitForNextFrame()
{
    int remaining = interval;

    while (remaining > 0)
    {
        if (terminationRequested.load())
            return false;

        const int slice = (remaining < POLL_SLICE_MS) ? remaining
                                                      : POLL_SLICE_MS;

        std::this_thread::sleep_for(std::chrono::milliseconds(slice));

        remaining -= slice;

        const int key = display.readKey();

        if (key == 'q' || key == 'Q')
        {
            logger.info("Exit requested by keypress.");
            return false;
        }

        /*
        A consumed key redraws at once. Waiting out the rest of the interval
        would make the stopwatch feel unresponsive: at the default one-second
        refresh, a lap keypress could sit invisible for most of a second.
        */
        if (handleKey(key))
            return true;

        // A running stopwatch needs a faster redraw than the configured
        // interval, or the hundredths would visibly jump.
        if (currentMode == Mode::Stopwatch && elapsedTimer.isRunning())
            return true;
    }

    return !terminationRequested.load();
}

int Application::run()
{
    if (!initialized)
        return EXIT_STARTUP_FAILED;

    running = true;

    logger.info("Entering main refresh loop.");

    while (running)
    {
        renderFrame();

        if (!waitForNextFrame())
            running = false;
    }

    if (terminationRequested.load())
        logger.info("Termination signal received.");

    logger.info("Shutdown requested.");

    shutdown();

    return EXIT_OK;
}

void Application::requestStop()
{
    running = false;
}

bool Application::isRunning() const
{
    return running;
}

int Application::refreshInterval() const
{
    return interval;
}

void Application::shutdown()
{
    if (!initialized)
        return;

    running = false;

    display.setStatusField("Status", "Stopped");
    display.shutdown();

    pluginManager.unloadAll();
    pluginLogger = nullptr;

    logger.info("Resources released.");
    logger.info("Shutdown complete.");
    logger.close();

    initialized = false;
}

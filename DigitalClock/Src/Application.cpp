#include "Application.hpp"

#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>

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

bool Application::initialize(const std::string &configPath)
{
    installSignalHandlers();

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

    // Stage 4b - Alarms (v1.1.0).
    configureAlarms();

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

    if (alarmsEnabled && alarmManager.count() > 0)
        display.setStatusField("Next Alarm", "-");
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
        }
    }
    else if (alertNotifier.isActive())
    {
        // Keep signalling while the alarm waits to be acknowledged.
        alertNotifier.pulse();
    }

    display.showNotification(
        alertNotifier.lines(display.screen().width()));

    if (alarmManager.count() > 0)
    {
        const std::string next = alarmManager.nextSummary(clock, date);

        display.setStatusField("Next Alarm", next.empty() ? "none" : next);
    }
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
    clock.update();
    date.update();

    updateAlarms();

    display.renderClock(formatter.formatTimeWide(clock));
    display.renderDate(formatter.formatDate(date));
    display.renderScreen();
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

        // Acknowledging an alarm redraws at once, rather than waiting for the
        // rest of the interval to elapse.
        if ((key == 's' || key == 'S') && snoozeAlarm())
            return true;

        if ((key == 'd' || key == 'D') && dismissAlarm())
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

    logger.info("Resources released.");
    logger.info("Shutdown complete.");
    logger.close();

    initialized = false;
}

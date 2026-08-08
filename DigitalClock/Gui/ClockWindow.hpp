#ifndef CLOCK_WINDOW_HPP
#define CLOCK_WINDOW_HPP

/******************************************************************************
 * @file ClockWindow.hpp
 * @brief The graphical front end's main window.
 * @author Adarsh Kumar
 * @date 2026
 *
 * This replaces the presentation layer -- Console, Screen, StatusBar, Display
 * and the refresh loop in Application -- and nothing else. The clock, date,
 * formatter, alarms, stopwatch, countdown, time zones, configuration, themes
 * and logging are the same objects the console front end drives, taken from
 * the same digitalclock_core library.
 *
 * That the business logic needed no changes at all to gain a second interface
 * is the return on the layering the project has kept since v1.0.0. It is also
 * why this window holds core objects directly rather than wrapping
 * Application: Application's job is the console lifecycle, and a window that
 * borrowed it would inherit a refresh loop it must not run.
 *
 * Reference: Architecture Document, section 3 (layer separation).
 ******************************************************************************/

#include <cstdint>
#include <ctime>

#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTimer>
#include <QWidget>

#include "AlarmManager.hpp"
#include "Clock.hpp"
#include "ConfigurationManager.hpp"
#include "CountdownTimer.hpp"
#include "Date.hpp"
#include "Logger.hpp"
#include "Notifier.hpp"
#include "ResourceManager.hpp"
#include "Stopwatch.hpp"
#include "ThemeManager.hpp"
#include "TimeFormatter.hpp"
#include "WorldClock.hpp"

/**
 * @class ClockWindow
 * @brief Shows the clock, stopwatch, countdown and world clock in a window.
 */
class ClockWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @enum Mode
     * @brief What the main readout is showing.
     *
     * Deliberately the same four modes as the console front end, in the same
     * order, so the two behave alike.
     */
    enum class Mode
    {
        Clock,
        Stopwatch,
        Timer,
        World
    };

    /**
     * @brief Builds the window and loads the configuration.
     * @param configPath Configuration file to read.
     * @param parent     Parent widget, usually none.
     */
    explicit ClockWindow(const QString &configPath, QWidget *parent = nullptr);

    ~ClockWindow() override;

    /**
     * @brief Returns the active mode.
     */
    Mode mode() const;

    /**
     * @brief Switches to a mode.
     */
    void setMode(Mode mode);

    /**
     * @brief Advances one frame.
     *
     * Exposed so a test or a headless smoke check can drive the window
     * without waiting on a timer.
     */
    void tick();

    /**
     * @brief Returns the configured refresh interval, in milliseconds.
     */
    int refreshInterval() const;

    /**
     * @brief Returns the number of unrecognised configuration keys.
     */
    std::size_t unknownKeyCount() const;

private slots:
    void onTick();
    void onCycleTheme();
    void onToggleTimeFormat();
    void onReloadConfiguration();
    void onPrimaryAction();
    void onResetAction();
    void onStopwatchStartStop();
    void onStopwatchLap();
    void onStopwatchReset();
    void onTimerStartPause();
    void onTimerReset();
    void onSnoozeAlarm();
    void onDismissAlarm();
    void onAbout();

private:
    void buildMenus();
    void buildLayout();
    void applyTheme();
    void applyConfiguration();
    void refreshReadout(std::int64_t nowMs);
    void refreshAlerts();
    void refreshStatus();
    void refreshWorldTable();
    void refreshTimerInterval();
    void showTransient(const QString &text);

    /**
     * @brief Returns a monotonic reading, as Application::monotonicNow() does.
     */
    static std::int64_t monotonicNow();

    // ---- Core objects, shared with the console front end ----
    Logger logger;
    ConfigurationManager config;
    ResourceManager resources;
    ThemeManager theme;
    Clock clock;
    Date date;
    TimeFormatter formatter;
    AlarmManager alarmManager;
    Stopwatch elapsedTimer;
    CountdownTimer countdown;
    WorldClock zones;
    Notifier alertNotifier;

    QString configPath;
    int interval;
    bool alarmsEnabled;
    std::size_t unknownKeys;
    Mode currentMode;
    std::int64_t messageUntilMs;

    // ---- Widgets ----
    QTimer *refreshTimer;

    QVBoxLayout *column;   ///< The window's main column.
    int topStretchIndex;   ///< Stretch above the readout, in @c column.
    int bottomStretchIndex;///< Stretch below the readout.
    int modePagesIndex;    ///< Position of the mode pages.

    QLabel *bannerLabel;
    QLabel *readoutLabel;
    QLabel *secondaryLabel;

    QWidget *alertPanel;
    QLabel *alertLabel;
    QPushButton *snoozeButton;
    QPushButton *dismissButton;

    QStackedWidget *modePages;
    QListWidget *lapList;
    QLabel *timerDurationLabel;
    QTableWidget *worldTable;

    QPushButton *stopwatchStartButton;
    QPushButton *stopwatchLapButton;
    QPushButton *timerStartButton;

    QLabel *statusLabel;
    QLabel *messageLabel;
};

#endif // CLOCK_WINDOW_HPP

#include "ClockWindow.hpp"

#include <chrono>
#include <string>
#include <vector>

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>

#include "GuiTheme.hpp"
#include "TimeZone.hpp"
#include "Utility.hpp"
#include "Version.hpp"

namespace
{
    const int DEFAULT_INTERVAL = 1000;
    const int MINIMUM_INTERVAL = 50;
    const int MAXIMUM_INTERVAL = 60000;

    /** How often to redraw while a stopwatch or countdown is running. */
    const int RUNNING_INTERVAL_MS = 50;

    /** How long a status message stays on screen. */
    const std::int64_t MESSAGE_DURATION_MS = 3000;

    QString toQt(const std::string &text)
    {
        return QString::fromStdString(text);
    }
}

ClockWindow::ClockWindow(const QString &path, QWidget *parent)
    : QMainWindow(parent),
      configPath(path),
      interval(DEFAULT_INTERVAL),
      alarmsEnabled(true),
      unknownKeys(0),
      currentMode(Mode::Clock),
      messageUntilMs(0),
      refreshTimer(nullptr),
      column(nullptr),
      topStretchIndex(0),
      bottomStretchIndex(0),
      modePagesIndex(0),
      bannerLabel(nullptr),
      readoutLabel(nullptr),
      secondaryLabel(nullptr),
      alertPanel(nullptr),
      alertLabel(nullptr),
      snoozeButton(nullptr),
      dismissButton(nullptr),
      modePages(nullptr),
      lapList(nullptr),
      timerDurationLabel(nullptr),
      worldTable(nullptr),
      stopwatchStartButton(nullptr),
      stopwatchLapButton(nullptr),
      timerStartButton(nullptr),
      statusLabel(nullptr),
      messageLabel(nullptr)
{
    logger.initialize("Logs/application.log");
    logger.info(
        std::string(Version::NAME) + " v" + Version::VERSION +
        " starting (graphical interface).");

    buildLayout();
    buildMenus();

    applyConfiguration();

    clock.update();
    date.update();

    setMode(Mode::Clock);

    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &ClockWindow::onTick);
    refreshTimer->start(interval);

    tick();

    setWindowTitle(
        toQt(std::string(Version::NAME) + " " + Version::VERSION));
    resize(720, 560);
}

ClockWindow::~ClockWindow()
{
    logger.info("Shutdown complete.");
    logger.close();
}

std::int64_t ClockWindow::monotonicNow()
{
    const auto now = std::chrono::steady_clock::now().time_since_epoch();

    return std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}

void ClockWindow::buildLayout()
{
    QWidget *central = new QWidget(this);
    column = new QVBoxLayout(central);
    column->setContentsMargins(24, 18, 24, 18);
    column->setSpacing(10);

    bannerLabel = new QLabel(toQt(Version::NAME), central);
    bannerLabel->setAlignment(Qt::AlignCenter);
    column->addWidget(bannerLabel);

    /*
    The readout is centred vertically by a stretch either side of it. Which
    of these actually expands depends on the mode: see setMode(). In clock
    mode there are no controls to show, so without this the clock would sit
    at the top of a mostly empty window.
    */
    column->addStretch(1);
    topStretchIndex = column->count() - 1;

    /*
    A fixed-width font for the readout. A proportional one re-flows as the
    digits change, so the clock would visibly shuffle every second -- the same
    reason the console version pads its fields.
    */
    readoutLabel = new QLabel("00 : 00 : 00", central);
    readoutLabel->setAlignment(Qt::AlignCenter);
    readoutLabel->setFont(
        QFontDatabase::systemFont(QFontDatabase::FixedFont));
    column->addWidget(readoutLabel);

    secondaryLabel = new QLabel(central);
    secondaryLabel->setAlignment(Qt::AlignCenter);
    column->addWidget(secondaryLabel);

    column->addStretch(1);
    bottomStretchIndex = column->count() - 1;

    // ---- Alert panel, hidden until something raises it ----
    alertPanel = new QWidget(central);
    QHBoxLayout *alertRow = new QHBoxLayout(alertPanel);
    alertRow->setContentsMargins(0, 0, 0, 0);

    alertLabel = new QLabel(alertPanel);
    alertLabel->setAlignment(Qt::AlignCenter);
    alertRow->addWidget(alertLabel, 1);

    snoozeButton = new QPushButton("Snooze", alertPanel);
    dismissButton = new QPushButton("Dismiss", alertPanel);
    alertRow->addWidget(snoozeButton);
    alertRow->addWidget(dismissButton);

    connect(snoozeButton, &QPushButton::clicked,
            this, &ClockWindow::onSnoozeAlarm);
    connect(dismissButton, &QPushButton::clicked,
            this, &ClockWindow::onDismissAlarm);

    alertPanel->hide();
    column->addWidget(alertPanel);

    // ---- Per-mode controls ----
    modePages = new QStackedWidget(central);

    // Clock: nothing beyond the readout above.
    modePages->addWidget(new QWidget(modePages));

    // Stopwatch.
    QWidget *stopwatchPage = new QWidget(modePages);
    QVBoxLayout *stopwatchColumn = new QVBoxLayout(stopwatchPage);
    stopwatchColumn->setContentsMargins(0, 0, 0, 0);

    lapList = new QListWidget(stopwatchPage);
    stopwatchColumn->addWidget(lapList);

    QHBoxLayout *stopwatchButtons = new QHBoxLayout();
    stopwatchStartButton = new QPushButton("Start", stopwatchPage);
    stopwatchLapButton = new QPushButton("Lap", stopwatchPage);
    QPushButton *stopwatchResetButton =
        new QPushButton("Reset", stopwatchPage);

    stopwatchButtons->addWidget(stopwatchStartButton);
    stopwatchButtons->addWidget(stopwatchLapButton);
    stopwatchButtons->addWidget(stopwatchResetButton);
    stopwatchColumn->addLayout(stopwatchButtons);

    connect(stopwatchStartButton, &QPushButton::clicked,
            this, &ClockWindow::onStopwatchStartStop);
    connect(stopwatchLapButton, &QPushButton::clicked,
            this, &ClockWindow::onStopwatchLap);
    connect(stopwatchResetButton, &QPushButton::clicked,
            this, &ClockWindow::onStopwatchReset);

    modePages->addWidget(stopwatchPage);

    // Countdown.
    QWidget *timerPage = new QWidget(modePages);
    QVBoxLayout *timerColumn = new QVBoxLayout(timerPage);
    timerColumn->setContentsMargins(0, 0, 0, 0);

    timerDurationLabel = new QLabel(timerPage);
    timerDurationLabel->setAlignment(Qt::AlignCenter);
    timerColumn->addWidget(timerDurationLabel);
    timerColumn->addStretch(1);

    QHBoxLayout *timerButtons = new QHBoxLayout();
    timerStartButton = new QPushButton("Start", timerPage);
    QPushButton *timerResetButton = new QPushButton("Reset", timerPage);

    timerButtons->addWidget(timerStartButton);
    timerButtons->addWidget(timerResetButton);
    timerColumn->addLayout(timerButtons);

    connect(timerStartButton, &QPushButton::clicked,
            this, &ClockWindow::onTimerStartPause);
    connect(timerResetButton, &QPushButton::clicked,
            this, &ClockWindow::onTimerReset);

    modePages->addWidget(timerPage);

    // World clock.
    QWidget *worldPage = new QWidget(modePages);
    QVBoxLayout *worldColumn = new QVBoxLayout(worldPage);
    worldColumn->setContentsMargins(0, 0, 0, 0);

    worldTable = new QTableWidget(0, 3, worldPage);
    worldTable->setHorizontalHeaderLabels({"Zone", "Time", "Offset"});
    worldTable->verticalHeader()->setVisible(false);
    worldTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    worldTable->setSelectionMode(QAbstractItemView::NoSelection);
    // Nothing here is interactive, so it should not take focus or show a
    // current-cell box that looks like a selection the user made.
    worldTable->setFocusPolicy(Qt::NoFocus);
    worldTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    worldColumn->addWidget(worldTable);

    modePages->addWidget(worldPage);

    column->addWidget(modePages, 1);
    modePagesIndex = column->count() - 1;

    // ---- Status rows ----
    statusLabel = new QLabel(central);
    statusLabel->setAlignment(Qt::AlignCenter);
    column->addWidget(statusLabel);

    messageLabel = new QLabel(central);
    messageLabel->setAlignment(Qt::AlignCenter);
    column->addWidget(messageLabel);

    setCentralWidget(central);
}

void ClockWindow::buildMenus()
{
#ifdef Q_OS_LINUX
    /*
    Keep the menu bar inside the window on Linux. Qt otherwise tries to
    export it to a desktop-provided global menu over D-Bus, and where no
    such panel exists -- a bare X session, a headless CI runner, most tiling
    window managers -- the bar is exported to nothing and simply vanishes.
    macOS genuinely has a global menu bar and expects to be given one, so it
    keeps the native behaviour.
    */
    menuBar()->setNativeMenuBar(false);
#endif

    QMenu *fileMenu = menuBar()->addMenu("&File");

    QAction *reloadAction = fileMenu->addAction("&Reload Configuration");
    reloadAction->setShortcut(QKeySequence("C"));
    connect(reloadAction, &QAction::triggered,
            this, &ClockWindow::onReloadConfiguration);

    fileMenu->addSeparator();

    QAction *quitAction = fileMenu->addAction("&Quit");
    quitAction->setShortcuts({QKeySequence("Q"), QKeySequence::Quit});
    connect(quitAction, &QAction::triggered, this, &QWidget::close);

    QMenu *viewMenu = menuBar()->addMenu("&View");

    QAction *themeAction = viewMenu->addAction("Next &Theme");
    themeAction->setShortcut(QKeySequence("T"));
    connect(themeAction, &QAction::triggered,
            this, &ClockWindow::onCycleTheme);

    QAction *formatAction = viewMenu->addAction("12 / 24-hour &Clock");
    formatAction->setShortcut(QKeySequence("F"));
    connect(formatAction, &QAction::triggered,
            this, &ClockWindow::onToggleTimeFormat);

    QMenu *modeMenu = menuBar()->addMenu("&Mode");

    // Exclusive, so the menu always shows which mode is active.
    QActionGroup *modeGroup = new QActionGroup(this);
    modeGroup->setExclusive(true);

    struct Entry
    {
        const char *label;
        Mode mode;
    };

    static const Entry ENTRIES[] = {
        {"&Clock", Mode::Clock},
        {"&Stopwatch", Mode::Stopwatch},
        {"&Timer", Mode::Timer},
        {"&World Clock", Mode::World}};

    for (const Entry &entry : ENTRIES)
    {
        QAction *action = modeMenu->addAction(entry.label);
        action->setCheckable(true);
        action->setChecked(entry.mode == currentMode);
        modeGroup->addAction(action);

        const Mode target = entry.mode;

        connect(action, &QAction::triggered,
                this, [this, target]() { setMode(target); });
    }

    modeMenu->addSeparator();

    QAction *cycleAction = modeMenu->addAction("&Next Mode");
    cycleAction->setShortcut(QKeySequence("M"));

    connect(cycleAction, &QAction::triggered, this,
            [this, modeGroup]()
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
                    // Skip an empty world clock, as the console front end
                    // does: a mode with nothing in it is a dead step.
                    setMode(zones.count() > 0 ? Mode::World : Mode::Clock);
                    break;

                case Mode::World:
                    setMode(Mode::Clock);
                    break;
                }

                const QList<QAction *> actions = modeGroup->actions();
                const int index = static_cast<int>(currentMode);

                if (index < actions.size())
                    actions.at(index)->setChecked(true);
            });

    /*
    The console front end drives everything from the keyboard, and someone
    moving between the two should not have to learn a second set of keys. So
    the same keys are bound here, acting on whichever mode is showing exactly
    as Application::handleKey() does.
    */
    QMenu *controlMenu = menuBar()->addMenu("&Controls");

    QAction *primaryAction = controlMenu->addAction("&Start / Stop");
    primaryAction->setShortcut(QKeySequence(Qt::Key_Space));
    connect(primaryAction, &QAction::triggered,
            this, &ClockWindow::onPrimaryAction);

    QAction *lapAction = controlMenu->addAction("&Lap");
    lapAction->setShortcut(QKeySequence("L"));
    connect(lapAction, &QAction::triggered, this, &ClockWindow::onStopwatchLap);

    QAction *resetAction = controlMenu->addAction("&Reset");
    resetAction->setShortcut(QKeySequence("R"));
    connect(resetAction, &QAction::triggered, this, &ClockWindow::onResetAction);

    controlMenu->addSeparator();

    QAction *snoozeAction = controlMenu->addAction("&Snooze Alarm");
    snoozeAction->setShortcut(QKeySequence("S"));
    connect(snoozeAction, &QAction::triggered,
            this, &ClockWindow::onSnoozeAlarm);

    QAction *dismissAction = controlMenu->addAction("&Dismiss Alarm");
    dismissAction->setShortcut(QKeySequence("D"));
    connect(dismissAction, &QAction::triggered,
            this, &ClockWindow::onDismissAlarm);

    QMenu *helpMenu = menuBar()->addMenu("&Help");

    QAction *aboutAction = helpMenu->addAction("&About");
    connect(aboutAction, &QAction::triggered, this, &ClockWindow::onAbout);
}

void ClockWindow::applyConfiguration()
{
    const std::string path = configPath.toStdString();

    const std::string resolved =
        resources.exists(path) ? resources.resolve(path) : path;

    if (config.load(resolved))
    {
        logger.info("Configuration loaded from " + resolved + ".");
    }
    else
    {
        logger.warning(
            "Configuration file '" + path +
            "' could not be read. Using default settings.");
    }

    // The same key list the console front end validates against, so a typo
    // is reported identically in both.
    const std::vector<std::string> unrecognised =
        config.unknownKeys({"TimeFormat", "DateFormat", "Theme",
                            "RefreshInterval", "Banner", "TimeZones",
                            "Alarms", "AlarmFile", "SnoozeMinutes",
                            "AlarmBell", "TimerDuration", "Logging",
                            "LogFile", "LogLevel", "ConsoleLog"});

    unknownKeys = unrecognised.size();

    for (const std::string &key : unrecognised)
    {
        logger.warning(
            "Configuration key '" + key +
            "' is not recognised and has no effect.");
    }

    interval = config.getInt(
        "RefreshInterval", DEFAULT_INTERVAL, MINIMUM_INTERVAL,
        MAXIMUM_INTERVAL);

    formatter.setTimeFormat(
        TimeFormatter::timeFormatFromString(config.getValue("TimeFormat", "24")));
    formatter.setDateFormat(
        TimeFormatter::dateFormatFromString(
            config.getValue("DateFormat", "LONG")));

    theme.loadTheme(config.getValue("Theme", "Dark"));

    alarmsEnabled = config.getBool("Alarms", true);
    alertNotifier.setBellEnabled(config.getBool("AlarmBell", true));
    alarmManager.setSnoozeMinutes(
        config.getInt("SnoozeMinutes", AlarmManager::DEFAULT_SNOOZE_MINUTES,
                      1, 240));

    if (alarmsEnabled)
        alarmManager.load(config.getValue("AlarmFile", "Config/alarms.ini"));
    else
        alarmManager.clear();

    // Leave a running countdown alone, for the reason given in
    // Application::configureTimer(): setDuration() resets.
    if (!countdown.isRunning())
    {
        std::int64_t milliseconds = 0;

        if (CountdownTimer::parseDuration(
                config.getValue("TimerDuration", "05:00"), milliseconds))
        {
            countdown.setDuration(milliseconds);
        }
        else
        {
            countdown.setDuration(CountdownTimer::DEFAULT_DURATION_MS);
        }
    }

    const std::string configuredZones = config.getValue("TimeZones", "");

    if (!Utility::trim(configuredZones).empty())
        zones.load(configuredZones);
    else
        zones.clear();

    if (currentMode == Mode::World && zones.count() == 0)
        setMode(Mode::Clock);

    applyTheme();
    refreshWorldTable();
}

void ClockWindow::applyTheme()
{
    setStyleSheet(GuiTheme::styleSheet(theme));

    struct Styled
    {
        QLabel *label;
        ThemeManager::Element element;
        int pointSize;
    };

    const Styled STYLED[] = {
        {bannerLabel, ThemeManager::Element::Header, 20},
        {readoutLabel, ThemeManager::Element::Time, 56},
        {secondaryLabel, ThemeManager::Element::Date, 16},
        {statusLabel, ThemeManager::Element::Status, 11},
        {messageLabel, ThemeManager::Element::Footer, 11}};

    for (const Styled &entry : STYLED)
    {
        if (entry.label == nullptr)
            continue;

        QFont font = entry.label->font();
        font.setPointSize(entry.pointSize);
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);

        GuiTheme::applyStyles(theme, entry.element, font);

        entry.label->setFont(font);
        entry.label->setStyleSheet(
            QString("color: %1;")
                .arg(GuiTheme::colorFor(theme, entry.element).name()));
    }

    if (alertLabel != nullptr)
    {
        QFont font = alertLabel->font();
        font.setPointSize(14);
        GuiTheme::applyStyles(theme, ThemeManager::Element::Alert, font);
        alertLabel->setFont(font);

        alertLabel->setStyleSheet(
            QString("color: %1;")
                .arg(GuiTheme::colorFor(theme, ThemeManager::Element::Alert)
                         .name()));
    }

    if (timerDurationLabel != nullptr)
    {
        timerDurationLabel->setStyleSheet(
            QString("color: %1;")
                .arg(GuiTheme::colorFor(theme, ThemeManager::Element::Footer)
                         .name()));
    }
}

ClockWindow::Mode ClockWindow::mode() const
{
    return currentMode;
}

void ClockWindow::setMode(Mode target)
{
    currentMode = target;

    if (modePages != nullptr)
        modePages->setCurrentIndex(static_cast<int>(target));

    /*
    Clock mode has no controls beneath the readout, so the space goes to the
    stretches either side of it and the clock sits in the middle of the
    window. Every other mode has something to show, so the space goes there
    instead and the readout moves up out of its way.
    */
    if (column != nullptr && modePages != nullptr)
    {
        const bool hasControls = (target != Mode::Clock);

        /*
        Hidden, not merely given a stretch factor of zero. A zero factor only
        withholds the *surplus* space; the widget still claims what its size
        policy asks for, which left the clock stranded near the top of the
        window with the empty page below it.
        */
        modePages->setVisible(hasControls);

        column->setStretch(topStretchIndex, hasControls ? 0 : 1);
        column->setStretch(bottomStretchIndex, hasControls ? 0 : 1);
        column->setStretch(modePagesIndex, hasControls ? 1 : 0);
    }

    refreshTimerInterval();
    tick();
}

int ClockWindow::refreshInterval() const
{
    return interval;
}

std::size_t ClockWindow::unknownKeyCount() const
{
    return unknownKeys;
}

void ClockWindow::refreshTimerInterval()
{
    if (refreshTimer == nullptr)
        return;

    /*
    A running stopwatch shows hundredths, which would visibly jump at the
    configured interval. The console front end solves this by returning early
    from its sleep; here the timer itself is retuned, and put back afterwards
    so an idle clock is not redrawn twenty times a second for nothing.
    */
    const bool needsFastRedraw =
        (currentMode == Mode::Stopwatch && elapsedTimer.isRunning()) ||
        (currentMode == Mode::Timer && countdown.isRunning());

    const int wanted = needsFastRedraw
                           ? RUNNING_INTERVAL_MS
                           : interval;

    if (refreshTimer->interval() != wanted)
        refreshTimer->start(wanted);
}

void ClockWindow::onTick()
{
    tick();
}

void ClockWindow::tick()
{
    const std::int64_t nowMs = monotonicNow();

    clock.update();
    date.update();

    if (messageUntilMs > 0 && nowMs >= messageUntilMs)
    {
        if (messageLabel != nullptr)
            messageLabel->clear();

        messageUntilMs = 0;
    }

    // Alarms and the countdown run whichever mode is on screen, so neither is
    // missed while the user is looking at something else.
    if (alarmsEnabled && alarmManager.poll(clock, date))
    {
        const Alarm *ringing = alarmManager.active();

        if (ringing != nullptr)
        {
            logger.info("Alarm fired: " + ringing->describe());
            alertNotifier.notify(*ringing, alarmManager.snoozeMinutes());
        }
    }

    if (countdown.poll(nowMs))
    {
        logger.info("Countdown timer finished.");

        alertNotifier.notify(
            "TIMER  " + CountdownTimer::format(countdown.duration()),
            "Countdown finished",
            "");
    }

    refreshReadout(nowMs);
    refreshAlerts();
    refreshStatus();
    refreshTimerInterval();
}

void ClockWindow::refreshReadout(std::int64_t nowMs)
{
    switch (currentMode)
    {
    case Mode::Stopwatch:
    {
        readoutLabel->setText(toQt(elapsedTimer.formatted(nowMs)));

        secondaryLabel->setText(
            elapsedTimer.isRunning() ? "Stopwatch - Running"
                                     : "Stopwatch - Stopped");

        stopwatchStartButton->setText(
            elapsedTimer.isRunning() ? "Stop" : "Start");
        stopwatchLapButton->setEnabled(elapsedTimer.isRunning());
        break;
    }

    case Mode::Timer:
    {
        readoutLabel->setText(toQt(countdown.formatted(nowMs)));

        QString state;

        if (countdown.isRunning())
            state = "Running";
        else if (countdown.hasExpired(nowMs))
            state = "Finished";
        else if (countdown.remaining(nowMs) == countdown.duration())
            state = "Ready";
        else
            state = "Paused";

        secondaryLabel->setText("Timer - " + state);

        timerDurationLabel->setText(
            "of " + toQt(CountdownTimer::format(countdown.duration())));

        timerStartButton->setText(
            countdown.isRunning() ? "Pause" : "Start");
        break;
    }

    case Mode::World:
    case Mode::Clock:
    {
        readoutLabel->setText(toQt(formatter.formatTimeWide(clock)));
        secondaryLabel->setText(toQt(formatter.formatDate(date)));
        break;
    }
    }

    if (currentMode == Mode::World)
        refreshWorldTable();
}

void ClockWindow::refreshWorldTable()
{
    if (worldTable == nullptr)
        return;

    const std::time_t utc = std::time(nullptr);
    const bool hour12 =
        formatter.timeFormat() == TimeFormatter::TimeFormat::Hour12;

    worldTable->setRowCount(static_cast<int>(zones.count()));

    for (std::size_t index = 0; index < zones.count(); ++index)
    {
        const TimeZone &zone = zones.at(index);
        const int row = static_cast<int>(index);

        const std::string label =
            zone.label().empty() ? zone.name() : zone.label();

        worldTable->setItem(row, 0, new QTableWidgetItem(toQt(label)));

        if (zone.resolves())
        {
            worldTable->setItem(
                row, 1, new QTableWidgetItem(toQt(zone.formatTime(utc, hour12))));
            worldTable->setItem(
                row, 2, new QTableWidgetItem(toQt(zone.formatOffset(utc))));
        }
        else
        {
            /*
            Marked rather than shown as UTC. POSIX treats an unknown TZ as
            UTC, so the alternative is a wrong time under a right-looking
            label -- the same decision the console front end makes.
            */
            worldTable->setItem(
                row, 1,
                new QTableWidgetItem("unavailable on this platform"));
            worldTable->setItem(row, 2, new QTableWidgetItem("-"));
        }
    }
}

void ClockWindow::refreshAlerts()
{
    if (alertPanel == nullptr)
        return;

    if (!alertNotifier.isActive())
    {
        alertPanel->hide();
        return;
    }

    const bool alarmRinging = alarmManager.isRinging();

    QString text = toQt(alertNotifier.heading());

    if (!alertNotifier.label().empty())
        text += "   " + toQt(alertNotifier.label());

    alertLabel->setText(text);

    snoozeButton->setVisible(alarmRinging);
    snoozeButton->setText(
        QString("Snooze %1m").arg(alarmManager.snoozeMinutes()));

    alertPanel->show();
}

void ClockWindow::refreshStatus()
{
    QStringList fields;

    fields << "Theme: " + toQt(theme.currentTheme());

    switch (currentMode)
    {
    case Mode::Stopwatch: fields << "Mode: Stopwatch"; break;
    case Mode::Timer:     fields << "Mode: Timer";     break;
    case Mode::World:     fields << "Mode: World";     break;
    case Mode::Clock:     fields << "Mode: Clock";     break;
    }

    if (alarmsEnabled && alarmManager.count() > 0)
    {
        const std::string next = alarmManager.nextSummary(clock, date);

        fields << "Next Alarm: " + toQt(next.empty() ? "none" : next);
    }

    if (unknownKeys > 0)
    {
        fields << QString("Config: %1 unrecognised key(s)")
                      .arg(static_cast<qulonglong>(unknownKeys));
    }

    statusLabel->setText(fields.join("      "));
}

void ClockWindow::showTransient(const QString &text)
{
    if (messageLabel != nullptr)
        messageLabel->setText(text);

    messageUntilMs = monotonicNow() + MESSAGE_DURATION_MS;
}

void ClockWindow::onCycleTheme()
{
    const std::string name = theme.cycleTheme();

    applyTheme();
    showTransient("Theme: " + toQt(name));

    logger.info("Theme changed to " + name + ".");

    tick();
}

void ClockWindow::onToggleTimeFormat()
{
    const bool wasTwelve =
        formatter.timeFormat() == TimeFormatter::TimeFormat::Hour12;

    formatter.setTimeFormat(wasTwelve ? TimeFormatter::TimeFormat::Hour24
                                      : TimeFormatter::TimeFormat::Hour12);

    showTransient(wasTwelve ? "24-hour clock" : "12-hour clock");

    tick();
}

void ClockWindow::onReloadConfiguration()
{
    applyConfiguration();

    if (refreshTimer != nullptr)
        refreshTimer->start(interval);

    showTransient("Configuration reloaded");

    logger.info("Configuration reloaded.");

    tick();
}

void ClockWindow::onPrimaryAction()
{
    // Space means "the obvious thing for what is on screen", as it does in
    // the console front end.
    switch (currentMode)
    {
    case Mode::Stopwatch:
        onStopwatchStartStop();
        break;

    case Mode::Timer:
        onTimerStartPause();
        break;

    case Mode::World:
    case Mode::Clock:
        break;
    }
}

void ClockWindow::onResetAction()
{
    switch (currentMode)
    {
    case Mode::Stopwatch:
        onStopwatchReset();
        break;

    case Mode::Timer:
        onTimerReset();
        break;

    case Mode::World:
    case Mode::Clock:
        break;
    }
}

void ClockWindow::onStopwatchStartStop()
{
    elapsedTimer.toggle(monotonicNow());

    refreshTimerInterval();
    tick();
}

void ClockWindow::onStopwatchLap()
{
    if (!elapsedTimer.lap(monotonicNow()))
        return;

    const std::size_t count = elapsedTimer.lapCount();

    lapList->insertItem(
        0,
        QString("Lap %1    %2")
            .arg(static_cast<qulonglong>(count))
            .arg(toQt(Stopwatch::format(elapsedTimer.lapSplit(count - 1)))));

    tick();
}

void ClockWindow::onStopwatchReset()
{
    elapsedTimer.reset();
    lapList->clear();

    refreshTimerInterval();
    tick();
}

void ClockWindow::onTimerStartPause()
{
    countdown.toggle(monotonicNow());

    refreshTimerInterval();
    tick();
}

void ClockWindow::onTimerReset()
{
    countdown.reset();
    alertNotifier.clear();

    refreshTimerInterval();
    tick();
}

void ClockWindow::onSnoozeAlarm()
{
    if (!alarmManager.isRinging())
        return;

    if (!alarmManager.snoozeActive(clock))
        return;

    alertNotifier.clear();

    logger.info(
        "Alarm snoozed for " + std::to_string(alarmManager.snoozeMinutes()) +
        " minutes.");

    tick();
}

void ClockWindow::onDismissAlarm()
{
    if (alarmManager.isRinging())
    {
        alarmManager.dismissActive();
        logger.info("Alarm dismissed.");
    }
    else if (countdown.isFinished())
    {
        countdown.acknowledge();
        logger.info("Countdown acknowledged.");
    }

    alertNotifier.clear();

    tick();
}

void ClockWindow::onAbout()
{
    QMessageBox::about(
        this,
        "About " + toQt(Version::NAME),
        toQt(
            std::string(Version::NAME) + " " + Version::VERSION + "\n\n" +
            "A console and graphical digital clock.\n\n"
            "Both interfaces share the same clock, alarms, stopwatch, "
            "countdown, time zones, configuration and themes.\n\n"
            "By " + Version::AUTHOR + "."));
}

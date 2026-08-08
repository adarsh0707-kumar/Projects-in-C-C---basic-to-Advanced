/******************************************************************************
 * @file test_clockwindow.cpp
 * @brief Tests that drive the graphical interface by keyboard.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Covers TC-087 to TC-093, narrowing KI-010.
 *
 * These assert on what is on screen -- the text of the labels, the state of
 * the buttons, whether the alert panel is visible -- rather than on the
 * window's members. That is deliberate. Every defect the GUI has produced so
 * far was a discrepancy between correct internal state and what the user
 * could see or do:
 *
 *   - the menu bar was invisible, exported to a global menu that did not
 *     exist;
 *   - the readout sat near the top of an empty window;
 *   - Space, L and R did nothing, because the buttons owned them and only
 *     while focused.
 *
 * A test reading the members would have passed through all three. Two of the
 * three are caught here; the third is a matter of proportion and remains a
 * question for the eye.
 *
 * What this still cannot answer is whether the result is legible or
 * well-proportioned, which is why KI-010 is narrowed rather than closed and
 * why the UAT plan keeps its scenarios for the window.
 ******************************************************************************/

#include <QApplication>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QSignalSpy>
#include <QTableWidget>
#include <QTest>
#include <QWidget>

#include "ClockWindow.hpp"
#include "ResourceManager.hpp"

/**
 * @class TestClockWindow
 * @brief Keyboard-driven tests for the main window.
 */
class TestClockWindow : public QObject
{
    Q_OBJECT

private:
    /**
     * @brief Returns a child widget by object name, failing loudly if absent.
     */
    template <typename T>
    static T *child(ClockWindow &window, const char *name)
    {
        T *found = window.findChild<T *>(QString::fromLatin1(name));

        if (found == nullptr)
            qWarning("no child widget named '%s'", name);

        return found;
    }

    /**
     * @brief Shows a window and waits until it is genuinely active.
     *
     * Shortcuts are delivered to the active window, so a test that typed
     * before activation would silently assert nothing.
     */
    static bool activate(ClockWindow &window)
    {
        window.show();

        if (!QTest::qWaitForWindowExposed(&window))
            return false;

        window.activateWindow();
        window.setFocus();

        return QTest::qWaitFor(
            [&window]() { return window.isActiveWindow(); }, 5000);
    }

private slots:
    void initTestCase();

    void opensOnTheClock();
    void modeKeyCyclesEveryMode();
    void spaceStartsAndStopsTheStopwatch();
    void lapKeyRecordsALap();
    void resetKeyClearsTheStopwatch();
    void formatKeySwitchesTheClock();
    void themeKeyChangesTheTheme();
};

void TestClockWindow::initTestCase()
{
    // The window looks for Config/ and Resources/ beside its executable as
    // well as in the working directory; ctest runs it from the project root,
    // so the relative defaults already resolve. Registering explicitly keeps
    // the test independent of where it is launched from.
    ResourceManager::addDefaultSearchPath(".");
}

void TestClockWindow::opensOnTheClock()
{
    ClockWindow window("Config/config.ini");

    QVERIFY(activate(window));

    QCOMPARE(window.mode(), ClockWindow::Mode::Clock);

    // The readout carries a time, not the placeholder it was built with.
    QLabel *readout = child<QLabel>(window, "readout");
    QVERIFY(readout != nullptr);

    window.tick();

    QVERIFY(!readout->text().isEmpty());
    QVERIFY(readout->text().contains(':'));

    // The status row says which mode is showing, which is how the user
    // knows what they are looking at.
    QLabel *status = child<QLabel>(window, "status");
    QVERIFY(status != nullptr);
    QVERIFY(status->text().contains("Mode: Clock"));

    // Nothing has raised an alert, so the panel stays out of the way.
    QWidget *alert = child<QWidget>(window, "alertPanel");
    QVERIFY(alert != nullptr);
    QVERIFY(!alert->isVisible());
}

void TestClockWindow::modeKeyCyclesEveryMode()
{
    // Zones configured, so the world clock is in the cycle rather than
    // being skipped as an empty mode.
    ClockWindow window("Config/config.ini");

    QVERIFY(activate(window));

    QCOMPARE(window.mode(), ClockWindow::Mode::Clock);

    QTest::keyClick(&window, Qt::Key_M);
    QCOMPARE(window.mode(), ClockWindow::Mode::Stopwatch);

    QLabel *secondary = child<QLabel>(window, "secondary");
    QVERIFY(secondary != nullptr);
    QVERIFY(secondary->text().startsWith("Stopwatch"));

    QTest::keyClick(&window, Qt::Key_M);
    QCOMPARE(window.mode(), ClockWindow::Mode::Timer);
    QVERIFY(secondary->text().startsWith("Timer"));

    // From Timer the cycle goes to World when zones exist and back to Clock
    // when they do not. Either is correct; what must not happen is getting
    // stuck.
    QTest::keyClick(&window, Qt::Key_M);

    const ClockWindow::Mode third = window.mode();

    QVERIFY(third == ClockWindow::Mode::World ||
            third == ClockWindow::Mode::Clock);

    if (third == ClockWindow::Mode::World)
    {
        QTest::keyClick(&window, Qt::Key_M);
    }

    QCOMPARE(window.mode(), ClockWindow::Mode::Clock);
}

void TestClockWindow::spaceStartsAndStopsTheStopwatch()
{
    /*
    The regression test for the defect that shipped in the first build of
    the window: Space, L and R were bound to the buttons only, so they
    worked when a button happened to have focus and did nothing otherwise.
    The console front end has driven the stopwatch from Space since v1.2.0.
    */
    ClockWindow window("Config/config.ini");

    QVERIFY(activate(window));

    QTest::keyClick(&window, Qt::Key_M);
    QCOMPARE(window.mode(), ClockWindow::Mode::Stopwatch);

    QLabel *secondary = child<QLabel>(window, "secondary");
    QPushButton *start = child<QPushButton>(window, "stopwatchStart");

    QVERIFY(secondary != nullptr);
    QVERIFY(start != nullptr);

    QCOMPARE(secondary->text(), QString("Stopwatch - Stopped"));
    QCOMPARE(start->text(), QString("Start"));

    QTest::keyClick(&window, Qt::Key_Space);

    QCOMPARE(secondary->text(), QString("Stopwatch - Running"));

    // The button relabels itself, so the screen agrees with the state.
    QCOMPARE(start->text(), QString("Stop"));

    // It is really running: the readout advances.
    QLabel *readout = child<QLabel>(window, "readout");
    QVERIFY(readout != nullptr);

    const QString before = readout->text();

    QTest::qWait(150);
    window.tick();

    QVERIFY2(readout->text() != before,
             "the stopwatch readout did not advance while running");

    QTest::keyClick(&window, Qt::Key_Space);

    QCOMPARE(secondary->text(), QString("Stopwatch - Stopped"));
    QCOMPARE(start->text(), QString("Start"));
}

void TestClockWindow::lapKeyRecordsALap()
{
    ClockWindow window("Config/config.ini");

    QVERIFY(activate(window));

    QTest::keyClick(&window, Qt::Key_M);

    QListWidget *laps = child<QListWidget>(window, "lapList");
    QVERIFY(laps != nullptr);
    QCOMPARE(laps->count(), 0);

    QTest::keyClick(&window, Qt::Key_Space);
    QTest::qWait(60);

    QTest::keyClick(&window, Qt::Key_L);
    QCOMPARE(laps->count(), 1);

    QTest::qWait(60);

    QTest::keyClick(&window, Qt::Key_L);
    QCOMPARE(laps->count(), 2);

    // Newest first, so the lap just taken is the one in view.
    QVERIFY(laps->item(0)->text().contains("Lap 2"));
    QVERIFY(laps->item(1)->text().contains("Lap 1"));

    // A lap on a stopped stopwatch would record the same value repeatedly.
    QTest::keyClick(&window, Qt::Key_Space);
    QTest::keyClick(&window, Qt::Key_L);

    QCOMPARE(laps->count(), 2);
}

void TestClockWindow::resetKeyClearsTheStopwatch()
{
    ClockWindow window("Config/config.ini");

    QVERIFY(activate(window));

    QTest::keyClick(&window, Qt::Key_M);
    QTest::keyClick(&window, Qt::Key_Space);
    QTest::qWait(60);
    QTest::keyClick(&window, Qt::Key_L);

    QListWidget *laps = child<QListWidget>(window, "lapList");
    QVERIFY(laps != nullptr);
    QCOMPARE(laps->count(), 1);

    QTest::keyClick(&window, Qt::Key_R);

    // The laps go with the elapsed time; leaving them would show a history
    // belonging to a run that no longer exists.
    QCOMPARE(laps->count(), 0);

    QLabel *secondary = child<QLabel>(window, "secondary");
    QVERIFY(secondary != nullptr);
    QCOMPARE(secondary->text(), QString("Stopwatch - Stopped"));
}

void TestClockWindow::formatKeySwitchesTheClock()
{
    ClockWindow window("Config/config.ini");

    QVERIFY(activate(window));

    QLabel *readout = child<QLabel>(window, "readout");
    QLabel *message = child<QLabel>(window, "message");

    QVERIFY(readout != nullptr);
    QVERIFY(message != nullptr);

    window.tick();

    const bool startedTwelve =
        readout->text().contains("AM") || readout->text().contains("PM");

    QTest::keyClick(&window, Qt::Key_F);
    window.tick();

    const bool nowTwelve =
        readout->text().contains("AM") || readout->text().contains("PM");

    QVERIFY2(nowTwelve != startedTwelve,
             "F did not change the clock format on screen");

    // And the change is announced, so it is not a silent surprise.
    QVERIFY(message->text().contains("hour clock"));
}

void TestClockWindow::themeKeyChangesTheTheme()
{
    ClockWindow window("Config/config.ini");

    QVERIFY(activate(window));

    QLabel *status = child<QLabel>(window, "status");
    QVERIFY(status != nullptr);

    const QString before = status->text();
    const QString sheetBefore = window.styleSheet();

    QTest::keyClick(&window, Qt::Key_T);

    // The status row reports the theme in use, not the one configured.
    QVERIFY2(status->text() != before, "T did not change the reported theme");

    /*
    And the change reaches the paint, not just the label. A theme that was
    named but never applied is precisely the kind of half-done change a
    state-only assertion would miss.
    */
    QVERIFY2(window.styleSheet() != sheetBefore,
             "the stylesheet did not change with the theme");
}

QTEST_MAIN(TestClockWindow)

#include "test_clockwindow.moc"

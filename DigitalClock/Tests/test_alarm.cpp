/******************************************************************************
 * @file test_alarm.cpp
 * @brief Tests for the alarm module introduced in v1.1.0.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Covers TC-026 to TC-041: alarm parsing, recurrence rules, due detection,
 * the once-per-minute firing rule, snooze, dismissal, notification rendering
 * and configuration loading.
 ******************************************************************************/

#include "TestFramework.hpp"

#include "Alarm.hpp"
#include "AlarmManager.hpp"
#include "Notifier.hpp"

namespace
{
    /**
     * @brief Builds a Clock fixed at the given time.
     */
    Clock clockAt(int hour, int minute, int second = 0)
    {
        Clock clock;
        clock.setTime(hour, minute, second);
        return clock;
    }

    /**
     * @brief Builds a Date fixed at the given calendar date.
     */
    Date dateAt(int year, int month, int day)
    {
        Date date;
        date.setDate(year, month, day);
        return date;
    }

    // 2026-08-03 is a Monday; 2026-08-08 is a Saturday.
    const int MONDAY_YEAR = 2026;
    const int MONDAY_MONTH = 8;
    const int MONDAY_DAY = 3;
    const int SATURDAY_DAY = 8;
}

TEST_CASE(TC_026, "Verify alarm parsing from configuration text")
{
    Alarm alarm;

    // Full form: time, label, recurrence.
    CHECK_TRUE(Alarm::parse("07:30 | Wake up | Weekdays", alarm));
    CHECK_EQ(alarm.getHour(), 7);
    CHECK_EQ(alarm.getMinute(), 30);
    CHECK_EQ(alarm.getLabel(), std::string("Wake up"));
    CHECK_TRUE(alarm.getRepeat() == Alarm::Repeat::Weekdays);
    CHECK_TRUE(alarm.isEnabled());

    // Label only; recurrence defaults to Once.
    CHECK_TRUE(Alarm::parse("13:00 | Lunch", alarm));
    CHECK_EQ(alarm.getLabel(), std::string("Lunch"));
    CHECK_TRUE(alarm.getRepeat() == Alarm::Repeat::Once);

    // Time only.
    CHECK_TRUE(Alarm::parse("22:15", alarm));
    CHECK_EQ(alarm.getHour(), 22);
    CHECK_EQ(alarm.getMinute(), 15);
    CHECK_EQ(alarm.getLabel(), std::string(""));

    // Whitespace around the fields is insignificant.
    CHECK_TRUE(Alarm::parse("  06:05  |  Early  |  Daily  ", alarm));
    CHECK_EQ(alarm.getHour(), 6);
    CHECK_EQ(alarm.getMinute(), 5);
    CHECK_EQ(alarm.getLabel(), std::string("Early"));

    // A leading '!' keeps the alarm but disarms it.
    CHECK_TRUE(Alarm::parse("!08:00 | Disabled | Daily", alarm));
    CHECK_FALSE(alarm.isEnabled());
    CHECK_EQ(alarm.getHour(), 8);
}

TEST_CASE(TC_027, "Verify malformed alarm text is rejected")
{
    Alarm alarm;

    CHECK_FALSE(Alarm::parse("", alarm));
    CHECK_FALSE(Alarm::parse("   ", alarm));
    CHECK_FALSE(Alarm::parse("not a time", alarm));
    CHECK_FALSE(Alarm::parse("7:30:00", alarm));
    CHECK_FALSE(Alarm::parse("24:00", alarm));
    CHECK_FALSE(Alarm::parse("07:60", alarm));
    CHECK_FALSE(Alarm::parse("-1:30", alarm));
    CHECK_FALSE(Alarm::parse("07", alarm));
    CHECK_FALSE(Alarm::parse(":30", alarm));
    CHECK_FALSE(Alarm::parse("07:", alarm));

    // An unrecognised recurrence rejects the whole entry rather than
    // silently falling back, so the user learns the file is wrong.
    CHECK_FALSE(Alarm::parse("07:30 | Label | Someday", alarm));

    // Boundary times are valid.
    CHECK_TRUE(Alarm::parse("00:00", alarm));
    CHECK_TRUE(Alarm::parse("23:59", alarm));
}

TEST_CASE(TC_028, "Verify recurrence rules select the correct days")
{
    Alarm daily(7, 0, "d", Alarm::Repeat::Daily);
    Alarm weekdays(7, 0, "w", Alarm::Repeat::Weekdays);
    Alarm weekends(7, 0, "e", Alarm::Repeat::Weekends);

    for (int day = 0; day < 7; ++day)
        CHECK_TRUE(daily.appliesOn(day));

    // 0 is Sunday, 6 is Saturday.
    CHECK_FALSE(weekdays.appliesOn(0));
    CHECK_TRUE(weekdays.appliesOn(1));
    CHECK_TRUE(weekdays.appliesOn(5));
    CHECK_FALSE(weekdays.appliesOn(6));

    CHECK_TRUE(weekends.appliesOn(0));
    CHECK_FALSE(weekends.appliesOn(3));
    CHECK_TRUE(weekends.appliesOn(6));

    // Out-of-range weekdays never match.
    CHECK_FALSE(daily.appliesOn(-1));
    CHECK_FALSE(daily.appliesOn(7));

    // An explicit day list.
    Alarm custom;
    CHECK_TRUE(Alarm::parse("07:00 | Gym | Mon,Wed,Fri", custom));
    CHECK_TRUE(custom.getRepeat() == Alarm::Repeat::Custom);
    CHECK_TRUE(custom.appliesOn(1));
    CHECK_FALSE(custom.appliesOn(2));
    CHECK_TRUE(custom.appliesOn(3));
    CHECK_TRUE(custom.appliesOn(5));
    CHECK_EQ(custom.repeatString(), std::string("Mon,Wed,Fri"));

    // A day list covering exactly a named set reports the friendlier name.
    Alarm named;
    CHECK_TRUE(Alarm::parse("07:00 | x | Sat,Sun", named));
    CHECK_TRUE(named.getRepeat() == Alarm::Repeat::Weekends);
}

TEST_CASE(TC_029, "Verify an alarm is due only at its configured time")
{
    const Date monday = dateAt(MONDAY_YEAR, MONDAY_MONTH, MONDAY_DAY);

    Alarm alarm(7, 30, "Wake up", Alarm::Repeat::Daily);

    CHECK_TRUE(alarm.isDueAt(clockAt(7, 30, 0), monday));

    // Due for the whole minute, regardless of seconds.
    CHECK_TRUE(alarm.isDueAt(clockAt(7, 30, 59), monday));

    CHECK_FALSE(alarm.isDueAt(clockAt(7, 29, 59), monday));
    CHECK_FALSE(alarm.isDueAt(clockAt(7, 31, 0), monday));
    CHECK_FALSE(alarm.isDueAt(clockAt(19, 30, 0), monday));

    // A disarmed alarm is never due.
    alarm.setEnabled(false);
    CHECK_FALSE(alarm.isDueAt(clockAt(7, 30, 0), monday));
}

TEST_CASE(TC_030, "Verify recurrence is honoured by due detection")
{
    const Date monday = dateAt(MONDAY_YEAR, MONDAY_MONTH, MONDAY_DAY);
    const Date saturday = dateAt(MONDAY_YEAR, MONDAY_MONTH, SATURDAY_DAY);

    CHECK_EQ(monday.getWeekday(), 1);
    CHECK_EQ(saturday.getWeekday(), 6);

    Alarm weekdays(7, 30, "Work", Alarm::Repeat::Weekdays);

    CHECK_TRUE(weekdays.isDueAt(clockAt(7, 30), monday));
    CHECK_FALSE(weekdays.isDueAt(clockAt(7, 30), saturday));

    Alarm weekends(9, 0, "Rest", Alarm::Repeat::Weekends);

    CHECK_FALSE(weekends.isDueAt(clockAt(9, 0), monday));
    CHECK_TRUE(weekends.isDueAt(clockAt(9, 0), saturday));
}

TEST_CASE(TC_031, "Verify snooze postpones an alarm by the configured delay")
{
    const Date monday = dateAt(MONDAY_YEAR, MONDAY_MONTH, MONDAY_DAY);

    Alarm alarm(7, 30, "Wake up", Alarm::Repeat::Daily);

    CHECK_FALSE(alarm.isSnoozed());

    CHECK_TRUE(alarm.snooze(7, 30, 5));
    CHECK_TRUE(alarm.isSnoozed());
    CHECK_EQ(alarm.getEffectiveHour(), 7);
    CHECK_EQ(alarm.getEffectiveMinute(), 35);

    // It is no longer due at its original time, but is at the snooze time.
    CHECK_FALSE(alarm.isDueAt(clockAt(7, 30), monday));
    CHECK_TRUE(alarm.isDueAt(clockAt(7, 35), monday));

    // A snooze that crosses the hour, and one that crosses midnight.
    CHECK_TRUE(alarm.snooze(7, 58, 5));
    CHECK_EQ(alarm.getEffectiveHour(), 8);
    CHECK_EQ(alarm.getEffectiveMinute(), 3);

    CHECK_TRUE(alarm.snooze(23, 58, 5));
    CHECK_EQ(alarm.getEffectiveHour(), 0);
    CHECK_EQ(alarm.getEffectiveMinute(), 3);

    // Invalid snoozes are rejected.
    CHECK_FALSE(alarm.snooze(7, 30, 0));
    CHECK_FALSE(alarm.snooze(7, 30, -5));

    alarm.clearSnooze();
    CHECK_FALSE(alarm.isSnoozed());
    CHECK_EQ(alarm.getEffectiveHour(), 7);
    CHECK_EQ(alarm.getEffectiveMinute(), 30);
}

TEST_CASE(TC_032, "Verify AlarmManager loads alarms from a file")
{
    const std::string path = TestFramework::writeTempFile(
        "alarms-good.ini",
        "# comment\n"
        "Alarm1=07:30 | Wake up | Weekdays\n"
        "Alarm2=13:00 | Lunch | Daily\n"
        "Alarm4=22:15 | Gap in numbering | Once\n");

    AlarmManager manager;

    manager.load("");
    CHECK_EQ(manager.count(), static_cast<std::size_t>(0));

    ResourceManager probe;
    CHECK_TRUE(probe.exists(path));

    CHECK_TRUE(manager.load(path));

    // Non-contiguous indices are all picked up.
    CHECK_EQ(manager.count(), static_cast<std::size_t>(3));
    CHECK_EQ(manager.invalidCount(), static_cast<std::size_t>(0));

    CHECK_EQ(manager.at(0).getLabel(), std::string("Wake up"));
    CHECK_EQ(manager.at(1).getLabel(), std::string("Lunch"));
    CHECK_EQ(manager.at(2).getHour(), 22);

    // A missing file is not an error; it means no alarms.
    AlarmManager empty;
    CHECK_FALSE(empty.load("Config/no-such-alarms.ini"));
    CHECK_EQ(empty.count(), static_cast<std::size_t>(0));
}

TEST_CASE(TC_033, "Verify malformed alarm entries are skipped, not fatal")
{
    const std::string path = TestFramework::writeTempFile(
        "alarms-mixed.ini",
        "Alarm1=07:30 | Good | Daily\n"
        "Alarm2=not a time\n"
        "Alarm3=25:00 | Bad hour | Daily\n"
        "Alarm4=09:00 | Also good | Daily\n"
        "Alarm5=\n");

    AlarmManager manager;

    CHECK_TRUE(manager.load(path));

    // The two good entries survive; the two bad ones are counted.
    CHECK_EQ(manager.count(), static_cast<std::size_t>(2));
    CHECK_EQ(manager.invalidCount(), static_cast<std::size_t>(2));

    CHECK_EQ(manager.at(0).getLabel(), std::string("Good"));
    CHECK_EQ(manager.at(1).getLabel(), std::string("Also good"));
}

TEST_CASE(TC_034, "Verify an alarm fires exactly once per occurrence")
{
    const Date monday = dateAt(MONDAY_YEAR, MONDAY_MONTH, MONDAY_DAY);

    AlarmManager manager;
    manager.add(Alarm(7, 30, "Wake up", Alarm::Repeat::Daily));

    CHECK_FALSE(manager.isRinging());

    // Not yet due.
    CHECK_FALSE(manager.poll(clockAt(7, 29, 59), monday));

    // Fires on the first poll of the matching minute.
    CHECK_TRUE(manager.poll(clockAt(7, 30, 0), monday));
    CHECK_TRUE(manager.isRinging());
    CHECK_TRUE(manager.active() != nullptr);
    CHECK_EQ(manager.active()->getLabel(), std::string("Wake up"));

    // Must not fire again while still ringing, nor later in the same minute.
    CHECK_FALSE(manager.poll(clockAt(7, 30, 1), monday));
    CHECK_FALSE(manager.poll(clockAt(7, 30, 59), monday));

    CHECK_TRUE(manager.dismissActive());
    CHECK_FALSE(manager.isRinging());

    // Still must not re-fire in the same minute after being dismissed.
    CHECK_FALSE(manager.poll(clockAt(7, 30, 30), monday));

    // The next day at the same time it fires again.
    const Date tuesday = dateAt(MONDAY_YEAR, MONDAY_MONTH, MONDAY_DAY + 1);
    CHECK_TRUE(manager.poll(clockAt(7, 30, 0), tuesday));
}

TEST_CASE(TC_035, "Verify dismissing a one-shot alarm disarms it")
{
    const Date monday = dateAt(MONDAY_YEAR, MONDAY_MONTH, MONDAY_DAY);

    AlarmManager manager;
    manager.add(Alarm(7, 30, "One off", Alarm::Repeat::Once));
    manager.add(Alarm(8, 0, "Recurring", Alarm::Repeat::Daily));

    CHECK_TRUE(manager.poll(clockAt(7, 30), monday));
    CHECK_TRUE(manager.dismissActive());

    // A one-shot alarm has served its purpose and disarms itself.
    CHECK_FALSE(manager.at(0).isEnabled());

    CHECK_TRUE(manager.poll(clockAt(8, 0), monday));
    CHECK_TRUE(manager.dismissActive());

    // A recurring alarm stays armed for tomorrow.
    CHECK_TRUE(manager.at(1).isEnabled());

    // Dismissing when nothing is ringing is a harmless no-op.
    CHECK_FALSE(manager.dismissActive());
}

TEST_CASE(TC_036, "Verify snoozing through the manager re-arms the alarm")
{
    const Date monday = dateAt(MONDAY_YEAR, MONDAY_MONTH, MONDAY_DAY);

    AlarmManager manager;
    manager.setSnoozeMinutes(5);
    manager.add(Alarm(7, 30, "Wake up", Alarm::Repeat::Daily));

    CHECK_TRUE(manager.poll(clockAt(7, 30), monday));
    CHECK_TRUE(manager.snoozeActive(clockAt(7, 30)));

    // Snoozing stops the ringing but keeps the alarm pending.
    CHECK_FALSE(manager.isRinging());
    CHECK_TRUE(manager.at(0).isSnoozed());

    // It rings again once the snooze expires.
    CHECK_FALSE(manager.poll(clockAt(7, 34), monday));
    CHECK_TRUE(manager.poll(clockAt(7, 35), monday));

    // Dismissing clears the snooze.
    CHECK_TRUE(manager.dismissActive());
    CHECK_FALSE(manager.at(0).isSnoozed());

    // Snoozing when nothing is ringing is a no-op.
    CHECK_FALSE(manager.snoozeActive(clockAt(9, 0)));

    // The snooze delay is configurable and validated.
    manager.setSnoozeMinutes(10);
    CHECK_EQ(manager.snoozeMinutes(), 10);
    manager.setSnoozeMinutes(0);
    CHECK_EQ(manager.snoozeMinutes(), 10);
}

TEST_CASE(TC_037, "Verify only one alarm rings at a time")
{
    const Date monday = dateAt(MONDAY_YEAR, MONDAY_MONTH, MONDAY_DAY);

    AlarmManager manager;
    manager.add(Alarm(7, 30, "First", Alarm::Repeat::Daily));
    manager.add(Alarm(7, 30, "Second", Alarm::Repeat::Daily));

    CHECK_TRUE(manager.poll(clockAt(7, 30), monday));
    CHECK_EQ(manager.active()->getLabel(), std::string("First"));

    // The second must wait rather than replacing the first.
    CHECK_FALSE(manager.poll(clockAt(7, 30), monday));
    CHECK_EQ(manager.active()->getLabel(), std::string("First"));

    CHECK_TRUE(manager.dismissActive());

    // Once the first is acknowledged the second rings.
    CHECK_TRUE(manager.poll(clockAt(7, 30), monday));
    CHECK_EQ(manager.active()->getLabel(), std::string("Second"));
}

TEST_CASE(TC_038, "Verify the next-alarm summary and countdown")
{
    const Date monday = dateAt(MONDAY_YEAR, MONDAY_MONTH, MONDAY_DAY);

    AlarmManager manager;

    // With no alarms there is nothing to report.
    CHECK_EQ(manager.nextSummary(clockAt(6, 0), monday), std::string(""));
    CHECK_EQ(manager.nextIndex(clockAt(6, 0), monday), -1);

    manager.add(Alarm(7, 30, "Wake up", Alarm::Repeat::Daily));
    manager.add(Alarm(13, 0, "Lunch", Alarm::Repeat::Daily));

    // The nearer alarm is reported.
    CHECK_EQ(manager.nextIndex(clockAt(6, 0), monday), 0);
    CHECK_CONTAINS(manager.nextSummary(clockAt(6, 0), monday), "07:30");
    CHECK_CONTAINS(manager.nextSummary(clockAt(6, 0), monday), "Wake up");
    CHECK_CONTAINS(manager.nextSummary(clockAt(6, 0), monday), "1h 30m");

    // After the first has passed, the second is next.
    CHECK_EQ(manager.nextIndex(clockAt(8, 0), monday), 1);
    CHECK_CONTAINS(manager.nextSummary(clockAt(8, 0), monday), "Lunch");

    // Past both, tomorrow's first alarm is next.
    CHECK_EQ(manager.nextIndex(clockAt(20, 0), monday), 0);

    // An alarm due this minute reads as "due now", not "in now".
    CHECK_CONTAINS(manager.nextSummary(clockAt(7, 30), monday), "due now");
    CHECK_NOT_CONTAINS(manager.nextSummary(clockAt(7, 30), monday), "in now");

    // Duration formatting.
    CHECK_EQ(AlarmManager::describeDuration(0), std::string("now"));
    CHECK_EQ(AlarmManager::describeDuration(45), std::string("45m"));
    CHECK_EQ(AlarmManager::describeDuration(90), std::string("1h 30m"));
    CHECK_EQ(AlarmManager::describeDuration(120), std::string("2h"));
    CHECK_EQ(AlarmManager::describeDuration(1500), std::string("1d 1h"));
}

TEST_CASE(TC_041, "Verify the countdown skips an alarm that already fired")
{
    const Date monday = dateAt(MONDAY_YEAR, MONDAY_MONTH, MONDAY_DAY);

    AlarmManager manager;
    manager.add(Alarm(7, 30, "Wake up", Alarm::Repeat::Daily));

    // Before it fires, the alarm is correctly reported as due now.
    CHECK_CONTAINS(manager.nextSummary(clockAt(7, 30), monday), "due now");

    CHECK_TRUE(manager.poll(clockAt(7, 30), monday));
    CHECK_TRUE(manager.dismissActive());

    /*
    Having already rung and been acknowledged, the alarm will not ring again
    until tomorrow. Reporting "due now" for the rest of the minute would tell
    the user an alarm is pending when none is.
    */
    const std::string summary = manager.nextSummary(clockAt(7, 30), monday);

    CHECK_NOT_CONTAINS(summary, "due now");

    // Exactly one day away, since the alarm recurs daily.
    CHECK_CONTAINS(summary, "1d");

    // A minute later the countdown reads as the remainder of that day.
    CHECK_CONTAINS(
        manager.nextSummary(clockAt(7, 31), monday), "23h 59m");

    // A weekday-only alarm dismissed on a Friday points at Monday, not
    // Saturday.
    AlarmManager weekly;
    weekly.add(Alarm(9, 0, "Standup", Alarm::Repeat::Weekdays));

    const Date friday = dateAt(MONDAY_YEAR, MONDAY_MONTH, MONDAY_DAY + 4);
    CHECK_EQ(friday.getWeekday(), 5);

    CHECK_TRUE(weekly.poll(clockAt(9, 0), friday));
    CHECK_TRUE(weekly.dismissActive());

    // Three days on from Friday is Monday: 71 hours and 60 minutes.
    CHECK_CONTAINS(weekly.nextSummary(clockAt(9, 0), friday), "3d");
}

TEST_CASE(TC_039, "Verify the notification panel renders the ringing alarm")
{
    Notifier notifier;

    CHECK_FALSE(notifier.isActive());
    CHECK_EQ(notifier.lines(80).size(), static_cast<std::size_t>(0));

    notifier.setBellEnabled(false);

    Alarm alarm(7, 30, "Wake up", Alarm::Repeat::Daily);
    notifier.notify(alarm, 5);

    CHECK_TRUE(notifier.isActive());
    CHECK_EQ(notifier.time(), std::string("07:30"));
    CHECK_EQ(notifier.label(), std::string("Wake up"));

    const std::vector<std::string> panel = notifier.lines(80);

    CHECK_TRUE(panel.size() >= 4);

    std::string joined;
    for (const std::string &line : panel)
        joined += line + "\n";

    CHECK_CONTAINS(joined, "ALARM");
    CHECK_CONTAINS(joined, "07:30");
    CHECK_CONTAINS(joined, "Wake up");
    CHECK_CONTAINS(joined, "[S] Snooze 5m");
    CHECK_CONTAINS(joined, "[D] Dismiss");

    // Every row is the same width, so the panel draws as a rectangle.
    for (const std::string &line : panel)
        CHECK_EQ(line.size(), panel[0].size());

    notifier.clear();
    CHECK_FALSE(notifier.isActive());
    CHECK_EQ(notifier.lines(80).size(), static_cast<std::size_t>(0));
}

TEST_CASE(TC_040, "Verify the alarm bell is emitted and can be disabled")
{
    Alarm alarm(7, 30, "Wake up", Alarm::Repeat::Daily);

    {
        Notifier notifier;
        notifier.setBellEnabled(true);

        TestFramework::OutputCapture capture;

        notifier.notify(alarm, 5);
        notifier.pulse();
        notifier.pulse();

        const std::string emitted = capture.text();

        CHECK_EQ(notifier.bellCount(), 3);
        CHECK_CONTAINS(emitted, "\a");
    }

    {
        Notifier notifier;
        notifier.setBellEnabled(false);

        TestFramework::OutputCapture capture;

        notifier.notify(alarm, 5);
        notifier.pulse();

        const std::string emitted = capture.text();

        CHECK_EQ(notifier.bellCount(), 0);
        CHECK_NOT_CONTAINS(emitted, "\a");
    }

    // pulse() on an inactive notifier does nothing.
    {
        Notifier notifier;
        notifier.setBellEnabled(true);

        TestFramework::OutputCapture capture;

        notifier.pulse();

        CHECK_EQ(notifier.bellCount(), 0);
    }
}

TEST_CASE(UT_100, "Alarm round-trips through its configuration form")
{
    const char *const SAMPLES[] = {
        "07:30 | Wake up | Weekdays",
        "13:00 | Lunch | Daily",
        "22:15 | Wind down | Mon,Wed,Fri",
        "09:00 | One off | Once",
        "!06:00 | Disabled | Daily"};

    for (const char *sample : SAMPLES)
    {
        Alarm parsed;

        if (!Alarm::parse(sample, parsed))
        {
            FAIL_TEST(std::string("failed to parse: ") + sample);
            continue;
        }

        const std::string rendered = parsed.toConfigString();

        Alarm reparsed;

        if (!Alarm::parse(rendered, reparsed))
        {
            FAIL_TEST("failed to re-parse: " + rendered);
            continue;
        }

        CHECK_EQ(reparsed.getHour(), parsed.getHour());
        CHECK_EQ(reparsed.getMinute(), parsed.getMinute());
        CHECK_EQ(reparsed.getLabel(), parsed.getLabel());
        CHECK_EQ(reparsed.getDayMask(), parsed.getDayMask());
        CHECK_EQ(reparsed.isEnabled(), parsed.isEnabled());
    }
}

TEST_CASE(UT_101, "Alarm describes itself readably")
{
    Alarm alarm(7, 5, "Wake up", Alarm::Repeat::Weekdays);

    CHECK_EQ(alarm.timeString(), std::string("07:05"));
    CHECK_EQ(alarm.repeatString(), std::string("Weekdays"));
    CHECK_EQ(alarm.describe(), std::string("07:05 Weekdays - Wake up"));

    alarm.snooze(7, 5, 10);
    CHECK_CONTAINS(alarm.describe(), "snoozed to 07:15");

    alarm.clearSnooze();
    alarm.setEnabled(false);
    CHECK_CONTAINS(alarm.describe(), "[disabled]");

    // An unlabelled alarm omits the separator rather than trailing it.
    Alarm bare(6, 0, "", Alarm::Repeat::Daily);
    CHECK_EQ(bare.describe(), std::string("06:00 Daily"));

    CHECK_EQ(Alarm::dayAbbreviation(0), std::string("Sun"));
    CHECK_EQ(Alarm::dayAbbreviation(6), std::string("Sat"));
    CHECK_EQ(Alarm::dayAbbreviation(9), std::string(""));
}

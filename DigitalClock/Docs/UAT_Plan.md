# User Acceptance Test Plan

Project: Digital Clock System
Application Version: **1.4.0**
Document Version: **1.0**
Status: **Ready for execution — not yet executed**

---

# Table of Contents

1. Purpose
2. What This Adds to the Automated Suite
3. Who Should Run It
4. Environment
5. Entry Criteria
6. How to Record Results
7. Test Scenarios
8. Exit Criteria
9. Sign-Off
10. Defect Record

---

# 1. Purpose

This plan exists to close **KI-007**, which records that no User Acceptance
Testing has been performed on the Digital Clock System.

It cannot close it on its own. A UAT plan is a set of questions; KI-007 stays
open until somebody sits at a terminal and answers them. What this document
does is make that a short, structured session rather than an open-ended one,
and give the result somewhere to live.

---

# 2. What This Adds to the Automated Suite

The project has 125 automated tests covering TC-001 to TC-081, 92.79% line
coverage, three platforms in CI, and no open defects. It is reasonable to ask
what a person at a terminal adds to that.

The automated suite answers **"does the code do what the developer intended?"**
It is written by the same person who wrote the code, from the same
understanding, so it inherits that understanding's blind spots. Every test in
it passes.

User Acceptance Testing asks a different question: **"is what it does what
someone actually wants?"** No assertion can answer that. A frame can be
composed correctly and still be hard to read. An alarm can fire exactly on
schedule and still be missed because the panel is not where the eye goes. A
configuration file can parse perfectly and still be confusing to edit.

The project has already seen the difference twice, and both times the running
application was the thing that found the defect:

- **DEF-007** — after an alarm was dismissed, the status bar said "due now"
  for the rest of that minute. `AlarmManager::poll()` and
  `Alarm::minutesUntil()` were each correct in isolation, and each had passing
  tests. Only their interaction across frames was wrong.
- **DEF-009** — with `Alarms=Disabled`, a finished countdown rang its bell
  once and displayed nothing at all. Found by TC-069 on its first run, which
  was itself only written because the refresh loop had never been executed by
  a test.

Neither was a coding error in a function. Both were failures of the system to
behave sensibly for a person using it. That is the category of problem this
session is looking for.

**So the scenarios below are deliberately not a re-run of the automated
suite.** Where a scenario overlaps a test case, it is noted, and the question
asked is about the experience rather than the value.

---

# 3. Who Should Run It

Ideally **someone who did not write the software**. The value of UAT comes
almost entirely from unfamiliarity: a person who knows that `M` cycles modes
will never discover that nothing on screen said so.

Two testers are better than one, and they should not watch each other.

| Role | What they bring |
|------|-----------------|
| A user unfamiliar with the project | Whether it is usable without being explained |
| A user who works in a terminal daily | Whether it behaves like a terminal application should |

If only one person is available, the unfamiliar user is the more valuable of
the two.

**Do not brief the tester beyond section 4.** Hand them the extracted archive
and this document. If they need to be told something before they can start,
that is a finding — record it under UAT-001.

---

# 4. Environment

Record what was actually used. "It worked on Linux" is not a result anyone can
act on later.

| Item | Value |
|------|-------|
| Operating system and version | |
| Terminal emulator and version | |
| Terminal size (columns × rows) | |
| Colour support (`echo $TERM`) | |
| Locale (`echo $LANG`) | |
| Application version (`--version`) | |
| Build source | release archive / built from source |
| Tester | |
| Date | |

Use a **release archive** if one is available, not a development build. The
archive is what a user receives, and it is the thing whose resource layout can
be wrong in ways a development tree hides.

---

# 5. Entry Criteria

UAT may begin when all of the following hold. All were true at v1.4.0.

- [ ] The automated suite passes in full (125 of 125).
- [ ] No open defects of any severity.
- [ ] CI is green on Linux, Windows and macOS.
- [ ] The release archive extracts and runs on the target platform.
- [ ] The User Manual matches the shipped keyboard controls.

---

# 6. How to Record Results

For each scenario, record one of:

| Result | Meaning |
|--------|---------|
| **Pass** | Behaved as described, and was not confusing |
| **Pass with comment** | Behaved as described, but something was awkward |
| **Fail** | Did not behave as described |
| **Blocked** | Could not be attempted; say why |
| **N/A** | Does not apply to this platform |

**"Pass with comment" is the most valuable result in this document.** A
scenario that technically works but made the tester hesitate is exactly what
the automated suite cannot see, and exactly what UAT is for. Record the
hesitation even if you are not sure it matters.

Write what you observed, not what you concluded. "The alarm panel appeared but
I had already looked away from that part of the screen" is useful. "Alarm
notification needs work" is not.

---

# 7. Test Scenarios

## 7.1 First contact

The most perishable part of any UAT session: it can only be run once per
tester, and it is the only chance to see the application through unfamiliar
eyes.

| ID | Scenario | Steps | Expected | Result |
|----|----------|-------|----------|--------|
| UAT-001 | Start it without being told anything | Extract the archive, run `./DigitalClock`. Do nothing else for thirty seconds. | The clock appears and updates. The tester can tell what it is and how to leave. | |
| UAT-002 | Work out how to exit | Without consulting the manual, leave the application. | The footer's exit hint is found and works. | |
| UAT-003 | Discover the other modes | Without consulting the manual, find the stopwatch. | The `[M] Mode` hint is noticed and understood. | |
| UAT-004 | Read the footer | Look at the footer and say what each key does. | The hints are legible and the abbreviations are guessable. `[F] 12/24` is the one most likely to fail this. | |

> UAT-004 is included because the v1.4.0 footer is more crowded than it was.
> If `[F] 12/24` is not guessable, that is worth knowing.

## 7.2 The clock itself — FR-001, FR-002, FR-003, FR-008

| ID | Scenario | Steps | Expected | Result |
|----|----------|-------|----------|--------|
| UAT-005 | The time is correct | Compare against a phone or wall clock. | Matches to the second. | |
| UAT-006 | The date is correct | Compare against a known-good source. | Correct day, date, month and year. | |
| UAT-007 | It updates smoothly | Watch for a full minute. | Ticks once a second. No flicker, no visible repaint, no drift. | |
| UAT-008 | It is readable at a glance | Look at it from across the room, or with peripheral vision. | The time is the most prominent thing on screen. | |
| UAT-009 | The minute boundary is clean | Watch a minute roll over. | No stutter, no duplicated or skipped second. | |

> UAT-007 and UAT-009 are the two most likely to reveal something. The
> flicker-free redraw is a design decision (cursor home and erase-to-end-of-line
> rather than clear-and-repaint) and only a human eye can judge whether it
> worked.

## 7.3 Formats — FR-004, FR-005

| ID | Scenario | Steps | Expected | Result |
|----|----------|-------|----------|--------|
| UAT-010 | 12-hour clock | Press `F`. | Switches to 12-hour with AM/PM. Correct either side of noon and midnight if you can wait for one. | |
| UAT-011 | Switching back | Press `F` again. | Returns to 24-hour. The transient message says which is active. | |
| UAT-012 | The setting is not persisted | Press `F`, quit, restart. | The clock returns to the configured `TimeFormat`. Is this what the tester expected? | |
| UAT-013 | Date formats | Set `DateFormat` to each of `LONG`, `DD-MM-YYYY`, `MM-DD-YYYY`, `YYYY-MM-DD` and restart. | Each renders as documented. | |

> UAT-012 asks about a deliberate decision rather than a behaviour. `F` is a
> session toggle and does not write to the file. If the tester expected it to
> persist, say so — the decision is defensible but not obvious.

## 7.4 Configuration — FR-006

| ID | Scenario | Steps | Expected | Result |
|----|----------|-------|----------|--------|
| UAT-014 | Edit the file | Open `Config/config.ini` and change the refresh interval. | The file is self-explanatory; the comments answer the question before it is asked. | |
| UAT-015 | Reload without restarting | With the clock running, change `Theme` in another window, then press `C`. | The theme changes. A message confirms the reload. | |
| UAT-016 | Misspell a key | Change `Theme=` to `Them=` and reload. | The status bar reports an unrecognised key. The tester can tell what is wrong from the screen alone. | |
| UAT-017 | Break a value | Set `RefreshInterval=0`, reload. | Falls back to the default rather than misbehaving. | |
| UAT-018 | Delete the file | Rename `config.ini` and restart. | Starts on defaults without complaint or crash. | |
| UAT-019 | Reload during a countdown | Start the countdown, change `TimerDuration`, press `C`. | The running countdown is not disturbed. | |

> UAT-016 is the scenario the v1.4.0 configuration work exists for. Before it,
> `Them=Light` was silently ignored and looked exactly like a working
> configuration. The question is not whether the warning appears — TC-077
> asserts that — but whether a person notices it and understands it.

## 7.5 Themes

| ID | Scenario | Steps | Expected | Result |
|----|----------|-------|----------|--------|
| UAT-020 | Cycle the themes | Press `T` five times. | Each theme applies immediately and is distinguishable from the last. | |
| UAT-021 | Judge each one | Look at each theme for a few seconds. | Each is legible on this terminal's background. Note any that are not. | |
| UAT-022 | High contrast | Select `HighContrast`. | Genuinely more legible, not merely different. | |
| UAT-023 | Styles render | On the dark theme, look at the banner and clock. | Bold is visibly bold; the footer is visibly dimmer. | |
| UAT-024 | Light theme on a light background | If the terminal has a light background, try the light theme. | Readable. | |
| UAT-025 | Colour is suppressed when redirected | Run `./DigitalClock --once > frame.txt`, open the file. | No escape sequences in the file. | |
| UAT-026 | `NO_COLOR` is honoured | Run `NO_COLOR=1 ./DigitalClock`. | No colour, still usable. | |

> UAT-021 and UAT-024 cannot be automated at all. Whether a colour pair is
> legible depends on the terminal's palette and background, which no test can
> see. UAT-022 asks whether the accessibility theme achieves its purpose,
> which is the only measure that matters for it.

## 7.6 Alarms

| ID | Scenario | Steps | Expected | Result |
|----|----------|-------|----------|--------|
| UAT-027 | Write an alarm | Add one to `Config/alarms.ini` for two minutes' time. | The format is clear from the existing entries. | |
| UAT-028 | It fires | Wait. | The panel appears and the bell sounds at the right minute. | |
| UAT-029 | It is noticed | Look away, or work in another window, while waiting. | The alarm gets the tester's attention. | |
| UAT-030 | Snooze | Press `S`. | The panel clears; the status bar shows the new time; it rings again after the snooze. | |
| UAT-031 | Dismiss | Let it ring again, press `D`. | The panel clears and does not return this minute. | |
| UAT-032 | The next-alarm countdown | Watch the status bar after dismissing. | Counts to the next occurrence. Never says "due now" for an alarm already handled. | |
| UAT-033 | A malformed entry | Add `Alarm9=not a time`. | Skipped; the other alarms still work. | |
| UAT-034 | Alarms while in another mode | Set an alarm, switch to the stopwatch, wait. | The alarm still fires and is still visible. | |

> UAT-029 is the one worth being patient for. The automated tests assert that
> the panel is drawn; only a person can say whether it is noticeable. UAT-032
> is the DEF-007 scenario, kept here as a regression check performed by eye.

## 7.7 Stopwatch and countdown

| ID | Scenario | Steps | Expected | Result |
|----|----------|-------|----------|--------|
| UAT-035 | Time something real | Press `M`, `Space`, time a kettle or a walk to the door. | Accurate against a phone stopwatch. | |
| UAT-036 | The hundredths are readable | Watch it run. | They update smoothly and do not visibly jump. | |
| UAT-037 | Laps | Press `L` a few times while running. | Each lap is recorded and the latest is shown. | |
| UAT-038 | Stop, resume, reset | Exercise all three. | Behaves as a stopwatch should; resuming continues rather than restarting. | |
| UAT-039 | It keeps running in other modes | Start it, press `M` to the clock, wait, come back. | Still running, with the elapsed time correct. | |
| UAT-040 | The countdown finishes | Set `TimerDuration=00:30`, run it out. | The alert appears at zero and the display reaches `00:00` only when it does. | |
| UAT-041 | The last second | Watch the final five seconds closely. | Counts 5, 4, 3, 2, 1, 00:00 — it does not sit on `00:00` for a whole second. | |
| UAT-042 | Countdown with alarms off | Set `Alarms=Disabled`, run the countdown out. | The alert still appears. | |

> UAT-041 covers a deliberate rounding decision: the countdown rounds up, so
> any non-zero remainder shows at least `00:01`. UAT-042 is the DEF-009
> scenario, checked by eye.

## 7.8 World clock

| ID | Scenario | Steps | Expected | Result |
|----|----------|-------|----------|--------|
| UAT-043 | Configure zones | Set `TimeZones` to three places you can verify. | The syntax is clear from the comments. | |
| UAT-044 | The times are right | Compare against a known source. | Correct, including any that are currently on daylight saving. | |
| UAT-045 | The rows line up | Look at the column of times. | Labels padded so the times align. | |
| UAT-046 | An unresolvable zone | Add a zone name your platform does not have. | Marked unavailable rather than shown as UTC under the wrong label. | |
| UAT-047 | No zones configured | Empty `TimeZones`, restart, cycle modes. | World mode is skipped rather than appearing empty. | |

## 7.9 Shutdown and robustness — FR-009

| ID | Scenario | Steps | Expected | Result |
|----|----------|-------|----------|--------|
| UAT-048 | Quit with `Q` | Press `Q`. | Exits promptly; the terminal is left usable, with the cursor visible and echo restored. | |
| UAT-049 | Quit with Ctrl+C | Restart, press Ctrl+C. | Same. Type something afterwards to confirm the terminal is intact. | |
| UAT-050 | Resize the window | Resize the terminal while it runs. | The layout follows without corruption. | |
| UAT-051 | A very narrow terminal | Shrink to about forty columns. | Degrades readably rather than scrambling. | |
| UAT-052 | Leave it running | Leave it for at least an hour, longer if convenient. | Still correct, no drift, no growth in memory, no visual artefacts. | |
| UAT-053 | Check the log | Open `Logs/application.log`. | Readable, useful, and free of noise. | |

> UAT-048 and UAT-049 matter more than they look. A terminal left without
> echo is the single most annoying thing a console application can do to
> someone, and it is invisible until the next command is typed.
>
> UAT-052 is the only scenario here that takes real time, and the only one
> that can catch a slow leak or gradual drift. Start it early in the session
> and check it at the end.

## 7.10 Platform — FR-010

Run on whichever platforms are available. CI verifies all three build and run;
this asks whether they are pleasant to use.

| ID | Scenario | Steps | Expected | Result |
|----|----------|-------|----------|--------|
| UAT-054 | Linux | Run a full pass. | | |
| UAT-055 | Windows | Run a full pass in Windows Terminal and in `cmd.exe`. | Colours and box characters render in both. | |
| UAT-056 | macOS | Run a full pass in Terminal.app and iTerm2. | | |

> Windows is the one to watch. Its support was implemented for a long time
> before it was ever compiled (KI-000), and `cmd.exe` handles ANSI sequences
> differently from Windows Terminal.

## 7.11 Documentation

| ID | Scenario | Steps | Expected | Result |
|----|----------|-------|----------|--------|
| UAT-057 | The README | Read it, then use the application. | Accurate; nothing described that does not exist, nothing important missing. | |
| UAT-058 | The User Manual keyboard table | Compare against the footer and actual behaviour. | Matches exactly. | |
| UAT-059 | `--help` | Run it. | Enough to get started without opening a document. | |
| UAT-060 | The Installation Guide | Follow it on a clean machine, if one is available. | Works as written. | |

---

# 8. Exit Criteria

UAT is complete when:

- [ ] Every scenario has a recorded result.
- [ ] No scenario is left **Fail** without a defect raised against it.
- [ ] Every **Pass with comment** has its comment written down.
- [ ] The environment table in section 4 is filled in.
- [ ] Section 9 is signed.

UAT is **successful** when, in addition:

- [ ] No **Fail** results remain open at Critical or High severity.
- [ ] Section 7.1 passed without the tester needing to be told anything.

Section 7.1 is called out because it is the only part that cannot be retried.
Once a tester has been shown how the application works, they can never again
tell you whether it was discoverable.

---

# 9. Sign-Off

| Field | Value |
|-------|-------|
| Application version tested | |
| Scenarios attempted | of 60 |
| Passed | |
| Passed with comment | |
| Failed | |
| Blocked / N/A | |
| Defects raised | |
| Overall verdict | Accepted / Accepted with comments / Rejected |
| Tester name | |
| Signature | |
| Date | |

On completion, update **KI-007** in `Docs/Testing_Report.md` and
`Docs/ChangeLog.md` with the outcome and the date. If UAT was performed but
found problems, record that — a UAT that found nothing and a UAT that was
never run look identical in a document unless the difference is written down.

---

# 10. Defect Record

Raise one row per defect found, and add it to the defect log in section 8.7
of the Testing Report using the next free `DEF-` number.

| ID | Scenario | What happened | What was expected | Severity | Status |
|----|----------|---------------|-------------------|----------|--------|
| | | | | | |
| | | | | | |
| | | | | | |

Severity follows the classification in section 8.8 of the Testing Report:
Critical, High, Medium, Low, Cosmetic.

A usability finding that is not a malfunction — something that worked but was
confusing — should still be recorded here, marked Low or Cosmetic. Those are
the findings this whole exercise exists to produce, and they are the ones most
easily lost.

---

# Document Information

| Item | Details |
|------|---------|
| Document | **UAT_Plan.md** |
| Document Version | **1.0** |
| Project | **Digital Clock System** |
| Application Version | **1.4.0** |
| Status | **Ready for execution — not yet executed** |
| Scenarios | 60, covering FR-001 to FR-010 and every shipped feature |
| Closes | Nothing yet. KI-007 remains open until section 9 is signed. |
| Prerequisite | The automated suite passing at 125 of 125 |
| Audience | Test Engineers, End Users, Project Maintainers |

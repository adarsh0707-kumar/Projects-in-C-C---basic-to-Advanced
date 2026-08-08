# Digital Clock System

A console digital clock in C++17. It displays the current system time and date
in real time, refreshing on a configurable interval, with recurring alarms, a
stopwatch, a countdown timer, a world clock, themeable colours, external
configuration and file-based logging.

No third-party dependencies — the standard library only.

```text
                ____  _       _ _        _    ____ _            _
               |  _ \(_) __ _(_) |_ __ _| |  / ___| | ___   ___| | __
               | | | | |/ _` | | __/ _` | | | |   | |/ _ \ / __| |/ /
               | |_| | | (_| | | || (_| | | | |___| | (_) | (__|   <
               |____/|_|\__, |_|\__\__,_|_|  \____|_|\___/ \___|_|\_\
                        |___/

                              16 : 42 : 43

                        Thursday, 06 August 2026

--------------------------------------------------------------------------------
 Theme        : Dark
 Refresh Rate : 1 Second
 Status       : Running
 Mode         : Clock
================================================================================
 [M] Mode  [T] Theme  [F] 12/24  [C] Reload   Press Q or Ctrl+C to Exit
```

---

## Download

Prebuilt archives are attached to each
[release](https://github.com/adarsh0707-kumar/Projects-in-C-C---basic-to-Advanced/releases).
No installation and no dependencies — extract and run.

| Platform | Archive |
| -------- | ------- |
| Linux (x86_64) | `digitalclock-*-linux-x86_64.tar.gz` |
| macOS (Apple Silicon) | `digitalclock-*-macos-arm64.tar.gz` |
| Windows (x64) | `digitalclock-*-windows-x64.zip` |

```bash
tar -xzf digitalclock-1.4.0-linux-x86_64.tar.gz
cd digitalclock-1.4.0-linux-x86_64
./DigitalClock
```

Run it **from inside the extracted folder**: the application reads `Config/`
and `Resources/` relative to the working directory. Each archive ships its own
configuration, themes and banner, and every one is extracted and run by CI
before the release is drafted.

---

## Building

Requires a C++17 compiler (GCC 8+, Clang 7+ or MSVC 2019+) and either `make` or
CMake 3.16+.

```bash
make                # builds Build/DigitalClock
make run            # builds and runs
make test           # builds and runs the test suite
make clean          # removes build output
make help           # lists all targets
```

With CMake:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

Or via the helper scripts:

```bash
./Scripts/build.sh          # make (default); also accepts "cmake" or "debug"
./Scripts/run.sh
./Scripts/test.sh           # optionally filter: ./Scripts/test.sh TC-01
./Scripts/clean.sh
```

---

## Running

```bash
./Build/DigitalClock                          # uses Config/config.ini
./Build/DigitalClock --config path/to.ini     # alternative configuration
./Build/DigitalClock --once                   # render one frame and exit
./Build/DigitalClock --version
./Build/DigitalClock --help
```

Press **Q** or **Ctrl+C** to exit. Both paths shut down gracefully: the
terminal state is restored and the shutdown is recorded in the log. While an
alarm is ringing, **S** snoozes it and **D** dismisses it. **T** changes
theme, **F** switches between the 12- and 24-hour clock, and **C** re-reads
the configuration file — all without restarting.

Run from the project root so that `Resources/` and `Config/` resolve. The
resource loader also searches the parent and grandparent directories, so
launching from `Build/` works too.

---

## Configuration

Settings live in [`Config/config.ini`](Config/config.ini). Keys are matched
case-insensitively. Any value that is missing or invalid falls back to the
default rather than stopping the application.

A key that is *not* in the table below is reported — in the log, and as an
`unrecognised key(s)` row in the status bar. A misspelt key has no effect, and
without the warning that is indistinguishable from a setting deliberately left
out: `Them=Light` looks like working configuration right up until the theme
does not change. It is shown on screen as well as logged because `Logging` can
be switched off in the very file the typo is in.

Press **C** while the clock is running to re-read the file without restarting.

| Key               | Values                                           | Default                |
| ----------------- | ------------------------------------------------ | ---------------------- |
| `TimeFormat`      | `12`, `24`                                       | `24`                   |
| `DateFormat`      | `LONG`, `DD-MM-YYYY`, `MM-DD-YYYY`, `YYYY-MM-DD` | `LONG`                 |
| `Theme`           | `Dark`, `Light`, `Blue`, `Green`, `HighContrast` | `Dark`                 |
| `RefreshInterval` | milliseconds, 50-60000                           | `1000`                 |
| `Banner`          | path to banner artwork                           | `Resources/banner.txt` |
| `Alarms`          | `Enabled`, `Disabled`                            | `Enabled`              |
| `AlarmFile`       | path to the alarm definitions                    | `Config/alarms.ini`    |
| `SnoozeMinutes`   | minutes, 1 - 240                                 | `5`                    |
| `AlarmBell`       | `true`, `false`                                  | `true`                 |
| `TimerDuration`   | `MM:SS`, `H:MM:SS`, or seconds; max 24h          | `05:00`                |
| `TimeZones`       | comma-separated zone list, max 8                 | *(empty)*              |
| `Logging`         | `Enabled`, `Disabled`                            | `Enabled`              |
| `LogFile`         | path; missing directories are created            | `Logs/application.log` |
| `LogLevel`        | `DEBUG`, `INFO`, `WARNING`, `ERROR`              | `INFO`                 |
| `ConsoleLog`      | `true`, `false`                                  | `false`                |

### Time formats

```ini
TimeFormat=24   ->  20:45:30
TimeFormat=12   ->  08:45:30 PM
```

### Date formats

```ini
DateFormat=LONG          ->  Monday, 03 August 2026
DateFormat=DD-MM-YYYY    ->  03-08-2026
DateFormat=MM-DD-YYYY    ->  08-03-2026
DateFormat=YYYY-MM-DD    ->  2026-08-03
```

### Alarms

Alarms live in [`Config/alarms.ini`](Config/alarms.ini), one per numbered key:

```ini
Alarm1=07:30 | Wake up     | Weekdays
Alarm2=13:00 | Lunch break | Daily
Alarm3=22:30 | Wind down   | Sun,Mon,Tue,Wed,Thu
Alarm4=09:00 | One-off     | Once
!Alarm5=06:00 | Kept but disarmed | Daily
```

The label and recurrence are optional; `07:30` on its own is a valid one-shot
alarm. Indices need not be contiguous. A leading `!` keeps an alarm on file
but disarmed.

| Recurrence | Meaning |
| ---------- | ------- |
| `Once` | Rings at the next occurrence, then disarms itself |
| `Daily` | Every day |
| `Weekdays` | Monday to Friday |
| `Weekends` | Saturday and Sunday |
| `Mon,Wed,Fri` | An explicit comma-separated day list |

When an alarm fires, an alert panel appears beneath the clock and the terminal
bell sounds each refresh until it is acknowledged:

```text
                 +--------------------------------------------+
                 | ALARM  07:30                               |
                 | Wake up                                    |
                 |                                            |
                 | [S] Snooze 5m   [D] Dismiss                |
                 +--------------------------------------------+
```

Press **S** to snooze for `SnoozeMinutes`, or **D** to dismiss. A one-shot
alarm disarms itself once dismissed; a recurring one stays armed for its next
occurrence. The status bar shows a countdown to whichever alarm is next.

A malformed entry is skipped and logged rather than discarding the rest of the
file, and a missing alarm file simply means no alarms are configured.

---

### Stopwatch and timer

Press **M** to cycle between the modes: clock, stopwatch, timer, and — when
`TimeZones` is set — the world clock. Each
supplies the large centre readout while it is on screen. Switching modes never
disturbs the others — a running stopwatch keeps running while you look at the
clock, and alarms and the countdown keep working whatever is displayed.

| Key | Clock | Stopwatch | Timer | World |
| --- | ----- | --------- | ----- | ----- |
| `M` | Cycle mode | Cycle mode | Cycle mode | Cycle mode |
| `Space` | — | Start / stop | Start / pause | — |
| `L` | — | Lap | — | — |
| `R` | — | Reset | Reset | — |
| `T` | Next theme | Next theme | Next theme | Next theme |
| `F` | 12 / 24-hour | 12 / 24-hour | 12 / 24-hour | 12 / 24-hour |
| `C` | Reload config | Reload config | Reload config | Reload config |
| `Q` | Quit | Quit | Quit | Quit |

While an alarm is ringing, **S** snoozes and **D** dismisses, in any mode.
**D** also acknowledges a finished countdown.

**T** cycles through the bundled themes without restarting. **F** switches the
clock between 12- and 24-hour form for the session, leaving `TimeFormat`
alone. **C** re-reads the configuration file, so a setting can be edited in
another window and applied without stopping the clock — a running stopwatch or
countdown is left alone by the reload.

The footer lists these keys where there is room for them; the stopwatch and
timer footers are already close to eighty columns, so they omit `[C] Reload`.
All of them work in every mode regardless.

The stopwatch reads `MM:SS.cc`, widening to `H:MM:SS.cc` past an hour, and
keeps up to 99 laps. It redraws faster than `RefreshInterval` while running, so
the hundredths do not visibly jump.

The countdown starts from `TimerDuration` and announces itself through the
same alert panel the alarms use:

```text
                 +--------------------------------------------+
                 | TIMER  00:05                               |
                 | Countdown finished                         |
                 |                                            |
                 | [D] Dismiss   [R] Reset                    |
                 +--------------------------------------------+
```

---

### World clock

Set `TimeZones` to a comma-separated list. Each entry is a zone with an
optional label after a pipe:

```ini
TimeZones=UTC, Asia/Kolkata | Home, America/New_York | NYC, -08:00 | Pacific
```

```text
              UTC      03:47  (UTC+00:00)
              Home     09:17  (UTC+05:30)
              NYC      23:47  (UTC-04:00)
              Pacific  19:47  (UTC-08:00)
```

Two kinds of zone, and the difference matters:

| Form | Example | Daylight saving | Platforms |
| ---- | ------- | --------------- | --------- |
| Named | `America/New_York` | **Followed** | Linux, macOS |
| Fixed offset | `-08:00`, `UTC+05:30` | Not applied | All, including Windows |

C++17 has no timezone database — `std::chrono::time_zone` arrived in C++20 —
so named zones are resolved through the platform's own database. That is
present on Linux and macOS. Windows does not expose zones under these names,
so use fixed offsets there.

A fixed offset is exact and portable but has no notion of daylight saving: a
fixed `-05:00` is an hour wrong for New York for the months it observes EDT.
Where the platform supports it, prefer the named form. A named zone that
cannot be resolved is marked `[unavailable on this platform]` rather than
silently displaying UTC under the wrong label.

Leave `TimeZones` empty and the world clock is skipped in the mode cycle.

---

### Themes

Theme files live in [`Resources/themes/`](Resources/themes/) and assign a
colour to each screen element, optionally followed by text styles:

```ini
HEADER=Cyan Bold
TIME=BrightGreen Bold
DATE=Yellow
STATUS=White
FOOTER=BrightBlack Dim
ACCENT=Blue
ALERT=BrightYellow Bold
ERROR=BrightRed Bold
```

Styles are `Bold`, `Dim`, `Italic`, `Underline`, `Reverse` and `Normal`. Order
does not matter and commas are accepted, so `Bold Cyan` and `Cyan, Bold` mean
the same thing. `Normal` clears any styles named before it. Not every terminal
renders `Italic`.

A style with no colour beside it keeps the colour already in place, so a theme
can add emphasis without restating what it is emphasising. A token that names
neither a colour nor a style is counted and reported rather than silently
dropped — otherwise a misspelt colour would be indistinguishable from an
element left unstyled.

Add a theme by dropping a new `.theme` file into that directory and naming it
in `Theme=`. An unknown theme falls back to the built-in default and logs a
warning. Colour output is suppressed automatically when standard output is not
a terminal, or when `NO_COLOR` is set — in which case styles are suppressed
with it.

Press **T** to cycle the bundled themes at runtime. `HighContrast` is bold
throughout and dims nothing, since dimming works against the legibility it
exists for.

---

## Architecture

A layered design; each layer depends only on the ones beneath it.

```text
                        User
                         ▲
   Presentation   Console · Display · Screen · Banner · StatusBar · Notifier
                         ▲
   Business logic   Clock · Date · TimeFormatter · Alarm · AlarmManager
                    Stopwatch · CountdownTimer · TimeZone · WorldClock
                         ▲
   Service        ConfigurationManager · ThemeManager · Logger
                  ResourceManager · Utility
                         ▲
                  Operating System
```

`Application` owns the component graph and drives startup, the refresh loop and
shutdown. `main()` only parses arguments and delegates.

| Component              | Responsibility                                     |
| ---------------------- | -------------------------------------------------- |
| `Application`          | Lifecycle: startup order, refresh loop, shutdown   |
| `Clock`                | System time snapshot; 12- and 24-hour accessors    |
| `Date`                 | System date snapshot; weekday and calendar helpers |
| `TimeFormatter`        | Applies the configured time and date formats       |
| `Alarm`                | One alarm: time, label, recurrence, snooze state    |
| `Stopwatch`            | Elapsed time with laps                              |
| `CountdownTimer`       | Counts down and fires once at zero                  |
| `TimeZone`             | Converts a UTC instant to another zone              |
| `WorldClock`           | The configured zone set, rendered as rows           |
| `AlarmManager`         | Loads alarms and decides when one fires             |
| `Notifier`             | Composes the alert panel and sounds the bell        |
| `Display`              | Coordinates the presentation layer                 |
| `Screen`               | Composes the layout; flicker-free redraw           |
| `Console`              | All terminal I/O: cursor, clearing, key polling    |
| `Banner`               | Header artwork, with a built-in fallback           |
| `StatusBar`            | Runtime information rows                           |
| `ConfigurationManager` | INI parsing with validation and defaults           |
| `ThemeManager`         | Maps screen elements to colours                    |
| `Logger`               | Thread-safe, timestamped, severity-filtered log    |
| `ResourceManager`      | Locates and reads external assets                  |
| `Utility`              | Shared string, parsing and filesystem helpers      |

### Design notes

- **Redraws don't flicker.** Rather than clearing and repainting, `Screen`
  returns the cursor home and overwrites each line, erasing to end-of-line as
  it goes and to end-of-screen at the end. This avoids both the flash of a full
  clear and the duplicate output that plain appending would produce.
- **The refresh loop stays responsive.** It sleeps in 40 ms slices rather than
  for the whole interval, so Q and Ctrl+C are handled promptly even when
  `RefreshInterval` is long.
- **Failures degrade, they don't stop the app.** A missing config, an unknown
  theme, absent banner artwork and an unwritable log are each logged and
  worked around. Only a terminal that cannot be prepared is fatal.
- **The signal handler only sets an atomic flag.** All real shutdown work
  happens on the main thread.
- **The stopwatch and timer are driven by a monotonic clock.** Both take the
  current reading as a parameter rather than reading a clock themselves, so a
  change to the system time cannot corrupt a measurement in progress, and
  expiry can be tested at an exact instant without sleeping.

---

## Project layout

```text
DigitalClock/
├── Build/          Compiled executables (generated)
├── Config/         config.ini
├── Docs/           Specification and design documents
├── Inc/            Headers
├── Logs/           Runtime logs (generated)
├── Obj/            Object files (generated)
├── Resources/      Banner, logo and theme files
├── Scripts/        build / run / test / clean helpers
├── Src/            Implementation
├── Tests/          Test suite
├── CMakeLists.txt
└── Makefile
```

---

## Tests

```bash
make test                      # all tests
./Build/DigitalClockTests      # same, run directly
./Build/DigitalClockTests TC-0 # filter by identifier

make SANITIZE=address test     # under AddressSanitizer
make SANITIZE=undefined test   # under UndefinedBehaviorSanitizer
make coverage                  # per-file line coverage
```

Coverage is measured with `gcov`, which ships with GCC, so there is nothing
extra to install. The total is **93.70%**; CI enforces a 92% floor.

130 tests, covering TC-001 through TC-085 from
[`Docs/Testing_Report.md`](Docs/Testing_Report.md) plus supporting unit tests
(`UT-*`) for the parsing and layout code. The harness is a small header in
`Tests/TestFramework.hpp` — adding a third-party framework would have been the
project's only external dependency.

Tests must run from the project root so they can find `Config/` and
`Resources/`; `make test`, `Scripts/test.sh` and `ctest` all handle this.

---

## Documentation

Full specification and design documents are in [`Docs/`](Docs/):
Software Requirements Specification, Software Design Document, Architecture,
API Documentation, Developer Guide, Installation Guide, User Manual, Testing
Report, UAT Plan and Change Log.

Two are worth pointing at directly:

- [`Docs/Testing_Report.md`](Docs/Testing_Report.md) records what has been
  verified and what has not, including the defects found and the coverage
  measured. Claims in it are dated and tied to executed runs.
- [`Docs/UAT_Plan.md`](Docs/UAT_Plan.md) is the one piece of testing the
  project has *not* done: 60 scenarios for a person at a terminal, written
  and ready but never run. It stays that way in the documentation until
  somebody signs for it.

---

## License

MIT — see [LICENSE](LICENSE).

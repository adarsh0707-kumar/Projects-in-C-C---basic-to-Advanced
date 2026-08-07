# Digital Clock System

A console digital clock in C++17. It displays the current system time and date
in real time, refreshing on a configurable interval, with recurring alarms,
themeable colours, external configuration and file-based logging.

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
================================================================================
 Press Q or Ctrl+C to Exit
```

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
./Build/DigitalClock --version
./Build/DigitalClock --help
```

Press **Q** or **Ctrl+C** to exit. Both paths shut down gracefully: the
terminal state is restored and the shutdown is recorded in the log. While an
alarm is ringing, **S** snoozes it and **D** dismisses it.

Run from the project root so that `Resources/` and `Config/` resolve. The
resource loader also searches the parent and grandparent directories, so
launching from `Build/` works too.

---

## Configuration

Settings live in [`Config/config.ini`](Config/config.ini). Keys are matched
case-insensitively. Any value that is missing or invalid falls back to the
default rather than stopping the application.

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

### Themes

Theme files live in [`Resources/themes/`](Resources/themes/) and assign a
colour to each screen element:

```ini
HEADER=Cyan
TIME=BrightGreen
DATE=Yellow
STATUS=White
FOOTER=BrightBlack
ACCENT=Blue
ALERT=BrightYellow
ERROR=BrightRed
```

Add a theme by dropping a new `.theme` file into that directory and naming it
in `Theme=`. An unknown theme falls back to the built-in default and logs a
warning. Colour output is suppressed automatically when standard output is not
a terminal, or when `NO_COLOR` is set.

---

## Architecture

A layered design; each layer depends only on the ones beneath it.

```text
                        User
                         ▲
   Presentation   Console · Display · Screen · Banner · StatusBar · Notifier
                         ▲
   Business logic   Clock · Date · TimeFormatter · Alarm · AlarmManager
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
```

78 tests, covering TC-001 through TC-041 from
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
Report and Change Log.

---

## License

MIT — see [LICENSE](LICENSE).

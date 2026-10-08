# C / C++ Systems & Application Projects

> A collection of self-contained C and C++ projects built to practice systems programming, native application design, testing, persistence, and cross-platform build engineering.

> **Status:** Active learning / portfolio repository. Each project is independent and has its own build and documentation.

---

## Projects

| Project | Language | Focus | Status |
|---|---|---|---|
| [DigitalClock](DigitalClock/) | C++17 | Terminal + optional Qt GUI, alarms, timers, world clock, themes, plugins | Active |
| [Calculator](Calculator/) | C / C++ | Scientific calculation, expression parsing, statistics, conversions, complex numbers, matrices, plotting | Active |
| [GuessTheNumber](GuessTheNumber/) | C11 | Input handling, game logic, regression testing | Complete |
| [UserManagement](UserManagement/) | C11 | Authentication, password hashing, lockout, binary persistence, audit logging | Complete / educational |

### Recommended starting points

- **DigitalClock** — the most substantial project in this repository, with layered architecture, an optional Qt frontend, plugins, configuration, testing, and cross-platform packaging.
- **UserManagement** — the strongest C systems example, covering manual storage, authentication logic, fixed-size records, lockout state, and audit logging.
- **Calculator** — broader application logic and parsing work.
- **GuessTheNumber** — deliberately small, but useful as an example of turning an input-handling bug into a regression-tested implementation.

---

## What this repository is for

This repository is a progression through native development rather than a single application:

~~~text
Small CLI programs
      ↓
Structured application logic
      ↓
Parsing / state / persistence
      ↓
Testing and failure handling
      ↓
Layered C++ application architecture
      ↓
Optional GUI + plugins + packaging
~~~

The projects are intentionally independent. A change in one project does not require the others to build or run.

---

## Engineering themes

Across the repository, the projects explore:

- C11 and C++17 development
- POSIX terminal and system APIs
- file-based persistence and fixed-size records
- input validation and failure handling
- authentication and account lockout design
- hashing and per-user salts
- layered application architecture
- C ABI boundaries for plugins
- optional Qt-based graphical interfaces
- unit / functional / regression testing
- sanitizers and compiler warnings
- Make and CMake build systems
- cross-platform build and release workflows

These are learning and portfolio projects. They should not be interpreted as production-ready security, database, or financial infrastructure.

---

## Project details

### DigitalClock

DigitalClock is a C++17 clock application with both a terminal interface and an optional Qt6 graphical interface.

Implemented capabilities include:

- live clock and date display
- 12/24-hour formatting
- alarms with recurrence, snooze, and dismissal
- stopwatch with laps
- countdown timer
- configurable world clock
- runtime theme switching
- configuration reload without restart
- file-based logging
- optional plugin modes through a C ABI
- shared core logic between console and GUI frontends

See the [DigitalClock README](DigitalClock/README.md) for the architecture, configuration format, tests, plugins, and build instructions.

### Calculator

Calculator is a scientific-calculator project focused on expression evaluation and numerical application logic.

The repository documentation covers functionality including:

- expression parsing
- variables
- statistics
- unit and base conversion
- complex numbers
- matrices
- plotting
- automated testing

Build and test it independently from the [Calculator](Calculator/) directory.

### GuessTheNumber

A small C11 terminal game that became a useful input-validation and regression-testing exercise.

The implementation reads complete input lines instead of relying on `scanf("%d", ...)`, allowing invalid input and EOF to be handled without leaving the program stuck on the same character.

The project includes automated tests and a regression case for the original invalid-input hang.

See [GuessTheNumber/README.md](GuessTheNumber/README.md).

### UserManagement

A CLI-based user management system written in C11 with no external runtime dependencies.

Implemented capabilities include:

- user registration and listing
- role updates
- password changes
- soft deletion
- per-user random salts
- SHA-256 hashing implemented in the project
- login lockout after repeated failures
- hidden terminal password input through `termios`
- fixed-size binary records in `data/users.dat`
- timestamped audit logging

Binary records are directly addressable by ID, so updating a record does not require rewriting the complete file.

See the [UserManagement README](UserManagement/README.md) and its [architecture documentation](UserManagement/docs/ARCHITECTURE.md).

**Security note:** UserManagement is an educational authentication exercise, not a production authentication service. It uses a single SHA-256 pass rather than an adaptive password-hashing scheme such as Argon2id or bcrypt, and it has no multi-process file locking or network-layer rate limiting.

---

## Build and test

Each project has its own build instructions. Start by entering the project directory:

~~~bash
cd DigitalClock
make
make test
~~~

For projects that provide CMake support:

~~~bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
~~~

Check the individual README before assuming that the same target names or optional dependencies apply to every project.

### Requirements

- C11 compiler for the C projects
- C++17 compiler for the C++ projects
- Make and/or CMake depending on the project
- Qt6 only for the optional DigitalClock GUI
- Linux/POSIX APIs are used by parts of the native applications

---

## Repository structure

~~~text
.
├── Calculator/
├── DigitalClock/
├── GuessTheNumber/
├── UserManagement/
├── .github/workflows/
├── CONTRIBUTING.md
├── SECURITY.md
└── LICENSE
~~~

Each project owns its source, tests, build files, and documentation. The root repository provides the shared entry point rather than a shared application framework.

---

## Testing philosophy

The goal is not to claim that every line is tested. The projects use tests where they provide useful protection against real defects:

- parser and calculation behavior in the calculator
- clock and application behavior in DigitalClock
- invalid-input regression coverage in GuessTheNumber
- functional authentication, CRUD, storage, and lockout scenarios in UserManagement

Where a project has a known limitation, its project-level documentation should state it explicitly.

---

## Limitations

This repository contains progressively more advanced learning projects, not a unified production software suite.

Some projects intentionally use simplified designs:

- local file persistence instead of a database
- terminal applications instead of network services
- educational authentication implementations
- optional GUI dependencies
- no shared deployment platform

The README for each project is the source of truth for its current implementation and limitations.

---

## Roadmap

- [ ] Continue expanding native systems projects.
- [ ] Add more focused data-structure and systems exercises.
- [ ] Improve automated test coverage where it adds meaningful protection.
- [ ] Add measured benchmarks to projects where performance is a relevant engineering question.
- [ ] Keep project documentation synchronized with implementation.

---

## License

MIT — see [LICENSE](LICENSE).
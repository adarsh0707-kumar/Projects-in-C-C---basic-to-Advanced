# Advanced Scientific Calculator in C/C++

![Language](<https://img.shields.io/badge/Language-C%20%7C%20C%2B%2B-blue>)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS%20%7C%20Windows-green)
![Compiler](https://img.shields.io/badge/Compiler-GCC%2FG%2B%2B-orange)
![Build](https://img.shields.io/badge/Build-Make-red)
![Tests](<https://img.shields.io/badge/Tests-486%20passing-brightgreen>)
![CI](https://github.com/adarsh0707-kumar/Projects-in-C-C---basic-to-Advanced/actions/workflows/ci.yml/badge.svg)
![License](https://img.shields.io/badge/License-MIT-brightgreen)
[![Release](https://img.shields.io/github/v/release/adarsh0707-kumar/Projects-in-C-C---basic-to-Advanced?filter=calculator-v*&label=download)](https://github.com/adarsh0707-kumar/Projects-in-C-C---basic-to-Advanced/releases/latest)

A modular, extensible, and production-style **Scientific Calculator** written in **C/C++**, demonstrating expression parsing, postfix evaluation, scientific functions, variables, memory operations, unit conversion, statistics, complex numbers, matrices, graph plotting, and comprehensive testing.

This project is intended as both a learning resource and a portfolio-quality software engineering project.

---

# Table of Contents

- [Download](#download)
- [Introduction](#introduction)
- [Features](#features)
- [Folder Structure](#folder-structure)
- [Architecture](#architecture)
- [Requirements](#requirements)
- [Build](#build)
- [Usage](#usage)
- [Testing](#testing)
- [Continuous Integration](#continuous-integration)
- [Documentation](#documentation)
- [Roadmap](#development-roadmap)
- [Contributing](#contributing)
- [License](#license)

---

# Download

**You don't need to build this.** Prebuilt packages for Linux, macOS, and Windows are on the
[latest release page](https://github.com/adarsh0707-kumar/Projects-in-C-C---basic-to-Advanced/releases/latest) — download one file and run it.

## Graphical version

Qt is bundled, so nothing needs installing.

| Platform | File | How to run |
|---|---|---|
| Linux | `Calculator-*-x86_64.AppImage` | `chmod +x` it, then run it |
| macOS (Apple Silicon) | `Calculator-*-macos-arm64.dmg` | Open it, drag Calculator to Applications |
| Windows (x64) | `Calculator-GUI-*-windows-x64.zip` | Unzip, run `calculator-gui.exe` |

## Command-line version

No dependencies at all, and roughly 50 KB.

| Platform | File |
|---|---|
| Linux | `calculator-*-linux-x86_64.tar.gz` |
| macOS (Apple Silicon) | `calculator-*-macos-arm64.tar.gz` |
| Windows (x64) | `calculator-*-windows-x64.zip` |

Extract it and run the `calculator` binary inside.

## First run: expect a security warning

The packages are **not code-signed** — certificates cost money and this is a free project — so
your system will warn you once:

- **macOS** — right-click the app, choose **Open**, then **Open** again.
- **Windows** — click **More info**, then **Run anyway**.
- **Linux** — no warning; just mark the AppImage executable.

Prefer to build it yourself instead? See [Build](#build); it's two commands.

## Where your history is kept

Outside the downloaded folder, so moving or deleting it loses nothing:

| Platform | Location |
|---|---|
| Linux | `~/.local/share/calculator/history.txt` |
| macOS | `~/Library/Application Support/Calculator/history.txt` |
| Windows | `%APPDATA%\Calculator\history.txt` |

Set `CALCULATOR_HISTORY_FILE` to put it somewhere else.

---

# Introduction

The Advanced Scientific Calculator goes beyond basic arithmetic by implementing a complete mathematical expression engine.

It supports:

- Expression parsing (infix -> postfix -> evaluated result)
- Scientific functions, variables, and constants
- Memory registers and calculation history
- Statistics, unit conversion, and number-base conversion
- Complex numbers and matrix algebra (C++)
- ASCII graph plotting from the terminal
- A modular architecture with 486 automated unit tests and zero exit() calls in the engine (every error path degrades gracefully instead of terminating the process)

---

# Features

## Basic Calculator

Addition, subtraction, multiplication, division, modulus, power, parentheses, unary minus, and decimals — including implicit multiplication (`2(3)`, `3pi`, `2sin(x)`).

## Scientific Functions

`sqrt`, `cbrt`, `sin`, `cos`, `tan`, `asin`, `acos`, `atan`, `sinh`, `cosh`, `tanh`, `log`, `ln`, `exp`, `abs`, `floor`, `ceil`, `round`, `trunc`, `gamma`, `fact` — plus degree/radian angle modes.

## Binary Functions

`pow`, `max`, `min`, `hypot`, `atan2`

## Variables, Constants, and Memory

```text
x = 10
y = 20
x + y            -> 30

pi
e

MS  MR  MC  M+  M-
```

## History

Every calculation is logged; view, clear, or recall with `!!` (last result) or `!5` (entry #5).

## Statistics

`mean`, `median`, `mode`... `sum`, `min`, `max`, `count`, `stddev`, `variance`, `pstddev`, `pvariance`

## Unit Conversion

Length, weight, time, and temperature, with an optional explicit target unit.

## Number Base Conversion

Binary, decimal, octal, and hexadecimal, in either direction.

## Complex Numbers

Addition, subtraction, multiplication, division, and `sqrt()` of negative/complex values — implemented in C++ with real operator overloading.

## Matrix Operations

Addition, subtraction, multiplication (matrix and scalar), transpose, determinant, and inverse — also C++, backed by `std::vector`.

## Graph Plotting

ASCII graphs of any supported expression, right in the terminal.

---

# Folder Structure

```text
Calculator/
├── Inc/                       # Public headers (Doxygen-documented)
├── Src/                        # Implementation (.c and .cpp)
├── Gui/                        # Qt6 GUI front end -- calls the same engine as the CLI
│   ├── icon.svg               # app icon source; `make icons` renders icons/*.png
│   ├── icons/                 # committed PNG renders, embedded via resources.qrc
│   └── calculator-gui.desktop.in  # launcher entry template (`make install-desktop`)
├── Tests/                      # 486 unit tests (Tests/test_main.c is the test runner's entry point)
├── docs/
│   ├── Doxyfile               # `make docs` config
│   ├── API.md  ARCHITECTURE.md  CHANGELOG.md  CONTRIBUTING.md
│   ├── DESIGN.md  MEMORY.md  PHASES.md  PRD.md  ROADMAP.md
│   ├── RULES.md  TEST_PLAN.md
│   └── html/                  # generated by `make docs` (gitignored)
├── CMakeLists.txt              # cross-platform build (Linux/macOS/Windows)
├── LICENSE
├── Makefile                    # Linux development build
└── Readme.md

(The CI and release workflows live at the *repository* root, in
.github/workflows/ -- GitHub Actions does not read them from a
subproject directory. Calculation history is no longer kept in the
project folder; see below.)
```

---

# Architecture

The calculator follows a modular layered architecture:

```
User Input
      │
      ▼
Parser (implicit multiplication)
      │
      ▼
Validator (syntax check)
      │
      ▼
Tokenizer / Postfix Generator (shunting-yard)
      │
      ▼
Evaluator
      │
      ▼
Functions · Variables · Memory · History
Statistics · Units · Base Conversion
Matrices (C++) · Complex Numbers (C++)
      │
      ▼
Output
```

## Error Handling

Every error path in the engine — division by zero, an undefined
variable, a malformed expression, an out-of-domain factorial, a
pathologically deep or complex expression — degrades gracefully
instead of terminating the process. `Inc/error.h` provides a
centralized `CalculatorError` enum
(`calculatorSetLastError()` / `calculatorGetLastError()` /
`calculatorErrorString()`); recoverable arithmetic errors additionally
propagate as `NaN` through the evaluator, which callers detect with a
simple `isfinite()` check. This means a single bad sample inside
`plot(...)` (e.g. the asymptote in `plot(1/x)`) is skipped and
rendered as a gap in the graph, rather than crashing the whole
session — see `docs/CHANGELOG.md` for the full history of this
migration.

See **docs/ARCHITECTURE.md** for the full write-up, including the rationale for the C/C++ split (C for the core engine; C++ specifically where operator overloading and RAII buy something real — complex numbers and matrices).

---

# Requirements

- GCC / G++ (C11 / C++17)
- GNU Make
- Linux, macOS, or Windows -- CI builds and tests all three on every push
- [Doxygen](https://www.doxygen.nl/) + optionally [Graphviz](https://graphviz.org/) — only needed for `make docs`

---

# Build

There are two build systems, and they build the same targets from the same sources:

- **CMake** — works on Linux, macOS, and Windows (MSVC, MinGW, Xcode, Ninja). Use this if you're on macOS or Windows, or packaging a release.
- **Makefile** — the Linux development build. It carries the sanitizer configurations and the `BUILD=<type>` object-directory layout, and is what CI runs today.

## CMake (any OS)

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

That produces `build/calculator` (CLI) and, **if Qt6 is installed**, `build/calculator-gui`. Qt6 missing is not an error — the CLI and the test suite have no external dependencies at all and build everywhere. `cmake --install build --prefix <dir>` stages a release layout.

On Windows, `cmake -B build` then `cmake --build build --config Release` works with Visual Studio's own compiler; no MSYS2 or Unix shell needed.

> On macOS and Windows the filesystem is case-insensitive, so a `build/` directory collides with the legacy `Build/` one. `Build/` is no longer used by anything (history moved to a per-user data directory), so deleting it is safe if you hit that.

## Makefile (Linux)

The Makefile supports four build configurations, each in its own object directory so switching configs never links stale objects:

```bash
make                 # debug build   (-g -O0)          -> ./calculator
make release         # optimized     (-O2 -DNDEBUG)    -> ./calculator
make asan            # + AddressSanitizer + UBSan       -> ./calculator
make ubsan           # + UndefinedBehaviorSanitizer only -> ./calculator

make run             # build (debug) and run
make gui             # build the Qt6 GUI -> ./calculator-gui
make run-gui         # build (debug) and run the GUI

make install-desktop # add a per-user launcher entry + icons (no root)
make uninstall-desktop
make icons           # re-render Gui/icons/*.png from Gui/icon.svg

make clean           # remove the current BUILD's objects/binaries
make distclean       # remove every build type's objects/binaries + generated docs
make rebuild         # clean + all
make docs            # generate Doxygen HTML docs into docs/html/
```

Header dependencies are tracked automatically (`-MMD -MP`), so editing any header correctly triggers a rebuild of exactly the files that include it.

`make gui`/`make run-gui` require Qt6 (`Qt6Widgets` via `pkg-config`, plus a matching `moc`/`rcc` — see the Makefile's Phase 31 section for how it locates them even if the system's default `moc` on `PATH` belongs to Qt5). Every other target works with or without Qt installed.

The application icon is compiled into `calculator-gui` from `Gui/resources.qrc`, so the window shows it wherever the binary is launched from and nothing needs installing. `make install-desktop` is only for getting the app into your desktop's launcher/menu; it writes to `~/.local/share` (never root) and `make uninstall-desktop` reverses it. `make icons` regenerates the committed PNGs from `Gui/icon.svg` and is the one target that needs `rsvg-convert` — a normal build does not.

---

# Usage

Run `./calculator` (or `make run`) and pick an option from the menu. A few examples of what each mode accepts:

**Arithmetic & scientific**

```text
> 2+3*5              -> 17
> sqrt(16)            -> 4
> pow(2,8)            -> 256
> x=5
> y=10
> x+y                 -> 15
> pi
> plot(sin(x))        -> ASCII graph
```

**Complex numbers**

```text
> (2+3i)*(4-5i)
> sqrt(-1)
```

**Matrices**

```text
> det([[1,2],[3,4]])
> inverse([[1,2],[3,4]])
> [[1,2],[3,4]] + [[5,6],[7,8]]
```

**Statistics**

```text
> mean(1,2,3,4)
> stddev(4,8,6,5,3,7)
```

**Units**

```text
> 10km
> 10km to miles
> 80F to C
```

**Number bases**

```text
> bin(25)
> hex(255)
> dec(FFh)
```

---

# Testing

```bash
make test
```

Runs all **460** unit tests covering: error reporting, stack, tokenizer, validator, parser, postfix conversion, functions, variables, history, memory, unit conversion, statistics, base conversion, plotting, complex numbers, and matrices. All tests also pass cleanly under AddressSanitizer and UndefinedBehaviorSanitizer (`make asan && make BUILD=asan test`).

---

# Continuous Integration

This project ships a GitHub Actions workflow at `.github/workflows/ci.yml` that builds (debug + release) and runs the full test suite on every push/PR, plus dedicated AddressSanitizer/UndefinedBehaviorSanitizer jobs.

> **Note:** this `Calculator/` folder is one subproject inside a larger multi-project repository. GitHub Actions only reads workflows from `.github/workflows/` at the **repository root** — so this file needs to be copied (or symlinked) to `<repo-root>/.github/workflows/ci.yml`, not left inside `Calculator/.github/workflows/`. It's already written with `working-directory: Calculator` so it runs correctly from that location; see the comment at the top of the file for the exact layout.

---

# Documentation

Hand-written design/process documentation lives in `docs/`:

| File            | Description                  |
| --------------- | ---------------------------- |
| PRD.md          | Product Requirement Document |
| ARCHITECTURE.md | Software Architecture        |
| DESIGN.md       | Design Decisions             |
| API.md          | Public API Reference         |
| ERROR_HANDLING.md | Error-handling design (status codes, NaN propagation) |
| MEMORY.md       | Development Memory           |
| CHANGELOG.md    | Project History              |
| ROADMAP.md      | Future Plans                 |
| PHASES.md       | Development Phases           |
| TEST_PLAN.md    | Testing Strategy             |
| CONTRIBUTING.md | Contribution Guide           |
| RULES.md        | Development Rules            |

Every header in `Inc/` is also documented inline with Doxygen comments. Generate a browsable, cross-referenced HTML API reference with:

```bash
make docs
# then open docs/html/index.html
```

---

# Development Roadmap

**v1.0 (this CLI calculator) is complete** — Phases 1–27: the core
engine, scientific functions, variables/memory/history, statistics,
unit/base conversion, complex numbers, matrix algebra, and ASCII graph
plotting, all backed by the 460-test suite above.

The calculator is planned across **36 development phases** in total.
Phases 28–36 are future work — a GUI (Qt, next up), dynamic data
structures, an AST-based evaluator, symbolic math, a plugin system,
scripting, packaging, and cross-platform builds — and are not required
for this project to be considered done as a CLI calculator.

Refer to **docs/ROADMAP.md** for the status-at-a-glance summary and
**docs/PHASES.md** for what each phase actually built.

---

# Contributing

Contributions are welcome! Please read `docs/CONTRIBUTING.md` and `docs/RULES.md` before submitting a Pull Request.

---

# License

This project is licensed under the [MIT License](LICENSE).

---

# Author

**Adarsh Kumar**

GitHub: **https://github.com/adarsh0707-kumar**

---

## Support

If you found this project helpful:

- Star the repository
- Fork the project
- Contribute new features
- Improve documentation

Your support helps the project grow and benefits the open-source community.

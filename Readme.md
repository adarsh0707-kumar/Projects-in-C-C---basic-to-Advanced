# Projects in C / C++

Three self-contained programs in C and C++17, from a small terminal game to a
clock with two interfaces and a plugin system. Each builds, tests and releases
independently.

![Language](https://img.shields.io/badge/Language-C%20%7C%20C%2B%2B17-blue)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS%20%7C%20Windows-green)
![License](https://img.shields.io/badge/License-MIT-brightgreen)

| Project | What it is | Version | Tests |
| ------- | ---------- | ------- | ----- |
| [**Digital Clock**](DigitalClock/) | A clock in a terminal *or* a window: alarms, stopwatch, countdown, world clock, themes, plugins | 2.1.0 | 136 |
| [**Calculator**](Calculator/) | Scientific calculator: expression parsing, variables, statistics, unit and base conversion, complex numbers, matrices, plotting | 1.1.2 | 493 |
| [**Guess The Number**](GuessTheNumber/) | A small terminal game | 1.0.0 | 50 checks |

[![DigitalClock CI](https://github.com/adarsh0707-kumar/Projects-in-C-C---basic-to-Advanced/actions/workflows/digitalclock-ci.yml/badge.svg)](https://github.com/adarsh0707-kumar/Projects-in-C-C---basic-to-Advanced/actions/workflows/digitalclock-ci.yml)
[![Calculator CI](https://github.com/adarsh0707-kumar/Projects-in-C-C---basic-to-Advanced/actions/workflows/ci.yml/badge.svg)](https://github.com/adarsh0707-kumar/Projects-in-C-C---basic-to-Advanced/actions/workflows/ci.yml)
[![GuessTheNumber CI](https://github.com/adarsh0707-kumar/Projects-in-C-C---basic-to-Advanced/actions/workflows/guessthenumber-ci.yml/badge.svg)](https://github.com/adarsh0707-kumar/Projects-in-C-C---basic-to-Advanced/actions/workflows/guessthenumber-ci.yml)

---

## Download

Prebuilt archives are attached to each
[release](https://github.com/adarsh0707-kumar/Projects-in-C-C---basic-to-Advanced/releases)
for Linux, macOS and Windows. Extract and run — the command-line programs
depend on nothing but the standard library.

The graphical builds are larger because they bundle the Qt runtime, which is
the point: there is still nothing to install.

Every archive is built on its own platform, then extracted somewhere
unrelated and actually run before the release is drafted. A build that works
only in the directory it was built in is not a release.

---

## Building

Any project builds on its own. Nothing here depends on anything else here.

```bash
cd DigitalClock          # or Calculator, or GuessTheNumber

make                     # build
make test                # build and run the test suite
make help                # list the targets
```

Or with CMake, which is what the Windows and macOS builds use:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

Requires a C11 / C++17 compiler — GCC 8+, Clang 7+ or MSVC 2019+ — and
optionally Qt 6.2+ for the graphical builds, which are skipped without it
rather than failing.

---

## What these projects have in common

Each one is held to the same standard, and CI enforces it rather than the
README claiming it:

- **Three platforms**, built *and run*. Compiling on Windows is not evidence
  that it works on Windows.
- **Zero warnings** under `-Wall -Wextra -Wpedantic -Wshadow -Wconversion`,
  with `-Werror` in CI. Two real defects in this repository were found by
  turning the last three of those on.
- **AddressSanitizer and UndefinedBehaviorSanitizer** on every change, plus
  ThreadSanitizer where threads are involved.
- **Measured line coverage** with an enforced floor, not a claim that
  "everything is tested". The floor guards against backsliding; it sits just
  below the current figure.
- **Hang detectors.** Two of the three programs once span forever on input
  that simply ran out. Both now have a CI job that feeds the real binary
  exhausted input under a timeout and requires it to stop.
- **Documentation kept in step with the code**, in the same commit as the
  behaviour it describes, with claims tied to runs that actually happened.

---

## Repository layout

```text
.
├── Calculator/          Scientific calculator (C, with an optional Qt GUI)
├── DigitalClock/        Console + graphical clock (C++17, optional Qt GUI)
├── GuessTheNumber/      Terminal game (C11)
└── .github/workflows/   CI and release pipelines for all three
```

**Every workflow lives at the repository root.** GitHub Actions only reads
`<repo-root>/.github/workflows/`, never a subdirectory's own, so each
workflow sets `working-directory:` and uses path filters to stay in its own
lane. A Calculator change does not run the clock's CI.

---

## Releases and tags

Tags are namespaced, because one repository holds several projects and a bare
`v1.0.0` would be ambiguous:

```text
calculator-v1.1.2
digitalclock-v2.1.0
guessthenumber-v1.0.0
```

Pushing such a tag builds the archives, verifies each by extracting and
running it, and opens a **draft** release. Nothing becomes public until
somebody reviews the draft and publishes it, so a broken pipeline cannot ship
a broken download.

---

## Documentation

Each project carries its own README. Digital Clock additionally has a full
document set in [`DigitalClock/Docs/`](DigitalClock/Docs/) — requirements,
design, architecture, API, user manual, testing report, change log and a UAT
plan.

Two of those are worth singling out, because they record what has *not* been
done as carefully as what has:

- [`Testing_Report.md`](DigitalClock/Docs/Testing_Report.md) — every defect
  found and closed, the measured coverage, and the gaps that remain open.
- [`UAT_Plan.md`](DigitalClock/Docs/UAT_Plan.md) — sixty scenarios for a
  person at a terminal. Written, ready, and deliberately still unsigned: a
  plan is a set of questions, and it closes when somebody answers them.

---

## License

MIT. See the LICENSE file in each project.

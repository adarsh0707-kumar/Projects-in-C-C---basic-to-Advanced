# Change Log

Version: 1.0
Project: Digital Clock System
Language: C++17

---

# Table of Contents

1. Introduction
2. Versioning Policy
3. Version History
4. Release Notes
5. Known Issues
6. Future Releases
7. Conclusion

# Chapter 1 – Introduction

---

# 1.1 Purpose

The **Change Log** documents the history of changes made to the **Digital Clock System** throughout its development lifecycle. It serves as a chronological record of software updates, feature additions, bug fixes, performance improvements, documentation revisions, and release information.

Maintaining an accurate change log helps developers, testers, project maintainers, and end users understand how the project has evolved over time. It also provides transparency for software releases and simplifies maintenance, debugging, and version tracking.

---

# 1.2 Scope

This document covers all significant modifications made to the Digital Clock System from the initial development phase through future releases.

The Change Log records:

- New features
- Bug fixes
- Performance improvements
- Security enhancements
- Documentation updates
- Build system changes
- Testing improvements
- Version releases

Minor formatting changes or internal code cleanup that do not affect functionality may not be recorded unless they significantly impact maintainability.

---

# 1.3 Objectives

The primary objectives of maintaining this Change Log are to:

- Maintain a complete history of project development.
- Record major software releases.
- Document newly implemented features.
- Track resolved defects.
- Record documentation updates.
- Improve communication among developers and stakeholders.
- Support software maintenance and future development.

A well-maintained change log contributes to better project management and release planning.

---

# 1.4 Intended Audience

This document is intended for:

- Software Developers
- Project Maintainers
- Test Engineers
- Technical Reviewers
- Project Managers
- Contributors
- End Users interested in release updates

Each audience can use this document to understand what has changed between software versions.

---

# 1.5 Importance of a Change Log

Maintaining a structured change log provides several benefits:

- Tracks the evolution of the software.
- Simplifies debugging by identifying when changes were introduced.
- Assists developers during maintenance.
- Improves collaboration within the development team.
- Supports version comparison.
- Provides transparency for users and stakeholders.
- Helps prepare release notes for software distributions.

Without a proper change log, it becomes difficult to identify the purpose and impact of historical changes.

---

# 1.6 Types of Recorded Changes

The Digital Clock System categorizes changes into several types.

| Change Type             | Description                                              |
| ----------------------- | -------------------------------------------------------- |
| **Added**         | New features, modules, or functionality                  |
| **Changed**       | Improvements or modifications to existing features       |
| **Fixed**         | Bug fixes and defect corrections                         |
| **Improved**      | Performance, usability, or maintainability enhancements  |
| **Removed**       | Deprecated or obsolete features removed from the project |
| **Security**      | Security-related fixes and improvements                  |
| **Documentation** | Updates to project documentation                         |

This categorization makes it easier to review changes across releases.

---

# 1.7 Change Management Process

The project follows a structured process for recording changes.

```text
Requirement or Issue
        │
        ▼
Implementation
        │
        ▼
Testing
        │
        ▼
Code Review
        │
        ▼
Merge into Main Branch
        │
        ▼
Update Change Log
        │
        ▼
Create Release
```

Updating the Change Log is a mandatory step before publishing a new software version.

---

# 1.8 Relationship with Other Documents

The Change Log complements the project's technical documentation.

| Document                            | Relationship                          |
| ----------------------------------- | ------------------------------------- |
| Software Requirements Specification | Records requirement-related changes   |
| Software Design Document            | Tracks design modifications           |
| Architecture Document               | Records architectural updates         |
| API Documentation                   | Tracks API additions or modifications |
| User Manual                         | Records user-facing feature updates   |
| Testing Report                      | Documents testing improvements        |
| Installation Guide                  | Records installation changes          |
| Developer Guide                     | Tracks development process updates    |

Together, these documents provide complete traceability for the project.

---

# 1.9 Version Tracking Principles

Each software release should include:

- Version number
- Release date
- Summary of changes
- New features
- Bug fixes
- Known issues
- Compatibility notes (if applicable)

Following consistent version tracking improves software maintenance and release management.

---

# 1.10 Chapter Summary

This chapter introduced the purpose and scope of the **Change Log** for the Digital Clock System. It explained the objectives, intended audience, importance of maintaining a change history, categories of recorded changes, the change management process, its relationship with other project documents, and the principles of version tracking.

The following chapter defines the versioning policy used by the project, including version numbering conventions and release management practices.

---

# Chapter 2 – Versioning Policy

---

# 2.1 Introduction

A well-defined versioning policy provides a consistent method for identifying software releases and tracking the evolution of the **Digital Clock System**. Version numbers help developers, testers, maintainers, and users determine the significance of changes introduced in each release.

The Digital Clock System follows a structured versioning approach to ensure that every release is uniquely identified and its purpose is clearly communicated.

---

# 2.2 Objectives of Versioning

The versioning policy aims to:

- Identify every software release uniquely.
- Distinguish between major, minor, and maintenance updates.
- Simplify release management.
- Improve compatibility tracking.
- Support debugging and maintenance.
- Provide a clear software evolution history.

Consistent versioning improves communication among all project stakeholders.

---

# 2.3 Version Number Format

The Digital Clock System uses the following version format:

```text
MAJOR.MINOR.PATCH
```

Example:

```text
1.0.0
```

Where:

| Component       | Description                                                                       |
| --------------- | --------------------------------------------------------------------------------- |
| **MAJOR** | Significant architectural or feature changes that may introduce incompatibilities |
| **MINOR** | New features or enhancements that remain backward compatible                      |
| **PATCH** | Bug fixes, documentation updates, performance improvements, or minor corrections  |

This format provides a simple and predictable versioning structure.

---

# 2.4 Version Number Examples

| Version         | Meaning                                                    |
| --------------- | ---------------------------------------------------------- |
| **1.0.0** | Initial stable release                                     |
| **1.1.0** | New features added                                         |
| **1.2.0** | Additional enhancements                                    |
| **1.2.1** | Bug fixes and minor improvements                           |
| **1.3.0** | Feature expansion                                          |
| **2.0.0** | Major architectural redesign or significant feature update |

Each version communicates the scale and impact of the associated changes.

---

# 2.5 Release Types

The project recognizes several categories of releases.

### Major Release

Major releases introduce significant functionality or architectural changes.

Examples:

- New application architecture
- Large-scale feature additions
- Breaking API changes
- Major redesign

---

### Minor Release

Minor releases introduce new functionality while maintaining backward compatibility.

Examples:

- Additional display themes
- Alarm functionality
- Stopwatch support
- User interface improvements

---

### Patch Release

Patch releases address defects and small improvements without introducing new features.

Examples:

- Bug fixes
- Performance optimizations
- Documentation corrections
- Minor configuration updates

---

# 2.6 Release Naming Convention

Each release should be identified using the following format:

```text
Version X.Y.Z
```

Example:

```text
Version 1.0.0
```

Optional release titles may be used for internal tracking.

Example:

```text
Version 1.0.0 – Initial Stable Release
```

This naming convention provides clarity and consistency across releases.

---

# 2.7 Version Lifecycle

The typical software version lifecycle is illustrated below.

```text
Planning
     │
     ▼
Development
     │
     ▼
Testing
     │
     ▼
Release Candidate
     │
     ▼
Stable Release
     │
     ▼
Maintenance
```

Each stage represents a milestone in the software development process.

---

# 2.8 Release Criteria

Before publishing a new version, the following criteria should be satisfied:

- All planned features are implemented.
- Critical defects have been resolved.
- Unit, integration, and system tests pass.
- Documentation is updated.
- Build process completes successfully.
- Code review has been completed.
- Change Log has been updated.

Meeting these criteria ensures that each release is stable and well documented.

---

# 2.9 Version Tagging

Git tags are used to mark official software releases.

Example:

```bash
git tag v1.0.0
git push origin v1.0.0
```

Tagging allows developers to reference specific releases easily and simplifies rollback when necessary.

---

# 2.10 Backward Compatibility

Whenever possible, new releases should remain compatible with previous stable versions.

Developers should:

- Preserve public interfaces.
- Maintain configuration compatibility.
- Avoid unnecessary breaking changes.
- Clearly document any incompatible modifications.

Maintaining backward compatibility reduces migration effort for users and developers.

---

# 2.11 Documentation Updates

Each release should include updates to the relevant project documentation.

The following documents should be reviewed:

| Document                            | Update Required |
| ----------------------------------- | --------------- |
| Software Requirements Specification | ✔              |
| Software Design Document            | ✔              |
| Architecture Document               | ✔              |
| API Documentation                   | ✔              |
| User Manual                         | ✔              |
| Testing Report                      | ✔              |
| Installation Guide                  | ✔              |
| Developer Guide                     | ✔              |
| ChangeLog                           | ✔              |

Keeping documentation synchronized with software releases improves traceability and maintainability.

---

# 2.12 Versioning Best Practices

To maintain a consistent version history:

- Increment version numbers according to the type of change.
- Tag all official releases.
- Record every significant modification in the Change Log.
- Avoid reusing version numbers.
- Keep release notes concise and informative.
- Update documentation before publishing a release.

These practices help maintain a clear and reliable release history.

---

# 2.13 Chapter Summary

This chapter defined the versioning policy for the Digital Clock System. It described the version numbering format, release categories, naming conventions, version lifecycle, release criteria, Git tagging practices, backward compatibility guidelines, documentation requirements, and versioning best practices.

A consistent versioning policy simplifies release management, improves project traceability, and provides a clear history of the software's evolution.

---

# Chapter 3 – Version History

---

# 3.1 Introduction

The **Version History** provides a chronological record of the evolution of the **Digital Clock System**. It documents significant milestones, software releases, feature additions, bug fixes, architectural improvements, testing activities, and documentation updates.

Maintaining a detailed version history allows developers, maintainers, and users to understand how the project has progressed over time and identify the changes introduced in each release.

---

# 3.2 Project Timeline

The Digital Clock System progressed through several development phases.

```text
Project Planning
        │
        ▼
Requirement Analysis
        │
        ▼
Software Design
        │
        ▼
Architecture Design
        │
        ▼
Core Development
        │
        ▼
Testing
        │
        ▼
Documentation
        │
        ▼
Version 1.0.0 Release
```

Each phase contributed to the successful completion of the project.

---

# 3.3 Version History Overview

The table below summarizes the official project versions.

| Version         | Status        | Description                             |
| --------------- | ------------- | --------------------------------------- |
| **0.1.0** | Development   | Initial project structure created       |
| **0.2.0** | Development   | Core modules implemented                |
| **0.3.0** | Development   | Display and configuration modules added |
| **0.4.0** | Testing       | Unit and integration testing introduced |
| **0.5.0** | Documentation | Technical documentation completed       |
| **1.0.0** | Stable        | Initial production release              |
| **1.1.0** | Stable        | Alarm module and notification support   |
| **1.2.0** | Stable        | Stopwatch and countdown timer           |

These versions represent the major milestones in the development lifecycle.

---

# 3.4 Version 0.1.0 – Initial Project Setup

### Release Status

Development

### Major Changes

#### Added

- Initial project directory structure.
- Build system configuration.
- Source and header directories.
- Documentation directory.
- Resource directory.
- Basic application framework.

#### Documentation

- Initial project planning.
- Development roadmap.

#### Notes

This version established the foundation for future development.

---

# 3.5 Version 0.2.0 – Core Development

### Release Status

Development

### Added

- Clock module.
- Date module.
- Display module.
- Utility module.
- Configuration module.

### Improved

- Project organization.
- Modular architecture.
- Source code structure.

### Notes

Core functionality required for the Digital Clock System was implemented during this phase.

---

# 3.6 Version 0.3.0 – Feature Expansion

### Release Status

Development

### Added

- Theme management.
- Resource loading.
- Logging support.
- Configuration handling improvements.

### Improved

- Display refresh logic.
- Error handling.
- File organization.

### Fixed

- Initial display formatting issues.
- Minor configuration parsing defects.

### Notes

This version focused on improving usability and maintainability.

---

# 3.7 Version 0.4.0 – Testing Phase

### Release Status

Testing

### Added

- Unit testing.
- Integration testing.
- System testing.
- Regression testing framework.

### Improved

- Test coverage.
- Error reporting.
- Build validation.

### Fixed

- Module integration issues.
- Resource loading defects.
- Configuration validation problems.

### Notes

Testing activities significantly improved application reliability.

---

# 3.8 Version 0.5.0 – Documentation Phase

### Release Status

Documentation

### Added

Complete technical documentation:

- Software Requirements Specification
- Software Design Document
- Architecture Document
- API Documentation
- User Manual
- Testing Report
- Installation Guide
- Developer Guide
- Change Log

### Improved

- Project documentation quality.
- Developer onboarding information.
- User guidance.

### Notes

This version completed the documentation required for the initial release.

---

# 3.9 Version 1.0.0 – Initial Stable Release

### Release Status

Stable

### Added

- Complete Digital Clock System.
- Modular architecture.
- Configuration support.
- Theme support.
- Logging functionality.
- Comprehensive documentation.
- Full testing suite.

### Improved

- Performance.
- Stability.
- Code organization.
- Documentation consistency.

### Fixed

- `main.cpp` used the configuration object before it was declared, which
  prevented the project from compiling at all (DEF-001).
- The CMake source list named an empty `Utils.cpp` while omitting
  `Banner.cpp`, `Screen.cpp` and `ResourceManager.cpp`, so the CMake build
  failed to link (DEF-002).
- `TimeFormatter.hpp` called `Clock::getHour()`, a method that did not exist
  (DEF-003).
- `Resources/config.ini` and `Config/config.ini` held conflicting settings
  and only one of them was ever read (DEF-004).
- Banner artwork was centred line by line, which sheared multi-line ASCII art
  (DEF-005).
- `Logs/application.log` was tracked in version control despite the `*.log`
  ignore rule (DEF-006).

### Release Verification

| Item | Result |
|------|--------|
| Release date | 2026-08-07 |
| Automated tests | 60 of 60 passed |
| Documented test cases | TC-001 – TC-025, all passed |
| Compiler warnings | 0 under `-Wall -Wextra -Wpedantic -Wshadow -Wconversion` |
| Build systems | `make` and CMake + CTest, both clean |
| Open defects | None |

### Notes

Version **1.0.0** is the first stable release of the Digital Clock System and
is intended for general use and future development.

It is verified on Linux. The Windows code paths are implemented but have not
been compiled or executed, so this release should not be described as
cross-platform verified. See section 5.6.

---

# 3.9a Version 1.1.0 - Alarm Module

### Release Status

Stable

### Added

- **Alarm module.** `Alarm` holds a time, label and recurrence rule;
  `AlarmManager` owns the configured set and decides when one fires.
- **Multiple alarms**, defined in `Config/alarms.ini` with numbered keys.
  Indices need not be contiguous.
- **Recurrence rules**: `Once`, `Daily`, `Weekdays`, `Weekends`, and explicit
  day lists such as `Mon,Wed,Fri`.
- **Snooze**, with the delay set by `SnoozeMinutes` (1-240, default 5).
  A snooze that crosses an hour or midnight wraps correctly.
- **Notification support.** `Notifier` composes an alert panel drawn beneath
  the clock and sounds the terminal bell each refresh until acknowledged.
- **Keyboard control**: `S` snoozes a ringing alarm, `D` dismisses it. Both
  redraw immediately rather than waiting out the refresh interval.
- **Next-alarm countdown** in the status bar.
- New theme element `ALERT`, added to all five bundled themes.
- Configuration keys `Alarms`, `AlarmFile`, `SnoozeMinutes` and `AlarmBell`.

### Design Notes

- An alarm reports itself due for its whole minute, so `AlarmManager` records
  the date and minute of each firing to keep it to a single ring. The date is
  part of the key so the same alarm still fires again the following day.
- Only one alarm rings at a time. A second alarm due in the same minute waits
  until the first is acknowledged rather than replacing it on screen.
- A one-shot alarm disarms itself when dismissed; a recurring one stays armed.
- A malformed alarm entry is skipped and counted, not fatal, so one bad line
  does not discard the rest of the file.

### Fixed

- The next-alarm countdown reported "due now" for the remainder of the minute
  in which an alarm had already rung and been acknowledged, implying an alarm
  was pending when none was. The countdown now excludes an alarm that has
  already fired in the current minute (TC-041).
- The countdown rendered a zero duration as "in now"; an alarm due this minute
  now reads "due now".

### Release Verification

| Item | Result |
|------|--------|
| Release date | 2026-08-07 |
| Automated tests | 78 of 78 passed |
| New test cases | TC-026 - TC-041 |
| Compiler warnings | 0 under `-Wall -Wextra -Wpedantic -Wshadow -Wconversion` |
| Build systems | `make` and CMake + CTest, both clean |
| End-to-end check | Alarm fired, snoozed and dismissed in the running application |
| Open defects | None |

### Notes

Version **1.1.0** delivers the alarm and notification functionality planned
for this release in section 6.3. Verified on Linux; the Windows gap recorded
as KI-000 is unchanged.

---

# 3.9b Version 1.2.0 - Stopwatch and Countdown Timer

### Release Status

Stable

### Added

- **Stopwatch** with start, stop, resume, lap and reset. Renders as
  `MM:SS.cc`, widening to `H:MM:SS.cc` past an hour, and retains up to 99
  laps with per-lap splits.
- **Countdown timer** with start, pause, resume and reset, starting from the
  `TimerDuration` setting. Announces expiry through the alert panel added in
  v1.1.0.
- **Three display modes** -- Clock, Stopwatch and Timer -- cycled with `M`.
  Whichever is active supplies the large centre readout.
- **Key bindings**: `Space` starts and stops, `L` records a lap, `R` resets.
  `S` and `D` remain alarm controls in every mode, and `D` also acknowledges
  a finished countdown.
- Configuration key `TimerDuration`, accepting `MM:SS`, `H:MM:SS` or a bare
  number of seconds, up to 24 hours.
- `Notifier::notify(heading, detail, hint)`, a general form of the existing
  alarm notification. The alarm overload now delegates to it.

### Design Notes

- **Both new components take the current time as a parameter** rather than
  reading a clock themselves. Two reasons: a monotonic reading
  (`std::chrono::steady_clock`) means a change to the system clock cannot
  corrupt a measurement in progress, and expiry can be asserted at an exact
  instant in tests without sleeping. A test that sleeps is slow and flaky.
- **Switching mode never disturbs the others.** A stopwatch started and left
  running keeps running while the clock is displayed, and alarms and the
  countdown are polled every frame regardless of what is on screen.
- **The stopwatch redraws faster than `RefreshInterval` while running.** At
  the default one-second refresh the hundredths would otherwise jump by a
  hundred at a time.
- **The countdown rounds its display up to the next whole second**, so it
  reads 3, 2, 1 and only then 00:00. Truncating would show 00:00 for the
  whole final second, which reads as finished before it is.
- **The timer fires exactly once per run**, matching the rule AlarmManager
  already applies: it is expired for as long as it sits at zero, but should
  announce itself a single time.

### Release Verification

| Item | Result |
|------|--------|
| Release date | 2026-08-07 |
| Automated tests | 91 of 91 passed |
| New test cases | TC-042 - TC-052 |
| Compiler warnings | 0 under `-Wall -Wextra -Wpedantic -Wshadow -Wconversion` |
| Platforms | Linux, Windows and macOS via CI |
| End-to-end check | Stopwatch started, lapped and read; timer counted down, expired and raised its panel |
| Open defects | None |

### Notes

Version **1.2.0** delivers the stopwatch and countdown timer planned for this
release in section 6.3.

---

# 3.10 Documentation Milestones

The following documentation was completed prior to the first stable release.

| Document                            | Status       |
| ----------------------------------- | ------------ |
| Software Requirements Specification | ✔ Completed |
| Software Design Document            | ✔ Completed |
| Architecture Document               | ✔ Completed |
| API Documentation                   | ✔ Completed |
| User Manual                         | ✔ Completed |
| Testing Report                      | ✔ Completed |
| Installation Guide                  | ✔ Completed |
| Developer Guide                     | ✔ Completed |
| Change Log                          | ✔ Completed |

This comprehensive documentation supports both users and developers.

---

# 3.11 Development Statistics

Project summary at Version **1.0.0**.

| Category                 | Status | Detail |
| ------------------------ | ------ | ------ |
| Architecture Designed    | ✔ | Four layers, 14 classes |
| Core Modules Implemented | ✔ | Clock, Date, TimeFormatter |
| Configuration System     | ✔ | INI parser with validation and defaults |
| Resource Management      | ✔ | Banner, logo and five themes |
| Logging System           | ✔ | Thread-safe, timestamped, severity-filtered |
| Testing Completed        | ✔ | 60 tests, all passing |
| Documentation Completed  | ✔ | Nine documents |
| Stable Release Published | ✔ | 2026-08-07 |

Measured figures at v1.0.0:

| Metric | Value |
|--------|-------|
| Classes | 14 |
| Header files | 16 |
| Source files | 16 |
| Test files | 11 |
| Automated tests | 60 |
| Test pass rate | 100% |
| Compiler warnings | 0 |
| External dependencies | 0 |
| Resident memory at runtime | 4,612 kB, stable |

These milestones indicate that the project has reached its initial
production-ready state on Linux.

---

# 3.12 Chapter Summary

This chapter presented the complete version history of the Digital Clock System. It documented the project's development timeline, major milestones, version progression, feature additions, testing phases, documentation completion, and the initial stable release.

Maintaining a detailed version history provides valuable traceability for future maintenance, enhancements, and software releases.

---

# Chapter 4 – Release Notes

---

# 4.1 Introduction

Release Notes provide a detailed summary of the features, improvements, bug fixes, documentation updates, and known limitations associated with each official release of the **Digital Clock System**. They serve as a communication bridge between the development team, testers, maintainers, and end users by highlighting what has changed since the previous version.

Every official release should be accompanied by release notes to ensure transparency and simplify software deployment, maintenance, and upgrades.

---

# 4.2 Purpose of Release Notes

The primary objectives of release notes are to:

- Inform users about new features.
- Document improvements made since the previous release.
- Record resolved defects.
- Highlight important configuration changes.
- Identify known issues.
- Provide upgrade information.
- Improve software traceability.

Proper release notes reduce confusion during software updates and maintenance.

---

# 4.3 Release Information

| Item                   | Details                |
| ---------------------- | ---------------------- |
| Project                | Digital Clock System   |
| Current Stable Version | **1.0.0**        |
| Release Type           | Initial Stable Release |
| Language               | C++17                  |
| Platform               | Cross-Platform Desktop |
| Build System           | Make / CMake           |

This release establishes the first production-ready version of the Digital Clock System.

---

# 4.4 Version 1.0.0 Release Summary

### Release Highlights

The **Version 1.0.0** release introduces the first complete implementation of the Digital Clock System.

Major achievements include:

- Complete modular application architecture.
- Real-time digital clock display.
- Date and time synchronization with the operating system.
- Configuration file support.
- Theme management.
- Logging framework.
- Comprehensive technical documentation.
- Complete testing suite.

This version serves as the foundation for future enhancements.

---

# 4.5 New Features

The following functionality has been introduced in Version **1.0.0**.

### Core Features

- Digital clock display.
- Automatic date display.
- Configurable refresh interval.
- Theme support.
- Resource management.
- Configuration loading.
- Application logging.

### Development Features

- Modular source code organization.
- Build automation.
- Cross-platform design.
- Comprehensive documentation.
- Testing framework.

These features provide a stable and extensible platform for future development.

---

# 4.6 Improvements

Several improvements were made during development.

### Architecture

- Improved modular design.
- Clear separation of responsibilities.
- Simplified project organization.

### Performance

- Reduced unnecessary processing.
- Optimized display updates.
- Efficient resource loading.

### Maintainability

- Improved code readability.
- Consistent coding standards.
- Better documentation coverage.

These improvements enhance software quality and maintainability.

---

# 4.7 Bug Fixes

The following issues were resolved before the stable release. Identifiers
match the defect summary in the Testing Report, section 8.7.

| ID | Issue | Severity | Resolution |
|----|-------|----------|------------|
| DEF-001 | `main.cpp` referenced the configuration object before declaring it, so the project did not compile | Critical | Fixed — configuration is now loaded before the components that read it |
| DEF-002 | CMake listed an empty `Utils.cpp` and omitted three real sources, so the build failed to link | Critical | Fixed — sources are built into a shared library used by both the executable and the tests |
| DEF-003 | `TimeFormatter.hpp` called a `Clock` method that did not exist | High | Fixed — the formatter now uses the documented accessors |
| DEF-004 | Two configuration files held conflicting settings and only one was read | Medium | Fixed — configuration lives solely in `Config/config.ini` |
| DEF-005 | Multi-line banner artwork was centred per line and appeared sheared | Cosmetic | Fixed — artwork is offset as a block |
| DEF-006 | The runtime log was tracked in version control | Low | Fixed — untracked; the logger recreates the directory |

All six are closed. Resolving DEF-001 through DEF-003 is what made the project
build for the first time.

---

# 4.8 Documentation Updates

The following documentation has been completed and reviewed.

| Document                            | Status       |
| ----------------------------------- | ------------ |
| Software Requirements Specification | ✔ Completed |
| Software Design Document            | ✔ Completed |
| Architecture Document               | ✔ Completed |
| API Documentation                   | ✔ Completed |
| User Manual                         | ✔ Completed |
| Testing Report                      | ✔ Completed |
| Installation Guide                  | ✔ Completed |
| Developer Guide                     | ✔ Completed |
| Change Log                          | ✔ Completed |

Complete documentation supports future development, testing, deployment, and maintenance.

---

# 4.9 Compatibility Information

Version **1.0.0** compatibility, separating what was verified from what was
only written:

| Component | Status | Basis |
| --------- | ------ | ----- |
| C++ Standard | C++17 | Required |
| GNU Compiler Collection (GCC) | **Verified** | GCC 16.1.1, zero warnings |
| GNU Make | **Verified** | GNU Make 4.4.1 |
| CMake | **Verified** | CMake 4.4.2, with CTest |
| Linux | **Verified** | Garuda Linux, kernel 7.1.5-zen1-2-zen (x86_64) |
| Windows | **Verified** | CI: MSVC 19.51, 78 tests passed, frame rendered |
| MSVC | **Verified** | MSVC 19.51.36252, Visual Studio 18 2026 |
| macOS | **Verified** | CI: suite passed, frame rendered |

Verified by CI run 31142229662 on 2026-08-07. Every change to `DigitalClock/`
re-runs this matrix, so it stays evidence-based.

---

# 4.10 Upgrade Notes

For users upgrading from earlier development versions:

- Replace previous source files with the latest version.
- Rebuild the application using the updated build configuration.
- Replace outdated configuration files if new options have been introduced.
- Review the updated documentation before deployment.

Following these steps ensures a smooth transition to Version **1.0.0**.

---

# 4.11 Known Limitations

The current release has the following limitations:

- Alarm functionality is not yet implemented.
- Stopwatch and countdown timer are planned for future versions.
- GUI support is not included in Version 1.0.0.
- Plugin support is reserved for future releases.
- Automatic update functionality is not available.

These limitations are planned to be addressed in subsequent releases.

---

# 4.12 Future Release Preview

Planned enhancements for future versions include:

- Alarm and reminder module.
- Stopwatch.
- Countdown timer.
- Multiple time zones.
- Graphical user interface.
- Additional themes.
- Localization support.
- Plugin architecture.
- Performance improvements.

These planned features will expand the functionality of the Digital Clock System while maintaining its modular architecture.

---

# 4.13 Chapter Summary

This chapter presented the official release notes for Version **1.0.0** of the Digital Clock System. It summarized the release information, new features, improvements, bug fixes, documentation updates, compatibility details, upgrade guidance, known limitations, and planned future enhancements.

Release Notes provide a concise overview of each software version and serve as an important reference for developers, testers, maintainers, and end users.

---

# Chapter 5 – Known Issues

---

# 5.1 Introduction

This chapter documents the **known issues and current limitations** of the **Digital Clock System** at the time of each software release. Recording known issues helps developers, testers, maintainers, and end users understand the current state of the application and provides transparency regarding features that are planned for future improvement.

Not every issue is considered critical. Some items listed in this chapter are limitations, enhancement requests, or planned improvements that do not affect the normal operation of the application.

---

# 5.2 Purpose

The objectives of maintaining a Known Issues section are to:

- Inform users about current limitations.
- Document unresolved defects.
- Help developers prioritize future work.
- Reduce duplicate bug reports.
- Improve release transparency.
- Support maintenance planning.

Maintaining an up-to-date list of known issues improves project management and user expectations.

---

# 5.3 Issue Classification

Known issues are categorized according to their severity.

| Severity    | Description                                         |
| ----------- | --------------------------------------------------- |
| Critical    | Prevents the application from functioning correctly |
| High        | Major functionality is affected                     |
| Medium      | Feature works with minor limitations                |
| Low         | Cosmetic or non-critical issue                      |
| Enhancement | Suggested improvement or future feature             |

This classification helps prioritize maintenance and future development efforts.

---

# 5.4 Current Known Issues (Version 1.0.0)

The following issues have been identified for Version **1.0.0**.

Verification gaps carried by this release:

| Issue ID | Description | Severity | Status |
| -------- | ----------- | -------- | ------ |
| ~~KI-000~~ | ~~Windows support is implemented but has never been compiled or executed~~ | Medium | **Closed 2026-08-07** — verified by CI under MSVC 19.51; 78 tests passed and the application rendered a frame |
| KI-007   | No User Acceptance Testing was performed | Low | Open |
| KI-008   | Line coverage is not measured; coverage is stated at component level only | Low | Open |
| KI-009   | `Console` has no direct automated tests; it is covered indirectly and by one manual check under a pseudo-terminal | Low | Open |

Features deferred to future releases:

| Issue ID | Description | Severity | Status |
| -------- | ----------- | -------- | ------ |
| ~~KI-001~~ | ~~Alarm functionality is not implemented~~ | Enhancement | **Closed in v1.1.0** |
| ~~KI-002~~ | ~~Stopwatch feature is unavailable~~ | Enhancement | **Closed in v1.2.0** |
| ~~KI-003~~ | ~~Countdown timer is not available~~ | Enhancement | **Closed in v1.2.0** |
| KI-004   | Multiple time zone support is unavailable | Enhancement | Open |
| KI-005   | Graphical User Interface (GUI) is not implemented | Enhancement | Open |
| KI-006   | Plugin architecture is not available | Enhancement | Open |

KI-000 is closed. It was the only item that affected a claim already made
about a release, and it was closed by evidence — a Windows CI run — rather
than by revising the claim. macOS was verified by the same run and is now
supported in practice as well as in intent.

KI-009 is partly addressed: `Console` still has no direct unit tests, but the
CI smoke step now executes it on all three platforms via `--once`.

The remaining items describe work not yet attempted. The enhancement items are
planned for consideration in future releases.

---

# 5.5 Functional Limitations

The current release has the following functional limitations:

- Displays only the local system time.
- Does not provide alarm notifications.
- Does not include stopwatch functionality.
- Does not support countdown timers.
- Configuration changes may require an application restart.
- No built-in update mechanism is available.

These limitations do not prevent normal clock functionality but define the scope of Version **1.0.0**.

---

# 5.6 Platform Limitations

Current platform-related limitations include:

| Platform | Limitation |
| -------- | ---------- |
| Linux | Fully supported and verified |
| Windows | Supported and verified in CI under MSVC. The Makefile is POSIX-only, so use the CMake build on Windows |
| macOS | Supported and verified in CI |
| Mobile Platforms | Not supported |

Additional platform support may be introduced in future versions.

---

# 5.7 Documentation Limitations

At the current release:

- Documentation is based on Version **1.0.0**.
- Future features are documented only as planned enhancements.
- API documentation will expand as new modules are introduced.
- User manual will be updated with new functionality in future releases.

Documentation will continue evolving alongside the software.

---

# 5.8 Workarounds

For some current limitations, the following workarounds are recommended.

| Limitation            | Suggested Workaround                              |
| --------------------- | ------------------------------------------------- |
| Alarm unavailable     | Use operating system alarm utilities              |
| Stopwatch unavailable | Use a separate stopwatch application              |
| Countdown unavailable | Use an external timer application                 |
| GUI unavailable       | Operate through the console interface             |
| Multiple time zones   | Change the operating system time zone if required |

These workarounds provide temporary alternatives until native functionality is implemented.

---

# 5.9 Reporting New Issues

When reporting a new issue, developers and users should include:

- Software version
- Operating system
- Compiler version (if applicable)
- Steps to reproduce
- Expected behavior
- Actual behavior
- Error messages or logs
- Screenshots (if applicable)

Complete reports enable faster investigation and resolution.

---

# 5.10 Issue Resolution Process

The project follows the workflow below for handling reported issues.

```text
Issue Reported
       │
       ▼
Issue Verification
       │
       ▼
Priority Assignment
       │
       ▼
Developer Assignment
       │
       ▼
Bug Fix Implementation
       │
       ▼
Testing & Validation
       │
       ▼
Documentation Update
       │
       ▼
Issue Closed
```

This process ensures consistent handling and tracking of defects.

---

# 5.11 Future Resolution Plan

The following enhancements are planned for future releases.

| Planned Feature      | Target Status |
| -------------------- | ------------- |
| Alarm Module         | Planned       |
| Stopwatch            | Planned       |
| Countdown Timer      | Planned       |
| GUI Version          | Planned       |
| Multiple Time Zones  | Planned       |
| Plugin System        | Planned       |
| Automatic Updates    | Planned       |
| Localization Support | Planned       |

These planned improvements will extend the capabilities of the Digital Clock System while maintaining its modular architecture.

---

# 5.12 Chapter Summary

This chapter documented the known issues and limitations of Version **1.0.0** of the Digital Clock System. It introduced issue classification, current open items, functional and platform limitations, documentation constraints, recommended workarounds, the issue reporting process, and the planned resolution strategy.

Maintaining an accurate list of known issues improves transparency, supports project planning, and helps guide future development and maintenance activities.

---

# Chapter 6 – Future Releases

---

# 6.1 Introduction

The **Digital Clock System** has been designed using a modular and extensible architecture, allowing new features and improvements to be integrated with minimal impact on existing components. This chapter outlines the planned direction for future releases, including feature enhancements, performance improvements, platform support, and long-term development goals.

The roadmap presented here is intended for planning purposes and may be updated based on project requirements, user feedback, and development priorities.

---

# 6.2 Future Development Objectives

The primary objectives for future releases are to:

- Expand application functionality.
- Improve performance and responsiveness.
- Enhance user experience.
- Increase platform compatibility.
- Strengthen security and reliability.
- Simplify configuration and customization.
- Maintain a modular and maintainable architecture.

These objectives provide a clear direction for the continued evolution of the project.

---

# 6.3 Planned Release Roadmap

The following roadmap outlines the expected progression of future versions.

| Version         | Planned Focus                                                       |
| --------------- | ------------------------------------------------------------------- |
| ~~**1.1.0**~~ | ~~Alarm module and notification support~~ - **delivered 2026-08-07** |
| ~~**1.2.0**~~ | ~~Stopwatch and countdown timer~~ - **delivered 2026-08-07** |
| **1.2.0** | Stopwatch and countdown timer                                       |
| **1.3.0** | Multiple time zone support                                          |
| **1.4.0** | Theme enhancements and improved configuration                       |
| **2.0.0** | Graphical User Interface (GUI) and major architectural improvements |

This roadmap is subject to change based on project priorities.

---

# 6.4 Planned Features

Future versions may introduce the following functionality.

### Time Management

- Alarm clock.
- Multiple alarms.
- Snooze functionality.
- Recurring alarms.
- Countdown timer.
- Stopwatch.

### Display Enhancements

- Multiple display themes.
- Custom color schemes.
- Adjustable fonts.
- Full-screen display mode.
- Improved date and time formatting.

### User Experience

- Simplified configuration.
- Enhanced keyboard shortcuts.
- Improved accessibility.
- Localization and multilingual support.

These enhancements will improve usability while maintaining the lightweight nature of the application.

---

# 6.5 Architecture Improvements

Future architectural enhancements may include:

- Plugin-based extension framework.
- Improved module isolation.
- Better dependency management.
- Configuration abstraction layer.
- Enhanced logging subsystem.
- Improved resource management.

Maintaining a modular architecture will simplify future expansion and maintenance.

---

# 6.6 Performance Improvements

Planned performance optimizations include:

- Faster application startup.
- Reduced CPU utilization.
- Improved memory management.
- Optimized display refresh logic.
- More efficient resource loading.
- Background processing for non-critical tasks.

Performance improvements will continue to be guided by profiling and benchmarking results.

---

# 6.7 Platform Expansion

Future releases may extend platform compatibility.

| Platform       | Planned Support                     |
| -------------- | ----------------------------------- |
| Linux          | Continued support                   |
| Windows        | Continued support                   |
| macOS          | Planned validation and optimization |
| Embedded Linux | Future evaluation                   |
| Raspberry Pi   | Planned evaluation                  |

Additional platform support will depend on community feedback and project requirements.

---

# 6.8 Documentation Improvements

As new functionality is introduced, the following documentation will be updated:

- Software Requirements Specification.
- Software Design Document.
- Architecture Document.
- API Documentation.
- User Manual.
- Testing Report.
- Installation Guide.
- Developer Guide.
- Change Log.

Maintaining synchronized documentation ensures consistency across the project.

---

# 6.9 Long-Term Vision

The long-term vision for the Digital Clock System includes:

- A feature-rich yet lightweight desktop application.
- High code quality through modular design.
- Comprehensive automated testing.
- Continuous documentation updates.
- Easy customization and extension.
- Stable cross-platform support.
- A collaborative development environment.

This vision supports sustainable growth while preserving maintainability.

---

# 6.10 Release Planning Process

Future releases should follow the development workflow below.

```text
Requirement Collection
         │
         ▼
Planning
         │
         ▼
Design
         │
         ▼
Implementation
         │
         ▼
Testing
         │
         ▼
Documentation
         │
         ▼
Release
```

A structured release process improves software quality and predictability.

---

# 6.11 Future Release Checklist

Before publishing a future release, verify the following.

| Verification Item          | Status |
| -------------------------- | ------ |
| Planned features completed | ✔     |
| Code reviewed              | ✔     |
| Testing completed          | ✔     |
| Documentation updated      | ✔     |
| Performance evaluated      | ✔     |
| Known issues reviewed      | ✔     |
| Version tagged             | ✔     |
| Change Log updated         | ✔     |

Following this checklist helps ensure that each release is complete, stable, and well documented.

---

# 6.12 Chapter Summary

This chapter presented the planned future direction of the Digital Clock System. It described the development objectives, projected release roadmap, planned features, architectural and performance improvements, platform expansion, documentation updates, long-term vision, release planning process, and release readiness checklist.

By following this roadmap, the project can continue to evolve in a structured and maintainable manner while delivering additional functionality and maintaining software quality.

---

# Chapter 7 – Conclusion

---

# 7.1 Introduction

This chapter concludes the **Change Log** for the **Digital Clock System**. It summarizes the project's version history, release management practices, and the importance of maintaining an accurate record of software changes throughout the development lifecycle.

The Change Log serves as the official historical record of the project, enabling developers, maintainers, testers, and users to understand how the software has evolved across different releases.

---

# 7.2 Document Summary

Throughout this document, the following topics have been covered:

- Purpose and scope of the Change Log.
- Versioning policy and release numbering.
- Complete version history.
- Release notes for Version **1.0.0**.
- Known issues and current limitations.
- Future release roadmap.
- Best practices for maintaining version history.

Together, these sections provide a comprehensive overview of the evolution of the Digital Clock System.

---

# 7.3 Importance of Maintaining a Change Log

Maintaining a structured Change Log offers several benefits:

- Provides complete traceability of software changes.
- Simplifies debugging and maintenance.
- Improves communication among developers.
- Helps testers identify changes between releases.
- Assists users in understanding new features and improvements.
- Supports project audits and software documentation.
- Preserves the historical evolution of the project.

An accurate Change Log is an essential component of professional software development and release management.

---

# 7.4 Release Management Best Practices

To ensure consistency across future releases, the following practices should be adopted:

- Assign a unique version number to every official release.
- Document all significant additions, changes, fixes, and improvements.
- Update the Change Log before publishing a new release.
- Tag official releases in the version control system.
- Maintain synchronization between the Change Log and other project documentation.
- Archive previous releases for future reference.

Following these practices improves transparency and simplifies long-term maintenance.

---

# 7.5 Recommendations for Future Updates

As the Digital Clock System continues to evolve, developers should ensure that the Change Log is updated whenever:

- New functionality is introduced.
- Existing features are modified.
- Bugs are fixed.
- Security improvements are implemented.
- Performance optimizations are completed.
- Documentation is revised.
- Build tools or dependencies change.
- New versions are released.

Consistent updates ensure that the document remains a reliable source of project history.

---

# 7.6 Relationship with Project Documentation

The Change Log complements the other documents within the Digital Clock System documentation suite.

| Document                            | Purpose                                                     |
| ----------------------------------- | ----------------------------------------------------------- |
| Software Requirements Specification | Defines functional and non-functional requirements          |
| Software Design Document            | Describes software design and module interactions           |
| Architecture Document               | Explains the overall system architecture                    |
| API Documentation                   | Documents application interfaces                            |
| User Manual                         | Guides end users in operating the application               |
| Testing Report                      | Records testing strategy and results                        |
| Installation Guide                  | Describes installation and configuration                    |
| Developer Guide                     | Assists developers in extending and maintaining the project |
| Change Log                          | Records the complete history of project changes             |

Together, these documents provide complete technical and user documentation for the project.

---

# 7.7 Final Release Status

The Digital Clock System Version **1.0.0** has achieved the following milestones.

| Milestone                    | Status |
| ---------------------------- | ------ |
| Requirements Completed       | ✔     |
| Software Design Completed    | ✔     |
| Architecture Completed       | ✔     |
| Core Development Completed   | ✔     |
| Testing Completed            | ✔     |
| Documentation Completed      | ✔     |
| Installation Guide Completed | ✔     |
| Developer Guide Completed    | ✔     |
| Change Log Completed         | ✔     |
| Initial Stable Release Ready | ✔     |

These milestones indicate that the project has reached its first stable and fully documented release.

---

# 7.8 Final Remarks

The **Digital Clock System** represents the successful completion of a structured software engineering project developed using modern development practices and comprehensive documentation.

The Change Log will continue to evolve alongside the project, recording future enhancements, maintenance updates, bug fixes, and new releases. Maintaining this document ensures transparency, accountability, and traceability throughout the software lifecycle.

Developers and maintainers are encouraged to update the Change Log as an integral part of every release, ensuring that the project's history remains accurate, complete, and valuable for future contributors.

---

# 7.9 Document Information

| Item | Details |
| ---- | ------- |
| Document | **09_ChangeLog.md** |
| Document Version | **1.4** |
| Project | **Digital Clock System** |
| Current Version | **1.2.0** |
| Release Date | **2026-08-07** |
| Language | **C++17** |
| Status | **Released** |
| Verified On | Linux (GCC 16.1.1), Windows (MSVC 19.51) and macOS, via CI |
| Test Result | 91 of 91 automated tests passed; no open defects |
| Known Gaps | No UAT (KI-007); line coverage not measured (KI-008) |
| Audience | Developers, Maintainers, Test Engineers, Project Managers, End Users |

---

# 7.10 Conclusion

The **Change Log** provides a complete historical record of the Digital Clock System's development, from initial planning through the first stable release. By documenting version history, release notes, known issues, and future plans, it supports effective project management, software maintenance, and continuous improvement.

Maintaining this document as part of every release ensures that the project's evolution remains transparent, organized, and easy to understand for all stakeholders.

---

# End of Document

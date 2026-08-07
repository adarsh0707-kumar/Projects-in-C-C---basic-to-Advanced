# 06_Testing_Report.md

# Chapter 1 – Introduction

---

# 1.1 Purpose

The purpose of this **Testing Report** is to document the testing strategy, methodology, test environment, test cases, and expected outcomes for the **Digital Clock System**. This report demonstrates that the application has been designed to meet its functional and non-functional requirements while maintaining reliability, stability, and performance.

The document serves as evidence that the system has been systematically verified and validated throughout its development lifecycle. It also provides a structured approach for future testing activities as the project evolves.

---

# 1.2 Scope

This Testing Report covers all major components and functionalities of the Digital Clock System, including:

- Clock module
- Date module
- Display module
- Configuration manager
- Theme manager
- Logger
- Resource manager
- Application startup
- Application shutdown
- Error handling
- File management
- User interface
- Integration between software components

Both functional and non-functional aspects of the system are included.

---

# 1.3 Objectives

The primary objectives of testing are:

- Verify that each software module functions correctly.
- Validate that the complete system satisfies the Software Requirements Specification (SRS).
- Identify defects before software release.
- Ensure reliable operation under normal conditions.
- Verify proper handling of invalid configurations and runtime errors.
- Confirm correct integration between software components.
- Evaluate application performance and stability.
- Ensure maintainability for future enhancements.

---

# 1.4 Importance of Testing

Testing is an essential phase of the Software Development Life Cycle (SDLC). It helps ensure that the application performs as expected and minimizes the risk of failures after deployment.

Benefits of testing include:

- Improved software quality
- Reduced software defects
- Increased reliability
- Better user experience
- Easier maintenance
- Lower long-term development costs
- Higher confidence before software release

For the Digital Clock System, testing verifies that the displayed time and date remain accurate, configuration settings are applied correctly, and the application continues to operate reliably over extended periods.

---

# 1.5 Testing Approach

The Digital Clock System follows a structured testing approach consisting of multiple testing levels.

```text
Requirements
      │
      ▼
Unit Testing
      │
      ▼
Integration Testing
      │
      ▼
System Testing
      │
      ▼
Acceptance Testing
      │
      ▼
Software Release
```

Each testing level focuses on different aspects of the application to ensure comprehensive verification.

---

# 1.6 Testing Types

The following testing activities are planned for the project.

| Testing Type | Purpose |
|--------------|---------|
| Unit Testing | Verify individual modules and functions |
| Integration Testing | Verify interaction between modules |
| System Testing | Validate complete application behavior |
| Functional Testing | Confirm all required features work correctly |
| Regression Testing | Ensure existing functionality remains unaffected after changes |
| Performance Testing | Measure speed, responsiveness, and resource usage |
| Compatibility Testing | Verify operation on supported platforms |
| User Acceptance Testing (UAT) | Confirm the application meets user expectations |

---

# 1.7 Testing Standards

Testing activities are performed in accordance with established software engineering practices.

The testing process emphasizes:

- Requirement-based testing
- Repeatable test procedures
- Independent verification
- Consistent documentation
- Traceability between requirements and test cases
- Accurate recording of test results

These principles help ensure reliable and maintainable testing throughout the project lifecycle.

---

# 1.8 Test Environment Overview

Testing is planned using the following environment.

| Component | Description |
|-----------|-------------|
| Operating System | Linux / Windows |
| Programming Language | C++17 |
| Compiler | GCC / G++ |
| Build System | Make / CMake |
| Version Control | Git |
| Documentation | Markdown |
| Terminal | Bash, Command Prompt, or PowerShell |

The environment reflects the supported development and execution platforms for the Digital Clock System.

---

# 1.9 Testing Goals

The Digital Clock System should satisfy the following goals during testing.

- Correctly display the current system time.
- Correctly display the current system date.
- Apply configuration settings successfully.
- Load themes without errors.
- Handle missing or invalid resources gracefully.
- Generate log files when enabled.
- Operate continuously without unexpected termination.
- Maintain acceptable CPU and memory usage.
- Shut down safely without resource leaks.

Achievement of these goals indicates that the application is ready for deployment.

---

# 1.10 Assumptions

The following assumptions apply to this testing report.

- The application is built successfully.
- Required configuration files are available.
- Resource files exist in the expected locations.
- The system clock is configured correctly.
- Supported compilers and operating systems are used.
- Users have appropriate permissions to execute the application.

These assumptions establish the baseline conditions for planned testing.

---

# 1.11 Document Organization

This Testing Report is organized into the following chapters.

| Chapter | Description |
|----------|-------------|
| Chapter 1 | Introduction |
| Chapter 2 | Test Planning |
| Chapter 3 | Test Environment |
| Chapter 4 | Unit Testing |
| Chapter 5 | Integration Testing |
| Chapter 6 | System Testing |
| Chapter 7 | Test Cases |
| Chapter 8 | Test Results |
| Chapter 9 | Validation |
| Chapter 10 | Conclusion |

Each chapter focuses on a specific aspect of the software testing process.

---

# 1.12 Chapter Summary

This chapter introduced the purpose, scope, objectives, and overall testing strategy for the Digital Clock System. It described the planned testing approach, testing types, standards, environment, goals, and assumptions that form the foundation of the testing process.

The following chapter presents the detailed **Test Planning**, including the testing strategy, schedule, entry and exit criteria, and planned testing deliverables.

---

# Chapter 2 – Test Planning

---

# 2.1 Introduction

Test planning is the foundation of the software testing process. It defines **what will be tested, how testing will be performed, who will perform the testing, and when the testing activities will occur**.

For the **Digital Clock System**, the test plan ensures that every software component is verified against the Software Requirements Specification (SRS) and Software Design Document (SDD). It also provides a structured roadmap for executing tests throughout the development lifecycle.

---

# 2.2 Test Objectives

The primary objectives of testing are:

- Verify all functional requirements.
- Validate non-functional requirements.
- Detect software defects early.
- Ensure stable application behavior.
- Verify module integration.
- Confirm proper error handling.
- Measure application performance.
- Improve software reliability.
- Ensure deployment readiness.

---

# 2.3 Testing Strategy

The Digital Clock System follows a **layered testing strategy**, where testing progresses from individual components to the complete application.

```text
             Software Requirements
                      │
                      ▼
               Unit Testing
                      │
                      ▼
            Integration Testing
                      │
                      ▼
              System Testing
                      │
                      ▼
        User Acceptance Testing
                      │
                      ▼
               Software Release
```

Each testing phase builds upon the results of the previous phase to ensure comprehensive verification.

---

# 2.4 Testing Levels

The project includes several levels of testing.

| Testing Level | Purpose |
|---------------|---------|
| Unit Testing | Verify individual classes and functions |
| Integration Testing | Verify communication between modules |
| System Testing | Validate the complete application |
| Regression Testing | Ensure existing functionality remains unaffected |
| Performance Testing | Measure speed and resource usage |
| Acceptance Testing | Confirm that the application satisfies user requirements |

---

# 2.5 Features to be Tested

The following features are included in the testing scope.

| Module | Test Coverage |
|----------|--------------|
| Clock Module | Time retrieval and updates |
| Date Module | Date formatting and display |
| Display Module | Console output |
| Configuration Manager | Reading configuration files |
| Logger | Log generation |
| Theme Manager | Theme loading |
| Resource Manager | Resource loading |
| Startup Process | Initialization |
| Shutdown Process | Cleanup |
| Error Handling | Exception and recovery mechanisms |

---

# 2.6 Features Not Tested

The following items are outside the scope of this testing report.

- Operating system internals
- Compiler implementation
- Hardware clock accuracy
- Third-party terminal software
- External libraries not developed as part of the project

These components are assumed to function according to their own specifications.

---

# 2.7 Test Schedule

The testing process is planned in sequential phases.

| Phase | Activity |
|--------|----------|
| Phase 1 | Unit Testing |
| Phase 2 | Integration Testing |
| Phase 3 | System Testing |
| Phase 4 | Regression Testing |
| Phase 5 | User Acceptance Testing |
| Phase 6 | Final Verification |

This phased approach helps identify issues early and ensures systematic validation.

---

# 2.8 Entry Criteria

Testing may begin only after the following conditions are satisfied.

- Source code compiles successfully.
- Build process completes without errors.
- Required documentation is available.
- Configuration files are prepared.
- Test environment is configured.
- Required resources are present.
- Test cases have been reviewed and approved.

Meeting these criteria ensures that testing begins under controlled and repeatable conditions.

---

# 2.9 Exit Criteria

Testing is considered complete when:

- All planned test cases have been executed.
- Critical defects have been resolved.
- No blocking issues remain.
- Functional requirements are satisfied.
- Acceptance criteria are met.
- Test documentation has been completed.
- Project stakeholders approve the release.

---

# 2.10 Test Deliverables

The testing process produces the following deliverables.

| Deliverable | Description |
|--------------|-------------|
| Test Plan | Overall testing strategy |
| Test Cases | Detailed testing procedures |
| Test Data | Input values used during testing |
| Test Execution Report | Results of executed tests |
| Defect Report | List of identified defects |
| Validation Report | Requirement verification |
| Final Testing Report | Complete testing summary |

---

# 2.11 Roles and Responsibilities

The following roles participate in the testing process.

| Role | Responsibility |
|------|----------------|
| Developer | Unit testing and defect fixing |
| Tester | Execute test cases |
| Reviewer | Review test reports |
| Project Maintainer | Approve testing completion |
| End User | Perform acceptance testing |

In smaller projects, one individual may perform multiple roles.

---

# 2.12 Risk Assessment

Potential risks during testing include:

| Risk | Mitigation |
|------|------------|
| Incomplete requirements | Review SRS before testing |
| Missing configuration | Verify project files |
| Environment mismatch | Standardize test environment |
| Resource file corruption | Maintain backup copies |
| Build failures | Verify compiler and build tools |
| Late defect discovery | Perform continuous testing |

Identifying risks early improves the effectiveness of the testing process.

---

# 2.13 Test Documentation

All testing activities should be documented for future reference.

Documentation includes:

- Test plans
- Test cases
- Test results
- Defect reports
- Validation reports
- Requirement traceability
- Regression test records

Maintaining detailed records improves project quality and simplifies maintenance.

---

# 2.14 Success Criteria

The Digital Clock System will be considered successfully tested if:

- All critical functions operate correctly.
- No critical or high-severity defects remain.
- The application starts successfully.
- Configuration files load correctly.
- Themes are applied correctly.
- Logging functions properly.
- The application operates continuously without unexpected termination.
- User Acceptance Testing is completed successfully.

---

# 2.15 Chapter Summary

This chapter described the overall **Test Planning** for the Digital Clock System, including testing objectives, strategy, testing levels, project scope, schedules, entry and exit criteria, deliverables, responsibilities, risks, documentation, and success criteria.

A well-defined test plan provides the framework for executing organized, repeatable, and effective testing activities throughout the software development lifecycle.

---

# Chapter 3 – Test Environment

---

# 3.1 Introduction

The **Test Environment** defines the hardware, software, tools, and configurations used to verify the functionality, reliability, and performance of the **Digital Clock System**. A consistent test environment ensures that test cases can be executed repeatedly with reliable and reproducible results.

This chapter describes the environment prepared for testing the application before deployment.

---

# 3.2 Test Environment Objectives

The objectives of the test environment are to:

- Provide a stable platform for testing.
- Ensure repeatable test execution.
- Verify compatibility across supported operating systems.
- Validate application behavior under standard conditions.
- Support debugging and defect analysis.

---

# 3.3 Hardware Environment

The following hardware configuration is recommended for testing.

| Component | Minimum Requirement | Recommended |
|-----------|---------------------|-------------|
| Processor | Dual-Core CPU | Intel Core i5 / AMD Ryzen 5 or higher |
| RAM | 2 GB | 8 GB or more |
| Storage | 100 MB Free Space | 1 GB Free Space |
| Display | 1024 × 768 | 1920 × 1080 |
| Keyboard | Standard Keyboard | Standard Keyboard |

The Digital Clock System has minimal hardware requirements due to its lightweight console-based design.

---

# 3.4 Software Environment

The environment in which the recorded results were produced is shown below.

| Component | Supported | Used for This Run |
|-----------|-----------|-------------------|
| Operating System | Linux / Windows | Garuda Linux, kernel 7.1.5-zen1-2-zen (x86_64) |
| Programming Language | C++17 | C++17 |
| Compiler | GCC / G++, Clang, MSVC (C++17) | GCC 16.1.1 (20260728) |
| Build Tool | Make / CMake | GNU Make 4.4.1 and CMake 4.4.2 |
| Version Control | Git | Git |
| Terminal | Bash, Command Prompt, PowerShell | Bash |
| Documentation Format | Markdown (`.md`) | Markdown |

The application depends on the C++ standard library only; no third-party
libraries are required to build or run the tests.

---

# 3.5 Development Tools

The following tools are used throughout development and testing.

| Tool | Purpose |
|------|---------|
| Visual Studio Code | Source code editing |
| GCC / G++ | Compilation |
| Make | Automated build |
| CMake | Cross-platform build configuration |
| Git | Version control |
| GitHub | Source code hosting |
| Terminal | Application execution |

These tools provide a complete environment for software development and testing.

---

# 3.6 Project Configuration

The project directory used during testing is organized as follows.

```text
DigitalClock/
│
├── Build/
├── Config/
├── Docs/
├── Inc/
├── Logs/
├── Resources/
├── Src/
├── Tests/
├── Makefile
└── CMakeLists.txt
```

Maintaining this structure ensures that the application can locate configuration files, resources, and test data correctly.

---

# 3.7 Configuration Files

The primary configuration file used during testing is:

```text
Config/config.ini
```

Example configuration:

```ini
TimeFormat=24
DateFormat=LONG
Theme=Dark
RefreshInterval=1000
Logging=Enabled
```

These settings remain constant during standard testing unless a test case specifically requires different values.

---

# 3.8 Test Data

Testing uses a combination of valid, invalid, and boundary inputs.

### Examples

- Valid configuration values
- Invalid theme names
- Missing configuration files
- Missing resource files
- Incorrect refresh intervals
- Corrupted configuration entries

Using varied test data helps evaluate the application's robustness.

---

# 3.9 Build Environment

The application is compiled using the supported build systems.

### Using Make

```bash
make
```

### Using CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

A successful build is required before executing any test cases.

---

# 3.10 Runtime Environment

During execution, the following conditions are maintained:

- Correct system date and time
- Accessible configuration files
- Available resource files
- Sufficient file permissions
- Stable operating system environment

These conditions help ensure consistent test execution.

---

# 3.11 Logging Environment

Logging is enabled during testing to record application events and detect unexpected behavior.

Example log file location:

```text
Logs/application.log
```

Typical log entries include:

```text
[INFO] Application Started
[INFO] Configuration Loaded
[INFO] Theme Applied
[INFO] Application Closed
```

Log files provide valuable information for debugging and validation.

---

# 3.12 Test Environment Verification

Before executing tests, the following checklist is completed.

| Verification Item | Status |
|-------------------|--------|
| Compiler installed | ✔ |
| Build completed successfully | ✔ |
| Configuration file available | ✔ |
| Resource files present | ✔ |
| Log directory accessible | ✔ |
| Application launches successfully | ✔ |
| Test data prepared | ✔ |

Only after successful verification does formal testing begin.

---

# 3.13 Environment Limitations

The planned test environment has the following limitations:

- Hardware clock accuracy is outside the scope of testing.
- Operating system internal functions are not tested.
- Third-party terminal applications are assumed to function correctly.
- Network connectivity is not required because the application operates locally.

These limitations help define the boundaries of the testing process.

---

# 3.14 Chapter Summary

This chapter described the test environment prepared for the Digital Clock System, including hardware and software requirements, development tools, project configuration, test data, build procedures, runtime conditions, logging environment, verification checklist, and environment limitations.

A standardized and well-documented test environment ensures that all planned test cases can be executed consistently and that the resulting outcomes are reliable and reproducible.

---

# Chapter 4 – Unit Testing

---

# 4.1 Introduction

Unit Testing is the first level of software testing performed during the development process. It focuses on verifying the correctness of individual classes, functions, and modules in isolation before they are integrated into the complete system.

For the **Digital Clock System**, unit testing ensures that each software component behaves according to its specification and produces the expected output for a given input. Detecting defects at this stage reduces development costs and simplifies debugging.

---

# 4.2 Unit Testing Objectives

The objectives of unit testing are to:

- Verify the correctness of individual functions.
- Validate class behavior independently.
- Detect logical and implementation errors.
- Ensure reliable handling of valid and invalid inputs.
- Confirm proper error handling.
- Simplify debugging before integration.
- Improve overall software quality.

---

# 4.3 Unit Testing Strategy

Each module is tested independently using predefined test cases.

```text
Source Code
      │
      ▼
Individual Module
      │
      ▼
Execute Test Cases
      │
      ▼
Compare Expected Output
      │
      ▼
Pass / Fail
      │
      ▼
Fix Defects (if required)
```

Testing is performed before integration with other modules.

---

# 4.4 Modules Covered

The following modules are included in unit testing.

| Module | Purpose |
|--------|---------|
| Clock Module | Retrieve and update system time |
| Date Module | Retrieve and format the current date |
| Display Module | Render formatted output to the console |
| Configuration Manager | Read and validate configuration settings |
| Logger | Record application events |
| Theme Manager | Load and apply themes |
| Resource Manager | Access external resource files |
| Utility Functions | Shared helper functions |

Each module is tested independently.

---

# 4.5 Clock Module Testing

### Objective

Verify that the Clock Module retrieves and updates the current system time correctly.

### Test Cases

| Test ID | Description | Expected Result |
|---------|-------------|-----------------|
| UT-001 | Retrieve current time | Current system time is returned |
| UT-002 | Update clock | Display updates correctly |
| UT-003 | Format 12-hour time | Correct 12-hour output |
| UT-004 | Format 24-hour time | Correct 24-hour output |

---

# 4.6 Date Module Testing

### Objective

Verify date retrieval and formatting.

### Test Cases

| Test ID | Description | Expected Result |
|---------|-------------|-----------------|
| UT-005 | Retrieve current date | Current system date is returned |
| UT-006 | Format date | Date displayed correctly |
| UT-007 | Leap year validation | Correct leap-year handling |
| UT-008 | Month transition | Correct month change |

---

# 4.7 Configuration Manager Testing

### Objective

Verify configuration loading and validation.

### Test Cases

| Test ID | Description | Expected Result |
|---------|-------------|-----------------|
| UT-009 | Read configuration | Values loaded successfully |
| UT-010 | Missing configuration file | Default values applied |
| UT-011 | Invalid configuration value | Error handled gracefully |
| UT-012 | Reload configuration | Updated values loaded correctly |

---

# 4.8 Theme Manager Testing

### Objective

Verify that themes are loaded and applied correctly.

### Test Cases

| Test ID | Description | Expected Result |
|---------|-------------|-----------------|
| UT-013 | Load valid theme | Theme applied successfully |
| UT-014 | Missing theme | Default theme selected |
| UT-015 | Invalid theme name | Error reported appropriately |

---

# 4.9 Logger Testing

### Objective

Verify log creation and event recording.

### Test Cases

| Test ID | Description | Expected Result |
|---------|-------------|-----------------|
| UT-016 | Create log file | Log file created |
| UT-017 | Write log entry | Entry stored correctly |
| UT-018 | Disable logging | No log generated |
| UT-019 | Invalid log path | Error handled gracefully |

---

# 4.10 Resource Manager Testing

### Objective

Verify loading of external resource files.

### Test Cases

| Test ID | Description | Expected Result |
|---------|-------------|-----------------|
| UT-020 | Load banner | Banner displayed |
| UT-021 | Missing resource | Error reported |
| UT-022 | Invalid resource path | Resource loading fails safely |

---

# 4.11 Error Handling Tests

The application should respond correctly to unexpected conditions.

| Test ID | Scenario | Expected Result |
|---------|----------|-----------------|
| UT-023 | Missing configuration | Default configuration loaded |
| UT-024 | Missing resource | Warning displayed |
| UT-025 | Invalid file format | Error handled without crash |
| UT-026 | File permission denied | Appropriate error message logged |

---

# 4.12 Unit Test Execution Summary

The table below records the unit tests that were implemented and executed. Counts are per source file in `Tests/`, and every test is automated.

| Module | Test File | Executed | Passed | Failed |
|--------|-----------|---------:|-------:|-------:|
| Utility | `test_utility.cpp` | 8 | 8 | 0 |
| Clock Module | `test_clock.cpp` | 5 | 5 | 0 |
| Date Module | `test_date.cpp` | 5 | 5 | 0 |
| Time Formatter | `test_formatter.cpp` | 5 | 5 | 0 |
| Configuration Manager | `test_config.cpp` | 7 | 7 | 0 |
| Logger | `test_logger.cpp` | 6 | 6 | 0 |
| Theme Manager | `test_theme.cpp` | 5 | 5 | 0 |
| Resource Manager | `test_resource.cpp` | 6 | 6 | 0 |
| Presentation Layer | `test_display.cpp` | 7 | 7 | 0 |
| Application Lifecycle | `test_application.cpp` | 6 | 6 | 0 |
| Alarm Module | `test_alarm.cpp` | 18 | 18 | 0 |
| Stopwatch & Timer | `test_timing.cpp` | 13 | 13 | 0 |
| World Clock | `test_timezone.cpp` | 10 | 10 | 0 |
| **Total** | | **101** | **101** | **0** |

The suite exceeds the 26 tests originally planned because several planned cases needed more than one assertion group to cover their boundary conditions.

---

# 4.13 Acceptance Criteria

A module is considered to have passed unit testing when:

- All planned test cases pass.
- Expected outputs match actual outputs.
- No critical defects remain.
- Invalid inputs are handled safely.
- Error messages are generated where appropriate.
- No unexpected application crashes occur.

Modules that fail these criteria must be corrected and retested.

---

# 4.14 Chapter Summary

This chapter described the unit testing strategy for the Digital Clock System. It outlined the objectives, testing process, covered modules, planned unit test cases, execution summary, and acceptance criteria.

Successful completion of unit testing provides confidence that each software component functions correctly before integration with other modules.

---

# Chapter 5 – Integration Testing

---

# 5.1 Introduction

Integration Testing is performed after successful completion of Unit Testing. The objective is to verify that individual modules work correctly when combined into a complete system.

For the **Digital Clock System**, Integration Testing focuses on validating communication between modules such as the Clock Module, Date Module, Configuration Manager, Display Module, Theme Manager, Logger, and Resource Manager. This phase ensures that data flows correctly across the application and that integrated components function as expected.

---

# 5.2 Objectives

The objectives of Integration Testing are to:

- Verify communication between software modules.
- Ensure correct data flow across components.
- Detect interface-related defects.
- Validate shared resources and dependencies.
- Confirm correct initialization and shutdown sequences.
- Ensure integrated modules behave consistently.

---

# 5.3 Integration Strategy

The Digital Clock System follows an **Incremental Integration Testing** approach. Modules are integrated one at a time and tested before adding additional components.

```text
Clock Module
      │
      ▼
Date Module
      │
      ▼
Display Module
      │
      ▼
Configuration Manager
      │
      ▼
Theme Manager
      │
      ▼
Logger
      │
      ▼
Resource Manager
      │
      ▼
Complete Digital Clock System
```

This strategy simplifies debugging by isolating integration issues early.

---

# 5.4 Modules Integrated

The following modules participate in integration testing.

| Module | Integrated With |
|---------|-----------------|
| Clock Module | Display Module |
| Date Module | Display Module |
| Configuration Manager | Theme Manager, Logger |
| Theme Manager | Display Module |
| Resource Manager | Display Module |
| Logger | All Modules |
| Main Application | All Components |

---

# 5.5 Clock and Display Integration

### Objective

Verify that the current system time is correctly retrieved and displayed.

### Test Cases

| Test ID | Description | Expected Result |
|---------|-------------|-----------------|
| IT-001 | Clock updates display | Current time displayed correctly |
| IT-002 | Time refresh | Display updates every refresh cycle |
| IT-003 | Time format change | Display reflects selected format |

---

# 5.6 Date and Display Integration

### Objective

Verify that the Date Module communicates correctly with the Display Module.

### Test Cases

| Test ID | Description | Expected Result |
|---------|-------------|-----------------|
| IT-004 | Display current date | Date shown correctly |
| IT-005 | Date format update | Selected format applied |
| IT-006 | Day transition | Date changes correctly at midnight |

---

# 5.7 Configuration Integration

### Objective

Verify that configuration settings are correctly applied across the application.

### Test Cases

| Test ID | Description | Expected Result |
|---------|-------------|-----------------|
| IT-007 | Load configuration | Settings applied successfully |
| IT-008 | Missing configuration | Default settings used |
| IT-009 | Invalid configuration | Error handled without crash |

---

# 5.8 Theme Integration

### Objective

Verify interaction between the Theme Manager and Display Module.

### Test Cases

| Test ID | Description | Expected Result |
|---------|-------------|-----------------|
| IT-010 | Load valid theme | Theme applied correctly |
| IT-011 | Missing theme | Default theme used |
| IT-012 | Theme change | Display updates successfully |

---

# 5.9 Logger Integration

### Objective

Verify that all modules correctly generate log entries.

### Test Cases

| Test ID | Description | Expected Result |
|---------|-------------|-----------------|
| IT-013 | Startup logging | Startup event recorded |
| IT-014 | Configuration logging | Configuration event logged |
| IT-015 | Error logging | Errors written to log file |
| IT-016 | Shutdown logging | Shutdown event recorded |

---

# 5.10 Resource Manager Integration

### Objective

Verify that external resources are available to the Display Module.

### Test Cases

| Test ID | Description | Expected Result |
|---------|-------------|-----------------|
| IT-017 | Banner loading | Banner displayed |
| IT-018 | Missing resource | Warning generated |
| IT-019 | Invalid resource path | Application continues safely |

---

# 5.11 Startup and Shutdown Integration

### Objective

Verify complete application initialization and cleanup.

### Test Cases

| Test ID | Description | Expected Result |
|---------|-------------|-----------------|
| IT-020 | Application startup | All modules initialize successfully |
| IT-021 | Application shutdown | Resources released properly |
| IT-022 | Restart application | Application starts normally after restart |

---

# 5.12 Integration Test Execution Summary

Integration is exercised through the `Application` and presentation-layer tests, which drive real components together rather than in isolation.

| Module Integration | Covered By | Result |
|--------------------|------------|--------|
| Clock + Formatter + Display | `TC-002`, `UT-086`, `UT-091` | **Pass** |
| Date + Formatter + Display | `TC-002`, `UT-086`, `UT-091` | **Pass** |
| Configuration + Application | `TC-001`, `UT-090`, `UT-091` | **Pass** |
| Theme Manager + Screen | `UT-084`, `UT-085`, `UT-091` | **Pass** |
| Logger + Application | `TC-001`, `TC-024` | **Pass** |
| Resource Manager + Banner | `TC-016`, `TC-017`, `TC-023` | **Pass** |
| Screen + StatusBar + Console | `UT-080`, `UT-081`, `UT-083` | **Pass** |
| Startup & Shutdown | `TC-001`, `TC-002`, `TC-020` | **Pass** |
| Alarm + Clock + Notifier + Screen | `TC-034`, `TC-037`, `TC-041` | **Pass** |
| Stopwatch + Timer + Application modes | `TC-052`, `UT-110` | **Pass** |
| WorldClock + TimeZone + Screen rows | `TC-060`, `UT-120` | **Pass** |

All eleven integration paths passed.

One defect was found at a module boundary rather than inside a module:
`AlarmManager::poll()` and `Alarm::minutesUntil()` were each correct alone,
but together left the status bar claiming an alarm was due after it had
already rung and been acknowledged (DEF-007). It was caught by running the
application, not by the suite, and TC-041 now covers it.

---

# 5.13 Acceptance Criteria

Integration testing is considered successful when:

- All integrated modules communicate correctly.
- Data is transferred accurately between components.
- Configuration settings are applied successfully.
- Themes and resources load without errors.
- Log entries are generated as expected.
- Startup and shutdown sequences complete without failures.
- No critical integration defects remain.

Modules that fail integration testing must be corrected and retested before proceeding to system testing.

---

# 5.14 Chapter Summary

This chapter presented the Integration Testing strategy for the Digital Clock System. It described the objectives, integration approach, participating modules, planned integration test cases, execution summary, and acceptance criteria.

Successful completion of integration testing confirms that the individual modules operate together as a unified and reliable application.

---

# Chapter 6 – System Testing

---

# 6.1 Introduction

System Testing is performed after the successful completion of Integration Testing. In this phase, the **Digital Clock System** is tested as a complete application to verify that it satisfies all functional and non-functional requirements defined in the Software Requirements Specification (SRS).

Unlike Unit and Integration Testing, System Testing evaluates the software from an end-user perspective. It validates the application's overall functionality, performance, reliability, usability, and compatibility in a realistic operating environment.

---

# 6.2 Objectives

The objectives of System Testing are to:

- Verify complete system functionality.
- Validate compliance with the Software Requirements Specification (SRS).
- Ensure all modules work together correctly.
- Evaluate application stability during continuous operation.
- Verify system performance and responsiveness.
- Confirm compatibility with supported operating systems.
- Validate error handling and recovery mechanisms.
- Ensure deployment readiness.

---

# 6.3 Scope of System Testing

The following features are included in system testing.

| Feature | Verification |
|---------|--------------|
| Application Startup | ✔ |
| Real-Time Clock Display | ✔ |
| Date Display | ✔ |
| Time Format Selection | ✔ |
| Date Format Selection | ✔ |
| Theme Loading | ✔ |
| Configuration Management | ✔ |
| Logging System | ✔ |
| Resource Loading | ✔ |
| Error Handling | ✔ |
| Application Shutdown | ✔ |

The entire application is tested as a single integrated system.

---

# 6.4 Functional Testing

Functional testing verifies that all user-visible features work according to their specifications.

### Test Cases

| Test ID | Description | Expected Result |
|---------|-------------|-----------------|
| ST-001 | Launch application | Application starts successfully |
| ST-002 | Display current time | Correct system time displayed |
| ST-003 | Display current date | Correct system date displayed |
| ST-004 | Update time every refresh cycle | Display updates automatically |
| ST-005 | Load configuration | Configuration applied successfully |
| ST-006 | Load selected theme | Theme displayed correctly |
| ST-007 | Exit application | Application closes safely |

---

# 6.5 User Interface Testing

Although the Digital Clock System uses a console interface, the user interface must remain clear and readable.

The following aspects are verified:

- Proper alignment of displayed information.
- Correct spacing and formatting.
- Readable colors (where themes are supported).
- No overlapping or truncated output.
- Stable display updates without flickering.

---

# 6.6 Performance Testing

Performance testing measures how efficiently the application uses system resources.

### Performance Metrics

| Metric | Target |
|--------|--------|
| Startup Time | Less than 2 seconds |
| Time Refresh | Configured interval (e.g., 1000 ms) |
| CPU Usage | Low during continuous execution |
| Memory Usage | Stable with no significant growth |
| Shutdown Time | Less than 1 second |

The Digital Clock System is expected to operate efficiently even during extended execution.

---

# 6.7 Compatibility Testing

Compatibility testing verifies that the application functions correctly on supported platforms.

| Platform | Status | Basis |
|----------|--------|-------|
| Linux | **Verified** | Suite executed; application run; CI job |
| Windows | **Verified** | CI job: MSVC 19.51, 78 tests passed, frame rendered |
| macOS | **Verified** | CI job: suite passed, frame rendered |
| Different Terminal Sizes | **Verified** | UT-082 |
| Redirected Output | **Verified** | UT-084 |
| GCC Compiler | **Verified** | GCC 16.1.1, zero warnings, `-Werror` in CI |
| MSVC | **Verified** | MSVC 19.51.36252 via Visual Studio 18 2026 |
| Make Build | **Verified** | `make`, `make debug`, `make test` in CI |
| CMake Build | **Verified** | `cmake --build` and `ctest` on all three platforms |
| AddressSanitizer | **Verified** | Suite and application clean |
| UndefinedBehaviorSanitizer | **Verified** | Suite and application clean |

**Windows and macOS were verified on 2026-08-07** by the continuous
integration pipeline added in `.github/workflows/digitalclock-ci.yml`. Both
compile the project, run all 78 tests, and execute the application itself with
`--once` so the terminal and rendering paths are exercised rather than merely
built. Evidence: [CI run 31142229662](https://github.com/adarsh0707-kumar/Projects-in-C-C---basic-to-Advanced/actions/runs/31142229662).

This replaces the earlier "implemented, not verified" record. The distinction
mattered: the code was written to be portable, but until this run nothing had
demonstrated it.

---

# 6.8 Reliability Testing

Reliability testing ensures the application remains stable during prolonged operation.

The following conditions are evaluated:

- Continuous execution for extended periods.
- Stable time updates.
- Consistent memory usage.
- Correct resource management.
- No unexpected application termination.

A reliable application should continue operating without degradation over time.

---

# 6.9 Recovery Testing

Recovery testing verifies that the application can handle unexpected situations gracefully.

### Test Cases

| Test ID | Scenario | Expected Result |
|---------|----------|-----------------|
| ST-008 | Missing configuration file | Default settings applied |
| ST-009 | Missing theme | Default theme loaded |
| ST-010 | Missing resource | Warning generated; application continues |
| ST-011 | Log file unavailable | Logging disabled without crash |
| ST-012 | Invalid configuration | Error handled gracefully |

The application should recover whenever possible without terminating unexpectedly.

---

# 6.10 System Test Execution Summary

System-level behaviour was verified by running the application itself and
observing the result, in addition to the automated suite.

| Test Category | Method | Result |
|---------------|--------|--------|
| Functional Testing | Automated suite, `TC-001` – `TC-025` | **Pass** |
| User Interface Testing | Rendered frame compared against the User Manual layout | **Pass** |
| Performance Testing | Measured startup, memory and CPU (see 8.5) | **Pass** |
| Compatibility Testing | CI across Linux, Windows and macOS; both build systems (see 8.6) | **Pass** |
| Reliability Testing | Continuous run with a stable resident set | **Pass** |
| Recovery Testing | Missing config, unknown theme, absent banner, unwritable log | **Pass** |

Recovery testing deserves particular note: each degraded condition was induced
deliberately, and in every case the application logged the problem and
continued running on its documented fallback.

---

# 6.11 Acceptance Criteria

The Digital Clock System passes System Testing when:

- All functional test cases pass.
- No critical or high-severity defects remain.
- Performance targets are achieved.
- The application remains stable during extended execution.
- Configuration and themes work correctly.
- Logging operates as expected.
- Recovery mechanisms handle errors without causing application failure.

Only after meeting these criteria should the software proceed to User Acceptance Testing or release.

---

# 6.12 Risks and Limitations

The following limitations apply to system testing:

- Hardware clock accuracy depends on the operating system.
- Terminal appearance may vary across platforms.
- Performance results may differ depending on system specifications.
- Third-party libraries are assumed to function correctly.

These factors are considered outside the direct control of the Digital Clock System.

---

# 6.13 Chapter Summary

This chapter described the System Testing process for the Digital Clock System. It covered testing objectives, scope, functional verification, user interface evaluation, performance, compatibility, reliability, recovery testing, execution summary, acceptance criteria, and testing limitations.

Successful completion of System Testing demonstrates that the application functions correctly as a complete system and is ready for final validation and acceptance.

---

# Chapter 7 – Test Cases

---

# 7.1 Introduction

This chapter defines the planned **test cases** for the Digital Clock System. Each test case verifies a specific feature or behavior of the application. The objective is to ensure that every functional requirement described in the Software Requirements Specification (SRS) is validated through systematic testing.

Each test case includes:

- Test ID
- Test Objective
- Preconditions
- Test Steps
- Expected Result
- Status (to be updated after execution)

Initially, the status is marked as **Not Executed (NE)** and should be updated during the testing phase.

---

# 7.2 Test Case Format

The following format is used throughout this report.

| Field | Description |
|--------|-------------|
| Test ID | Unique identifier |
| Module | Component under test |
| Objective | Purpose of the test |
| Preconditions | Required conditions before execution |
| Test Steps | Actions performed by the tester |
| Expected Result | Expected application behavior |
| Status | Pass / Fail / Not Executed |

---

# 7.3 Startup Test Cases

## Test Case TC-001

| Field | Value |
|------|-------|
| Test ID | TC-001 |
| Module | Application Startup |
| Objective | Verify successful application startup |
| Preconditions | Application is built successfully |
| Test Steps | Run the executable |
| Expected Result | Application starts without errors |
| Actual Result | Application started and rendered its first frame; the full initialization sequence was recorded in `Logs/application.log`. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

## Test Case TC-002

| Field | Value |
|------|-------|
| Test ID | TC-002 |
| Module | Startup |
| Objective | Verify initialization of all modules |
| Preconditions | Required files available |
| Test Steps | Launch application |
| Expected Result | All modules initialize correctly |
| Actual Result | Logger, configuration, resources, theme, console, clock, date and display all initialized; a frame rendered immediately afterwards. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

# 7.4 Clock Module Test Cases

## Test Case TC-003

| Field | Value |
|------|-------|
| Test ID | TC-003 |
| Module | Clock |
| Objective | Verify current system time |
| Preconditions | System clock configured correctly |
| Test Steps | Start application |
| Expected Result | Current time displayed accurately |
| Actual Result | Displayed hour and minute matched an independent read of the system clock; seconds agreed within one tick. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

## Test Case TC-004

| Field | Value |
|------|-------|
| Test ID | TC-004 |
| Module | Clock |
| Objective | Verify automatic time updates |
| Preconditions | Application running |
| Test Steps | Observe display for one minute |
| Expected Result | Time updates according to refresh interval |
| Actual Result | The displayed second advanced across the refresh interval; a snapshot did not drift between updates. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

## Test Case TC-005

| Field | Value |
|------|-------|
| Test ID | TC-005 |
| Module | Clock |
| Objective | Verify 12-hour display format |
| Preconditions | TimeFormat=12 |
| Test Steps | Restart application |
| Expected Result | Time displayed in 12-hour format with AM/PM |
| Actual Result | 20:45:30 rendered as `08:45:30 PM`. Midnight rendered as `12:00:00 AM` and noon as `12:00:00 PM`. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

## Test Case TC-006

| Field | Value |
|------|-------|
| Test ID | TC-006 |
| Module | Clock |
| Objective | Verify 24-hour display format |
| Preconditions | TimeFormat=24 |
| Test Steps | Restart application |
| Expected Result | Time displayed in 24-hour format |
| Actual Result | 20:45:30 rendered as `20:45:30`; 09:05:03 kept its leading zeros and no meridiem indicator was emitted. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

# 7.5 Date Module Test Cases

## Test Case TC-007

| Field | Value |
|------|-------|
| Test ID | TC-007 |
| Module | Date |
| Objective | Verify current date display |
| Preconditions | Correct system date |
| Test Steps | Launch application |
| Expected Result | Current date displayed correctly |
| Actual Result | Day, month, year and weekday all matched an independent read of the system date. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

## Test Case TC-008

| Field | Value |
|------|-------|
| Test ID | TC-008 |
| Module | Date |
| Objective | Verify configured date format |
| Preconditions | Date format configured |
| Test Steps | Launch application |
| Expected Result | Date displayed in selected format |
| Actual Result | All four documented formats rendered correctly: `Monday, 03 August 2026`, `03-08-2026`, `08-03-2026` and `2026-08-03`. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

# 7.6 Configuration Test Cases

## Test Case TC-009

| Field | Value |
|------|-------|
| Test ID | TC-009 |
| Module | Configuration |
| Objective | Verify configuration loading |
| Preconditions | Valid `config.ini` |
| Test Steps | Start application |
| Expected Result | Configuration loaded successfully |
| Actual Result | `Config/config.ini` loaded; every documented key was read, and lookups matched case-insensitively. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

## Test Case TC-010

| Field | Value |
|------|-------|
| Test ID | TC-010 |
| Module | Configuration |
| Objective | Verify default configuration |
| Preconditions | Remove configuration file |
| Test Steps | Launch application |
| Expected Result | Default settings applied |
| Actual Result | A missing file returned false without throwing; every getter then returned its supplied default. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

## Test Case TC-011

| Field | Value |
|------|-------|
| Test ID | TC-011 |
| Module | Configuration |
| Objective | Verify invalid configuration handling |
| Preconditions | Invalid configuration values |
| Test Steps | Start application |
| Expected Result | Error handled without application crash |
| Actual Result | Malformed lines were skipped and the valid entries either side of them were retained; a non-numeric integer fell back to its default. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

# 7.7 Theme Test Cases

## Test Case TC-012

| Field | Value |
|------|-------|
| Test ID | TC-012 |
| Module | Theme Manager |
| Objective | Verify theme loading |
| Preconditions | Valid theme available |
| Test Steps | Launch application |
| Expected Result | Selected theme applied |
| Actual Result | All five bundled themes loaded, each reporting its own name and element colours. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

## Test Case TC-013

| Field | Value |
|------|-------|
| Test ID | TC-013 |
| Module | Theme Manager |
| Objective | Verify missing theme handling |
| Preconditions | Theme removed |
| Test Steps | Start application |
| Expected Result | Default theme loaded |
| Actual Result | An unknown theme fell back to the built-in default, which supplied a complete colour set. A warning was logged. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

# 7.8 Logger Test Cases

## Test Case TC-014

| Field | Value |
|------|-------|
| Test ID | TC-014 |
| Module | Logger |
| Objective | Verify log file creation |
| Preconditions | Logging enabled |
| Test Steps | Launch application |
| Expected Result | Log file created |
| Actual Result | The log file was created together with its missing parent directories, and a session header was written. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

## Test Case TC-015

| Field | Value |
|------|-------|
| Test ID | TC-015 |
| Module | Logger |
| Objective | Verify log entry generation |
| Preconditions | Logging enabled |
| Test Steps | Run application |
| Expected Result | Events recorded in log |
| Actual Result | Timestamped entries were recorded at all four severities; entries below the minimum level were discarded. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

# 7.9 Resource Manager Test Cases

## Test Case TC-016

| Field | Value |
|------|-------|
| Test ID | TC-016 |
| Module | Resource Manager |
| Objective | Verify banner loading |
| Preconditions | Resource available |
| Test Steps | Start application |
| Expected Result | Banner displayed successfully |
| Actual Result | Banner and logo both loaded from `Resources/`, and the banner reported a non-zero line count. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

## Test Case TC-017

| Field | Value |
|------|-------|
| Test ID | TC-017 |
| Module | Resource Manager |
| Objective | Verify missing resource handling |
| Preconditions | Remove banner file |
| Test Steps | Launch application |
| Expected Result | Warning displayed without crash |
| Actual Result | A missing banner left the built-in artwork in place, so the header was still drawn. A warning was logged. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

# 7.10 Error Handling Test Cases

## Test Case TC-018

| Field | Value |
|------|-------|
| Test ID | TC-018 |
| Module | Error Handling |
| Objective | Verify invalid file path handling |
| Preconditions | Invalid resource path |
| Test Steps | Launch application |
| Expected Result | Appropriate error message generated |
| Actual Result | Empty, whitespace, missing and directory paths were each rejected without throwing; content came back empty. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

## Test Case TC-019

| Field | Value |
|------|-------|
| Test ID | TC-019 |
| Module | Error Handling |
| Objective | Verify permission error handling |
| Preconditions | Restricted file permissions |
| Test Steps | Start application |
| Expected Result | Error logged and application continues safely |
| Actual Result | An unwritable path was reported by `initialize()`; subsequent logging calls were safe no-ops and the application continued. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

# 7.11 Shutdown Test Cases

## Test Case TC-020

| Field | Value |
|------|-------|
| Test ID | TC-020 |
| Module | Application Shutdown |
| Objective | Verify graceful shutdown |
| Preconditions | Application running |
| Test Steps | Exit application |
| Expected Result | Resources released and application exits normally |
| Actual Result | The terminal was restored, shutdown was logged, and repeated `shutdown()` calls were safe. `run()` on an uninitialized application returned the failure status rather than looping. |
| Executed | 2026-08-07 |
| Status | **Pass** |

---

# 7.12 Boundary and Negative Test Cases

| Test ID | Scenario | Expected Result | Actual Result | Status |
|---------|----------|-----------------|---------------|--------|
| TC-021 | Empty configuration file | Default settings applied | Load succeeded with zero settings; a file of only comments behaved identically. Defaults applied. | **Pass** |
| TC-022 | Invalid refresh interval | Configuration validation error | Zero, negative, oversized and non-numeric values were all replaced by the default, and the rejection was logged as a warning. | **Pass** |
| TC-023 | Corrupted resource file | Warning displayed | An empty banner was rejected in favour of the built-in artwork; binary content and CRLF line endings were both handled without error. | **Pass** |
| TC-024 | Log directory unavailable | Logging disabled safely | `initialize()` returned false and the application continued; all subsequent logging calls were safe no-ops. | **Pass** |
| TC-025 | Unsupported theme name | Default theme loaded | Empty, whitespace, unknown and path-traversal names all resolved to the built-in default theme. | **Pass** |

Executed 2026-08-07.

---

# 7.12a Alarm Module Test Cases (v1.1.0)

Introduced with the alarm module. All were executed on 2026-08-07 and passed.

| Test ID | Objective | Actual Result | Status |
|---------|-----------|---------------|--------|
| TC-026 | Verify alarm parsing from configuration text | Full, label-only and time-only forms all parsed; whitespace tolerated; a leading `!` disarmed the alarm without discarding it | **Pass** |
| TC-027 | Verify malformed alarm text is rejected | Empty, non-numeric, out-of-range and structurally wrong entries were all rejected; `00:00` and `23:59` accepted | **Pass** |
| TC-028 | Verify recurrence rules select the correct days | Daily, Weekdays, Weekends and explicit day lists each matched the expected weekdays; a list equal to a named set reported the friendlier name | **Pass** |
| TC-029 | Verify an alarm is due only at its configured time | Due for the whole of its minute regardless of seconds; not due a minute either side; never due when disarmed | **Pass** |
| TC-030 | Verify recurrence is honoured by due detection | A weekday alarm did not ring on Saturday; a weekend alarm did not ring on Monday | **Pass** |
| TC-031 | Verify snooze postpones an alarm by the configured delay | Snooze moved the alarm forward and wrapped correctly across an hour and across midnight; zero and negative delays rejected | **Pass** |
| TC-032 | Verify AlarmManager loads alarms from a file | Three alarms loaded from non-contiguous indices; a missing file returned false with no alarms and no error | **Pass** |
| TC-033 | Verify malformed alarm entries are skipped, not fatal | Two valid entries survived alongside two malformed ones, which were counted | **Pass** |
| TC-034 | Verify an alarm fires exactly once per occurrence | Fired once on the first poll of its minute, not again within that minute nor after dismissal, but did fire again the next day | **Pass** |
| TC-035 | Verify dismissing a one-shot alarm disarms it | The one-shot alarm disarmed itself; the recurring alarm stayed armed | **Pass** |
| TC-036 | Verify snoozing through the manager re-arms the alarm | Ringing stopped, the alarm rang again when the snooze expired, and dismissal cleared the snooze | **Pass** |
| TC-037 | Verify only one alarm rings at a time | The second alarm waited until the first was dismissed rather than replacing it | **Pass** |
| TC-038 | Verify the next-alarm summary and countdown | The nearer alarm was reported and the countdown advanced correctly across the day boundary | **Pass** |
| TC-039 | Verify the notification panel renders the ringing alarm | The panel carried the time, label and both key hints, and every row was the same width | **Pass** |
| TC-040 | Verify the alarm bell is emitted and can be disabled | Three bells emitted across notify and two pulses; none when disabled; none when pulsing an inactive notifier | **Pass** |
| TC-041 | Verify the countdown skips an alarm that already fired | After dismissal the countdown reported the next occurrence rather than "due now" | **Pass** |

TC-041 covers a defect found during end-to-end verification rather than by the
suite: the countdown claimed an alarm was due for the remainder of the minute
in which it had already rung and been acknowledged. See the Change Log,
section 3.9a.

---

# 7.12b Stopwatch and Timer Test Cases (v1.2.0)

Executed 2026-08-07; all passed.

| Test ID | Objective | Actual Result | Status |
|---------|-----------|---------------|--------|
| TC-042 | Verify the stopwatch measures elapsed time | Elapsed advanced with the supplied reading; starting an already-running stopwatch was refused; a reading before the start yielded zero rather than a negative | **Pass** |
| TC-043 | Verify stop and resume accumulate correctly | A stopped stopwatch held its value indefinitely; resuming continued from the banked total rather than restarting | **Pass** |
| TC-044 | Verify stopwatch laps and splits | Laps recorded total elapsed time; splits reported per-lap durations; an out-of-range index yielded zero | **Pass** |
| TC-045 | Verify the stopwatch caps its lap history | Laps were refused beyond MAX_LAPS rather than growing without bound | **Pass** |
| TC-046 | Verify stopwatch formatting | `MM:SS.cc` throughout, widening to `H:MM:SS.cc` at an hour; negatives rendered as zero | **Pass** |
| TC-047 | Verify the countdown timer counts down | Remaining decreased with the reading and never went negative | **Pass** |
| TC-048 | Verify the countdown timer pauses and resumes | A paused timer held its remainder; pausing twice did not subtract twice | **Pass** |
| TC-049 | Verify the countdown timer fires exactly once | Fired on the poll observing the zero crossing, never again for that run; expiry stopped the countdown; a reset allowed it to fire again | **Pass** |
| TC-050 | Verify countdown duration parsing | `MM:SS`, `H:MM:SS` and bare seconds accepted; malformed and out-of-range values rejected; 24 hours enforced as the cap | **Pass** |
| TC-051 | Verify countdown formatting rounds up | Any non-zero remainder rendered as at least `00:01`, so the display reaches zero only when the timer does | **Pass** |
| TC-052 | Verify mode switching leaves the other modes undisturbed | Cycling wrapped Clock to Stopwatch to Timer; a running stopwatch continued while other modes were displayed; every mode rendered without error | **Pass** |

TC-051 is worth noting. Truncating rather than rounding would display `00:00`
for the whole of the final second, which reads as finished a second before it
is. The rounding rule is asserted rather than left to inspection.

---

# 7.12c World Clock Test Cases (v1.3.0)

Executed 2026-08-07; all passed.

| Test ID | Objective | Actual Result | Status |
|---------|-----------|---------------|--------|
| TC-053 | Verify UTC offset parsing | `UTC`, `GMT`, `UTC+05:30`, `+05:30`, `-08:00`, `+09` and the +14:00/-12:00 extremes all parsed | **Pass** |
| TC-054 | Verify malformed offsets are rejected | Empty, unsigned, incomplete and out-of-range specifications all refused | **Pass** |
| TC-055 | Verify fixed-offset zones convert exactly | 12:00 UTC rendered as 17:30 at +05:30 and 04:00 at -08:00; fixed offsets always resolve | **Pass** |
| TC-056 | Verify zone time and offset formatting | 24-hour and 12-hour forms correct, including the midnight and noon boundaries; offsets rendered as +HH:MM | **Pass** |
| TC-057 | Verify zone specification parsing | Offsets, named zones and optional labels parsed; a '/' distinguishes a named zone; malformed entries refused | **Pass** |
| TC-058 | Verify the world clock loads a zone list | Three zones loaded with labels; whitespace and empty entries between commas tolerated | **Pass** |
| TC-059 | Verify malformed zone entries are skipped, not fatal | Two valid entries survived alongside two malformed ones, which were counted; the zone count is capped | **Pass** |
| TC-060 | Verify world clock rows are aligned and labelled | Rows carried label, time and offset; labels padded so times form a column; 12-hour rendering carried a meridiem | **Pass** |

Two supporting cases carry `UT-` identifiers because they assert platform
behaviour rather than a documented requirement:

- **UT-120** holds whether or not the platform has a timezone database. The
  assertion is the invariant either way: a zone reporting itself resolvable
  must convert, and one that does not must still produce a usable time.
- **UT-121** asserts that resolving a named zone restores the process `TZ`
  variable. The mechanism mutates global state, and a failure to restore
  would corrupt every later `localtime()` call in the process -- a fault that
  would surface far from its cause.

---

# 7.13 Test Case Execution Summary

| Category | Test Cases | Executed | Passed | Failed |
|----------|-----------:|---------:|-------:|-------:|
| Startup | 2 | 2 | 2 | 0 |
| Clock Module | 4 | 4 | 4 | 0 |
| Date Module | 2 | 2 | 2 | 0 |
| Configuration | 3 | 3 | 3 | 0 |
| Theme Manager | 2 | 2 | 2 | 0 |
| Logger | 2 | 2 | 2 | 0 |
| Resource Manager | 2 | 2 | 2 | 0 |
| Error Handling | 2 | 2 | 2 | 0 |
| Shutdown | 1 | 1 | 1 | 0 |
| Boundary & Negative | 5 | 5 | 5 | 0 |
| Alarm Module (v1.1.0) | 16 | 16 | 16 | 0 |
| Stopwatch & Timer (v1.2.0) | 11 | 11 | 11 | 0 |
| World Clock (v1.3.0) | 8 | 8 | 8 | 0 |
| **Total** | **60** | **60** | **60** | **0** |

Executed 2026-08-07. Two additional boundary cases, TC-005A and TC-006A, were
added during implementation to cover the midnight and noon conversions that
the FR-004 acceptance criteria require; both pass. TC-026 to TC-041 arrived
with the v1.1.0 alarm module, TC-042 to TC-052 with the v1.2.0 stopwatch
and timer, and TC-053 to TC-060 with the v1.3.0 world clock. The wider
automated suite contains 101 tests in total, the remainder carrying `UT-`
identifiers.

---

# 7.14 Test Case Traceability

| Requirement | Test Cases |
|-------------|------------|
| Display Current Time | TC-003, TC-004, TC-005, TC-006 |
| Display Current Date | TC-007, TC-008 |
| Configuration Management | TC-009, TC-010, TC-011 |
| Theme Management | TC-012, TC-013 |
| Logging | TC-014, TC-015 |
| Resource Loading | TC-016, TC-017 |
| Error Handling | TC-018, TC-019 |
| Application Shutdown | TC-020 |
| Boundary Validation | TC-021 – TC-025 |

This traceability matrix ensures that every major functional requirement is covered by one or more planned test cases.

---

# 7.15 Chapter Summary

This chapter presented the complete set of planned test cases for the Digital Clock System. It included startup, clock, date, configuration, theme, logging, resource management, error handling, shutdown, and boundary condition tests. Each test case is uniquely identified and mapped to the corresponding software requirement, providing comprehensive coverage of the application's functionality.

---

# Chapter 8 – Test Results

---

# 8.1 Introduction

This chapter presents the **planned test execution results** for the Digital Clock System. Since the software implementation is still in progress, the results documented here represent the expected outcome of the planned testing activities.

Once development is completed, this chapter should be updated with the actual execution results, including passed and failed test cases, defect reports, execution dates, and supporting evidence such as screenshots or log files.

---

# 8.2 Test Execution Summary

The table below records actual execution. The automated suite is a single
binary, `Build/DigitalClockTests`, containing 101 tests; the phases below
describe what those tests cover rather than separate executables, so a single
test may contribute to more than one phase.

| Testing Phase | Test Cases | Executed | Passed | Failed | Status |
|---------------|-----------:|---------:|-------:|-------:|--------|
| Unit Testing | 101 | 101 | 101 | 0 | **Complete** |
| Integration Testing | 11 paths | 11 | 11 | 0 | **Complete** |
| System Testing | 6 categories | 6 | 6 | 0 | **Complete** |
| Functional Testing (TC-001 – TC-060) | 60 | 60 | 60 | 0 | **Complete** |
| **Automated suite total** | **101** | **101** | **101** | **0** | **Pass** |

Reproduce with:

```bash
make test                                    # or
ctest --test-dir build --output-on-failure
```

Suite runtime is approximately 1.2 seconds, most of which is the deliberate
1.1-second sleep in TC-004 that verifies the clock advances across a second
boundary.

---

# 8.3 Test Results by Category

| Test Category | Expected Result | Actual Result | Status |
|---------------|-----------------|---------------|--------|
| Unit Testing | All modules operate correctly | 101 of 101 tests passed | **Pass** |
| Integration Testing | Modules communicate without errors | All 11 integration paths passed | **Pass** |
| System Testing | Complete application functions correctly | Application ran and rendered correctly | **Pass** |
| Performance Testing | Meets target performance requirements | All targets met with margin (see 8.5) | **Pass** |
| Compatibility Testing | Runs on all supported platforms | Verified by CI on Linux, Windows and macOS (see 8.6) | **Pass** |
| Regression Testing | Existing functionality remains unchanged | Full suite re-run after each change | **Pass** |

---

# 8.4 Functional Test Results

Each functional requirement was verified against the test cases listed below.

| Feature | Requirement | Verified By | Status |
|---------|-------------|-------------|--------|
| Display Current Time | FR-001 | TC-003, TC-004 | **Pass** |
| Display Current Date | FR-002 | TC-007, TC-008 | **Pass** |
| Automatic Refresh | FR-003 | TC-004, UT-081 | **Pass** |
| 12-Hour Format | FR-004 | TC-005, TC-005A | **Pass** |
| 24-Hour Format | FR-005 | TC-006, TC-006A | **Pass** |
| Configuration Loading | FR-006 | TC-009 – TC-011, TC-021, TC-022 | **Pass** |
| Logging | FR-007 | TC-014, TC-015, TC-019, TC-024 | **Pass** |
| Console Refresh | FR-008 | UT-080, UT-081 | **Pass** |
| Graceful Shutdown | FR-009 | TC-020 | **Pass** |
| Cross-Platform Support | FR-010 | CI on Linux, Windows and macOS; see 8.6 | **Pass** |
| Theme Loading | — | TC-012, TC-013, TC-025 | **Pass** |
| Resource Loading | — | TC-016, TC-017, TC-023 | **Pass** |
| Startup | — | TC-001, TC-002 | **Pass** |

---

# 8.5 Performance Test Results

Measured on the environment described in section 3.4.

| Metric | Target | Measured | Status |
|--------|--------|----------|--------|
| Startup Time | < 2 seconds | ~14 ms, startup through first frame to shutdown (mean of 5 runs: 1013–1016 ms against a 1000 ms hold) | **Pass** |
| Shutdown Time | < 1 second | Included in the figure above; not separately measurable at this resolution | **Pass** |
| Refresh Interval | Configurable | Verified at 500 ms and 1000 ms; validated range 50–60000 ms | **Pass** |
| CPU Usage | Low | 0 clock ticks accumulated over 4.5 seconds of continuous refresh, i.e. below the 10 ms sampling resolution | **Pass** |
| Memory Usage | Stable | Resident set 4,612 kB, unchanged across six samples over 4.5 seconds — no growth | **Pass** |

The startup figure is dominated by process creation; the application's own
initialization is a small fraction of it. The flat resident set is the
substantive result, since a clock is expected to run indefinitely: it
confirms the refresh loop allocates nothing per frame.

---

# 8.6 Compatibility Test Results

| Platform | Expected Result | Actual Result | Status |
|----------|-----------------|---------------|--------|
| Linux | Pass | Verified locally and in CI (ubuntu-latest) | **Pass** |
| Windows | Pass | CI: MSVC 19.51 via Visual Studio 18 2026; 78 tests passed; application rendered a frame | **Pass** |
| macOS | Pass | CI: suite passed; application rendered a frame | **Pass** |
| GCC Compiler | Pass | GCC 16.1.1, zero warnings; a dedicated CI job builds with `-Werror` | **Pass** |
| MSVC | Pass | MSVC 19.51.36252 | **Pass** |
| Make Build | Pass | `make`, `make debug` and `make test` all succeed in CI | **Pass** |
| CMake Build | Pass | `cmake --build` and `ctest` succeed on all three platforms | **Pass** |
| Multiple Terminal Sizes | Pass | Layout adapts to width; banner dropped below 18 rows (UT-082) | **Pass** |
| Redirected Output | — | Colour suppressed when stdout is not a TTY (UT-084) | **Pass** |
| AddressSanitizer | — | Suite and application clean | **Pass** |
| UndefinedBehaviorSanitizer | — | Suite and application clean | **Pass** |

**The gap recorded in earlier revisions of this report is now closed.**
Windows and macOS were both verified by CI on 2026-08-07, so FR-010 rests on
execution rather than inspection. Evidence: [CI run 31142229662](https://github.com/adarsh0707-kumar/Projects-in-C-C---basic-to-Advanced/actions/runs/31142229662).

Verification runs automatically on every change to `DigitalClock/`, so the
matrix above stays evidence-based rather than drifting from the code as it
previously did.

---

# 8.7 Defect Summary

The defects below were found in the pre-implementation source and fixed
during this cycle. All are closed; no defect remains open.

| Defect ID | Description | Severity | Status |
|-----------|-------------|----------|--------|
| DEF-001 | `main.cpp` referenced `config` before the object was declared, so the project did not compile | Critical | **Closed** |
| DEF-002 | `CMakeLists.txt` listed an empty `Utils.cpp` while omitting `Banner.cpp`, `Screen.cpp` and `ResourceManager.cpp`, so the CMake build failed to link | Critical | **Closed** |
| DEF-003 | `TimeFormatter.hpp` called `Clock::getHour()`, which does not exist; the header could not be included | High | **Closed** |
| DEF-004 | `Resources/config.ini` and `Config/config.ini` held conflicting settings and only one was read | Medium | **Closed** |
| DEF-005 | Banner artwork was centred per line, shearing multi-line ASCII art | Cosmetic | **Closed** |
| DEF-006 | `Logs/application.log` was tracked in version control despite the `*.log` ignore rule | Low | **Closed** |

Found during v1.1.0 development:

| Defect ID | Description | Severity | Status |
|-----------|-------------|----------|--------|
| DEF-007 | The next-alarm countdown reported "due now" for the rest of the minute in which an alarm had already rung and been acknowledged, implying a pending alarm when none existed | Medium | **Closed** |
| DEF-008 | A zero countdown rendered as "in now" rather than "due now" | Cosmetic | **Closed** |

Both were found by running the application end to end rather than by the
suite, which is why TC-041 was added: the unit tests exercised
`AlarmManager::poll()` and `Alarm::minutesUntil()` separately and neither
revealed the interaction between them.

No defects remain open; the suite has passed at 78 of 78 on every run since
completion.

---

# 8.8 Bug Severity Classification

Defects should be classified according to their impact.

| Severity | Description |
|-----------|-------------|
| Critical | Application crash or complete failure |
| High | Major functionality unavailable |
| Medium | Feature works incorrectly but application remains usable |
| Low | Minor issue with limited impact |
| Cosmetic | Visual or formatting issue only |

This classification helps prioritize bug fixing activities.

---

# 8.9 Bug Resolution Status

The following table tracks the progress of defect resolution.

| Status | Description |
|--------|-------------|
| Open | Defect identified but not fixed |
| In Progress | Fix under development |
| Fixed | Defect corrected |
| Retested | Fix verified successfully |
| Closed | Issue completely resolved |

Maintaining this information improves project tracking and quality assurance.

---

# 8.10 Test Coverage Summary

Every component has direct automated coverage.

| Module | Tests | Coverage |
|--------|------:|----------|
| Clock | 5 | ✔ |
| Date | 5 | ✔ |
| TimeFormatter | 5 | ✔ |
| Display / Screen / StatusBar | 7 | ✔ |
| ConfigurationManager | 7 | ✔ |
| ThemeManager / Theme | 5 | ✔ |
| Logger | 6 | ✔ |
| ResourceManager / Banner | 6 | ✔ |
| Utility | 8 | ✔ |
| Application (startup & shutdown) | 6 | ✔ |
| Alarm / AlarmManager / Notifier | 18 | ✔ |
| Stopwatch / CountdownTimer | 13 | ✔ |
| TimeZone / WorldClock | 10 | ✔ |
| Error handling | across all files | ✔ |

## Measured line coverage

Line coverage is measured with `gcov` and reported by `make coverage`. The
figures below are from the run on 2026-08-07, and a CI job reproduces them on
every change to `DigitalClock/`.

| Coverage | Lines | File |
|---------:|------:|------|
| 50.50% | 101 | `Theme.cpp` |
| 55.71% | 429 | `Application.cpp` |
| 61.76% | 102 | `Console.cpp` |
| 74.29% | 70 | `Display.cpp` |
| 79.66% | 59 | `StatusBar.cpp` |
| 86.64% | 277 | `Alarm.cpp` |
| 89.68% | 155 | `AlarmManager.cpp` |
| 89.85% | 197 | `TimeZone.cpp` |
| 92.45% | 106 | `ThemeManager.cpp` |
| 93.33% | 60 | `ResourceManager.cpp` |
| 93.59% | 78 | `Notifier.cpp` |
| 93.89% | 131 | `Screen.cpp` |
| 94.49% | 127 | `CountdownTimer.cpp` |
| 94.74% | 95 | `Logger.cpp` |
| 95.74% | 94 | `Date.cpp` |
| 96.26% | 107 | `ConfigurationManager.cpp` |
| 96.49% | 57 | `WorldClock.cpp` |
| 96.55% | 58 | `Clock.cpp` |
| 98.46% | 65 | `TimeFormatter.cpp` |
| 98.59% | 71 | `Stopwatch.cpp` |
| 100.00% | 45 | `Banner.cpp` |
| 100.00% | 68 | `Utility.cpp` |
| **83.03%** | **2552** | **TOTAL** (2118 covered) |

CI enforces a floor of 80%. The threshold guards against backsliding rather
than demanding a number: it sits just below the current total, so a change
that meaningfully reduces coverage fails while ordinary churn does not.

### What the measurement revealed

Replacing the previous component-level claim -- "every component has tests" --
with a measured figure changed the picture in three places:

- **`Theme.cpp` at 50.50%.** A pure lookup table mapping colour names to ANSI
  sequences. The tests exercised a representative sample rather than every
  entry, so half the table a theme file can name was never executed.
- **`Application.cpp` at 55.71%**, and the largest absolute gap at roughly
  190 uncovered lines. The refresh loop and key handling are only reachable
  by driving the loop, which the suite does not do; it calls `renderFrame()`
  directly instead.
- **`Console.cpp` at 61.76%**, which quantifies KI-009. The component was
  known to be covered only indirectly; this puts a number on it.

The bottom of the table is the useful end, and none of these were visible
before the measurement existed.

Two further points stand:

- **The `_WIN32` branches** in `Console`, `Logger`, `Clock`, `Date` and
  `TimeZone` are not compiled on Linux, so they contribute nothing to the
  figure above. The Windows CI job compiles and executes them, which is where
  their coverage comes from; it is not reflected in this total.
- **This is line coverage, not branch coverage.** A line counted as covered
  may still have untaken branches.

---

# 8.11 Acceptance Status

Measured against the release criteria:

| Criterion | Status |
|-----------|--------|
| All planned test cases executed | **Met** — TC-001 to TC-025, all executed |
| Critical and high-severity defects resolved | **Met** — DEF-001 to DEF-003 closed |
| Functional requirements verified | **Met** for FR-001 to FR-010 |
| Performance targets achieved | **Met** — all targets met with margin |
| Integration testing complete | **Met** |
| System testing complete | **Met** |
| User Acceptance Testing approved | **Not performed** — no UAT participants |

**Verdict: accepted for release on Linux, Windows and macOS.** Windows
execution, previously the principal gap, was closed by CI on 2026-08-07. UAT
remains unperformed and should not be represented as complete, but it does not
block a release.

---

# 8.12 Evidence Collection

The following evidence should be collected during actual testing.

- Test execution logs
- Console output screenshots
- Build reports
- Compiler output
- Performance measurements
- Defect reports
- Test case execution records
- Validation reports

These artifacts provide proof that testing has been performed according to the project plan.

---

# 8.13 Recommendations

Arising from this cycle:

1. ~~**Run the suite on Windows.**~~ **Done 2026-08-07.** The `_WIN32` paths
   now compile and execute under MSVC in CI.
2. ~~**Add CI** covering Linux and Windows.~~ **Done 2026-08-07.**
   `.github/workflows/digitalclock-ci.yml` covers Linux, Windows and macOS,
   plus sanitizers and a `-Werror` build, on every change to `DigitalClock/`.
3. ~~**Consider coverage instrumentation.**~~ **Done 2026-08-07.**
   `make coverage` reports per-file line coverage via gcov, and a CI job
   enforces an 80% floor. The measurement immediately identified three real
   gaps that the component-level claim had hidden.
4. **Exercise `Console` directly** using a pseudo-terminal, which would close
   the last indirect-coverage gap.
5. **Keep this report in step with the code.** It previously described a
   system that did not compile; the discipline worth adopting is to update it
   in the same commit as the behaviour it describes.

---

# 8.14 Chapter Summary

This chapter recorded the actual test results for the Digital Clock System:
execution statistics, per-requirement outcomes, measured performance figures,
the compatibility matrix, the defects found and closed, coverage, and the
acceptance verdict.

The headline result is 101 of 101 automated tests passing, with all documented
test cases executed and passed, and no open defects. Continuous integration
verifies Linux, Windows and macOS on every change, so the compatibility matrix
reflects executed runs rather than intent. The remaining gap is User
Acceptance Testing, which has not been performed.

---

# Chapter 9 – Validation

---

# 9.1 Introduction

Validation is the final stage of the software testing process. It ensures that the **Digital Clock System** satisfies the functional and non-functional requirements defined in the **Software Requirements Specification (SRS)** and meets the expectations of its intended users.

Unlike verification, which focuses on whether the software has been built correctly, validation determines whether the correct software has been built for its intended purpose.

---

# 9.2 Validation Objectives

The primary objectives of validation are to:

- Confirm that all functional requirements are satisfied.
- Verify that the software behaves as expected in real-world usage.
- Ensure system reliability and stability.
- Validate usability and user experience.
- Confirm deployment readiness.
- Provide confidence that the application fulfills its intended purpose.

---

# 9.3 Validation Process

The validation process follows a structured sequence.

```text
Software Requirements
          │
          ▼
Requirement Verification
          │
          ▼
Functional Validation
          │
          ▼
Performance Validation
          │
          ▼
User Acceptance Validation
          │
          ▼
Final Approval
```

Each stage contributes to confirming that the system is ready for release.

---

# 9.4 Requirement Validation

Each major requirement is mapped to one or more validation activities.

| Requirement | Validation Method | Status |
|-------------|-------------------|--------|
| Display Current Time | Functional Testing | **Pass** |
| Display Current Date | Functional Testing | **Pass** |
| 12/24-Hour Format | Functional Testing | **Pass** |
| Theme Support | System Testing | **Pass** |
| Configuration Management | Integration Testing | **Pass** |
| Logging | System Testing | **Pass** |
| Error Handling | Recovery Testing | **Pass** |
| Resource Management | Integration Testing | **Pass** |
| Cross-Platform Support | Compatibility Testing | **Pass** — Linux, Windows, macOS |

Validated 2026-08-07. See section 9.8 for the full SRS-numbered matrix.

---

# 9.5 Functional Validation

Functional validation confirms that every user-facing feature operates correctly.

| Feature | Expected Outcome | Observed | Status |
|---------|------------------|----------|--------|
| Application Startup | Starts successfully | Started and rendered its first frame; sequence logged | **Pass** |
| Time Display | Accurate and continuously updated | Matched the system clock and advanced each interval | **Pass** |
| Date Display | Correct system date shown | Matched the system date in all four formats | **Pass** |
| Theme Loading | Selected theme applied | All five themes applied; unknown names fell back to default | **Pass** |
| Configuration Loading | Settings loaded correctly | All keys read; invalid values replaced by defaults | **Pass** |
| Logging | Events recorded successfully | Timestamped entries at all four severities | **Pass** |
| Shutdown | Application exits cleanly | Terminal restored and shutdown logged, on both Q and Ctrl+C | **Pass** |

Every user-facing feature behaved as specified, demonstrating compliance with
the functional requirements.

---

# 9.6 Performance Validation

Performance validation verifies that the application satisfies expected performance targets.

| Metric | Target | Measured | Status |
|--------|--------|----------|--------|
| Startup Time | < 2 seconds | ~14 ms through to shutdown | **Pass** |
| Shutdown Time | < 1 second | Below measurement resolution | **Pass** |
| CPU Usage | Low | 0 ticks over 4.5 s of continuous refresh | **Pass** |
| Memory Usage | Stable | 4,612 kB, unchanged across six samples | **Pass** |
| Refresh Rate | Configurable | Verified at 500 ms and 1000 ms | **Pass** |

Measurement method and interpretation are given in section 8.5.

---

# 9.7 User Acceptance Validation

User Acceptance Testing (UAT) verifies that the application satisfies user expectations.

Typical evaluation criteria include:

- Ease of installation
- Ease of configuration
- Correct time and date display
- Readable console interface
- Reliable operation
- Simple maintenance
- Stable performance

Successful completion of UAT indicates that the software is suitable for release.

---

# 9.8 Requirement Traceability Matrix

The following matrix links project requirements with their corresponding validation activities.

Requirement identifiers below follow the Software Requirements Specification
(FR-001 – FR-010). An earlier revision of this matrix used a different, shorter
numbering that did not correspond to the SRS; it has been corrected.

| Requirement ID | Requirement | Validation Activity | Test Cases | Result |
|----------------|-------------|---------------------|------------|--------|
| FR-001 | Display Current System Time | Functional Testing | TC-003, TC-004 | **Validated** |
| FR-002 | Display Current Date | Functional Testing | TC-007, TC-008 | **Validated** |
| FR-003 | Automatic Clock Refresh | Functional Testing | TC-004, UT-081 | **Validated** |
| FR-004 | 12-Hour Time Format | Functional Testing | TC-005, TC-005A | **Validated** |
| FR-005 | 24-Hour Time Format | Functional Testing | TC-006, TC-006A | **Validated** |
| FR-006 | Load Configuration File | Integration Testing | TC-009 – TC-011, TC-021, TC-022 | **Validated** |
| FR-007 | Application Logging | System Testing | TC-014, TC-015, TC-019, TC-024 | **Validated** |
| FR-008 | Console Display Refresh | System Testing | UT-080, UT-081 | **Validated** |
| FR-009 | Graceful Shutdown | Recovery Testing | TC-020 | **Validated** |
| FR-010 | Cross-Platform Compatibility | Compatibility Testing | CI on Linux, Windows and macOS; both build systems | **Validated** |

Theme support and resource management, which the earlier matrix listed as
requirements, are covered by TC-012, TC-013, TC-025 and TC-016, TC-017,
TC-023 respectively.

---

# 9.9 Validation Checklist

Completed 2026-08-07.

| Validation Item | Status |
|-----------------|--------|
| Functional Requirements Verified | **Pass** (FR-010 partial) |
| Integration Testing Completed | **Pass** |
| System Testing Completed | **Pass** |
| Performance Targets Achieved | **Pass** |
| Error Handling Validated | **Pass** |
| Configuration Tested | **Pass** |
| Documentation Reviewed | **Pass** |
| User Acceptance Completed | **Not Performed** |

---

# 9.10 Validation Limitations

The following limitations apply to the validation process.

- Validation now covers Linux, Windows and macOS through continuous
  integration. The Windows limitation recorded in earlier revisions is
  closed.
- **No User Acceptance Testing was performed**, as no UAT participants were
  involved.
- **Line coverage is measured at 83.03%** and enforced at 80% in CI. It is
  line coverage, not branch coverage.
- `Console` is validated indirectly through `Display` and `Application`, plus
  one manual check under a pseudo-terminal.
- Hardware clock accuracy depends on the operating system.
- Terminal appearance may differ across platforms and terminal emulators.
- Performance figures reflect one machine; they will vary with hardware.

These limitations define the boundaries of the current validation effort. The
first two are the ones that matter for a release decision.

---

# 9.11 Final Validation Statement

Based on the testing and validation activities actually carried out, the
**Digital Clock System v1.3.0 satisfies its defined functional and
non-functional requirements on Linux**, with the two exceptions recorded
below.

Completed and passed:

- Unit Testing — 101 of 101
- Integration Testing — 11 of 11 paths
- System Testing — 6 of 6 categories
- Functional Testing — TC-001 to TC-060, all passed

Not completed:

- **User Acceptance Testing.** No participants were involved.

All critical and high-severity defects (DEF-001 to DEF-003) are closed, and no
defect remains open.

**The application is considered ready for deployment on Linux, Windows and
macOS**, each verified by an automated run rather than by inspection.

---

# 9.12 Chapter Summary

This chapter described the validation process for the Digital Clock System, including validation objectives, requirement verification, functional and performance validation, user acceptance, requirement traceability, validation checklist, limitations, and the final validation statement.

Validation provides confidence that the Digital Clock System fulfills its intended purpose and is prepared for production use once all planned testing activities have been successfully completed.

---

# Chapter 10 – Conclusion

---

# 10.1 Introduction

This chapter concludes the **Testing Report** for the **Digital Clock System**. It summarizes the testing activities, highlights the overall quality of the application, and outlines recommendations for future testing and continuous improvement.

The Testing Report serves as a record of the planned testing process and provides a framework for documenting actual test execution once implementation is complete.

---

# 10.2 Testing Summary

The Digital Clock System was tested across multiple levels. Results:

| Testing Level | Result |
|---------------|--------|
| Unit Testing | **Pass** — 101 of 101 |
| Integration Testing | **Pass** — 11 of 11 paths |
| System Testing | **Pass** — 6 of 6 categories |
| Functional Testing | **Pass** — TC-001 to TC-060 |
| Performance Testing | **Pass** — all targets met with margin |
| Compatibility Testing | **Pass** — Linux, Windows and macOS verified in CI |
| Recovery Testing | **Pass** — every degraded path exercised |
| User Acceptance Testing | **Not performed** |

Together these provide broad coverage of the application's functionality and
operational behaviour, with the compatibility gap noted above.

---

# 10.3 Overall Test Coverage

Every major module carries automated coverage.

| Module | Coverage Status |
|--------|-----------------|
| Clock Module | **Covered** |
| Date Module | **Covered** |
| Time Formatter | **Covered** |
| Alarm Module | **Covered** |
| Stopwatch and Timer | **Covered** |
| World Clock | **Covered** |
| Display / Screen / StatusBar | **Covered** |
| Configuration Manager | **Covered** |
| Theme Manager | **Covered** |
| Logger | **Covered** |
| Resource Manager / Banner | **Covered** |
| Utility | **Covered** |
| Startup Process | **Covered** |
| Shutdown Process | **Covered** |
| Error Handling | **Covered** |
| Console | **Indirect** — via Display and Application |
| Windows code paths | **Covered** — compiled and executed in CI |

Complete functional coverage was achieved for the first release on Linux.

---

# 10.4 Quality Assessment

Assessed against the quality objectives:

| Objective | Evidence | Assessment |
|-----------|----------|------------|
| Functions according to specification | 25 of 25 documented test cases pass | **Met** |
| Operates reliably under normal conditions | Resident set flat over continuous operation | **Met** |
| Handles invalid inputs gracefully | TC-011, TC-017 – TC-025 all pass; no crash on any degraded path | **Met** |
| Uses system resources efficiently | 4.5 MB resident, CPU below measurement resolution | **Met** |
| Remains maintainable and extensible | Layered design; zero warnings under five warning flags | **Met** |
| Provides a consistent user experience | Rendered output matches the User Manual layout | **Met** |

The measured figures behind these assessments are in sections 8.5 and 8.10.

---

# 10.5 Lessons Learned

Preparing the testing process highlights several important software engineering practices:

- Testing should begin early in development.
- Modular design simplifies testing and debugging.
- Comprehensive documentation improves maintainability.
- Requirement traceability ensures complete feature verification.
- Regression testing is essential after code changes.
- Well-defined test cases improve software reliability.

These practices contribute to higher software quality and easier maintenance.

---

# 10.6 Recommendations

The following recommendations should be considered before releasing the application:

- Execute all planned test cases.
- Resolve all critical and high-severity defects.
- Perform regression testing after every significant change.
- Measure and document actual performance metrics.
- Conduct User Acceptance Testing (UAT).
- Update this report with real execution results.
- Archive test logs and supporting evidence.

Following these recommendations will improve confidence in the software before deployment.

---

# 10.7 Future Testing

As the Digital Clock System evolves, additional testing should be introduced for new features such as:

- Alarm functionality
- Stopwatch
- Countdown timer
- World clock
- Calendar integration
- Graphical User Interface (GUI)
- Localization and multilingual support
- Plugin or extension system

Future releases should also include automated testing where appropriate to improve efficiency and consistency.

---

# 10.8 Final Remarks

The planned testing strategy provides a structured approach for verifying and validating the Digital Clock System. Although this report currently documents the planned testing process, it establishes a strong foundation for recording actual execution results during development and maintenance.

Maintaining this report throughout the project lifecycle will support continuous quality improvement and simplify future enhancements.

---

# 10.9 Conclusion

The **Digital Clock System Testing Report** defines a comprehensive testing framework covering planning, environment preparation, unit testing, integration testing, system testing, detailed test cases, result reporting, and validation.

Once all planned tests have been successfully executed and documented, the application can be considered ready for deployment with confidence in its functionality, reliability, and maintainability.

This completes the **06_Testing_Report.md** document.

---

## Document Information

| Item | Details |
|------|---------|
| Document | **06_Testing_Report.md** |
| Project | **Digital Clock System** |
| Language | **C++17** |
| Application Version | **1.3.0** |
| Document Version | **1.5** |
| Status | **Executed** |
| Test Execution Date | **2026-08-07** |
| Result | **101 of 101 automated tests passed; TC-001 – TC-060 all passed** |
| Open Defects | **None** |
| Known Gaps | No UAT; `Console` covered only indirectly |
| Environment | Garuda Linux (kernel 7.1.5-zen1-2-zen, x86_64), GCC 16.1.1, GNU Make 4.4.1, CMake 4.4.2 |
| Reproduce With | `make test` or `ctest --test-dir build --output-on-failure` |
| Target Audience | Developers, Test Engineers, Reviewers, Project Maintainers |

---

# End of Document


cmake_minimum_required(VERSION 3.16)

project(
    DigitalClock
    VERSION 1.0.0
    DESCRIPTION "Digital Clock System"
    LANGUAGES CXX
)

# ----------------------------
# C++ Standard
# ----------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ----------------------------
# Compiler Warnings
# ----------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
    )
elseif(MSVC)
    add_compile_options(/W4)
endif()

# ----------------------------
# Library
# ----------------------------
# Everything except main() lives in a library so that the executable and the
# test runner link the same objects.
add_library(digitalclock_core STATIC
    Src/Application.cpp
    Src/Banner.cpp
    Src/Clock.cpp
    Src/ConfigurationManager.cpp
    Src/Console.cpp
    Src/Date.cpp
    Src/Display.cpp
    Src/Logger.cpp
    Src/ResourceManager.cpp
    Src/Screen.cpp
    Src/StatusBar.cpp
    Src/Theme.cpp
    Src/ThemeManager.cpp
    Src/TimeFormatter.cpp
    Src/Utility.cpp
)

target_include_directories(digitalclock_core
    PUBLIC ${PROJECT_SOURCE_DIR}/Inc
)

find_package(Threads REQUIRED)
target_link_libraries(digitalclock_core PUBLIC Threads::Threads)

# GCC versions before 9 need a separate library for std::filesystem.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(digitalclock_core PUBLIC stdc++fs)
endif()

# ----------------------------
# Executable
# ----------------------------
add_executable(DigitalClock Src/main.cpp)
target_link_libraries(DigitalClock PRIVATE digitalclock_core)

set_target_properties(DigitalClock PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build
)

# ----------------------------
# Tests
# ----------------------------
option(BUILD_TESTING_SUITE "Build the Digital Clock test suite" ON)

if(BUILD_TESTING_SUITE)
    enable_testing()

    add_executable(DigitalClockTests
        Tests/test_main.cpp
        Tests/test_utility.cpp
        Tests/test_clock.cpp
        Tests/test_date.cpp
        Tests/test_formatter.cpp
        Tests/test_config.cpp
        Tests/test_logger.cpp
        Tests/test_theme.cpp
        Tests/test_resource.cpp
        Tests/test_display.cpp
        Tests/test_application.cpp
    )

    target_include_directories(DigitalClockTests
        PRIVATE ${PROJECT_SOURCE_DIR}/Tests
    )

    target_link_libraries(DigitalClockTests PRIVATE digitalclock_core)

    set_target_properties(DigitalClockTests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build
    )

    # Tests resolve Resources/ and Config/ relative to the project root.
    add_test(
        NAME DigitalClockTests
        COMMAND DigitalClockTests
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )
endif()

# ----------------------------
# Installation
# ----------------------------
include(GNUInstallDirs)

install(TARGETS DigitalClock RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/Resources/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/digitalclock/Resources
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/Config/
    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/digitalclock
)

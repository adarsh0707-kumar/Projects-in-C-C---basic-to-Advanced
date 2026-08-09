cmake_minimum_required(VERSION 3.16)

project(
    DigitalClock
    VERSION 2.1.0
    DESCRIPTION "Digital Clock System"
    LANGUAGES CXX C
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
    Src/Alarm.cpp
    Src/AlarmManager.cpp
    Src/Application.cpp
    Src/Banner.cpp
    Src/Clock.cpp
    Src/ConfigurationManager.cpp
    Src/CountdownTimer.cpp
    Src/Console.cpp
    Src/Date.cpp
    Src/Display.cpp
    Src/Logger.cpp
    Src/Notifier.cpp
    Src/PluginManager.cpp
    Src/ResourceManager.cpp
    Src/Screen.cpp
    Src/Stopwatch.cpp
    Src/StatusBar.cpp
    Src/Theme.cpp
    Src/ThemeManager.cpp
    Src/TimeFormatter.cpp
    Src/TimeZone.cpp
    Src/Utility.cpp
    Src/WorldClock.cpp
)

target_include_directories(digitalclock_core
    PUBLIC ${PROJECT_SOURCE_DIR}/Inc
)

find_package(Threads REQUIRED)
target_link_libraries(digitalclock_core PUBLIC Threads::Threads)

# dlopen(): part of libc on current glibc, a separate library on older
# ones and elsewhere. CMAKE_DL_LIBS is empty where it is not needed.
target_link_libraries(digitalclock_core PUBLIC ${CMAKE_DL_LIBS})

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
# Plugins (v2.1.0)
# ----------------------------
# Built as MODULE libraries: loaded at runtime with dlopen, never linked
# against. Each compiles against Inc/Plugin.h alone -- no part of the
# application -- which is what lets a plugin be built by a different
# compiler than the clock.
#
# PREFIX and SUFFIX are set explicitly so the filenames match what
# PluginManager::librarySuffix() looks for. CMake would otherwise produce
# libpomodoro.so here and, on macOS, give a MODULE the .so suffix while
# the loader expects .dylib.
if(APPLE)
    set(DIGITALCLOCK_PLUGIN_SUFFIX ".dylib")
elseif(WIN32)
    set(DIGITALCLOCK_PLUGIN_SUFFIX ".dll")
else()
    set(DIGITALCLOCK_PLUGIN_SUFFIX ".so")
endif()

function(digitalclock_add_plugin target source destination)
    add_library(${target} MODULE ${source})

    target_include_directories(${target} PRIVATE ${PROJECT_SOURCE_DIR}/Inc)

    set_target_properties(${target} PROPERTIES
        PREFIX ""
        SUFFIX "${DIGITALCLOCK_PLUGIN_SUFFIX}"
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
        LIBRARY_OUTPUT_DIRECTORY "${destination}"
    )
endfunction()

# The bundled example: a Pomodoro mode.
digitalclock_add_plugin(pomodoro Plugins/pomodoro.c
                        ${PROJECT_SOURCE_DIR}/Plugins)

# ----------------------------
# Tests
# ----------------------------
# Declared before the GUI section because that section builds GUI tests
# when Qt6::Test is present, and an option consulted before it is
# declared is simply empty -- the GUI tests would have been silently
# skipped on every machine.
option(BUILD_TESTING_SUITE "Build the Digital Clock test suite" ON)

# ----------------------------
# Graphical interface (v2.0.0)
# ----------------------------
# The console application has no third-party dependencies and keeps none:
# it links digitalclock_core and the standard library, nothing else. A
# graphical interface cannot make that promise, so it is a separate,
# optional target rather than a change to the existing one. Neither
# build affects the other.
#
# The option exists so a build can opt out even when Qt *is* installed.
# Without it, whether a release archive contains a GUI binary would
# depend on whether the build machine happened to have Qt, and an
# unbundled binary simply fails to start on a user's clean machine. The
# CLI release workflow therefore passes OFF explicitly, so the archive's
# contents are a decision rather than an accident of the environment.
option(DIGITALCLOCK_BUILD_GUI
       "Build the Qt6 graphical interface (requires Qt6; harmless if absent)"
       ON)

if(DIGITALCLOCK_BUILD_GUI)
    find_package(Qt6 6.2 COMPONENTS Widgets QUIET)
else()
    message(STATUS "DIGITALCLOCK_BUILD_GUI=OFF -- skipping the GUI target")
endif()

if(Qt6_FOUND)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    # WIN32 suppresses the console window on Windows; MACOSX_BUNDLE
    # produces the .app a macOS user expects to launch.
    add_executable(digitalclock-gui WIN32 MACOSX_BUNDLE
        Gui/main.cpp
        Gui/ClockWindow.cpp
        Gui/ClockWindow.hpp
        Gui/GuiTheme.cpp
        Gui/GuiTheme.hpp
        Gui/resources.qrc
    )

    # The whole point of the layering: the GUI links the same core the
    # console front end does, and replaces only the presentation layer.
    target_link_libraries(digitalclock-gui PRIVATE digitalclock_core Qt6::Widgets)

    set_target_properties(digitalclock-gui PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build
        MACOSX_BUNDLE_BUNDLE_NAME "Digital Clock"
        MACOSX_BUNDLE_GUI_IDENTIFIER "io.github.adarsh0707-kumar.digitalclock"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    )

    message(STATUS "Qt6 ${Qt6_VERSION} found -- building digitalclock-gui")

    # ---- GUI tests (KI-010) ----
    #
    # A separate target, and a separate find_package, because Qt6::Test is
    # a component a Qt installation can legitimately lack. Where it does,
    # the GUI still builds; only its tests are skipped.
    #
    # These drive the window by keyboard and assert on what is displayed,
    # so they need a display: ctest runs them under the offscreen platform,
    # which is enough for widgets, layout and shortcuts.
    if(BUILD_TESTING_SUITE)
        find_package(Qt6 ${Qt6_VERSION} COMPONENTS Test QUIET)

        if(Qt6Test_FOUND)
            enable_testing()

            add_executable(digitalclock_gui_tests
                Tests/gui/test_clockwindow.cpp
                Gui/ClockWindow.cpp
                Gui/ClockWindow.hpp
                Gui/GuiTheme.cpp
                Gui/GuiTheme.hpp
                Gui/resources.qrc
            )

            target_include_directories(digitalclock_gui_tests
                PRIVATE ${PROJECT_SOURCE_DIR}/Gui
            )

            target_link_libraries(digitalclock_gui_tests
                PRIVATE digitalclock_core Qt6::Widgets Qt6::Test
            )

            set_target_properties(digitalclock_gui_tests PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build
            )

            add_test(
                NAME digitalclock_gui_tests
                COMMAND digitalclock_gui_tests
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            )

            # Without a display the tests would fail rather than skip, and
            # a failure that means "no X server" is not a useful signal.
            set_tests_properties(digitalclock_gui_tests PROPERTIES
                ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
            )

            message(STATUS "Qt6 Test found -- building digitalclock_gui_tests")
        else()
            message(STATUS "Qt6 Test not found -- skipping the GUI tests")
        endif()
    endif()
else()
    message(STATUS "Qt6 not found -- building the console application and "
                   "tests only (this is fine; the GUI is optional)")
endif()

# ----------------------------
# Tests
# ----------------------------

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
        Tests/test_alarm.cpp
        Tests/test_timing.cpp
        Tests/test_timezone.cpp
        Tests/test_console.cpp
        Tests/test_lifecycle.cpp
        Tests/test_plugin.cpp
    )

    target_include_directories(DigitalClockTests
        PRIVATE ${PROJECT_SOURCE_DIR}/Tests
    )

    target_link_libraries(DigitalClockTests PRIVATE digitalclock_core)

    # Deliberately broken plugins, so the loader's refusals are tested
    # rather than assumed. Built into Build/ where the tests look.
    digitalclock_add_plugin(badabi Tests/fixtures/badabi.c
                            ${PROJECT_SOURCE_DIR}/Build)
    digitalclock_add_plugin(notaplugin Tests/fixtures/notaplugin.c
                            ${PROJECT_SOURCE_DIR}/Build)

    # ctest must not run before the plugins it loads exist.
    add_dependencies(DigitalClockTests pomodoro badabi notaplugin)

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

# Two layouts, because they answer different questions.
#
# The default is FHS -- bin/, share/, etc/ -- which is what a system
# install into /usr/local should look like.
#
# A downloadable archive needs something else. ResourceManager resolves
# Resources/ and Config/ relative to the working directory (and its
# parents), so an FHS tree would leave an extracted archive unable to find
# its own theme and configuration files: it would start, silently fall back
# to the built-in banner and default theme, and look subtly broken. The
# portable layout puts everything in one directory so that extracting and
# running works, which is the only promise a download makes.
option(DIGITALCLOCK_PORTABLE_INSTALL
       "Install a self-contained, relocatable directory tree" OFF)

if(DIGITALCLOCK_PORTABLE_INSTALL)
    install(TARGETS DigitalClock RUNTIME DESTINATION .)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/Resources/ DESTINATION Resources)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/Config/ DESTINATION Config)

    install(
        FILES
            ${PROJECT_SOURCE_DIR}/README.md
            ${PROJECT_SOURCE_DIR}/LICENSE
        DESTINATION .
    )
else()
    install(TARGETS DigitalClock RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/Resources/
        DESTINATION ${CMAKE_INSTALL_DATADIR}/digitalclock/Resources
    )

    install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/Config/
        DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/digitalclock
    )
endif()

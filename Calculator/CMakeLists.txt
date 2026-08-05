# ---------------------------------------------------------------------
# Calculator -- cross-platform build (Phase 35)
#
# The Makefile beside this file remains the Linux development build and
# is not going away: it carries the sanitizer configs, the BUILD=<type>
# object-directory layout, and the moc/rcc discovery notes. This file
# exists because that Makefile is POSIX-shell-only (mkdir -p, command
# -v, pkg-config, shell loops), so Windows/MSVC and macOS/Xcode need a
# generator-agnostic description of the same targets.
#
#   cmake -B build -DCMAKE_BUILD_TYPE=Release
#   cmake --build build
#   ctest --test-dir build --output-on-failure
#
# Keep the source lists below in sync with the Makefile's SRCS /
# CXX_SRCS / TEST_SRCS. They are written out explicitly rather than
# globbed for the same reason the Makefile lists them: a file appearing
# on disk shouldn't silently join the build.
# ---------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16)

project(Calculator
    VERSION 1.1.0
    DESCRIPTION "Scientific calculator with variables, matrices, complex numbers, and plotting"
    LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Matches the Makefile: no compiler extensions, so the code stays
# portable rather than quietly depending on GNU ones.
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Binaries land beside the build directory root, so `build/calculator`
# works the same way `./calculator` does with the Makefile.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# ---------------------------------------------------------------------
# Warnings -- same intent as the Makefile's -Wall -Wextra, spelled for
# whichever compiler is in use. Not errors: a new compiler version
# finding a new warning shouldn't break someone's download-and-build.
# ---------------------------------------------------------------------
if(MSVC)
    add_compile_options(/W4)
    # The engine uses strcpy/sprintf/getenv, which MSVC deprecates in
    # favour of its own _s variants. Those aren't portable C, so the
    # code deliberately keeps the standard ones.
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra)
endif()

# ---------------------------------------------------------------------
# Engine -- every module except Src/main.c, which is the CLI entry
# point. Sharing it as a library is what lets the CLI, the test binary,
# and the GUI all build against exactly the same objects.
# ---------------------------------------------------------------------
add_library(calculator_engine STATIC
    Src/angle_mode.c
    Src/base.c
    Src/constants.c
    Src/error.c
    Src/evaluate.c
    Src/function_info.c
    Src/functions.c
    Src/history.c
    Src/memory.c
    Src/parser.c
    Src/plot.c
    Src/postfix.c
    Src/stack.c
    Src/stats.c
    Src/tokenizer.c
    Src/units.c
    Src/validator.c
    Src/variables.c

    Src/Complex.cpp
    Src/complex_eval.cpp
    Src/Matrix.cpp
    Src/matrix_eval.cpp)

target_include_directories(calculator_engine PUBLIC Inc)

# libm is a separate library on Unix; on Windows it's part of the CRT.
if(UNIX)
    target_link_libraries(calculator_engine PUBLIC m)
endif()

# ---------------------------------------------------------------------
# CLI -- no external dependencies at all, which is why it's the target
# that can ship to every platform first.
# ---------------------------------------------------------------------
add_executable(calculator Src/main.c)
target_link_libraries(calculator PRIVATE calculator_engine)

# ---------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------
enable_testing()

add_executable(calculator-tests
    Tests/test_main.c
    Tests/test_base.c
    Tests/test_complex.c
    Tests/test_error.c
    Tests/test_functions.c
    Tests/test_history.c
    Tests/test_matrix.c
    Tests/test_memory.c
    Tests/test_parser.c
    Tests/test_plot.c
    Tests/test_postfix.c
    Tests/test_stack.c
    Tests/test_stats.c
    Tests/test_tokenizer.c
    Tests/test_units.c
    Tests/test_validator.c
    Tests/test_variables.c)

target_include_directories(calculator-tests PRIVATE Tests)
target_link_libraries(calculator-tests PRIVATE calculator_engine)

add_test(NAME unit_tests COMMAND calculator-tests)

# The suite writes a scratch history file into the working directory
# (see Tests/test_history.c), so give it the build dir rather than
# whatever directory ctest happened to be invoked from.
set_tests_properties(unit_tests PROPERTIES
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

# ---------------------------------------------------------------------
# GUI -- optional. Qt6 missing is not an error: the CLI and the tests
# are the parts that must build everywhere, exactly as `make` works
# with or without Qt installed.
# ---------------------------------------------------------------------
find_package(Qt6 6.2 COMPONENTS Widgets QUIET)

if(Qt6_FOUND)
    # AUTOMOC/AUTORCC replace the Makefile's explicit moc and rcc rules,
    # and pick the tooling matching the Qt6 found above -- which is the
    # same Qt5/Qt6 mismatch problem the Makefile solves via
    # `qmake6 -query QT_HOST_LIBEXECS`.
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    # WIN32 suppresses the console window on Windows; MACOSX_BUNDLE
    # produces a .app, which is what a macOS user expects to launch and
    # what macdeployqt/DMG packaging needs.
    add_executable(calculator-gui WIN32 MACOSX_BUNDLE
        Gui/main.cpp
        Gui/MainWindow.cpp
        Gui/MainWindow.hpp
        Gui/PlotWidget.cpp
        Gui/PlotWidget.hpp
        Gui/resources.qrc)

    target_link_libraries(calculator-gui PRIVATE calculator_engine Qt6::Widgets)

    set_target_properties(calculator-gui PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "Calculator"
        MACOSX_BUNDLE_GUI_IDENTIFIER "io.github.adarsh0707-kumar.calculator"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})

    message(STATUS "Qt6 ${Qt6_VERSION} found -- building calculator-gui")
else()
    message(STATUS "Qt6 not found -- building CLI and tests only "
                   "(this is fine; the GUI is optional)")
endif()

# ---------------------------------------------------------------------
# Install rules -- what a packaged release contains. Kept minimal on
# purpose; the release archives and installers are assembled by CI.
# ---------------------------------------------------------------------
include(GNUInstallDirs)

install(TARGETS calculator
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(Qt6_FOUND)
    install(TARGETS calculator-gui
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION .)
endif()

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
              ${CMAKE_CURRENT_SOURCE_DIR}/Readme.md
    DESTINATION ${CMAKE_INSTALL_DOCDIR})

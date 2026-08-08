cmake_minimum_required(VERSION 3.16)

project(
    GuessTheNumber
    VERSION 1.0.0
    DESCRIPTION "Guess The Number"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wconversion)
elseif(MSVC)
    add_compile_options(/W4)
endif()

# The rules live in a library so the game and the tests link the same
# object, rather than the tests re-compiling a copy that could drift.
add_library(guessthenumber_core STATIC Src/Game.c)

target_include_directories(guessthenumber_core
    PUBLIC ${PROJECT_SOURCE_DIR}/Inc
)

add_executable(guessthenumber Src/main.c)
target_link_libraries(guessthenumber PRIVATE guessthenumber_core)

set_target_properties(guessthenumber PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build
)

option(BUILD_TESTING_SUITE "Build the test suite" ON)

if(BUILD_TESTING_SUITE)
    enable_testing()

    add_executable(guessthenumber_tests Tests/test_game.c)
    target_link_libraries(guessthenumber_tests PRIVATE guessthenumber_core)

    set_target_properties(guessthenumber_tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build
    )

    add_test(NAME guessthenumber_tests COMMAND guessthenumber_tests)
endif()

include(GNUInstallDirs)
install(TARGETS guessthenumber RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

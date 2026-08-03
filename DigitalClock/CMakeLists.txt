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
endif()

# ----------------------------
# Include Directories
# ----------------------------
include_directories(
    ${PROJECT_SOURCE_DIR}/Inc
)

# ----------------------------
# Source Files
# ----------------------------
set(SOURCES
    Src/main.cpp
    Src/Clock.cpp
    Src/Date.cpp
    Src/Display.cpp
    Src/Theme.cpp
    Src/Config.cpp
    Src/Logger.cpp
    Src/Utils.cpp
)

# ----------------------------
# Executable
# ----------------------------
add_executable(
    DigitalClock
    ${SOURCES}
)

# ----------------------------
# Output Directory
# ----------------------------
set_target_properties(DigitalClock PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build
)
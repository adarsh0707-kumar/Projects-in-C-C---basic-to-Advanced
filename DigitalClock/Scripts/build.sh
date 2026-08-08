#!/usr/bin/env bash
#
# Builds the Digital Clock System.
#
# Usage:
#   ./Scripts/build.sh            Build with make (default)
#   ./Scripts/build.sh cmake      Build with CMake into build/
#   ./Scripts/build.sh debug      Build with debug symbols
#
set -euo pipefail

cd "$(dirname "$0")/.."

MODE="${1:-make}"

case "$MODE" in
    make)
        make -j"$(nproc 2>/dev/null || echo 2)"
        ;;
    debug)
        make debug
        ;;
    cmake)
        cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
        cmake --build build -j"$(nproc 2>/dev/null || echo 2)"
        ;;
    *)
        echo "Usage: $0 [make|debug|cmake]" >&2
        exit 1
        ;;
esac

echo "Build complete: Build/DigitalClock"

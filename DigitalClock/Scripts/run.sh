#!/usr/bin/env bash
#
# Builds if necessary, then runs the Digital Clock System.
#
# Any arguments are forwarded to the executable, for example:
#   ./Scripts/run.sh --config Config/config.ini
#
set -euo pipefail

cd "$(dirname "$0")/.."

if [ ! -x Build/DigitalClock ]; then
    echo "Executable not found; building first."
    make -j"$(nproc 2>/dev/null || echo 2)"
fi

# Run from the project root so Resources/ and Config/ resolve.
exec ./Build/DigitalClock "$@"

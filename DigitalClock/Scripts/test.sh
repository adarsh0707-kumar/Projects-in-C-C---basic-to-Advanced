#!/usr/bin/env bash
#
# Builds and runs the test suite.
#
# An optional argument filters by test identifier:
#   ./Scripts/test.sh TC-01
#
set -euo pipefail

cd "$(dirname "$0")/.."

make -j"$(nproc 2>/dev/null || echo 2)" Build/DigitalClockTests

# Run from the project root so the suite can find Resources/ and Config/.
exec ./Build/DigitalClockTests "$@"

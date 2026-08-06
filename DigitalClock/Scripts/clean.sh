#!/usr/bin/env bash
#
# Removes every build artefact, including the CMake tree.
#
set -euo pipefail

cd "$(dirname "$0")/.."

make distclean

echo "Clean complete."

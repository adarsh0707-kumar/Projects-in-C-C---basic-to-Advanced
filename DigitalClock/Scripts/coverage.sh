#!/usr/bin/env bash
#
# Reports line coverage for the Digital Clock sources.
#
# Expects an instrumented build to have been made and the test suite to have
# been run already; `make coverage` does both. Uses gcov, which ships with
# GCC, so there is nothing extra to install.
#
# Usage:
#   ./Scripts/coverage.sh            report only
#   ./Scripts/coverage.sh 80         report, and fail if total is below 80%
#
set -uo pipefail

cd "$(dirname "$0")/.."

THRESHOLD="${1:-0}"
OBJDIR="Obj"

if [ ! -d "$OBJDIR" ]; then
    echo "No $OBJDIR directory. Run 'make coverage' rather than this script." >&2
    exit 1
fi

if ! ls "$OBJDIR"/*.gcda > /dev/null 2>&1; then
    echo "No coverage data in $OBJDIR. Was the suite built with COVERAGE=1 and run?" >&2
    exit 1
fi

mkdir -p coverage
rm -f coverage/*.gcov

# gcov writes its .gcov files into the working directory, so run it from the
# coverage directory to keep the source tree clean.
(
    cd coverage
    for source in ../Src/*.cpp; do
        gcov -o "../$OBJDIR" "$source" 2>/dev/null
    done
) > coverage/gcov.log 2>&1

# gcov prints, per file:
#   File 'Src/Clock.cpp'
#   Lines executed:95.24% of 42
# Pair those two lines up and weight each file by its line count, so the
# total is real coverage rather than an average of percentages.
awk '
    /^File .*Src\// {
        file = $0
        sub(/^File .*Src\//, "", file)
        sub(/.$/, "", file)
        next
    }
    /^Lines executed:/ && file != "" {
        split($0, parts, ":")
        split(parts[2], values, "% of ")
        percent = values[1] + 0
        lines   = values[2] + 0
        covered = percent * lines / 100.0

        totalLines   += lines
        totalCovered += covered

        printf "%8.2f  %6d  %s\n", percent, lines, file
        file = ""
    }
    END {
        if (totalLines == 0) {
            print "NO_DATA"
            exit
        }
        printf "TOTAL %.2f %d %d\n", \
               (totalCovered * 100.0 / totalLines), totalCovered, totalLines
    }
' coverage/gcov.log | sort -n > coverage/summary.txt

if grep -q '^NO_DATA' coverage/summary.txt; then
    echo "gcov produced no usable data." >&2
    exit 1
fi

TOTAL_LINE=$(grep '^TOTAL' coverage/summary.txt || true)
TOTAL_PCT=$(echo "$TOTAL_LINE" | awk '{print $2}')
TOTAL_COV=$(echo "$TOTAL_LINE" | awk '{print $3}')
TOTAL_ALL=$(echo "$TOTAL_LINE" | awk '{print $4}')

echo
echo "============================================================"
echo "Line coverage - Digital Clock System"
echo "============================================================"
printf "%8s  %6s  %s\n" "COVER" "LINES" "FILE"
echo "------------------------------------------------------------"

# Worst-covered first: the interesting end of the list is the bottom.
grep -v '^TOTAL' coverage/summary.txt

echo "------------------------------------------------------------"
printf "%8s  %6s  %s\n" "$TOTAL_PCT%" "$TOTAL_ALL" "TOTAL ($TOTAL_COV covered)"
echo "============================================================"

if [ "$THRESHOLD" != "0" ]; then
    if awk -v t="$TOTAL_PCT" -v m="$THRESHOLD" 'BEGIN { exit (t < m) ? 0 : 1 }'; then
        echo
        echo "FAIL: total coverage ${TOTAL_PCT}% is below the ${THRESHOLD}% threshold." >&2
        exit 1
    fi

    echo
    echo "Total coverage ${TOTAL_PCT}% meets the ${THRESHOLD}% threshold."
fi

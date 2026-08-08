#!/usr/bin/env bash
#
# Reports line coverage for the Calculator sources.
#
# Expects an instrumented build to have been made and the test suite run
# already; `make coverage` does both. Uses gcov, which ships with GCC, so
# there is nothing extra to install.
#
# Usage:
#   ./Scripts/coverage.sh            report only
#   ./Scripts/coverage.sh 70         report, and fail below 70%
#
set -uo pipefail

cd "$(dirname "$0")/.."

THRESHOLD="${1:-0}"

# The library objects and the test objects are built into separate
# directories, and both carry coverage data: the library's is the part that
# matters, but gcov needs to be pointed at whichever directory holds the
# .gcda for the source it is asked about.
OBJDIRS="Obj/coverage Obj/coverage-tests"

FOUND=0

for dir in $OBJDIRS; do
    if ls "$dir"/*.gcda > /dev/null 2>&1; then
        FOUND=1
    fi
done

if [ "$FOUND" -eq 0 ]; then
    echo "No coverage data. Run 'make coverage' rather than this script." >&2
    exit 1
fi

mkdir -p coverage
rm -f coverage/*.gcov

# gcov writes its .gcov files into the working directory, so run it from the
# coverage directory to keep the source tree clean.
#
# main.c is excluded: it is the interactive REPL, driven by a terminal the
# suite does not have, and including it would report the shell loop as dead
# code rather than as code the tests deliberately do not reach.
(
    cd coverage
    for source in ../Src/*.c ../Src/*.cpp; do
        [ -e "$source" ] || continue

        case "$(basename "$source")" in
            main.c) continue ;;
        esac

        for dir in $OBJDIRS; do
            gcov -o "../$dir" "$source" 2>/dev/null
        done
    done
) > coverage/gcov.log 2>&1

# gcov prints, per file:
#   File 'Src/parser.c'
#   Lines executed:95.24% of 42
# Pair those up and weight each file by its line count, so the total is real
# coverage rather than an average of percentages.
#
# A file can appear more than once when both object directories are probed;
# the highest figure for each is kept, since that is the run where the file
# was actually exercised.
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

        if (!(file in bestPercent) || percent > bestPercent[file]) {
            bestPercent[file] = percent
            bestLines[file]   = lines
        }

        file = ""
    }
    END {
        for (f in bestPercent) {
            lines   = bestLines[f]
            percent = bestPercent[f]
            covered = percent * lines / 100.0

            totalLines   += lines
            totalCovered += covered

            printf "%8.2f  %6d  %s\n", percent, lines, f
        }

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
echo "Line coverage - Calculator"
echo "============================================================"
printf "%8s  %6s  %s\n" "COVER" "LINES" "FILE"
echo "------------------------------------------------------------"

# Worst-covered first: the interesting end of the list is the bottom.
grep -v '^TOTAL' coverage/summary.txt

echo "------------------------------------------------------------"
printf "%8s  %6s  %s\n" "$TOTAL_PCT%" "$TOTAL_ALL" "TOTAL ($TOTAL_COV covered)"
echo "============================================================"
echo
echo "Excludes Src/main.c, the interactive loop, which the suite does not"
echo "drive and which would otherwise be reported as untested code."

if [ "$THRESHOLD" != "0" ]; then
    if awk -v t="$TOTAL_PCT" -v m="$THRESHOLD" 'BEGIN { exit (t < m) ? 0 : 1 }'; then
        echo
        echo "FAIL: total coverage ${TOTAL_PCT}% is below the ${THRESHOLD}% threshold." >&2
        exit 1
    fi

    echo
    echo "Total coverage ${TOTAL_PCT}% meets the ${THRESHOLD}% threshold."
fi

# Test Plan

## Scope

This document describes the actual automated test suite under
`Tests/`, built and run via `make test`. It intentionally covers only
what's really there — see [Coverage Gaps](#coverage-gaps) for what
isn't tested yet.

## How it Runs

`Tests/test_main.c` supplies the suite's single `main()`. The Makefile
links it against the real `Src/*.c` module files directly (not through
a library) so tests exercise production code, not a reimplementation
of it — see the Makefile's "Phase 20 — Unit tests" section.

```bash
make test                       # debug build, run once
make asan && make BUILD=asan test   # same suite under ASan + UBSan
```

A shared pass/fail counter (`test_framework.h`) prints a running total;
the run exits non-zero if anything failed. As of this writing: **460
tests, all passing**, clean under AddressSanitizer and
UndefinedBehaviorSanitizer.

## Test Framework

`Tests/test_framework.h` provides:

- `ASSERT_TRUE(cond, msg)` / `ASSERT_INT_EQ` / `ASSERT_DOUBLE_EQ` (1e-6
  tolerance) / `ASSERT_STR_EQ` — increment the shared counters and
  print a `FAIL [func:line] msg` line on failure; a failure never
  aborts the run, so one bad assertion doesn't hide the rest.
- `ASSERT_EXITS_NONZERO(stmt, msg)` — forks a child process to run
  `stmt` and asserts the child terminated with a non-zero exit status.
  This was needed while the engine still had `exit()` call sites (see
  `docs/RULES.md`'s "never call exit() from library modules" rule) so
  the suite could confirm a failure path really did fail loudly,
  without taking the whole test binary down with it. As of the
  2026-08-04 fix to `evaluate.c`'s `applyOperation()` (the last
  remaining `exit()` sites in `Src/`), there are zero `exit()` calls
  left in the engine, so **no current test uses this macro** — see
  `Tests/test_functions.c` and `Tests/test_stack.c` for comments on
  the checks it used to guard. Kept in `test_framework.h` in case a
  future intentional `exit()` path needs it again.

## Coverage by Module

| Test file             | Exercises                          | What's covered |
| ---------------------- | ---------------------------------- | --------------- |
| `test_error.c`          | `error.c`                          | Set/get/clear last-error state, and every `CalculatorError` value maps to its expected string |
| `test_stack.c`          | `stack.c`                          | Push/pop/peek ordering and overflow/underflow status codes for `DoubleStack`, `CharStack`, `StringStack`, `TokenStack` |
| `test_tokenizer.c`      | `tokenizer.c`                      | `readNumber()` on integers, decimals, negatives, and a whole-string number |
| `test_validator.c`      | `validator.c`                      | `validateExpression()` accepts well-formed input and rejects malformed input (trailing operator, mismatched parens, wrong arg count, ...), and writes the exact diagnostic into the caller's buffer — including an empty buffer on success and a null-terminated result when that buffer is too small |
| `test_parser.c`         | `parser.c`                         | Implicit-multiplication insertion (`2(3)`, `3pi`, ...); parenthesis balance validation, including the distinct message for each failure mode (unclosed `(`, surplus `)`, nesting past the `CharStack`'s capacity) |
| `test_postfix.c`        | `postfix.c`                        | Infix→postfix conversion, `isOperator()`/`precedence()` helpers, end-to-end evaluation via `evaluatePostfix()`, and postfix-conversion error paths (undefined variable, malformed number) |
| `test_functions.c`      | `functions.c`, `angle_mode.c`      | Unary/binary function dispatch, trig respecting the current angle mode, factorial, and error paths for out-of-domain/unknown input |
| `test_variables.c`      | `variables.c`                      | Built-in constants (`pi`, `e`), their read-only enforcement, user variable set/get, undefined-variable lookup, enumeration by index (added for the GUI variable manager), and `ans` |
| `test_history.c`        | `history.c`                        | Adding an entry and recalling it (`!!`), recall by number (`!N`), entry count and full-line lookup by number (added for the GUI history panel), and clearing history |
| `test_memory.c`         | `memory.c`                         | MS/MR/M+/M-/MC |
| `test_units.c`          | `units.c`                          | Parsing `<value><unit>`, unit-category lookup, bare and `to <unit>` conversion forms, `evaluateUnitExpression()` (added for the GUI's Units tab, since the print-only conversion functions can't be used by a GUI), and category-mismatch/unknown-unit error paths |
| `test_stats.c`          | `stats.c`                          | `mean`/`median`/`sum`/`min`/`max`/`count`, sample `stddev`/`variance` (n-1) vs. population `pstddev`/`pvariance` (n), and error paths (unparseable argument list, too few values, unknown function) |
| `test_base.c`           | `base.c`                           | `bin`/`hex`/`oct` (decimal → base, including negative sign-magnitude), `dec` (base → decimal via a b/o/h suffix), and error paths (bad digit, missing/unknown suffix) |
| `test_plot.c`           | `plot.c`                           | Successful plotting, the `plot(1/x)` asymptote-doesn't-crash regression by name, syntax-error paths (`extractPlotArgument()`), and the undefined-variable path that intentionally leaves `errorMsg` empty |
| `test_complex.c`        | `Complex.cpp`, `complex_eval.cpp`  | Arithmetic (`+ - * /`), `sqrt()` of a negative real, output formatting (purely real, purely imaginary, signed imaginary part), and parser error paths |
| `test_matrix.c`         | `Matrix.cpp`, `matrix_eval.cpp`    | Addition/subtraction/scalar and matrix multiplication, `det`/`transpose`/`inverse`, and error paths (dimension mismatch, non-square, singular, ragged rows) |

Closing this gap surfaced one dead function worth noting:
`parseValueWithUnit()` (`units.h`) is fully implemented and tested,
but nothing in the actual CLI pipeline calls it — `main.c`'s unit
converter only ever calls `parseConversion()`. Not removed as part of
this pass since it's plausible future-GUI code wants a
single-value-no-"to"-clause parser, but worth knowing it's currently
unreferenced production code.

## Coverage Gaps

Being direct about what isn't under test, so this section doesn't
silently go stale the way the rest of this file did before today:

- **No dedicated tests for `evaluate.c`'s `applyOperation()`** beyond
  what `test_postfix.c` exercises indirectly through
  `evaluatePostfix()`. Division-by-zero, modulus on non-integers, and
  the unknown-operator path aren't asserted directly.
- **`main.c` is entirely untested** by design — `test_main.c` replaces
  it as the test binary's entry point, so its CLI-glue logic (menu
  dispatch, buffer handling, history-shortcut parsing) is only
  covered by manual testing.
- **No fuzzing or property-based testing.** The three stack-buffer
  overflows found and fixed during the 2026-08-04 security pass
  (`main.c`'s `infix`/`variableName` handling, `tokenizer.c`'s
  `readNumber()`) were all found by manually constructing long input,
  not by the test suite — none of the hand-written unit tests happened
  to try a 100-character variable name or a 100-digit number literal.
  A fuzz target over `infixToPostfix()`/`evaluatePostfix()` would have
  caught all three automatically and is a reasonable next investment
  before Phase 28+ work begins.

## Adding a Test

1. Pick (or create) `Tests/test_<module>.c`.
2. Add `void run_<module>_tests(void)` that calls your `static void
   test_*(void)` functions.
3. Declare `run_<module>_tests(void)` in `Tests/tests.h` and call it
   from `Tests/test_main.c`.
4. Add the file to `TEST_SRCS` in the Makefile.
5. `make test` — the total test count updates automatically from the
   shared counters, nothing else to keep in sync by hand.

Testing a C++ module (`Complex.cpp`/`Matrix.cpp` and friends) works
the same way, but the test file itself stays plain C: call the
module's `extern "C"` entry point (`complex_eval.h`/`matrix_eval.h`),
not the C++ classes directly. This is exactly what `test_complex.c`
and `test_matrix.c` do — see them for the pattern. The Makefile
already builds `$(CXX_SRCS)` into the test binary
(`TESTED_CXX_OBJS`) and links `$(TEST_TARGET)` with `g++`, so no
Makefile changes are needed to add more C++-module tests.

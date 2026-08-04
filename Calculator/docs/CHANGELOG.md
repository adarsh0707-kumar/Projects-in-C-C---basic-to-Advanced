# Changelog

All notable changes to this project are documented in this file.

The format is based on **Keep a Changelog** and follows **Semantic Versioning** principles where possible.

---

# [Unreleased]

## 2026-08-04 (3) — Phase 31 (GUI) Step 1: Qt6 arithmetic keypad

### Added

- `Gui/MainWindow.hpp`, `Gui/MainWindow.cpp`, `Gui/main.cpp` — a Qt6
  Widgets window with a `QLineEdit` display and a button-grid keypad
  (digits, `+ - * / ^ % ( ) . !`, `=`, clear, backspace). Calls the
  existing C engine (`infixToPostfix`, `evaluatePostfix`,
  `validateExpression`, `validateParentheses`,
  `insertImplicitMultiplication`, `setVariable`/`setAns`,
  `addHistory`) directly through an `extern "C"` include block in
  `MainWindow.cpp`, rather than duplicating any parsing/evaluation
  logic or touching the ~10 existing C headers that don't have an
  `extern "C"` guard.
- Makefile: `make gui` / `make run-gui` targets (Qt6, via
  `pkg-config Qt6Widgets`), reusing the same engine objects the CLI
  binary links (`ENGINE_OBJS_FOR_GUI`, mirroring the test target's
  existing `TESTED_OBJS` filter-out-main.o pattern). Opt-in and
  Qt-only — every other target (`all`, `test`, `release`, `asan`,
  ...) is unaffected whether or not Qt is installed.
- Two build-environment issues hit and fixed along the way, both
  worth knowing about for anyone extending the GUI: (1) this
  machine's default `moc` on `PATH` is Qt5's even though
  `Qt6Widgets` is what's actually being built against — mismatched
  moc output fails to compile against the newer headers, so the
  Makefile now resolves Qt6's specific `moc` via
  `qmake6 -query QT_HOST_LIBEXECS`. (2) linking a QObject subclass's
  translation unit without `-fPIC` fails at link time ("copy
  relocation against non-copyable protected symbol ... QObject") —
  Qt's own qmake/CMake tooling always compiles against Qt with
  `-fPIC` for this reason, so the GUI's compile rules now do too
  (`moc` itself doesn't take the flag; it's compile-only).

### Verified

Manually, via a real X11 session (`xdotool` + `import`, screenshots
inspected): `2+3*5` -> 17, `sqrt(16)` -> 4, `x=5` then `x+3` -> 8
(assignment + recall), `2^10` -> 1024, `5/0` -> shows the error and
the window stays open, all via both typed keyboard input and actual
button clicks. `make test`/`make BUILD=asan test` (433 cases) and
`./calculator` (CLI) confirmed unaffected — nothing in `Src/` changed.

### Known limitation (not fixed this step)

`validateExpression()`/`validateParentheses()` report their specific
failure reason via `printf()` to stdout, which is fine for the CLI
but invisible to a GUI window — the GUI currently shows a generic
"Invalid expression."/"Mismatched parentheses." instead of the
specific reason `validator.c` actually detected. Every other engine
error path already returns its message in a buffer; these two are the
exceptions. Flagged in `docs/ROADMAP.md`'s Phase 31 section as a
candidate for Step 7 (polish) rather than fixed now, since it touches
a stable, already-tested module (`validator.c`) for GUI-only benefit.

## 2026-08-04 (2) — Close the test-coverage gaps flagged in TEST_PLAN.md

### Added

- `Tests/test_units.c`, `test_stats.c`, `test_base.c`, `test_plot.c`
  covering `units.c`, `stats.c`, `base.c`, `plot.c` — previously zero
  automated coverage, only exercised manually through the CLI.
- `Tests/test_complex.c`, `test_matrix.c` covering the C++ engine
  (`Complex.cpp`/`complex_eval.cpp`, `Matrix.cpp`/`matrix_eval.cpp`)
  via their `extern "C"` entry points, so the test files themselves
  stay plain C. The Makefile's test target now also builds
  `$(CXX_SRCS)` into the test binary (`TESTED_CXX_OBJS`) and links
  with `g++` instead of `gcc`, since linking C++ objects (STL,
  exceptions) needs libstdc++ — same reasoning as the main
  `$(TARGET)` link rule. Test suite: 271 -> 433 cases.
- `plot(1/x)`'s asymptote-doesn't-crash behavior — the exact
  regression this project's docs call out by name — now has a
  dedicated test asserting the overall call succeeds despite the
  x=0 sample, not just manual verification.

### Found while writing tests (no code changes)

- `parseValueWithUnit()` (`units.h`) is fully implemented but never
  called anywhere in the actual CLI pipeline (`main.c`'s unit
  converter only calls `parseConversion()`). Left as-is — plausible
  future-GUI use — but now flagged in `docs/TEST_PLAN.md` rather than
  silently unreferenced.

## 2026-08-04 — Repository repair, security fixes, and documentation sync

### Fixed

- **Unresolved git conflict markers committed straight into HEAD**
  across `Inc/calculator.h`, `Inc/functions.h`, `Src/evaluate.c`,
  `Src/postfix.c` (~25 separate blocks), `Src/plot.c`,
  `Tests/test_postfix.c`, `Readme.md`, and this file — the project did
  not compile at all beforehand. Resolved in favor of the
  error-handling-migration code (status codes instead of `exit()`),
  and fixed two declarations that were missing from headers entirely
  (`getLastEvalError()`, `isOperator()`) as a result.
- **Three stack buffer overflows**, all reachable through the normal
  CLI with nothing more exotic than long input, confirmed with
  AddressSanitizer:
  - `Src/main.c`: `infix`/`postfix` were `char[256]` but fed from a
    `char[512]` `processed` buffer after implicit-multiplication
    expansion (triggered by e.g. 100 repetitions of `"2("`). Grown to
    512/1024 bytes.
  - `Src/main.c`: the variable-assignment path copied everything
    before `=` into `variableName[32]` with no length check
    (triggered by a ~100-character variable name). Added the check.
  - `Src/tokenizer.c`: `readNumber()` copied a digit run into
    `temp[64]` with no length check, unlike every sibling parser in
    the codebase (triggered by a 100-digit literal). Added the check.
- **`applyOperation()` (`Src/evaluate.c`) still called `exit()`** on
  division by zero, modulus, and unknown operators — the last
  remaining `exit()` call sites in `Src/`, contradicting this file's
  own record of "zero `exit()` calls" and specifically reintroducing
  the `plot(1/x)`-crashes-the-session bug this file already recorded
  as fixed. Now returns `NAN` + `calculatorSetLastError()` like the
  rest of the engine. Modulus (`%`) was previously a stub that always
  errored regardless of its operands ("supported only for integers"
  was never actually implemented); implemented properly as part of
  this fix (integer-only, via `fmod`).
- **`Tests/test_error.c` never compiled and was never wired into the
  build** — it referenced `CALC_ERR_MEMORY` (not a real
  `CalculatorError` value) and `ASSERT_EQ` (not a real macro; the
  framework provides `ASSERT_INT_EQ`/`ASSERT_STR_EQ`). Rewritten
  against the real API and added to the Makefile's `TEST_SRCS`,
  closing `error.c`'s previous zero-test-coverage gap. Test suite:
  257 -> 271 cases.

### Documentation

Synced `docs/` to the current codebase after finding it had drifted
significantly — some of it never updated since roughly the core-engine
milestone, well before angle modes, unit/base conversion, statistics,
complex numbers, matrices, and plotting were built:

- `docs/PHASES.md`: added Phases 21–27 for the above features (all
  were built but never recorded as their own phase); renumbered the
  genuinely-still-future phases to 28–36 to make room, with no change
  to their content.
- `docs/ROADMAP.md`: was three full duplicate roadmaps stacked
  together, each with a *different* phase-numbering scheme, none
  matching `docs/PHASES.md` or each other. Replaced with one
  status-summary document that defers phase detail to `PHASES.md`
  instead of duplicating it — the duplication is what caused the
  three-copies problem in the first place.
- `docs/TEST_PLAN.md`: contained `RULES.md`'s content (truncated),
  not a test plan, since the only commit that ever touched it.
  Written fresh from the actual `Tests/` suite, including an honest
  Coverage Gaps section.
- `docs/PRD.md`: trimmed a ~46-line duplicate fragment tacked on after
  the real conclusion.
- This file: removed three duplicate copies of the entire changelog
  that had accumulated below the real one; fixed a couple of
  truncated sentences in the "Development Statistics" section.
- `docs/RULES.md`: "Libraries not allowed: STL" contradicted
  `Src/Matrix.cpp`/`Src/complex_eval.cpp`, which already use
  `std::vector`/`std::string` deliberately. Scoped the rule to the C
  core engine instead of banning STL outright.
- `docs/API.md`: fixed a stale `void infixToPostfix()` signature
  (it's been `int`-returning since the error-handling migration);
  added the six modules that had no entry at all
  (`error.h`, `angle_mode.h`, `units.h`, `stats.h`, `base.h`,
  `plot.h`, plus the C++ complex/matrix modules); rewrote the "Error
  Handling" and "Future API Extensions" sections, which both still
  described the pre-migration `exit()`-based behavior and listed
  shipped features as unbuilt.
- `docs/ARCHITECTURE.md`, `docs/DESIGN.md`, `docs/MEMORY.md`,
  `docs/CONTRIBUTING.md`: fixed the same "lists a shipped feature as
  future work" pattern (angle modes/units/stats/base/complex/matrix/
  plotting) and the same stale `exit(EXIT_FAILURE)` error-handling
  description, wherever it appeared.
- `docs/ERROR_HANDLING.md`: was an empty placeholder, never linked
  from the README's documentation table. Written and linked.

## Added (continued, part 2)

- **The engine now has zero `exit()` calls anywhere in `Src/`.**
  Completed the error-handling migration:
  - `functions.c`: `factorial()`, `applyBinaryFunction()`'s
    unknown-function path, and `applyFunction()`'s unknown-function
    path (previously a *silent* bug -- an unrecognized name returned
    `0` instead of erroring at all) all now return `NAN` +
    `calculatorSetLastError()` instead of exiting or, in
    `applyFunction()`'s case, silently returning a wrong answer.
  - `evaluate.c`: the three remaining "should never happen" internal
    checks (identifier too long, unknown function, unsupported
    argument count while evaluating already-generated postfix) and
    `tokenizer.c`'s `readNumber()` (malformed number while evaluating
    already-generated postfix) are now graceful too. All four are
    unreachable through the normal CLI pipeline today -- the earlier
    `infixToPostfix()`/registry checks already prevent malformed
    postfix from being generated in the first place -- so this is
    pure defense-in-depth for any future caller.
  - Simplified `evaluate.c`'s factorial dispatch (the `!` operator and
    `"fact"` function): since `factorial()` now fails gracefully on
    its own, the pre-validation duplicate-logic added during the
    earlier `plot()`-crash fix is no longer needed and was removed.
  - `Tests/test_functions.c`'s error-path tests no longer need
    `ASSERT_EXITS_NONZERO` (fork + check exit status) since nothing
    left in this codebase exits; they assert directly on the
    returned `NAN` + recorded `CalculatorError` instead.
  - Test suite: 252 -> 257 cases.

## Added (continued)

- `infixToPostfix()` now returns `1`/`0` instead of `void`. Its
  remaining `exit()` paths (undefined variable, malformed number,
  identifier too long, mismatched parentheses, misplaced comma, an
  expression too complex to fit the operator stack) all now report
  failure gracefully instead of terminating the process, completing
  the follow-up flagged in the previous entry. `main.c`'s two call
  sites and `plot.c`'s per-sample loop all check the new return
  value. `plot(y)` (an undefined variable) previously crashed the
  whole calculator; it now reports "Undefined variable 'y'" once and
  returns to the menu. `Tests/test_postfix.c` gained coverage for
  both the undefined-variable and malformed-number failure paths.
  Test suite: 243 -> 252 cases.

## Added

- `Inc/error.h` / `Src/error.c`: a centralized `CalculatorError` enum
  and last-error accessor (`calculatorSetLastError()` /
  `calculatorGetLastError()` / `calculatorErrorString()` /
  `calculatorClearError()`), per docs/RULES.md's own "never call
  exit() from library modules" guidance. Being adopted incrementally,
  module by module, rather than all at once.

## Changed

- **`stack.c` migrated off `exit()`.** `pushDouble`/`popDouble`/
  `peekDouble`, `pushChar`/`popChar`/`peekChar`,
  `pushString`/`popString`/`peekString`, and
  `pushToken`/`popToken`/`peekToken` all now return `1`/`0` (success/
  failure, via an out-parameter for pop/peek) and record *why*
  through the new error module, instead of printing a message and
  terminating the process on overflow/underflow. This is the module
  most directly exposed to adversarial or simply very large input
  (e.g. a pathologically deep expression), so it's the first one
  migrated. `evaluate.c` now integrates stack failures into its
  existing NAN-based graceful-error propagation (see the earlier
  `plot()`-crash fix); `parser.c`'s `validateParentheses()` treats a
  stack failure as "invalid expression" instead of exiting.
  `postfix.c`'s `TokenStack` usage is wired to the new API via local
  wrapper functions that intentionally preserve its *existing*
  fail-loud behavior for now (unknown variables, malformed numbers,
  mismatched parentheses, etc. in `infixToPostfix()` are a separate,
  larger follow-up: they need `infixToPostfix()`'s own signature to
  change, plus updates to every caller).
- Note: in the currently-wired CLI pipeline, `validator.c`'s own
  64-deep parenthesis-nesting cap (`MAX_PAREN_DEPTH`) already caught
  pathologically nested input before it could reach `stack.c`'s
  100-element limit, so this change is primarily defense-in-depth for
  any future/direct caller of the stack API, not a newly-reachable
  CLI crash fix on top of the earlier `plot()`/modulus fixes.
- `Tests/test_stack.c` rewritten against the new status-code API;
  overflow/underflow are now asserted directly (checking the returned
  status and `calculatorGetLastError()`) instead of via
  `ASSERT_EXITS_NONZERO`, since those two paths no longer exit().
  Test suite grew from 114 to 243 cases in the process.

## Fixed

- `%` (modulus) operator was unconditionally erroring, even for valid
  integer operands (e.g. `10 % 3`). Now computes a real result via
  `fmod()` and only errors on non-integer or zero operands.
- A fatal math error anywhere (division by zero, invalid factorial
  input, etc.) used to call `exit()` and terminate the *entire*
  program, including from inside `plot()`'s per-sample evaluation
  loop, e.g. `plot(1/x)` or `plot(x!)` would crash the whole
  calculator instead of just skipping the invalid sample. Errors
  during expression evaluation now propagate as `NaN` + a retrievable
  message (`getLastEvalError()`) instead of terminating the process,
  while single-value functions that are meant to fail loudly on
  invalid direct input (e.g. `factorial()` called outside the
  evaluator) keep their existing behavior.
- Unknown-function detection in the postfix evaluator had a dead
  sentinel check (`functionArgumentCount()` returned `0` instead of
  `-1` for unrecognized names).

## Changed

- **Build system**: the Makefile now supports four build
  configurations (`BUILD=debug|release|asan|ubsan`), each with its
  own object directory so switching configurations never links stale
  objects. Header dependencies are tracked automatically
  (`-MMD -MP`) instead of via a manually maintained file list.
- Added a GitHub Actions CI workflow that builds and runs the full
  test suite on every push/PR, in both debug and release
  configurations, plus dedicated AddressSanitizer/UndefinedBehavior-
  Sanitizer jobs that exercise the calculator, complex, matrix, and
  plotting features.

---

## Overview

The project has evolved from a simple arithmetic calculator into a modular scientific calculator with support for variables, scientific functions, history, memory registers, parser improvements, implicit multiplication, documentation, and a scalable architecture.

Current completion status:

- Phase 20 Completed
- Stable Build
- Modular Architecture
- Documentation Introduced

---

## Added

### Scientific Calculator

- Scientific function engine
- Unary mathematical functions
- Binary mathematical functions
- Constants
- Variables
- Scientific expression evaluation

Supported unary functions

- sqrt()
- sin()
- cos()
- tan()
- asin()
- acos()
- log()
- ln()
- exp()
- abs()
- cbrt()

Supported binary functions

- pow()
- max()
- min()
- hypot()

---

### Variable System

Added variable storage system.

Supports

```text
x = 10
y = 20
radius = 15
```

Built-in variables

```text
ans
```

Features

- create variable
- update variable
- retrieve variable
- display variables

---

### Constants

Added predefined mathematical constants.

Supported

```text
pi
e
```

Examples

```text
pi
2*pi
sin(pi/2)
```

---

### Parser

Implemented parser responsible for expression preprocessing.

Features

- implicit multiplication
- parser cleanup
- expression normalization

Supported

```text
2(3+4)

2pi

3sqrt(16)

(pi+1)(3+4)

2(x+y)
```

Parser automatically converts them into

```text
2*(3+4)

2*pi

3*sqrt(16)

(pi+1)*(3+4)

2*(x+y)
```

---

### Postfix Conversion

Implemented complete Shunting Yard Algorithm.

Supports

- operator precedence
- parentheses
- unary minus
- function tokens
- variable tokens
- constants
- comma separators
- function stack

---

### Evaluator

Implemented postfix evaluator.

Supports

Arithmetic

```text
+
-
*
/
%
^
```

Scientific

```text
sqrt()
sin()
cos()
tan()
log()
ln()
pow()
max()
min()
hypot()
```

Variables

Constants

Functions

---

### History

Added persistent history.

Menu

```
View History

Clear History
```

History stored in

```
Build/history.txt
```

---

### Memory Register

Implemented calculator memory.

Supported

```
MS

MR

MC

M+

M-
```

---

### Documentation

Created complete project documentation.

Added

```
README.md

PRD.md

ARCHITECTURE.md

RULES.md

PHASES.md

DESIGN.md

MEMORY.md

ROADMAP.md

API.md

TEST_PLAN.md

CHANGELOG.md

CONTRIBUTING.md
```

---

## Improved

### Project Structure

Converted project into modular architecture.

Before

```
Few source files
```

After

```
Inc/

Src/

Obj/

Build/

Documentation
```

---

### Code Organization

Separated code into modules.

Created dedicated modules for

- stack
- parser
- tokenizer
- validator
- postfix
- evaluator
- variables
- functions
- function metadata
- constants
- history
- memory

---

### Expression Validation

Improved validator.

Now detects

- invalid operators
- invalid operands
- invalid numbers
- missing operands
- unmatched parentheses
- expression ending with operator
- misplaced commas

---

### Function Recognition

Improved tokenizer.

Now distinguishes between

```text
sqrt
```

and

```text
variable named sqrt
```

Supports function metadata.

---

### Error Messages

Added meaningful runtime errors.

Examples

```
Division by zero

Undefined variable

Unknown function

Stack overflow

Stack underflow

Invalid number

Invalid operator

Mismatched parentheses

Misplaced comma
```

---

### Build System

Improved Makefile.

Supports

```
make

make clean

make rebuild

make run
```

Added

```
Object directory

Include directory

Math library linking
```

---

## Fixed

### Parser

Fixed implicit multiplication bugs.

Examples

Before

```
3sqrt(16)

↓

3*sqrt*(16)
```

After

```
3sqrt(16)

↓

3*sqrt(16)
```

---

Fixed

```
2pi
```

Previously

```
2p*i
```

Now

```
2*pi
```

---

Fixed function detection.

Before

```
sqrt

↓

Undefined variable
```

Now

```
Recognized as function.
```

---

Fixed ans variable expansion.

Before

```
ans*5

↓

Operand expected
```

Now

Works correctly.

---

Fixed postfix conversion for

```
pow(2,8)

max(5,10)

hypot(3,4)
```

---

Fixed scientific constant parsing.

```
pi

e

sin(pi/2)
```

---

Fixed parser handling for

```
2(x)

2pi

2sqrt(16)

(pi+1)(4)
```

---

Fixed evaluation order for

```
^

Unary functions

Binary functions
```

---

Fixed stack underflow in malformed expressions.

---

Fixed Makefile object generation.

---

## Refactored

Major refactoring completed.

Improved

- readability
- modularity
- maintainability
- scalability

Removed duplicated logic.

Separated parser from evaluator.

Separated validator from tokenizer.

Introduced function metadata module.

Introduced token abstraction.

Introduced reusable stack implementations.

---

# [1.5.0]

## Added

Scientific calculator foundation.

- variables
- ans
- functions
- constants
- history
- memory

---

# [1.0.0]

Initial calculator release.

Features

- addition
- subtraction
- multiplication
- division
- postfix conversion
- postfix evaluation
- stack implementation
- parentheses

---

# Upcoming Releases

## Version 2.1

Planned

- Degree/Radian mode
- More scientific functions
- Better parser optimization

---

## Version 2.2

Planned

- Statistics engine
- Matrix operations
- Complex numbers
- Base conversion

---

## Version 3.0

Planned

- GTK GUI
- Graph plotting
- Dynamic memory optimization
- Plugin architecture
- Unit testing framework

---

# Development Statistics

Current Modules

- Stack
- Parser
- Validator
- Tokenizer
- Postfix Converter
- Evaluator
- Variables
- Constants
- Functions
- Function Metadata
- History
- Memory
- Main Application

Approximate Source Files

12+

Header Files

10+

Completed Phases

27 (see `docs/PHASES.md`)

Planned Phases

9 (Phases 28–36 — dynamic data structures, AST, symbolic math, GUI,
plugins, scripting, packaging, cross-platform, v2.0)

Current Stability

★★★★★ — 271 unit tests passing, clean under AddressSanitizer/UBSan

Overall Progress

The CLI scientific calculator (v1.0) is feature-complete: expression
engine, scientific functions, variables/memory/history, statistics,
unit and base conversion, complex numbers, matrices, and ASCII graph
plotting. Future work (Phase 31+) is a GUI front end and beyond,
rather than rebuilding the existing core engine.

Version 0.1

Initial Stack

Version 0.2

Parser

Version 0.3

Variables

Version 0.4

Scientific Functions

Version 0.5

History

Version 1.0

Scientific Calculator Complete (CLI)


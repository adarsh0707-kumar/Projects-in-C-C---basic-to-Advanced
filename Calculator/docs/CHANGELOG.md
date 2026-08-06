# Changelog

All notable changes to this project are documented in this file.

The format is based on **Keep a Changelog** and follows **Semantic Versioning** principles where possible.

---

# [Unreleased]

## 2026-08-05 (7) — Phase 35 (cross-platform), part 1: portability groundwork

Goal: someone downloads a release from GitHub and runs it on Linux,
macOS, or Windows without building anything. This entry is the code and
build-system half; CI, release archives, and installers follow.

### Fixed

- `Src/history.c`: the history file was the literal relative path
  `"./Build/history.txt"`, with nothing creating that directory. Any
  user running the binary from anywhere other than the project root —
  which is *every* user of a downloaded release, on every OS including
  Linux — silently lost all history: `fopen()` returned NULL and the
  read paths just reported "No history available!". The path is now
  resolved at runtime by `historyFilePath()` to the per-user data
  directory each platform expects (`%APPDATA%\Calculator` on Windows,
  `~/Library/Application Support/Calculator` on macOS,
  `$XDG_DATA_HOME`/`~/.local/share/calculator` elsewhere), creating it
  `mkdir -p` style. Verified by running the binary from `/tmp` with no
  `Build/` anywhere: history now works and lands in
  `~/.local/share/calculator/history.txt`.

### Added

- `CMakeLists.txt`: a second build system covering Windows/MSVC,
  macOS/Xcode, Ninja, and Make. The existing Makefile is untouched and
  remains the Linux development build — it carries the sanitizer
  configs and `BUILD=<type>` layout — but it is POSIX-shell-only
  (`mkdir -p`, `command -v`, `pkg-config`, shell loops), so it can
  never serve Windows. Source lists are written out explicitly rather
  than globbed, matching the Makefile's reasoning. The GUI target is
  conditional on `find_package(Qt6)`: Qt missing is not an error, since
  the CLI and tests have no external dependencies and must build
  everywhere. `AUTOMOC`/`AUTORCC` replace the hand-written moc/rcc
  rules and pick tooling matching the Qt6 that was found, which is the
  same Qt5/Qt6 mismatch the Makefile solves via `qmake6 -query`.
- `Inc/history.h`: `historyFilePath()` and `setHistoryFilePath()` are
  now public. The setter exists because the test suite needs a scratch
  file and `setenv()` is not ISO C — it's unavailable under a strict
  `-std=c11`, so an environment variable alone could not be driven
  portably from a test. `$CALCULATOR_HISTORY_FILE` is still honoured
  for users, since `getenv()` *is* ISO C.

### Changed

- `Tests/test_framework.h`: `ASSERT_EXITS_NONZERO` used `fork()`,
  `waitpid()`, `<unistd.h>` and `<sys/wait.h>` unconditionally, so the
  suite could not compile on Windows at all. Those are now behind
  `#if !defined(_WIN32)`, with a Windows branch that reports each such
  case as **SKIP** and counts it in a new `testsSkipped` total printed
  by `Tests/test_main.c`, so a Windows summary can never claim coverage
  it didn't have. In practice the macro currently has **no call sites**
  — the cases that once used it migrated to returned status codes — so
  all three platforms run the same 471 tests and `testsSkipped` stays
  0. The guard was still required: those POSIX includes broke the MSVC
  compile regardless of whether the macro was used.
- `Tests/test_history.c`: replaced the backup-and-restore dance against
  the user's real history file with `setHistoryFilePath()` pointing at
  a scratch file. The old approach duplicated a path constant that has
  now moved, and lost the user's history outright if the suite crashed
  between backup and restore. The suite no longer opens the real
  history file at all.

### Verified

`cmake -B build && cmake --build build` produces all three targets
(CLI, tests, GUI) with **zero warnings**, `ctest` passes, and the
resulting `calculator-tests` reports **471 passed** — the same count
the Makefile produces, which still builds and passes unchanged
alongside it. `cmake --install` stages a correct release layout. The
CMake-built GUI launches under `QT_QPA_PLATFORM=offscreen`. The
`_WIN32` branch of `test_framework.h` was syntax-checked by compiling
it on Linux with `_WIN32` forced; that proves the branch is well-formed
C but is **not** a substitute for a real Windows build, which arrives
with the CI matrix.

### Still to do for Phase 35

CI matrix (`ubuntu`/`macos`/`windows`), a tag-triggered release
workflow, release archives, and the non-developer installers
(AppImage, `.dmg`, Windows installer). Nothing here has been built or
tested on macOS or Windows yet — only made capable of it.

## 2026-08-05 (6) — Drop the redundant validateParentheses() call from the pipeline

### Changed

- `Src/main.c`, `Src/plot.c`, `Gui/MainWindow.cpp`: every one of the
  six `validateExpression()` → `validateParentheses()` pairs is now a
  single `validateExpression()` call. The second check could never
  fire: `validateExpression()` tracks `(` / `)` with the same rules —
  it has to, to know where a function's argument list ends — and
  rejects every unbalanced expression on its own, so the six error
  branches behind it were unreachable code (including, awkwardly, the
  three specific messages added in the previous entry).
- `Inc/calculator.h`, `docs/API.md`: `validateParentheses()` is kept
  and still tested — it's a reasonable standalone check for a caller
  that wants parens verified *without* a full syntax check — but is
  now documented as not being part of the evaluation pipeline, so the
  calls don't get "restored" later as a fix for a bug they can't
  prevent.

### Verified

Rather than relying on reading the state machine, this was checked by
fuzzing the two functions against each other over **8M random inputs**
across two alphabets (one general, one parenthesis-dense), of which
~421k passed `validateExpression()`: **zero** were rejected by
`validateParentheses()`. Both harnesses ran under
AddressSanitizer/UBSan with no findings. The CLI produces byte-identical
output before and after for `sin(2,3)`, `(()`, `)(`, `2+3*4`,
`sqrt(16)`, and `plot(sin(x))`. 471 tests pass in both the normal and
`BUILD=asan` builds; `make all`/`gui`/`test` clean under `-Wall
-Wextra`.

### Known issue found while verifying this (not fixed here)

`infixToPostfix()` is lenient about an unclosed `(` in a way its own
`Inc/calculator.h` doc contradicts: that doc says it returns 0 on
"mismatched parentheses", and it does for a surplus `)` (`2+3)`, `)(`),
but for an unclosed `(` it returns **1** and silently drops it —
`(2+3` converts to `2 3 +`, `sin(2` to `2 sin`, and `(()` to an empty
postfix string. No memory-safety issue (confirmed under ASan/UBSan);
`validateExpression()` rejects all of these long before they reach it,
so no user-visible behavior depends on this today. Worth fixing on its
own merits, since the contract is documented and a direct caller of
`infixToPostfix()` would be misled.

## 2026-08-05 (5) — Phase 31 (GUI) Step 7 complete: application icon

### Added

- `Gui/icon.svg`: the icon source, drawn entirely from plain rectangles
  in the same palette as the GUI's own stylesheet (`#21222f` display
  well, `#2d2f45` body, `#33354c` keys, `#5468ff` accent on the "="
  key, `#eab876` display text) so the icon matches the window it opens.
  No SVG `<text>` element anywhere: `rsvg-convert` would otherwise need
  the right font installed to reproduce the PNGs, and `make icons` is
  meant to produce identical bytes on any machine (verified by
  regenerating and diffing checksums).
- `Gui/icons/calculator-{16,24,32,48,64,128,256}.png`: committed
  renders, so a normal build needs neither `rsvg-convert` nor a
  generation step. `make icons` re-renders them and is the only target
  that needs librsvg.
- `Gui/resources.qrc` + a `rcc` rule in the Makefile: the PNGs are
  compiled into `calculator-gui` rather than read from disk, so the
  icon appears no matter which directory the binary is launched from
  and without installing anything. `RCC` is located via
  `qmake6 -query QT_HOST_LIBEXECS` for the same Qt5/Qt6 mismatch reason
  the existing `MOC` line documents.
- `Gui/main.cpp`: sets the application name, display name, and
  `setDesktopFileName("calculator-gui")` (what lets a Wayland/GNOME
  session match the running window to its launcher entry), then builds
  one `QIcon` holding *every* embedded size. A window manager picks the
  closest match and scales it, and a 16 px title-bar icon downscaled
  from the 256 px render loses the keypad grid that makes the shape
  read as a calculator at all.
- `Gui/calculator-gui.desktop.in` + `make install-desktop` /
  `make uninstall-desktop`: an optional per-user launcher entry
  (`~/.local/share`, never root) plus hicolor icons at every size and
  the SVG in `scalable/`. `Exec` is substituted with this build tree's
  absolute path, so the entry works without installing the binary
  anywhere -- and is quoted, since a project path containing a space
  would otherwise be split into arguments by the desktop spec.

### Verified

`desktop-file-validate` passes with no warnings. The embedded
resources were checked by linking `qrc_resources.o` into a small
harness that reads each path back out of the resource system: all
seven resolve, the composed `QIcon` is non-null, `availableSizes()`
reports all seven, and a 16 px request returns a native 16x16 pixmap
rather than a downscale of the 256 px one. `make gui` is clean under
`-Wall -Wextra`, the binary launches under `QT_QPA_PLATFORM=offscreen`,
and `make test` still passes 471 cases (nothing in `Src/` changed).

**This completes Phase 31.** All 7 steps are done; `docs/ROADMAP.md`
and `docs/PHASES.md` updated accordingly.

## 2026-08-05 (4) — Phase 31 (GUI) Step 7 remainder: validators report their own errors

### Changed

- `Src/validator.c`, `Inc/validator.h`, `Inc/calculator.h`:
  `validateExpression()` now takes `char errorMsg[]` / `size_t
  errorSize` and writes its diagnostic there via `snprintf()` instead
  of `printf()`-ing it to stdout. This closes the limitation flagged
  in the previous entry: the Qt GUI could see only the return value,
  so it had to show a generic "Invalid expression." no matter which of
  the eight specific reasons (`Operand expected.`, `Function 'sin'
  expects 1 argument(s), got 2.`, ...) actually fired. The buffer is
  cleared to `""` on entry, so a caller can also use "is it empty" as
  the success test.
- `Src/parser.c`: `validateParentheses()` gained the same
  `errorMsg`/`errorSize` pair. It never printed anything — it just
  returned 0 for all three of its failure modes, leaving every caller
  to print one hardcoded "Mismatched parentheses." It now distinguishes
  `Error: Unclosed '('.`, `Error: Too many ')'.`, and `Error: Too many
  nested parentheses.` (the `CharStack` overflow path that gives up
  mid-walk rather than exiting the process).
- `Src/main.c`, `Src/plot.c`, `Gui/MainWindow.cpp`: all eight call
  sites updated. `evaluatePlotExpression()` forwards its own
  `errorMsg` buffer straight into both validators, so a bad inner
  expression now surfaces the real reason instead of the buffer being
  left empty on the assumption the validator had already printed it —
  and `plot.c` no longer needs its own hardcoded mismatched-parens
  message. The GUI's assignment path appends `(right of '=')` to
  preserve the context its old message carried, which is always
  accurate since only the right-hand side reaches a validator.
- `Inc/plot.h`, `Inc/validator.h`: the doc comments claimed `errorMsg`
  was "left untouched if valid"; both functions actually clear it, and
  the comments now say so.
- `docs/API.md`: both signatures were still documented in their
  one-argument form.

### Note

`errorSize` is `size_t` in both signatures, matching
`evaluatePlotExpression()`, so forwarding a buffer between them needs
no cast.

In the CLI/GUI pipeline `validateExpression()` runs first and rejects
every unbalanced expression on its own (it tracks `balance` with the
same rules and a shallower depth cap), so `validateParentheses()`'s new
messages are a backstop for direct callers rather than something a user
will normally see — confirmed by feeding `(()` and `)(` through
`./calculator`, which report `Error: Operand expected.` from the
expression validator. They're covered by direct unit tests instead.

### Verified

`make all`, `make gui`, and `make test` all clean under `-Wall
-Wextra` with zero warnings; **471 tests pass** (up from 466 —
`test_validator.c` gained `test_error_messages()` and
`test_parser.c` gained `test_validate_parentheses_messages()`,
covering the exact message text for each failure mode, the empty
buffer on success, and null-termination when the caller's buffer is
too small to hold the message). The four CLI error paths (arity
mismatch, missing operator, trailing operator, malformed `plot(...)`)
each print their specific message exactly once — previously the plot
path printed from inside the validator and returned an empty buffer.
The GUI launches cleanly under `QT_QPA_PLATFORM=offscreen`.

## 2026-08-05 (3) — Phase 31 (GUI) Step 7 follow-up: menu-driven layout

### Added

- `Gui/MainWindow.cpp`: the always-visible alpha keyboard and
  nine-tab side panel made the default window busy for what's meant
  to be, first and foremost, a plain calculator. Both are now opt-in.
  The side `QTabWidget` (History/Variables/Memory/Complex/Matrix/
  Statistics/Units/Base/Plot) moved into a `QDockWidget` titled
  "Functions & History", hidden by default via `addDockWidget()`; the
  alpha keyboard's `QGridLayout` moved into its own container
  `QWidget` (`alphaContainer`), also hidden by default. A new
  `QMenuBar` menu ("Menu") holds two checkable entries: "Alpha
  Keyboard" (a plain `QAction` wired directly to `alphaContainer`'s
  `setVisible()` slot) and "Functions & History (Advanced)" -- the
  latter reuses `QDockWidget::toggleViewAction()` instead of a
  hand-wired second `QAction`, so the menu checkbox and the dock's own
  close ("x") button stay in sync automatically with no manual
  bookkeeping. The `QSplitter` used since Step 2 is gone --
  `calculatorPanel` is now the plain `setCentralWidget()`, and Qt's
  native dock-area geometry handles the resizing the splitter used to.
- `Gui/MainWindow.cpp` (stylesheet): the now-unused `QSplitter::handle`
  rule was replaced with `QDockWidget::title`/`QMainWindow::separator`/
  `QMenuBar`/`QMenu` rules so the dock's title bar and the menu match
  the existing dark theme.

### Verified

Manually, via a real X11 session (`xdotool` + `import`, screenshots
inspected): a fresh launch shows nothing but the plain numeric keypad
-- no side panel, no alpha keyboard. On this machine's desktop
environment, Qt's menu bar renders in a global top panel rather than
inline in the window (confirmed by screenshotting the full screen,
not just the window, and seeing the "Menu" entry appear there once the
calculator window is focused) -- functionally identical to an inline
menu bar, just a desktop-level rendering choice. Opening the menu and
checking "Alpha Keyboard" shows the QWERTY grid; checking "Functions &
History (Advanced)" opens the dock beside the keypad with History
pre-populated and correctly styled. `make`/`make BUILD=asan test` (460
cases, unchanged -- no engine changes) and `./calculator` confirmed
unaffected; `make gui` builds clean with zero `-Wall -Wextra`
warnings.

## 2026-08-05 (2) — Phase 31 (GUI) Step 7 (partial): dark keypad/widget theme

### Added

- `Gui/MainWindow.cpp`: a single QSS stylesheet (`kStyleSheet`,
  applied once via `setStyleSheet()` in the constructor) replacing
  the plain default-widget look with a cohesive dark theme. Every
  keypad button gets a `"keyType"` dynamic property
  (`digit`/`operator`/`equals`/`clear`/`backspace`/`alpha`) and a
  matching QSS attribute selector: digits stay neutral, operators
  turn blue, `=` turns green, `C` turns red, backspace turns amber,
  and the alpha keyboard gets a muted, smaller-font look distinct
  from the numeric keys, so color alone signals what a key does.
  `QLineEdit`/`QTabBar`/`QListWidget`/`QTableWidget`/`QHeaderView`/
  `QSplitter::handle` got matching rounded-corner, hover, and
  selected-state styling. Deliberately does *not* use a blanket
  `QWidget {...}` rule -- Step 6's `PlotWidget` paints its own white
  canvas via `QPalette`/`autoFillBackground`, and a universal QSS
  selector matching every `QWidget` would silently override that once
  any stylesheet is active anywhere in the window.

### Verified

Manually, via a real X11 session (`xdotool` + `import`, screenshots
inspected): all six key-type colors render as intended with working
hover/pressed feedback; the Plot tab's white canvas is unaffected
(screenshotted both before and after this change, `cos(x)` still
plots correctly on it). `make`/`make BUILD=asan test` (460 cases,
unchanged -- no engine changes) and `./calculator` confirmed
unaffected; `make gui` builds clean with zero `-Wall -Wextra`
warnings.

## 2026-08-05 — Phase 31 (GUI) Step 6: real graphical plotting

### Added

- `Gui/PlotWidget.{hpp,cpp}`: a plain `QWidget` subclass with a
  `paintEvent()` override that draws a real line plot with
  `QPainter`, replacing Step 1-5's text-label approach for this one
  tab. Deliberately not a `QObject`/`Q_OBJECT` -- it has no signals or
  slots of its own, just a public `setSamples()`/`clearPlot()` and the
  paint override, so it needs no `moc` step at all, unlike
  `MainWindow.hpp`. It's also engine-agnostic: it only knows how to
  draw `(x, y, valid)` arrays handed to it, nothing about the
  calculator itself.
- `Gui/MainWindow.{hpp,cpp}`: a new "Plot" tab (input line + Plot
  button + `PlotWidget`) and a new `plotCurrentExpression()` slot that
  does the sampling, mirroring `Src/plot.c`'s own pipeline exactly --
  `insertImplicitMultiplication` -> `validateExpression` ->
  `validateParentheses` once (syntax doesn't depend on x), then
  per-sample `setVariable("x", ...)` + `infixToPostfix` +
  `evaluatePostfix` over `Src/plot.c`'s own `PLOT_XMIN`/`PLOT_XMAX`
  (`-10`/`10`) -- but at 400 samples instead of `plot.c`'s 61 ASCII
  columns, since a drawn line benefits from a smoother curve. Unlike
  `plot.c`'s CLI syntax, the tab's input takes the bare expression
  (`sin(x)`, not `plot(sin(x))`) since the tab itself is already the
  plot command. Non-finite samples are marked invalid and skipped when
  drawing line segments -- the same gap-at-the-discontinuity treatment
  `plot.c` gives asymptotes like `1/x`, just drawn instead of left
  blank in an ASCII canvas.
- `Makefile`: `GUI_SRCS` gained `Gui/PlotWidget.cpp`. `MOC_HEADERS`
  did not, per the no-`Q_OBJECT` point above.
- `.gitignore`: added `calculator-gui` alongside the existing
  `calculator` entry -- it's the same kind of build artifact and
  wasn't previously ignored.

### Verified

Manually, via a real X11 session (`xdotool` + `import`, screenshots
inspected): `sin(x)` draws a correct sine wave across the full
`[-10, 10]` domain with dashed x=0/y=0 reference lines; `1/x` shows
the steep rise on each side of the origin; `sqrt(x)` draws a curve
only for `x >= 0`, with the y-axis auto-ranged from the valid half
only -- confirming the gap-skip logic, not just a steep slope, is
what's driving the missing negative half; typing an invalid expression
(e.g. `foo(x`) shows a red "Error: Invalid expression." label and
clears the canvas rather than drawing anything stale.
`make`/`make BUILD=asan test` (460 cases, unchanged -- no engine
changes this time) and `./calculator` confirmed unaffected; `make gui`
builds clean with zero `-Wall -Wextra` warnings.

## 2026-08-04 (8) — GUI: QWERTY alpha keyboard

### Added

- `Gui/MainWindow.cpp`: a QWERTY letter grid below the numeric keypad.
  The numeric keypad (Step 1) had no way to produce a letter at all,
  so typing a function name (`sin`, `sqrt`, ...) or a variable name
  required a physical keyboard -- a real gap for a GUI whose whole
  point is not needing one. Button labels are uppercase (matching
  physical keys), but each button inserts lowercase, since every
  function/variable name in this engine is matched case-sensitively
  in lowercase (`functions.c`/`variables.c`: `strcmp()` against
  `"sin"`, `"sqrt"`, ...).
- `Gui/main.cpp`: grew the window's default/minimum size to fit the
  three extra rows.

### Verified

Manually, via a real X11 session (`xdotool` + `import`, screenshots
inspected): clicking S, I, N on the new alpha keyboard followed by
`( 3 0 )` on the numeric keypad produces `sin(30)` in the display;
pressing Enter evaluates it to `-0.988032` (radian mode) and logs it
to history -- identical to typing it on a physical keyboard.
`make test`/`make BUILD=asan test` (460 cases, unchanged -- no engine
changes this time) and `./calculator` confirmed unaffected.

## 2026-08-04 (7) — Phase 31 (GUI) Step 5: Complex/Matrix/Statistics/Units/Base tabs

### Added

- `Src/units.c`/`Inc/units.h`: `evaluateUnitExpression()`. Unlike
  `stats.c`/`base.c`/`complex_eval.cpp`/`matrix_eval.cpp`, which all
  already had a buffer-returning `evaluate*Expression(const char*,
  char*, int)` entry point, `units.c`'s `convertToSingleUnit()`/
  `convertAndPrint()` only printed to stdout -- no use to a GUI. The
  new function mirrors both of their existing logic (category checks,
  temperature special-casing, the full conversion table for the bare
  form) into a buffer instead. Covered by a new
  `test_evaluate_unit_expression()` in `Tests/test_units.c`. Test
  suite: 449 -> 460 cases.
- `Gui/MainWindow`: five more tabs -- Complex, Matrix, Statistics,
  Units, Base -- each a thin input line + Evaluate button (+
  Enter-to-submit) + result/error label around one
  `evaluate*Expression()` call. Since all five (including the new
  `evaluateUnitExpression()`) share the identical signature, one
  `buildEvalTab()` helper builds every tab instead of five
  near-identical copies -- a plain C function pointer plus a local
  (non-slot) lambda per tab, connected via Qt's functor-based
  `connect()`, which needs no MOC-generated machinery beyond what
  `MainWindow` already has.

### Verified

Manually, via a real X11 session (`xdotool` + `import`, screenshots
inspected): `(2+3i)*(4-5i)` -> `23+2i` and an unknown-identifier
error case (Complex); `det([[1,2],[3,4]])` -> `-2` (Matrix);
`stddev(4,8,6,5,3,7)` -> `1.87083` (Statistics); `10km to miles` ->
`6.21371 miles`, the bare `10km` full table, and a parse-error case
(Units); `hex(255)` -> `FF` (Base). `make test`/
`make BUILD=asan test` (460 cases) and `./calculator` confirmed
unaffected.

### Testing note

With 8 tabs now, the default 560px-wide window needs the tab bar's
scroll arrows (or a manual resize) to reach the later tabs -- expected
given the tab count, not a bug, and listed under Step 7 (polish) as
worth reconsidering. Also: mid-session, clicks briefly stopped
reaching the test window entirely (even on already-working buttons)
because another window had regained OS-level focus between separate
tool invocations -- not a Calculator bug, but worth remembering for
future GUI testing in this environment: activate/raise the target
window and verify focus *within the same command* as the interaction,
since focus can be stolen between separate steps.

## 2026-08-04 (6) — Phase 31 (GUI) Step 4: memory panel

### Added

- `Gui/MainWindow`: a fourth tab, "Memory", with MS/MR/M+/M-/MC
  buttons and a label showing the current register value. Unlike
  Steps 2-3, this needed **no new engine-side function** -- `memory.c`
  is a single register, not a list, and the existing `memoryRecall()`
  already gives the panel everything it needs to display.
- `MainWindow::m_lastResult`: a new member tracking the most recently
  evaluated result, set in both of `evaluate()`'s success paths. MS/
  M+/M- operate on it, deliberately extending the CLI's own MS
  convention (`Src/main.c`: `memoryStore(lastResult)`) to M+/M- too --
  the CLI instead prompts for a fresh value via `scanf()` for those
  two, which has no natural equivalent in a modal-free GUI, and
  reusing the already-visible result is the more calculator-like
  behavior anyway.

### Verified

Manually, via a real X11 session (`xdotool` + `import`, screenshots
inspected): MS stores `100`; M+ makes it `150`; M- brings it back to
`100`; MR recalls it into the display without changing it; MC resets
it to `0`. `make test`/`make BUILD=asan test` (449 cases, unchanged --
no engine code touched this step) and `./calculator` confirmed
unaffected.

## 2026-08-04 (5) — Phase 31 (GUI) Step 3: variable manager panel

### Added

- `Src/variables.c`/`Inc/variables.h`: `getVariableCount()` and
  `getVariableByIndex()`, mirroring the module's existing style
  exactly (same as Step 2's history additions) -- no existing function
  could enumerate variables programmatically for a GUI table; the
  existing accessors either look up a single variable by name or
  print directly to stdout. Covered by a new
  `test_variable_enumeration()` in `Tests/test_variables.c`. Test
  suite: 441 -> 449 cases.
- `Gui/MainWindow`: the side panel is now a `QTabWidget` (History /
  Variables) instead of a single widget next to the keypad, so Steps
  4-5's additional panels (Memory, Complex/Matrix/Stats/Units/Base)
  have somewhere to go without the window growing wider indefinitely.
  The Variables tab is a read-only `QTableWidget` (Name, Value)
  populated from the two functions above. Creating/updating a
  variable goes through a name/value field pair and a "Set" button
  (calling the existing `setVariable()`) rather than in-place table
  cell editing, which would need signal-blocking to stop
  `refreshVariables()`'s own repopulation from re-triggering the edit
  handler. The name field is validated against the same identifier
  rule the expression parser uses (`postfix.c`: starts with a letter,
  then letters/digits/`_`), so a variable created here is actually
  usable once typed into an expression, not just storable.

### Verified

Manually, via a real X11 session (`xdotool` + `import`, screenshots
inspected): `pi`/`e`/`ans` populate the table on launch; setting
`myvar = 42.5` via the fields adds a row and clears the fields;
`myvar+7.5` on the keypad correctly evaluates to `50`, and `ans`'s row
updates live to match; attempting to set `pi` is rejected with a
visible error message and `pi`'s row is unchanged. `make test`/
`make BUILD=asan test` (449 cases) and `./calculator` confirmed
unaffected by the GUI changes.

## 2026-08-04 (4) — Phase 31 (GUI) Step 2: history panel

### Added

- `Src/history.c`/`Inc/history.h`: `getHistoryCount()` and
  `getHistoryLineByNumber()`, mirroring the module's existing style
  exactly (same file-open/loop pattern as `getHistoryExpressionByNumber()`/
  `showRecentHistory()`). Needed because no existing function could
  enumerate history entries programmatically for a GUI list — the
  existing accessors only fetch a single entry by number or print
  directly to stdout. Covered by a new `test_count_and_line_by_number()`
  in `Tests/test_history.c`. Test suite: 433 -> 441 cases.
- `Gui/MainWindow`: a history side panel (`QListWidget` in a
  `QSplitter` next to the keypad) populated from the two functions
  above — the same on-disk log (`Build/history.txt`) the CLI reads
  and writes, so both front ends always agree on what's in history.
  Clicking an entry recalls just its expression (via the existing
  `getHistoryExpressionByNumber()`) into the display. A "Clear
  History" button calls the existing `clearHistory()`.

### Verified

Manually, via a real X11 session (`xdotool` + `import`, screenshots
inspected): existing history entries populate on launch; evaluating
`100/4` appends `"100/4 = 25"` and auto-scrolls the list; clicking a
`"sin(0) = 0"` entry recalls `sin(0)` (not the result) into the
display; Clear History empties the panel. `make test`/
`make BUILD=asan test` (441 cases) and `./calculator` confirmed
unaffected by the GUI changes.

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


# Roadmap

**Project:** Advanced Scientific Calculator in C/C++
**Language:** C11 + C++17
**Build:** Makefile
**Architecture:** Modular (C core engine; C++ for complex numbers and matrices)

---

# Vision

Evolve this project from a console-based scientific calculator into a
fully featured scientific computing platform — while keeping the
existing CLI engine solid, well-tested, and crash-free at every step
along the way.

---

# Development Philosophy

Every phase should:

- Be independently testable
- Be fully documented
- Follow the existing modular architecture
- Maintain backward compatibility
- Include unit tests
- Update `docs/API.md` and `docs/CHANGELOG.md`
- Keep complexity manageable

---

# Where This Document Fits

This file tracks **status at a glance** and what's next. For what each
phase actually built (goal, features, files, deliverables), see
**`docs/PHASES.md`** — that's the single source of truth for
phase-by-phase detail. Keeping the detail in one place instead of two
is a deliberate choice: this file used to duplicate that detail and,
over several rounds of edits, ended up with three different phase
numberings that all disagreed with `PHASES.md` and with each other. Update
`PHASES.md` when a phase's status changes; update this file's summary
table to match.

---

# Status Summary

**v1.0 (CLI engine): shipped.** All of Phases 1–27 are complete — the
full expression engine, scientific functions, variables/memory/history,
statistics, unit and base conversion, complex numbers, matrices, and
ASCII graph plotting, backed by 486 passing unit tests (clean under
AddressSanitizer/UndefinedBehaviorSanitizer).

| Phase(s) | Area                                        | Status        |
| -------- | -------------------------------------------- | ------------- |
| 1–20     | Core engine, testing, docs, v1.0 production  | ✅ Completed  |
| 21       | Angle modes                                  | ✅ Completed  |
| 22       | Unit conversion                              | ✅ Completed  |
| 23       | Base conversion                              | ✅ Completed  |
| 24       | Statistics engine                            | ✅ Completed  |
| 25       | Complex numbers (C++)                        | ✅ Completed  |
| 26       | Matrix operations (C++)                      | ✅ Completed  |
| 27       | Graph plotting (ASCII)                       | ✅ Completed  |
| 28       | Dynamic data structures                      | 🟡 Planned    |
| 29       | Abstract Syntax Tree (AST)                   | 🟡 Planned    |
| 30       | Symbolic mathematics                         | 🟡 Planned    |
| 31       | Graphical User Interface (Qt)                | ✅ Completed  |
| 32       | Plugin architecture                          | 🟡 Planned    |
| 33       | Calculator scripting                         | 🟡 Planned    |
| 34       | Package manager                              | 🟡 Planned    |
| 35       | Cross-platform builds                        | 🟠 In Progress — builds, tests and packages on all 3 OSes; no release tagged yet |
| 36       | Version 2.0 release                          | 🔵 Future     |

See `docs/PHASES.md` for the full breakdown of every phase above.

---

# Completed: Phase 31 — GUI (Qt)

The chosen toolkit is **Qt6** (`Qt6Widgets`, via `pkg-config`),
matching what `docs/RULES.md` and this roadmap anticipated. Being
built step by step, each step its own commit, CLI kept working
throughout — the GUI is an additional front end, not a replacement.

**Step 1 (done):** `Gui/MainWindow.{hpp,cpp}` + `Gui/main.cpp`, a
basic arithmetic keypad calling the existing engine directly
(`insertImplicitMultiplication`, `validateExpression`,
`validateParentheses`, `infixToPostfix`, `evaluatePostfix`,
`setVariable`/`setAns`, `addHistory`) through an `extern "C"` include
block, exactly the CLI's case-1 logic minus the text-shortcut commands
(`!!`, `mode deg`) that don't make sense for a keypad. `make gui`/
`make run-gui` added to the Makefile — Qt-only, opt-in, every other
target unaffected whether or not Qt is installed. Verified: `2+3*5`,
`sqrt(16)`, `x=5` then `x+3` (assignment + recall), `2^10`, and
`5/0` (shows the error, window stays open) all work via both
keyboard and button clicks.

**Step 2 (done):** history panel. Added a `QListWidget` beside the
keypad (via a `QSplitter`) that reads `history.c`'s on-disk log — the
same file the CLI reads/writes, so both front ends always agree on
what's in history. Required two small additions to `history.h`/
`history.c`, mirroring its existing style exactly:
`getHistoryCount()` and `getHistoryLineByNumber()` (the latter
returns the full `"expr = result"` line, unlike the existing
`getHistoryExpressionByNumber()` which only returns the expression
part — used separately for click-to-recall so recalling an entry
puts just the expression back in the display, not the whole line).
Both new functions have test coverage in `Tests/test_history.c`.
Also added a "Clear History" button (`clearHistory()`). Verified:
existing history entries populate on launch, a new calculation
appends and auto-scrolls, clicking an entry recalls its expression,
Clear History empties the panel — all via a real X11 session.

**Step 3 (done):** variable manager panel. The side panel is now a
`QTabWidget` (History / Variables) instead of a single widget next to
the keypad — set up this way so Steps 4–5's additional panels (Memory,
Complex/Matrix/Stats/Units/Base) have somewhere to go without the
window growing wider indefinitely. The Variables tab is a read-only
`QTableWidget` (Name, Value) populated from two new functions added to
`variables.h`/`variables.c` (mirroring its existing style, same as
Step 2's history additions): `getVariableCount()` and
`getVariableByIndex()`. Creating/updating a variable goes through a
name/value field pair and a "Set" button — deliberately not in-place
table cell editing, which would need signal-blocking to stop
`refreshVariables()`'s own repopulation from re-triggering the edit
handler. The name field is validated against the same identifier rule
the expression parser uses (`postfix.c`: starts with a letter, then
letters/digits/`_`), so a variable created here is actually usable
once typed into an expression. Test coverage in
`Tests/test_variables.c`. Verified: `pi`/`e`/`ans` populate on launch;
setting `myvar = 42.5` adds a row, and `myvar+7.5` on the keypad
correctly evaluates to `50`, updating `ans`'s row live; attempting to
set `pi` is rejected with a visible error, `pi` unchanged.

**Step 4 (done):** memory panel, as a fourth tab. Unlike Steps 2–3,
this needed no new engine-side function at all — `memory.c` is a
single register, not a list, and `memoryRecall()` already gives the
panel everything it needs to display. MS/M+/M- operate on the most
recently evaluated result (`m_lastResult`, a new member tracked in
`evaluate()`), deliberately extending the CLI's own MS convention
(`Src/main.c`: `memoryStore(lastResult)`) to M+/M- too, rather than
the CLI's separate scanf-prompt behavior for those two specifically —
a modal-free GUI has no natural equivalent to "prompt for a fresh
value," and reusing the already-visible result is the more calculator-
like behavior anyway. Verified end to end: MS stores `100`, M+ makes
it `150`, M- brings it back to `100`, MR recalls it into the display
without changing it, MC resets it to `0`.

**Step 5 (done):** five more tabs — Complex, Matrix, Statistics,
Units, Base. All five wrap an `evaluate*Expression(const char*,
char*, int)` entry point, and since that signature is identical
across all five, one shared `buildEvalTab()` helper builds every tab
(input line + Evaluate button + Enter-to-submit + result/error label)
instead of five near-copies. `units.c` turned out to be the odd one
out: `convertToSingleUnit()`/`convertAndPrint()` only printed to
stdout, with no buffer-returning entry point like the other four
modules already had. Added `evaluateUnitExpression()` to
`units.c`/`units.h`, mirroring those two functions' existing logic
(category checks, temperature special-casing, the full conversion
table) into a buffer instead, covered by `Tests/test_units.c`. Test
suite: 449 -> 460 cases. Verified end to end via a real X11 session:
`(2+3i)*(4-5i)` -> `23+2i` (plus an unknown-identifier error case),
`det([[1,2],[3,4]])` -> `-2`, `stddev(4,8,6,5,3,7)` -> `1.87083`,
`10km to miles` -> `6.21371 miles` (plus the bare-form table and a
parse-error case), `hex(255)` -> `FF`.

**Testing note:** with 8 tabs now, the default 560px-wide window
needs the tab bar's scroll arrows or a manual resize to reach the
later tabs — expected given how many tabs there are, not a bug, and
listed under Step 7 (polish) as something to reconsider (e.g. a
narrower tab-bar font, or a different navigation widget entirely).

**Alpha keyboard (Step 1 follow-up, done):** the numeric keypad had no
way to produce a letter at all, so typing a function name (`sin`,
`sqrt`, ...) or a variable name required a physical keyboard — a real
gap given the whole point of a GUI is not needing one. Added a QWERTY
grid (`Gui/MainWindow.cpp`) below the numeric keypad: button labels
are uppercase (matching physical keys), but each button inserts
lowercase, since every function/variable name in this engine is
matched case-sensitively in lowercase (`functions.c`/`variables.c`:
`strcmp()` against `"sin"`, `"sqrt"`, ...). Window's default/minimum
size grown (`Gui/main.cpp`) to fit the extra rows. Verified: `sin(30)`
typed entirely via the S/I/N alpha keys plus the numeric keypad's
`( 3 0 )` evaluates to `-0.988032` and logs to history, exactly as
typing it on a physical keyboard would.

**Step 6 (done):** real graphical plotting. A new `PlotWidget`
(`Gui/PlotWidget.{hpp,cpp}`) is a plain `QWidget` subclass with a
`paintEvent()` override — deliberately *not* a `QObject` with
`Q_OBJECT`, since it has no signals/slots of its own (just a public
`setSamples()`/`clearPlot()` and the paint override), so it needs no
`moc` step at all, unlike `MainWindow.hpp`. It's also deliberately
engine-agnostic: it only knows how to draw `(x, y, valid)` arrays it's
handed, nothing about the calculator. The sampling itself lives in a
new `MainWindow::plotCurrentExpression()`, mirroring `Src/plot.c`'s
own pipeline exactly (`insertImplicitMultiplication` →
`validateExpression` → `validateParentheses`, then per-sample
`setVariable("x", ...)` + `infixToPostfix` + `evaluatePostfix` over
`PLOT_XMIN`/`PLOT_XMAX = -10/10`, same as the CLI) but building arrays
for `PlotWidget` instead of an ASCII canvas, and sampling 400 points
instead of `plot.c`'s 61 columns since a drawn line benefits from a
smoother curve. Non-finite samples are marked invalid and simply
skipped when drawing line segments — the same gap-at-the-
discontinuity treatment `plot.c` gives asymptotes like `1/x`, just
drawn instead of left blank in an ASCII canvas. Unlike `plot.c`'s CLI
syntax, the new "Plot" tab's input takes the bare expression (`sin(x)`,
not `plot(sin(x))`) since the tab itself is already the plot command.
`Makefile`'s `GUI_SRCS` gained `PlotWidget.cpp`; `MOC_HEADERS` did not,
for the reason above. Verified via a real X11 session: `sin(x)` draws
a correct sine wave across the full domain; `1/x` shows the steep rise
on each side of the origin; `sqrt(x)` shows a curve only for `x >= 0`
with the y-axis auto-ranged from the valid half only (confirming the
gap logic, not just a steep slope, is what's driving it); an invalid
expression shows a red error label and clears the canvas rather than
drawing anything stale.

**Step 7 (partial — theme done, icon/validator-message still open):**
a single QSS stylesheet (`MainWindow`'s new `kStyleSheet` constant,
applied once via `setStyleSheet()` in the constructor) replaces the
plain default-widget look with a cohesive dark theme: every keypad
button gets a `"keyType"` dynamic property (`digit`/`operator`/
`equals`/`clear`/`backspace`/`alpha`) and a matching QSS attribute
selector, so digits stay neutral, operators turn blue, `=` turns
green, `C` turns red, backspace turns amber, and the alpha keyboard
gets a muted, smaller-font look distinct from the numeric keys —
color alone now tells you what a key does before you read its label.
`QLineEdit`/`QTabBar`/`QListWidget`/`QTableWidget`/`QHeaderView` got
matching rounded-corner, hover, and selected-state styling.
Deliberately *not* a blanket `QWidget {...}` rule: Step 6's
`PlotWidget` paints its own white canvas via `QPalette`/
`autoFillBackground`, and a universal QSS selector matching every
`QWidget` would silently override that once any stylesheet is active
anywhere in the window — confirmed unaffected by screenshotting the
Plot tab both before and after this change. Verified via a real X11
session: all six key-type colors render as intended, hover/pressed
feedback works, `cos(x)` still plots correctly with an unchanged
white canvas, and the full 460-test suite plus `./calculator` are
unaffected (no engine changes). An application icon and the
validator-message limitation below remain open.

**Menu-driven layout (Step 7 follow-up, done):** the always-visible
alpha keyboard and nine-tab side panel made the default window busy
for what's supposed to be, first and foremost, a plain calculator.
Both are now opt-in: the side `QTabWidget` (History/Variables/Memory/
Complex/Matrix/Statistics/Units/Base/Plot) moved into a `QDockWidget`
titled "Functions & History", hidden by default and added via
`addDockWidget(Qt::RightDockWidgetArea, ...)`; the alpha keyboard's
`QGridLayout` moved into its own container `QWidget`
(`alphaContainer`), also hidden by default. A `QMenuBar` menu
("Menu") holds two checkable entries: "Alpha Keyboard" (a plain
`QAction` wired to `alphaContainer`'s `setVisible()` slot) and
"Functions & History (Advanced)" -- which reuses
`QDockWidget::toggleViewAction()` rather than a hand-wired second
`QAction`, so the checkbox and the dock's own close ("x") button stay
in sync automatically with no manual bookkeeping. The former
`QSplitter` is gone -- `calculatorPanel` is now the plain
`setCentralWidget()`, and Qt's own dock-area geometry handles the
resize-to-fit that the splitter used to. The stylesheet's leftover
`QSplitter::handle` rule was replaced with `QDockWidget::title`/
`QMainWindow::separator`/`QMenuBar`/`QMenu` rules so the dock's title
bar and the menu match the existing dark theme. Verified via a real
X11 session: a fresh launch shows nothing but the plain keypad (no
menu-bar row is visible inline in-window on this machine's desktop
environment, which renders Qt menu bars in a global top panel
instead -- confirmed by screenshotting the full screen, not just the
window, and seeing the "Menu" entry appear there once the calculator
window is focused); opening the menu and checking "Alpha Keyboard"
shows the QWERTY grid; checking "Functions & History (Advanced)"
opens the dock beside the keypad with History pre-populated and
correctly styled.

**Known Step 1 limitation (fixed 2026-08-05):** `validateExpression()`
reported its specific failure reason via `printf()` to stdout (fine for
the CLI, invisible to a GUI window) and `validateParentheses()` reported
nothing at all, so the GUI showed a generic "Invalid expression."/
"Mismatched parentheses." instead of the reason actually detected. Both
now take an `errorMsg`/`errorSize` buffer like every other error path
(`infixToPostfix`, `evaluatePostfix`, the stats/units/base/complex/
matrix modules), and the CLI, the GUI, and `evaluatePlotExpression()`
all display what they write.

**Step 7 (done — application icon):** `Gui/icon.svg` is the source of
truth, drawn from plain shapes in the same palette as the GUI's own
stylesheet, and rendered to PNGs at 16/24/32/48/64/128/256 px by
`make icons` (needs `rsvg-convert`; the PNGs are committed, so a
normal build doesn't). They're compiled into the binary through
`Gui/resources.qrc` via `rcc` rather than read from disk, so the icon
appears no matter where `calculator-gui` is launched from. Every size
goes into one `QIcon` because a 16 px title-bar icon downscaled from
256 px loses the keypad grid that makes the shape legible at all.
`make install-desktop` additionally installs a per-user `.desktop`
entry plus the hicolor icons so the app shows up in the desktop
launcher; `make uninstall-desktop` reverses it.

**Phase 31 is complete.** All 7 steps are done.

Phases 28–30 (dynamic data structures, AST, symbolic math) are
foundational rewrites of the evaluation engine itself and are
deliberately *not* prerequisites for the GUI — the GUI can and should
ship against the current postfix-based engine.

---

# Longer-Term (Phases 32–36)

Plugin architecture, scripting, packaging, and cross-platform builds
remain genuinely future work with no committed timeline. Version 2.0
(Phase 36) is the point at which enough of Phases 28–35 have landed to
justify calling this "a professional scientific computing platform"
rather than "a calculator." See `docs/PHASES.md` for what each of
those phases is scoped to include.

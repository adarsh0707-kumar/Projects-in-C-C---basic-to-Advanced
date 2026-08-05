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
ASCII graph plotting, backed by 449 passing unit tests (clean under
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
| 31       | Graphical User Interface (Qt)                | 🟠 In Progress — Steps 1–4 done |
| 32       | Plugin architecture                          | 🟡 Planned    |
| 33       | Calculator scripting                         | 🟡 Planned    |
| 34       | Package manager                              | 🟡 Planned    |
| 35       | Cross-platform builds                        | 🟡 Planned    |
| 36       | Version 2.0 release                          | 🔵 Future     |

See `docs/PHASES.md` for the full breakdown of every phase above.

---

# In Progress: Phase 31 — GUI (Qt)

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

**Known Step 1 limitation (unchanged):** `validateExpression()`/
`validateParentheses()` report their specific failure reason via
`printf()` to stdout (fine for the CLI, invisible to a GUI window) —
the GUI currently shows a generic "Invalid expression."/"Mismatched
parentheses." instead of the specific reason. Every other error path
(`infixToPostfix`, `evaluatePostfix`, the stats/units/base/complex/
matrix modules) already returns its message in a buffer and doesn't
have this gap.

**Remaining steps** (each its own later commit, not all at once):

5. Tabs for Complex / Matrix / Statistics / Units / Base — each a thin
   form calling the existing `evaluate*Expression()` entry points.
6. Real graphical plotting: a custom `QWidget::paintEvent` sampling
   the expression the same way `plot.c` does, drawn with `QPainter`
   instead of ASCII.
7. Polish: theme, icon, error styling; consider fixing the Step 1
   validator-message limitation above (would need
   `validateExpression()`/`validateParentheses()` to return a message
   into a buffer like every other module already does).

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

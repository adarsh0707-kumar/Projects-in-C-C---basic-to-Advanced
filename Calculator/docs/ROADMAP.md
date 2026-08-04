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
ASCII graph plotting, backed by 433 passing unit tests (clean under
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
| 31       | Graphical User Interface (Qt)                | 🟠 In Progress — Step 1 done |
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

**Known Step 1 limitation:** `validateExpression()`/
`validateParentheses()` report their specific failure reason via
`printf()` to stdout (fine for the CLI, invisible to a GUI window) —
the GUI currently shows a generic "Invalid expression."/"Mismatched
parentheses." instead of the specific reason. Every other error path
(`infixToPostfix`, `evaluatePostfix`, the stats/units/base/complex/
matrix modules) already returns its message in a buffer and doesn't
have this gap.

**Remaining steps** (each its own later commit, not all at once):

2. History panel (reuse `history.c`), click-to-recall.
3. Variable manager panel (reuse `variables.c`).
4. Memory panel (MS/MR/M+/M-/MC, reuse `memory.c`).
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

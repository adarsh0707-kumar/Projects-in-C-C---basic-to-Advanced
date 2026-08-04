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
ASCII graph plotting, backed by 271 passing unit tests (clean under
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
| 31       | Graphical User Interface (Qt)                | 🟡 Planned — next up |
| 32       | Plugin architecture                          | 🟡 Planned    |
| 33       | Calculator scripting                         | 🟡 Planned    |
| 34       | Package manager                              | 🟡 Planned    |
| 35       | Cross-platform builds                        | 🟡 Planned    |
| 36       | Version 2.0 release                          | 🔵 Future     |

See `docs/PHASES.md` for the full breakdown of every phase above.

---

# Next Up: Phase 31 — GUI (Qt)

The chosen toolkit is **Qt**, matching what `docs/RULES.md` and this
roadmap have anticipated since the CLI-only rule was written. Rough
shape of the work, to be broken into its own sub-plan when picked up:

- Extract the CLI's per-mode logic (`Src/main.c`'s `switch` cases) into
  functions the GUI can call directly, instead of `scanf`/`fgets`-driven
  I/O — the GUI and CLI should share the same engine calls
  (`infixToPostfix`, `evaluatePostfix`, `evaluateComplexExpression`,
  etc.), not duplicate parsing logic.
- Scientific keypad, history panel, variable manager, memory panel
  (mirrors the CLI's existing menu options 1–10)
- Plotting mode reuses `Src/plot.c`'s sampling logic, rendered as an
  actual line plot instead of ASCII
- Keep the CLI (`make run`) working throughout — the GUI is an
  additional front end, not a replacement

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

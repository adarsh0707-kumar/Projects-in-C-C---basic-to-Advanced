
# Scientific Calculator Development Phases

Version: 1.0

This document defines the complete development lifecycle of the Scientific Calculator project.

Each phase contains:

- Goal
- Features
- Files
- Deliverables
- Status

---

# Phase 1 — Project Setup

## Goal

Create the project structure and build system.

## Features

- Folder structure
- Makefile
- Build directory
- Object directory
- Header organization
- Coding standards

## Files

main.c
Makefile
calculator.h

## Deliverables

- Project builds successfully
- Basic Hello World

Status:
✅ Completed

---

# Phase 2 — Stack Implementation

## Goal

Implement reusable stack data structures.

## Features

- Double stack
- Character stack
- String stack
- Stack operations
- Overflow detection
- Underflow detection

## Files

stack.c
stack.h

## Deliverables

Reusable stack library.

Status:
✅ Completed

---

# Phase 3 — Expression Tokenizer

## Goal

Read mathematical expressions and tokenize input.

## Features

- Numbers
- Operators
- Variables
- Parentheses
- Spaces
- Decimal support

## Files

tokenizer.c

## Deliverables

Tokenizer module.

Status:
✅ Completed

---

# Phase 4 — Expression Validator

## Goal

Reject invalid expressions before evaluation.

## Features

- Parentheses validation
- Operator validation
- Operand validation
- Decimal validation
- Invalid symbols
- Syntax checking

## Files

validator.c

## Deliverables

Expression validator.

Status:
✅ Completed

---

# Phase 5 — Infix to Postfix

## Goal

Convert expressions into postfix notation.

## Features

- Shunting-yard algorithm
- Operator precedence
- Associativity
- Parentheses
- Unary minus

## Files

postfix.c

## Deliverables

Working postfix generator.

Status:
✅ Completed

---

# Phase 6 — Postfix Evaluation

## Goal

Evaluate postfix expressions.

## Features

- Arithmetic operators
- Floating-point support
- Error handling

## Files

evaluate.c

## Deliverables

Expression evaluator.

Status:
✅ Completed

---

# Phase 7 — Variables

## Goal

Support named variables.

## Features

- Variable storage
- Variable lookup
- Assignment
- ans variable

## Files

variables.c

## Deliverables

Variable system.

Status:
✅ Completed

---

# Phase 8 — History System

## Goal

Store previous calculations.

## Features

- Save history
- View history
- Clear history

## Files

history.c

## Deliverables

Persistent history.

Status:
✅ Completed

---

# Phase 9 — Memory Operations

## Goal

Implement calculator memory.

## Features

MS
MR
M+
M-
MC

## Files

memory.c

## Deliverables

Memory module.

Status:
✅ Completed

---

# Phase 10 — Scientific Functions

## Goal

Support mathematical functions.

## Features

Unary Functions

- sqrt
- sin
- cos
- tan
- asin
- acos
- atan
- sinh
- cosh
- tanh
- log
- ln
- exp
- abs
- cbrt
- floor
- ceil
- round
- trunc

Binary Functions

- pow
- max
- min
- hypot
- atan2

## Files

functions.c
function_info.c

## Deliverables

Scientific calculator support.

Status:
✅ Completed

---

# Phase 11 — Constants

## Goal

Support mathematical constants.

## Features

- pi
- e

## Files

constants.c

## Deliverables

Constant lookup system.

Status:
✅ Completed

---

# Phase 12 — Implicit Multiplication

## Goal

Allow natural mathematical notation.

## Features

2pi

2(x+1)

3sqrt(9)

5sin(pi)

## Files

parser.c

## Deliverables

Implicit multiplication parser.

Status:
✅ Completed

---

# Phase 13 — Advanced Parsing

## Goal

Improve parser intelligence.

## Features

Nested functions

Nested variables

Function arguments

Comma support

Unary operators

Factorial parsing

## Files

parser.c
postfix.c

## Deliverables

Production-quality parser.

Status:
✅ Completed

---

# Phase 14 — Multi-Argument Functions

## Goal

Support multiple function arguments.

## Features

pow()

max()

min()

hypot()

atan2()

Comma parsing

Argument counting

## Files

function_info.c
functions.c
evaluate.c

## Deliverables

Multi-argument evaluator.

Status:
✅ Completed

---

# Phase 15 — Error Handling

## Goal

Improve user feedback.

## Features

Division by zero

Undefined variables

Unknown functions

Invalid expressions

Stack errors

Meaningful messages

## Deliverables

Robust error handling.

Status:
✅ Completed

---

# Phase 16 — Testing

## Goal

Verify every module.

## Tests

Stack

Variables

Functions

Parser

Tokenizer

Validator

Evaluator

History

Memory

## Deliverables

Stable calculator.

Status:
✅ Completed

---

# Phase 17 — Performance Optimization

## Goal

Improve execution speed.

## Features

Efficient parser

Reduced copies

Optimized stacks

Improved tokenizer

## Deliverables

Faster execution.

Status:
✅ Completed

---

# Phase 18 — Documentation

## Goal

Professional documentation.

## Features

README

Architecture

API

Design

Rules

Memory

Roadmap

Testing

## Deliverables

Developer documentation.

Status:
✅ Completed

---

# Phase 19 — Code Refactoring

## Goal

Improve maintainability.

## Features

Cleaner modules

Consistent naming

Reduced duplication

Modular APIs

## Deliverables

Professional codebase.

Status:
✅ Completed

---

# Phase 20 — Production Ready

## Goal

Release Version 1.0

## Features

Bug fixes

Stress testing

Code cleanup

Documentation review

Release build

GitHub release

## Deliverables

Scientific Calculator v1.0

Status:
✅ Completed

---

> **Note (editorial):** Phases 21–27 below were built but never recorded
> as their own phases at the time — `docs/CHANGELOG.md` was last updated
> around the core-engine/v1.0 milestone and still lists statistics,
> units, base conversion, complex numbers, matrices, and plotting as
> "Upcoming" even though all of them shipped. Rather than guess at the
> exact historical order, they're logged here now, after Phase 20, in
> the numbering that was actually free. The previously-numbered Phases
> 21–30 (all still genuinely unbuilt) were renumbered to 28–36 to make
> room, with no change to their content.

# Phase 21 — Angle Modes

## Goal

Let trig functions work in either degrees or radians.

## Features

- `mode deg` / `mode rad` / `mode` (show current mode)
- Applies to `sin`/`cos`/`tan`/`asin`/`acos`/`atan2`; hyperbolic
  functions are unaffected (they operate on plain reals)

## Files

angle_mode.c
angle_mode.h

## Deliverables

Degree/radian angle mode switch.

Status:
✅ Completed

---

# Phase 22 — Unit Conversion

## Goal

Convert values between units of the same category.

## Features

- Length, weight, time, temperature
- Bare form (`10km` -> prints the full conversion table) and explicit
  target form (`10km to miles`)

## Files

units.c
units.h

## Deliverables

Unit converter.

Status:
✅ Completed

---

# Phase 23 — Base Conversion

## Goal

Convert integers between number bases.

## Features

- `bin()`, `hex()`, `oct()` (decimal -> target base)
- `dec()` (base, via a `b`/`o`/`h` suffix, -> decimal)
- Negative integers (sign-magnitude)

## Files

base.c
base.h

## Deliverables

Number base converter.

Status:
✅ Completed

---

# Phase 24 — Statistics Engine

## Goal

Compute descriptive statistics over a list of values.

## Features

- `mean`, `median`, `sum`, `min`, `max`, `count`
- `stddev`/`variance` (sample, n-1) and `pstddev`/`pvariance`
  (population, n)

## Files

stats.c
stats.h

## Deliverables

Statistics function family.

Status:
✅ Completed

---

# Phase 25 — Complex Numbers

## Goal

Support complex-number arithmetic.

## Features

- Addition, subtraction, multiplication, division
- `sqrt()` of negative/complex values
- Real operator overloading (C++)

## Files

Complex.cpp
Complex.hpp
complex_eval.cpp
complex_eval.h

## Deliverables

Complex-number calculator mode.

Status:
✅ Completed

---

# Phase 26 — Matrix Operations

## Goal

Support matrix arithmetic.

## Features

- Addition, subtraction, matrix/scalar multiplication
- Transpose, determinant, inverse
- Backed by `std::vector` (C++, RAII storage)

## Files

Matrix.cpp
Matrix.hpp
matrix_eval.cpp
matrix_eval.h

## Deliverables

Matrix calculator mode.

Status:
✅ Completed

---

# Phase 27 — Graph Plotting

## Goal

Visualize mathematical functions.

## Features

- ASCII graphs (`Src/plot.c`, `plot(<expr in x>)`)
- Function plotting over a fixed x-range, with NaN samples (e.g. the
  asymptote in `plot(1/x)`) rendered as a gap instead of crashing

## Not done (deferred to Phase 31 — GUI)

- Zoom / pan (the ASCII view is a fixed x-range window)
- Graphical (non-ASCII) 2D plots

## Files

plot.c
plot.h

## Deliverables

Terminal ASCII graph plotting.

Status:
✅ Completed (ASCII plotting) — zoom/pan/graphical plots deferred to Phase 31

---

# Phase 28 — Dynamic Data Structures

## Goal

Remove fixed-size limitations.

## Features

Dynamic stacks

Dynamic token arrays

Dynamic variables

Resizable buffers

Memory management

## Files

stack.c

tokenizer.c

variables.c

Status:
🟡 Planned

---

# Phase 29 — Abstract Syntax Tree (AST)

## Goal

Replace postfix-only evaluation.

## Features

AST Nodes

Expression Tree

Tree Evaluation

Tree Optimization

Tree Traversal

## Files

ast.c

ast.h

parser.c

Status:
🟡 Planned

---

# Phase 30 — Symbolic Mathematics

## Goal

Support symbolic calculations.

## Features

Simplify

Expand

Differentiate

Integrate

Equation solving

Status:
🟡 Planned

---

# Phase 31 — Graphical User Interface

## Goal

Desktop scientific calculator.

## Features

- Qt6 GUI (`Gui/MainWindow.{hpp,cpp}`, `Gui/main.cpp`) — **Step 1
  done:** basic arithmetic keypad, calling the same engine functions
  as the CLI (`infixToPostfix`, `evaluatePostfix`,
  `validateExpression`, `setVariable`/`setAns`, `addHistory`) rather
  than duplicating any parsing logic. `make gui` / `make run-gui`.
- Scientific keypad — done (Step 1: digits, `+ - * / ^ % ( ) . !`,
  `=`, clear, backspace)
- History panel — done (Step 2: `QListWidget` reads `history.c`'s
  on-disk log via two new functions, `getHistoryCount()` and
  `getHistoryLineByNumber()`, added to that module and covered by
  `Tests/test_history.c`; click-to-recall via the existing
  `getHistoryExpressionByNumber()`; a "Clear History" button)
- Variable manager — done (Step 3: read-only `QTableWidget` populated
  from two new functions, `getVariableCount()` and
  `getVariableByIndex()`, added to `variables.c` and covered by
  `Tests/test_variables.c`; create/update via name+value fields and a
  "Set" button calling the existing `setVariable()`; the side panel
  is now a `QTabWidget` (History / Variables) instead of a single
  widget, to leave room for Steps 4–5's additional panels)
- Memory panel — not yet (Step 4)
- Theme support — not yet (Step 7)

## Files

Gui/MainWindow.hpp
Gui/MainWindow.cpp
Gui/main.cpp
Inc/history.h (2 new functions)
Src/history.c (2 new functions)
Inc/variables.h (2 new functions)
Src/variables.c (2 new functions)

## Deliverables

Step 1: a working Qt6 arithmetic calculator window, verified against
`2+3*5`, `sqrt(16)`, variable assignment/recall, `2^10`, and graceful
division-by-zero handling — via both keyboard and button-click input.

Step 2: a history side panel sharing the CLI's on-disk history file,
verified for populate-on-launch, append-on-calculate, click-to-recall,
and Clear History — via a real X11 session.

Step 3: a variable manager tab, verified for populate-on-launch,
setting a new variable and using it in a calculation (`myvar = 42.5`
then `myvar+7.5` -> `50`), live update of `ans`'s row, and rejection
of writes to read-only constants (`pi`) with a visible error — via a
real X11 session.

Status:
🟠 In Progress — Steps 1–3 (of 7 planned steps; see
`docs/ROADMAP.md`'s "In Progress: Phase 31" section for the full
breakdown) complete

---

# Phase 32 — Plugin Architecture

## Goal

Support external extensions.

## Features

Dynamic libraries

Plugin loading

Plugin API

Custom functions

Status:
🟡 Planned

---

# Phase 33 — Calculator Scripting

## Goal

Add scripting language support.

## Features

Variables

Loops

Conditions

Functions

Scripts

Status:
🟡 Planned

---

# Phase 34 — Package Manager

## Goal

Install calculator extensions.

## Features

Plugin installer

Plugin repository

Dependency management

Version management

Status:
🟡 Planned

---

# Phase 35 — Cross Platform

## Goal

Support multiple operating systems.

## Features

Linux

Windows

macOS

CI/CD

Cross compilation

Status:
🟡 Planned

---

# Phase 36 — Version 2.0 Release

## Goal

Professional Scientific Computing Platform.

## Features

GUI

Symbolic Math

Plotting

Plugins

Scripting

Documentation

Testing

Performance

Status:
🔵 Future

# Roadmap

> **Project:** Advanced Scientific Calculator in C/C++
>
> **Language:** C11 + C++17
>
> **Build:** Makefile
>
> **Architecture:** Modular
>
> **Goal:** Build an industry-level scientific calculator with parser, evaluator, mathematical engine, unit conversion, plotting, GUI, scripting, plugins, networking, testing, packaging, and extensibility.

---

# Overall Progress

| Phase        | Status       |
| ------------ | ------------ |
| Phase 1–20  | ✅ Completed |
| Phase 21–30 | 🚧 Planned   |

---

# Phase 1 — Project Setup

### Goal

Create the initial project architecture.

### Deliverables

- Makefile
- Src/
- Inc/
- Obj/
- Build/
- README

### Result

A clean modular C project.

---

# Phase 2 — Stack Implementation

### Goal

Implement stack data structures.

### Deliverables

- Double Stack
- Character Stack
- String Stack
- Token Stack

### Files

```
stack.c
stack.h
```

---

# Phase 3 — Tokenizer

### Goal

Convert input into tokens.

### Features

- Numbers
- Operators
- Parentheses
- Variables
- Functions

### Files

```
tokenizer.c
token.h
```

---

# Phase 4 — Expression Validator

### Goal

Reject invalid expressions before parsing.

### Supports

- Parentheses
- Invalid operators
- Invalid numbers
- Missing operands
- Unary minus

### Files

```
validator.c
validator.h
```

---

# Phase 5 — Parser

### Goal

Normalize mathematical expressions.

### Features

- Implicit multiplication
- Variable assignment
- Function recognition
- Expression cleanup

Example

```
2pi

↓

2*pi
```

---

# Phase 6 — Infix → Postfix Conversion

### Goal

Implement Shunting Yard Algorithm.

### Supports

- Operator precedence
- Associativity
- Functions
- Variables
- Constants
- Parentheses

Files

```
postfix.c
```

---

# Phase 7 — Expression Evaluation

### Goal

Evaluate postfix expressions.

### Supports

- + - * / %
- ^
- unary operators
- functions
- variables

Files

```
evaluate.c
```

---

# Phase 8 — Variables

### Goal

Store user variables.

Example

```
x=10
y=20

x+y
```

Supports

- Assignment
- Retrieval
- ans variable

Files

```
variables.c
```

---

# Phase 9 — Scientific Functions

### Unary

```
sin
cos
tan
asin
acos
atan

sqrt
cbrt

log
ln
exp

abs

floor
ceil
round
trunc
```

### Binary

```
pow
max
min
hypot
atan2
```

Files

```
functions.c
function_info.c
```

---

# Phase 10 — Constants

Built-in constants

```
pi
e
```

Automatic replacement

```
sin(pi/2)
```

---

# Phase 11 — History

Features

- Save calculations
- View history
- Clear history

Files

```
history.c
```

---

# Phase 12 — Memory Register

Calculator memory

```
MS
MR
MC
M+
M-
```

Files

```
memory.c
```

---

# Phase 13 — Factorial

Supports

```
5!

(3+2)!

```

Large factorial calculations.

---

# Phase 14 — Function Metadata

Maintain information for every function.

Includes

- argument count
- category
- validation

Files

```
function_info.c
```

---

# Phase 15 — Better Error Handling

Improved diagnostics

Examples

```
Division by zero

Unknown function

Undefined variable

Stack overflow

Stack underflow

Mismatched parentheses
```

---

# Phase 16 — Unit Testing

Tests

- Stack
- Parser
- Validator
- Tokenizer
- Variables
- Functions
- History
- Memory

Folder

```
Tests/
```

---

# Phase 17 — Code Refactoring

Improve

- readability
- modularity
- naming
- comments
- header organization

---

# Phase 18 — Documentation

Documentation written

```
API.md

ARCHITECTURE.md

RULES.md

CHANGELOG.md

DESIGN.md

MEMORY.md

ROADMAP.md

PHASES.md

PRD.md
```

---

# Phase 19 — Performance Optimization

Improve

- tokenizer
- parser
- evaluator

Reduce

- allocations
- copies

Improve

- lookup speed

---

# Phase 20 — Production Ready CLI

Finished

- Interactive menu
- Memory
- Variables
- Scientific functions
- History
- Error handling
- Documentation
- Tests

---

# Phase 21 — Angle Modes

Support

```
mode rad

mode deg

mode grad
```

Functions affected

```
sin
cos
tan

asin
acos
atan
```

---

# Phase 22 — Statistics Engine

Functions

```
mean()

median()

mode()

variance()

stddev()

sum()

product()

percentile()
```

---

# Phase 23 — Unit Conversion

Length

```
km

m

cm

mm

inch

ft

mile
```

Weight

```
kg

g

lb

oz
```

Temperature

```
C

F

K
```

Time

```
sec

min

hour

day
```

Area

Volume

Speed

Pressure

Energy

Power

---

# Phase 24 — Base Conversion

Supports

```
Binary

Octal

Decimal

Hexadecimal
```

Examples

```
bin(25)

hex(255)

oct(64)

dec(1111b)
```

---

# Phase 25 — Complex Numbers

Supports

```
3+4i

sqrt(-1)

polar()

conjugate()

magnitude()

phase()
```

Files

```
Complex.cpp

complex_eval.cpp
```

---

# Phase 26 — Matrix Calculator

Supports

```
Addition

Subtraction

Multiplication

Transpose

Inverse

Determinant

Rank

Identity

LU decomposition
```

Files

```
Matrix.cpp

matrix_eval.cpp
```

---

# Phase 27 — Graph Plotting

CLI Graph

```
plot(sin(x))

plot(x²)

plot(log(x))
```

ASCII renderer

Later

GTK Graph

Qt Graph

---

# Phase 28 — Expression Scripting

Support

```
script.calc

include

variables

loops

macros
```

Batch execution

---

# Phase 29 — GUI Application

Using

- GTK
  or
- Qt

Features

- Scientific keypad
- Plotting
- History panel
- Variables panel
- Memory buttons
- Theme support
- Matrix editor
- Complex calculator

---

# Phase 30 — Professional Release

Final production release.

Includes

- Installer
- Packaging
- CI/CD
- GitHub Actions
- Doxygen documentation
- Benchmarks
- Examples
- Plugin system
- Localization
- Cross-platform support
- Windows
- Linux
- macOS

---

# Final Vision

After completing all 30 phases, the project will provide:

- Arithmetic Calculator
- Scientific Calculator
- Programmer Calculator
- Statistical Calculator
- Matrix Calculator
- Complex Number Calculator
- Unit Converter
- Base Converter
- Graph Plotter
- Expression Language
- CLI Application
- Desktop GUI
- Comprehensive Test Suite
- Professional Documentation
- Production-ready Open Source Project

This roadmap transforms the project from a basic expression evaluator into a complete, extensible scientific computing application suitable for portfolio, educational use, and real-world developmen

# Roadmap

> **Project:** Advanced Scientific Calculator in C/C++
>
> **Language:** C11 + C++17
>
> **Build:** Makefile
>
> **Architecture:** Modular
>
> **Goal:** Build an industry-level scientific calculator with parser, evaluator, mathematical engine, unit conversion, plotting, GUI, scripting, plugins, networking, testing, packaging, and extensibility.

---

# Overall Progress

| Phase        | Status       |
| ------------ | ------------ |
| Phase 1–20  | ✅ Completed |
| Phase 21–30 | 🚧 Planned   |

---

# Phase 1 — Project Setup

### Goal

Create the initial project architecture.

### Deliverables

- Makefile
- Src/
- Inc/
- Obj/
- Build/
- README

### Result

A clean modular C project.

---

# Phase 2 — Stack Implementation

### Goal

Implement stack data structures.

### Deliverables

- Double Stack
- Character Stack
- String Stack
- Token Stack

### Files

```
stack.c
stack.h
```

---

# Phase 3 — Tokenizer

### Goal

Convert input into tokens.

### Features

- Numbers
- Operators
- Parentheses
- Variables
- Functions

### Files

```
tokenizer.c
token.h
```

---

# Phase 4 — Expression Validator

### Goal

Reject invalid expressions before parsing.

### Supports

- Parentheses
- Invalid operators
- Invalid numbers
- Missing operands
- Unary minus

### Files

```
validator.c
validator.h
```

---

# Phase 5 — Parser

### Goal

Normalize mathematical expressions.

### Features

- Implicit multiplication
- Variable assignment
- Function recognition
- Expression cleanup

Example

```
2pi

↓

2*pi
```

---

# Phase 6 — Infix → Postfix Conversion

### Goal

Implement Shunting Yard Algorithm.

### Supports

- Operator precedence
- Associativity
- Functions
- Variables
- Constants
- Parentheses

Files

```
postfix.c
```

---

# Phase 7 — Expression Evaluation

### Goal

Evaluate postfix expressions.

### Supports

- + - * / %
- ^
- unary operators
- functions
- variables

Files

```
evaluate.c
```

---

# Phase 8 — Variables

### Goal

Store user variables.

Example

```
x=10
y=20

x+y
```

Supports

- Assignment
- Retrieval
- ans variable

Files

```
variables.c
```

---

# Phase 9 — Scientific Functions

### Unary

```
sin
cos
tan
asin
acos
atan

sqrt
cbrt

log
ln
exp

abs

floor
ceil
round
trunc
```

### Binary

```
pow
max
min
hypot
atan2
```

Files

```
functions.c
function_info.c
```

---

# Phase 10 — Constants

Built-in constants

```
pi
e
```

Automatic replacement

```
sin(pi/2)
```

---

# Phase 11 — History

Features

- Save calculations
- View history
- Clear history

Files

```
history.c
```

---

# Phase 12 — Memory Register

Calculator memory

```
MS
MR
MC
M+
M-
```

Files

```
memory.c
```

---

# Phase 13 — Factorial

Supports

```
5!

(3+2)!

```

Large factorial calculations.

---

# Phase 14 — Function Metadata

Maintain information for every function.

Includes

- argument count
- category
- validation

Files

```
function_info.c
```

---

# Phase 15 — Better Error Handling

Improved diagnostics

Examples

```
Division by zero

Unknown function

Undefined variable

Stack overflow

Stack underflow

Mismatched parentheses
```

---

# Phase 16 — Unit Testing

Tests

- Stack
- Parser
- Validator
- Tokenizer
- Variables
- Functions
- History
- Memory

Folder

```
Tests/
```

---

# Phase 17 — Code Refactoring

Improve

- readability
- modularity
- naming
- comments
- header organization

---

# Phase 18 — Documentation

Documentation written

```
API.md

ARCHITECTURE.md

RULES.md

CHANGELOG.md

DESIGN.md

MEMORY.md

ROADMAP.md

PHASES.md

PRD.md
```

---

# Phase 19 — Performance Optimization

Improve

- tokenizer
- parser
- evaluator

Reduce

- allocations
- copies

Improve

- lookup speed

---

# Phase 20 — Production Ready CLI

Finished

- Interactive menu
- Memory
- Variables
- Scientific functions
- History
- Error handling
- Documentation
- Tests

---

# Phase 21 — Angle Modes

Support

```
mode rad

mode deg

mode grad
```

Functions affected

```
sin
cos
tan

asin
acos
atan
```

---

# Phase 22 — Statistics Engine

Functions

```
mean()

median()

mode()

variance()

stddev()

sum()

product()

percentile()
```

---

# Phase 23 — Unit Conversion

Length

```
km

m

cm

mm

inch

ft

mile
```

Weight

```
kg

g

lb

oz
```

Temperature

```
C

F

K
```

Time

```
sec

min

hour

day
```

Area

Volume

Speed

Pressure

Energy

Power

---

# Phase 24 — Base Conversion

Supports

```
Binary

Octal

Decimal

Hexadecimal
```

Examples

```
bin(25)

hex(255)

oct(64)

dec(1111b)
```

---

# Phase 25 — Complex Numbers

Supports

```
3+4i

sqrt(-1)

polar()

conjugate()

magnitude()

phase()
```

Files

```
Complex.cpp

complex_eval.cpp
```

---

# Phase 26 — Matrix Calculator

Supports

```
Addition

Subtraction

Multiplication

Transpose

Inverse

Determinant

Rank

Identity

LU decomposition
```

Files

```
Matrix.cpp

matrix_eval.cpp
```

---

# Phase 27 — Graph Plotting

CLI Graph

```
plot(sin(x))

plot(x²)

plot(log(x))
```

ASCII renderer

Later

GTK Graph

Qt Graph

---

# Phase 28 — Expression Scripting

Support

```
script.calc

include

variables

loops

macros
```

Batch execution

---

# Phase 29 — GUI Application

Using

- GTK
  or
- Qt

Features

- Scientific keypad
- Plotting
- History panel
- Variables panel
- Memory buttons
- Theme support
- Matrix editor
- Complex calculator

---

# Phase 30 — Professional Release

Final production release.

Includes

- Installer
- Packaging
- CI/CD
- GitHub Actions
- Doxygen documentation
- Benchmarks
- Examples
- Plugin system
- Localization
- Cross-platform support
- Windows
- Linux
- macOS

---

# Final Vision

After completing all 30 phases, the project will provide:

- Arithmetic Calculator
- Scientific Calculator
- Programmer Calculator
- Statistical Calculator
- Matrix Calculator
- Complex Number Calculator
- Unit Converter
- Base Converter
- Graph Plotter
- Expression Language
- CLI Application
- Desktop GUI
- Comprehensive Test Suite
- Professional Documentation
- Production-ready Open Source Project

This roadmap transforms the project from a basic expression evaluator into a complete, extensible scientific computing application suitable for portfolio, educational use, and real-world developmen

# Scientific Calculator Roadmap

**Project:** Scientific Calculator in C/C++

**Version:** 2.0

**Status:** Active Development

---

# Vision

The goal of this project is to build a **production-quality Scientific Calculator** using **C11 and C++17**, evolving from a basic CLI calculator into a modular scientific computation engine with advanced mathematical capabilities, automated testing, comprehensive documentation, and a graphical user interface.

The roadmap is divided into multiple phases. Each phase introduces new capabilities while preserving backward compatibility and maintaining a clean architecture.

---

# Development Philosophy

Every phase should:

- Be independently testable
- Be fully documented
- Follow modular architecture
- Maintain backward compatibility
- Include unit tests
- Update API documentation
- Update changelog
- Keep complexity manageable

---

# Project Status

| Phase    | Title                           | Status       |
| -------- | ------------------------------- | ------------ |
| Phase 1  | Project Setup                   | ✅ Completed |
| Phase 2  | Stack Implementation            | ✅ Completed |
| Phase 3  | Expression Validation           | ✅ Completed |
| Phase 4  | Tokenizer                       | ✅ Completed |
| Phase 5  | Infix to Postfix                | ✅ Completed |
| Phase 6  | Postfix Evaluation              | ✅ Completed |
| Phase 7  | Variables & Constants           | ✅ Completed |
| Phase 8  | History & Memory                | ✅ Completed |
| Phase 9  | Scientific Functions            | ✅ Completed |
| Phase 10 | Multi-Argument Functions        | ✅ Completed |
| Phase 11 | Additional Scientific Functions | ✅ Completed |
| Phase 12 | Angle Mode                      | ⏳ Planned   |
| Phase 13 | Unit Conversion                 | ⏳ Planned   |
| Phase 14 | Complex Numbers                 | ⏳ Planned   |
| Phase 15 | Matrix Engine                   | ⏳ Planned   |
| Phase 16 | Statistics Engine               | ⏳ Planned   |
| Phase 17 | Base Conversion                 | ⏳ Planned   |
| Phase 18 | Advanced History                | ⏳ Planned   |
| Phase 19 | Graph Plotting                  | ⏳ Planned   |
| Phase 20 | Unit Testing Framework          | ✅ Completed |
| Phase 21 | Performance Optimization        | ⏳ Planned   |
| Phase 22 | GUI Application                 | ⏳ Planned   |
| Phase 23 | Plugin System                   | ⏳ Future    |
| Phase 24 | Expression Compiler             | ⏳ Future    |
| Phase 25 | Stable Release v1.0             | ⏳ Future    |

---

# Phase 1 — Project Initialization

## Objective

Create the project structure.

## Features

- Repository setup
- Makefile
- Source folders
- Header folders
- Documentation folders

Deliverables

- Initial project structure
- Build system

Status

✅ Completed

---

# Phase 2 — Stack Library

## Objective

Develop reusable stack data structures.

Implemented

- DoubleStack
- CharStack
- StringStack
- TokenStack

Operations

- Push
- Pop
- Peek
- Empty
- Overflow detection
- Underflow detection

Status

✅ Completed

---

# Phase 3 — Expression Validator

## Objective

Validate expressions before evaluation.

Implemented

- Parentheses validation
- Operator validation
- Operand validation
- Decimal validation
- Syntax validation

Examples

```
5++

↓

Rejected
```

```
((5+2)

↓

Rejected
```

Status

✅ Completed

---

# Phase 4 — Tokenizer

## Objective

Convert raw text into tokens.

Recognizes

- Numbers
- Operators
- Variables
- Constants
- Functions
- Parentheses
- Commas

Status

✅ Completed

---

# Phase 5 — Infix to Postfix

## Objective

Implement the Shunting Yard Algorithm.

Supports

- Operator precedence
- Associativity
- Functions
- Variables
- Constants
- Parentheses

Status

✅ Completed

---

# Phase 6 — Postfix Evaluator

## Objective

Evaluate postfix expressions.

Supports

- Arithmetic
- Unary operators
- Scientific functions
- Variables
- Constants

Status

✅ Completed

---

# Phase 7 — Variables & Constants

Implemented

Variables

```
x=10

radius=5

ans
```

Constants

```
pi

e
```

Status

✅ Completed

---

# Phase 8 — History & Memory

Implemented

History

- Save
- Display
- Clear

Memory

- MS
- MR
- MC
- M+
- M-

Status

✅ Completed

---

# Phase 9 — Scientific Functions

Implemented

Arithmetic

```
sqrt
```

Trigonometry

```
sin

cos

tan

asin

acos
```

Logarithms

```
log

ln
```

Other

```
exp

abs

cbrt
```

Status

✅ Completed

---

# Phase 10 — Multi-Argument Functions

Implemented

```
pow()

max()

min()

hypot()
```

Parser Improvements

- Comma handling
- Function metadata
- Argument counting

Status

✅ Completed

---

# Phase 11 — Additional Scientific Functions

Implemented

- Factorial
- Unary enhancements
- Scientific parser improvements

Future

- gamma()
- erf()
- lgamma()
- remainder()

Status

✅ Completed

---

# Phase 12 — Angle Mode

Objective

Support Degrees and Radians.

Commands

```
mode deg

mode rad
```

Supported Functions

```
sin

cos

tan

asin

acos

atan
```

Example

```
Mode DEG

sin(90)

↓

1
```

Deliverables

- angle_mode.c
- angle_mode.h

Status

⏳ Planned

---

# Phase 13 — Unit Conversion

Categories

Length

Weight

Area

Temperature

Speed

Time

Volume

Examples

```
10km

↓

10000m
```

```
100F

↓

37.8C
```

Status

⏳ Planned

---

# Phase 14 — Complex Numbers

Features

```
3+4i

sqrt(-1)

abs()

arg()

polar()
```

Deliverables

Complex class

Complex evaluator

Status

⏳ Planned

---

# Phase 15 — Matrix Engine

Operations

Addition

Subtraction

Multiplication

Inverse

Transpose

Determinant

Rank

Eigenvalues (future)

Status

⏳ Planned

---

# Phase 16 — Statistics Engine

Functions

```
mean()

median()

mode()

sum()

variance()

stddev()

percentile()
```

Future

Regression

Correlation

Status

⏳ Planned

---

# Phase 17 — Base Conversion

Conversions

Binary

Octal

Decimal

Hexadecimal

Examples

```
bin(25)

↓

11001
```

Status

⏳ Planned

---

# Phase 18 — Advanced History

Features

```
!!

!5

history()

history(20)

clear history
```

Future

Search

Export

Status

⏳ Planned

---

# Phase 19 — Graph Plotting

Console

ASCII plotting

Examples

```
plot(sin(x))
```

Future

GTK plotting

Qt plotting

PNG export

SVG export

Status

⏳ Planned

---

# Phase 20 — Testing Framework

Implemented

Unit Tests

- Stack
- Parser
- Validator
- Tokenizer
- Postfix
- Functions
- Variables
- Memory
- History

Future

Continuous Integration

Coverage reports

Status

✅ Completed

---

# Phase 21 — Performance Optimization

Goals

Replace

Static arrays

↓

Dynamic containers

Optimize

- Parser
- Evaluator
- Variable lookup
- Function lookup

Add

Benchmark suite

Profiling

Status

⏳ Planned

---

# Phase 22 — GUI

Framework

GTK4

Future

Qt

Features

- Scientific keypad
- History panel
- Variable panel
- Memory buttons
- Plot window
- Matrix editor
- Dark mode

Status

⏳ Planned

---

# Phase 23 — Plugin System

Goal

Allow external mathematical modules.

Examples

```
Financial

Physics

Statistics

Engineering
```

Status

⏳ Future

---

# Phase 24 — Expression Compiler

Goal

Compile expressions into reusable bytecode.

Benefits

- Faster repeated evaluation
- Optimization
- Reduced parsing overhead

Status

⏳ Future

---

# Phase 25 — Stable Release v1.0

Requirements

- All major modules complete
- 90%+ unit test coverage
- Complete documentation
- Cross-platform compatibility
- Stable GUI
- Performance optimization complete

Deliverables

- Source code
- Documentation
- Test suite
- Binary releases
- Installation guide
- User manual

Status

⏳ Future

---

# Long-Term Vision (v2.x)

Future ideas beyond v1.0 include:

- Symbolic Algebra
- Equation Solver
- Numerical Integration
- Numerical Differentiation
- Polynomial Solver
- Linear Programming
- Big Integer Support
- Arbitrary Precision Arithmetic
- Scripting Language
- Embedded Calculator Library
- Mobile Application
- Web Version (WebAssembly)
- Plugin Marketplace
- Cloud Synchronization
- Expression Auto-Completion
- LaTeX Formula Rendering
- Spreadsheet Functions
- AI-Assisted Formula Suggestions

---

# Success Metrics

The project will be considered mature when it achieves:

- 100+ mathematical functions
- Matrix and complex number support
- Graph plotting
- Unit conversion
- GUI application
- 90%+ automated test coverage
- Complete documentation
- Cross-platform builds
- Stable v1.0 release

---

# Contribution Guidelines

Every completed phase must include:

- Source code
- Unit tests
- Documentation updates
- API documentation
- Changelog entry
- Memory update
- Roadmap update

No feature is considered complete until all of the above are finished.

---

# Final Goal

Transform this project from a simple command-line calculator into a professional, modular scientific computation platform suitable for education, engineering, research, and open-source collaboration while demonstrating modern C/C++ software engineering practice

Current Version

v1.0

v1.1

Performance

v1.2

GUI

v1.3

Plotting

v2.0

Symbolic Calculator

v2.5

Plugin System

v3.0

IDE

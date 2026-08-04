
# Architecture Documentation

**Project:** Scientific Calculator in C/C++

**Language:** C11 + C++17 (Advanced Modules)

**Architecture Version:** 2.0

---

# Overview

The Scientific Calculator follows a **modular layered architecture**, where each component has a single responsibility and communicates through well-defined public interfaces.

The project is designed to be:

- Modular
- Maintainable
- Extensible
- Testable
- Platform Independent

Instead of writing all logic inside `main.c`, every feature is implemented as an independent module.

---

# High-Level Architecture

```
                 User
                   │
                   ▼
            Console Interface
                 (main.c)
                   │
                   ▼
        ┌────────────────────┐
        │   Input Parser      │
        └────────────────────┘
                   │
                   ▼
        ┌────────────────────┐
        │ Expression Validator│
        └────────────────────┘
                   │
                   ▼
        ┌────────────────────┐
        │ Infix → Postfix     │
        │ (Shunting Yard)     │
        └────────────────────┘
                   │
                   ▼
        ┌────────────────────┐
        │ Postfix Evaluator   │
        └────────────────────┘
                   │
      ┌────────────┼────────────┐
      ▼            ▼            ▼
 Variables     Functions    Constants
      │            │            │
      └────────────┼────────────┘
                   ▼
               Final Result
                   │
        ┌──────────┴──────────┐
        ▼                     ▼
     History               Memory
```

---

# Project Structure

```
Calculator/
```

The project is divided into multiple directories.

---

# Inc/

Contains all public header files.

These files expose APIs used by the source files.

```
Inc/
```

### calculator.h

Core calculator APIs.

Responsibilities

- Infix conversion
- Evaluation
- Parser utilities
- Operator precedence

---

### stack.h

Implements all stack data structures.

Contains

- DoubleStack
- CharStack
- StringStack
- TokenStack

---

### validator.h

Responsible for expression validation.

Checks

- Syntax
- Parentheses
- Invalid operators
- Invalid operands

---

### variables.h

Variable management.

Supports

```
x = 10

radius = 25

ans
```

---

### constants.h

Built-in mathematical constants.

Examples

```
pi

e
```

---

### functions.h

Scientific function declarations.

Supports

```
sqrt

sin

cos

tan

log

pow

max

hypot
```

---

### function_info.h

Metadata about every function.

Stores

- argument count
- function type
- validation data

---

### history.h

Persistent calculation history.

---

### memory.h

Calculator memory register.

Implements

```
MS

MR

MC

M+

M-
```

---

### token.h

Defines tokenizer structures.

```
Token

TokenType
```

---

### angle_mode.h

Implemented (Phase 21). Global degree/radian mode for trig functions.

Modes

```
DEG

RAD
```

---

### stats.h

Implemented (Phase 24). Statistics engine.

Supports

```
mean

median

sum, min, max, count

stddev, variance     (sample, divides by n-1)

pstddev, pvariance   (population, divides by n)
```

---

### units.h

Implemented (Phase 22). Unit conversion engine.

Supports

```
Length

Weight

Temperature

Time
```

---

### base.h

Implemented (Phase 23). Number base conversion.

Supports

```
Binary

Octal

Decimal

Hexadecimal
```

---

### plot.h

Implemented (Phase 27). ASCII terminal graph plotting. A graphical
(GUI) plot view is future work, tracked under Phase 31 (GUI), not a
separate plotting phase.

---

### Complex.hpp

Implemented (Phase 25). Complex number implementation (C++ class with
operator overloading).

---

### Matrix.hpp

Implemented (Phase 26). Matrix class, backed by
`std::vector<std::vector<double>>`.

---

### complex_eval.h

Complex expression evaluator.

---

### matrix_eval.h

Matrix operations.

---

# Src/

Contains implementation files.

Each source file implements one module.

---

## main.c

Program entry point.

Responsibilities

- Menu
- User interaction
- Dispatch
- Calling APIs

No mathematical logic should exist here.

---

## parser.c

Preprocesses expressions.

Responsibilities

- Remove extra spaces
- Insert implicit multiplication
- Normalize expressions

Example

```
2pi

↓

2*pi
```

---

## validator.c

Checks whether an expression is valid before evaluation.

---

## tokenizer.c

Splits expressions into tokens.

Recognizes

- Numbers
- Operators
- Variables
- Constants
- Functions

---

## postfix.c

Implements the **Shunting Yard Algorithm**.

Converts

```
Infix

↓

Postfix
```

---

## evaluate.c

Evaluates postfix expressions.

Uses

- Stack
- Function engine
- Variables
- Constants

---

## stack.c

Implements all stack operations.

---

## variables.c

Stores user variables.

Supports

```
setVariable()

getVariable()

showVariables()
```

---

## constants.c

Stores mathematical constants.

```
pi

e
```

---

## functions.c

Scientific function implementations.

Unary

```
sqrt

sin

cos

tan

ln

log
```

Binary

```
pow

max

min

hypot
```

---

## function_info.c

Stores metadata.

Example

```
pow

↓

2 arguments
```

---

## memory.c

Memory register implementation.

---

## history.c

Persistent history.

File

```
Build/history.txt
```

---

## angle_mode.c

Implemented.

Responsible for

```
Degrees

Radians
```

---

## stats.c

Implemented. Statistics engine.

---

## units.c

Implemented. Unit conversion.

---

## base.c

Implemented. Number base conversion.

---

## plot.c

ASCII graph plotting.

---

## Complex.cpp

Complex arithmetic.

---

## Matrix.cpp

Matrix implementation.

---

## complex_eval.cpp

Complex expression evaluator.

---

## matrix_eval.cpp

Matrix calculations.

---

# Tests/

Contains independent unit tests.

Each module has a dedicated test.

```
test_stack.c

test_parser.c

test_validator.c

test_postfix.c

test_functions.c

test_variables.c

test_history.c

test_memory.c
```

Purpose

- Regression testing
- Automated validation
- Bug prevention

---

# docs/

Project documentation.

Contains

```
API

Architecture

Design

Roadmap

Memory

PRD

Rules

Testing

Contributing

Changelog
```

---

# Execution Flow

```
User Input

↓

Parser

↓

Validator

↓

Tokenizer

↓

Postfix Converter

↓

Evaluator

↓

Functions

↓

Variables

↓

Constants

↓

Answer

↓

History

↓

Memory
```

---

# Data Flow

```
Expression

↓

Parser

↓

Normalized Expression

↓

Validator

↓

Postfix Converter

↓

Postfix Expression

↓

Stack Evaluation

↓

Result
```

---

# Dependency Graph

```
main

│

├── parser

├── validator

├── postfix

│     │

│     ├── stack

│     ├── token

│     └── function_info

│

├── evaluate

│     │

│     ├── functions

│     ├── variables

│     ├── constants

│     └── stack

│

├── history

└── memory
```

---

# Design Principles

The architecture follows several software engineering principles.

## Single Responsibility Principle

Each module performs only one task.

Examples

```
stack.c

↓

Only stack operations.
```

```
history.c

↓

Only history.
```

---

## Encapsulation

Modules expose only their public API through header files.

Internal implementation remains hidden.

---

## Loose Coupling

Modules communicate through function interfaces.

No module directly manipulates another module's internal data.

---

## High Cohesion

All related functionality stays together.

Example

```
functions.c

contains only mathematical functions.
```

---

## Reusability

Modules are designed to be reusable in other projects.

Examples

- Stack library
- Validator
- Tokenizer

---

# Build Architecture

Compilation Flow

```
Source Files

↓

Compiler

↓

Object Files

↓

Linker

↓

calculator
```

Managed by

```
Makefile
```

---

# Error Handling Strategy

The project uses defensive programming.

Errors detected include:

- Stack Overflow
- Stack Underflow
- Division by Zero
- Undefined Variables
- Invalid Functions
- Invalid Numbers
- Invalid Operators
- Invalid Parentheses
- Misplaced Commas

As of the 2026-08-04 error-handling migration (see
`docs/CHANGELOG.md`), there are **zero `exit()` calls anywhere in
`Src/`** — every error path above returns a status code (`0`/`NAN`
+ `calculatorSetLastError()`) instead of terminating the process. This
replaces an earlier version of this document that described fatal
errors as calling `exit(EXIT_FAILURE)`, which was true of the code at
one point but is not anymore.

---

# Future Architecture

Angle Mode, Statistics, Matrix, Complex Number, Unit Conversion, Base
Conversion, and the ASCII Graph Plotter are all implemented (Phases
21–27 — see `docs/PHASES.md`). Genuinely upcoming modules:

- GUI (Qt — Phase 31)
- Plugin System (Phase 32)
- Scripting (Phase 33)

The modular architecture allows these features to be added with minimal changes to the existing codebase.

---

# Technology Stack

| Layer            | Technology                              |
| ---------------- | --------------------------------------- |
| Language         | C11                                     |
| Advanced Modules | C++17                                   |
| Build System     | GCC + Make                              |
| Math Library     | libm (`-lm`)                          |
| Version Control  | Git                                     |
| Testing          | Custom Unit Test Framework              |
| Documentation    | Markdown                                |
| Future GUI       | Qt (Phase 31)                           |
| Platform         | Linux (Primary), Cross-platform capable |

---

# Architecture Goals

- Clean and modular codebase
- Easy to extend with new mathematical features
- Independent modules with minimal coupling
- Comprehensive documentation
- High test coverage
- Production-ready project structure
- Scalable foundation for future GUI and advanced mathematical engines


# Calculator Project Memory

Version: 2.0
Status: Phase 27 Completed (CLI v1.0) — see docs/PHASES.md
Language: C/C++
Architecture: Modular
Build System: Makefile

---

# Project Status

The calculator has successfully evolved from a simple arithmetic calculator into a modular scientific calculator.

Current development has completed **Phase 20**.

The project is stable and organized into separate modules.

---

# Completed Phases

---

# Phase 1

## Project Setup

Completed

Features

- Created project folder structure
- Created Makefile
- Added Inc/
- Added Src/
- Added Obj/
- Added Build/
- GCC build support

Files

Makefile

---

# Phase 2

## Stack Implementation

Completed

Implemented

DoubleStack

Operations

- init
- push
- pop
- peek
- empty check

CharStack

Operations

- init
- push
- pop
- peek
- empty check

Later Added

StringStack

Later Added

TokenStack

Files

stack.c
stack.h

Status

Stable

---

# Phase 3

## Tokenizer

Completed

Features

- number detection
- decimal detection
- operator detection
- unary minus support
- identifier detection

Files

tokenizer.c

Status

Stable

---

# Phase 4

## Infix to Postfix Conversion

Completed

Implemented

Shunting Yard Algorithm

Supports

/

%

^

parentheses

Variables

Functions

Comma handling

Unary minus

Operator precedence

Operator associativity

Files

postfix.c

Status

Stable

---

# Phase 5

## Postfix Evaluation

Completed

Supports

/

%

^

Unary functions

Binary functions

Variables

Constants

Factorial

Files

evaluate.c

Status

Stable

---

# Phase 6

## Expression Validation

Completed

Checks

Invalid operator

Invalid operand

Missing operand

Mismatched parentheses

Invalid decimal numbers

Expression ending with operator

Files

validator.c

Status

Stable

---

# Phase 7

## Parser

Completed

Features

Implicit multiplication

Examples

2(3+4)

2pi

3sqrt(16)

(pi+2)(4)

Parser inserts

only where required.

Files

parser.c

Status

Stable

---

# Phase 8

## History

Completed

Features

Store calculation

View history

Clear history

Persistent file

Build/history.txt

Files

history.c

history.h

Status

Stable

---

# Phase 9

## Memory Register

Completed

Supports

MS

MR

MC

M+

M-

Files

memory.c

memory.h

Status

Stable

---

# Phase 10

## Variables

Completed

Supports

Assignment

Example

x=5

y=x+2

area=pi*r^2

Built-in variable

ans

Features

Store

Update

Read

Display

Files

variables.c

variables.h

Status

Stable

---

# Phase 11

## Mathematical Constants

Completed

Supports

pi

e

Files

constants.c

constants.h

Status

Stable

---

# Phase 12

## Scientific Functions

Completed

Unary Functions

sqrt

sin

cos

tan

asin

acos

log

ln

exp

abs

cbrt

Files

functions.c

functions.h

Status

Stable

---

# Phase 13

## Binary Functions

Completed

Supports

pow

max

min

hypot

Function metadata system

Function argument counting

Files

function_info.c

function_info.h

functions.c

Status

Stable

---

# Phase 14

## Function Metadata

Completed

Added

Function lookup

Argument count

Validation

Allows evaluator to determine

Unary

Binary

Future N-ary functions

Files

function_info.c

Status

Stable

---

# Phase 15

## Token System

Completed

Introduced Token structure

Token Types

Operator

Function

Left Parenthesis

Comma

TokenStack

Files

token.h

stack.c

stack.h

Status

Stable

---

# Phase 16

## Main Application

Completed

Menu

New Calculation

History

Clear History

Memory

Variables

Exit

Assignment support

Expression evaluation

Persistent ans

Files

main.c

Status

Stable

---

# Phase 17

## Error Handling

Completed

Implemented

Division by zero

Undefined variable

Unknown function

Stack underflow

Stack overflow

Invalid number

Mismatched parentheses

Misplaced comma

Invalid operator

Status

Stable

---

# Phase 18

## Code Refactoring

Completed

Separated

Parser

Validator

Tokenizer

Functions

Variables

History

Memory

Evaluation

Postfix conversion

Reduced duplicated logic

Improved readability

Status

Completed

---

# Phase 19

## Build System

Completed

Improved Makefile

Supports

make

make clean

make rebuild

make run

Object directory

Include directory

Math library

Status

Stable

---

# Phase 20

## Project Documentation

Completed

Created documentation

PRD

Architecture

Rules

Design

Roadmap

Memory

Contributing

Test Plan

API

Changelog

Project now follows documentation-first development.

Status

Completed

---

# Current Folder Structure


```
📁 Calculator
│   ├── 📁 Inc
│   │   ├── ⚡ Complex.hpp
│   │   ├── ⚡ Matrix.hpp
│   │   ├── ⚡ angle_mode.h
│   │   ├── ⚡ base.h
│   │   ├── ⚡ calculator.h
│   │   ├── ⚡ complex_eval.h
│   │   ├── ⚡ constants.h
│   │   ├── ⚡ function_info.h
│   │   ├── ⚡ functions.h
│   │   ├── ⚡ history.h
│   │   ├── ⚡ matrix_eval.h
│   │   ├── ⚡ memory.h
│   │   ├── ⚡ plot.h
│   │   ├── ⚡ stack.h
│   │   ├── ⚡ stats.h
│   │   ├── ⚡ token.h
│   │   ├── ⚡ units.h
│   │   ├── ⚡ validator.h
│   │   └── ⚡ variables.h
│   ├── 📁 Src
│   │   ├── ⚡ Complex.cpp
│   │   ├── ⚡ Matrix.cpp
│   │   ├── 📄 angle_mode.c
│   │   ├── 📄 base.c
│   │   ├── ⚡ complex_eval.cpp
│   │   ├── 📄 constants.c
│   │   ├── 📄 evaluate.c
│   │   ├── 📄 function_info.c
│   │   ├── 📄 functions.c
│   │   ├── 📄 history.c
│   │   ├── 📄 main.c
│   │   ├── ⚡ matrix_eval.cpp
│   │   ├── 📄 memory.c
│   │   ├── 📄 parser.c
│   │   ├── 📄 plot.c
│   │   ├── 📄 postfix.c
│   │   ├── 📄 stack.c
│   │   ├── 📄 stats.c
│   │   ├── 📄 tokenizer.c
│   │   ├── 📄 units.c
│   │   ├── 📄 validator.c
│   │   └── 📄 variables.c
│   ├── 📁 Tests
│   │   ├── ⚡ test_framework.h
│   │   ├── 📄 test_functions.c
│   │   ├── 📄 test_history.c
│   │   ├── 📄 test_main.c
│   │   ├── 📄 test_memory.c
│   │   ├── 📄 test_parser.c
│   │   ├── 📄 test_postfix.c
│   │   ├── 📄 test_stack.c
│   │   ├── 📄 test_tokenizer.c
│   │   ├── 📄 test_validator.c
│   │   ├── 📄 test_variables.c
│   │   └── ⚡ tests.h
│   ├── 📁 docs
│   │   ├── 📝 API.md
│   │   ├── 📝 ARCHITECTURE.md
│   │   ├── 📝 CHANGELOG.md
│   │   ├── 📝 CONTRIBUTING.md
│   │   ├── 📝 DESIGN.md
│   │   ├── 📝 MEMORY.md
│   │   ├── 📝 PHASES.md
│   │   ├── 📝 PRD.md
│   │   ├── 📝 ROADMAP.md
│   │   ├── 📝 RULES.md
│   │   └── 📝 TEST_PLAN.md
│   ├── ⚙️ .gitignore
│   ├── 📄 Makefile
│   ├── 📝 Readme.md
│   ├── 📄 calculator
│   └── 📄 run_tests
```

---

# Current Supported Features

Arithmetic

✓ Addition

✓ Subtraction

✓ Multiplication

✓ Division

✓ Power

✓ Modulus

✓ Factorial

Parentheses

Variables

Constants

Functions

Binary functions

History

Memory

Validation

Implicit multiplication

Persistent ans

Scientific calculator

---

# Known Working Expressions

5+5

2*(3+4)

2(3+4)

2pi

pi/2

sin(pi/2)

sqrt(16)

3sqrt(16)

pow(2,8)

max(5,10)

min(5,2)

hypot(3,4)

x=10

y=20

x+y

ans*5

---

# Current Architecture Status

Parser

Complete

Validator

Complete

Tokenizer

Complete

Postfix Converter

Complete

Evaluator

Complete

Variable System

Complete

Function System

Complete

History

Complete

Memory

Complete

Main Menu

Complete

Documentation

Complete

Angle Mode, Unit Conversion, Base Conversion, Statistics, Complex
Numbers, Matrix Operations, Graph Plotting

Complete (Phases 21–27)

---

# Remaining Roadmap

This section used to list Angle Mode, Statistics, Matrix Support,
Complex Numbers, Base Conversion, Unit Conversion, and Graph Plotting
as remaining work under a phase numbering that never matched
`docs/PHASES.md`. All seven shipped (Phases 21–27). Current source of
truth for phase status is `docs/PHASES.md`; summary in
`docs/ROADMAP.md`. What's actually still remaining:

Phase 28 — Dynamic Data Structures

Phase 29 — Abstract Syntax Tree (AST)

Phase 30 — Symbolic Mathematics

Phase 31 — Graphical User Interface (Qt)

Phase 32 — Plugin Architecture

Phase 33 — Calculator Scripting

Phase 34 — Package Manager

Phase 35 — Cross Platform

Phase 36 — Version 2.0 Release

---

# Current Stability

Compilation

PASS

Runtime

PASS

Memory

PASS

Parser

PASS

Scientific Functions

PASS

Variables

PASS

History

PASS

Documentation

PASS

Overall Project Status

v1.0 (CLI engine) is complete: Phases 1–27, including statistics,
matrices, complex numbers, and graphing — see `docs/PHASES.md`. The
remaining phases (28–36) are GUI and beyond, not fixes to the existing
architecture.

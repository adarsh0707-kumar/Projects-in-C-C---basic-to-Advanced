
# Advanced Scientific Calculator in C/C++

![Language](<https://img.shields.io/badge/Language-C%20%7C%20C%2B%2B-blue>)
![Platform](https://img.shields.io/badge/Platform-Linux-green)
![Compiler](https://img.shields.io/badge/Compiler-GCC-orange)
![Build](https://img.shields.io/badge/Build-Make-red)
![License](https://img.shields.io/badge/License-MIT-brightgreen)

A modular, extensible, and production-style **Scientific Calculator** written in **C/C++**, designed to demonstrate advanced programming concepts including expression parsing, postfix evaluation, scientific functions, variables, memory operations, unit conversion, statistics, complex numbers, matrices, graph plotting, and comprehensive testing.

This project is intended as both a learning resource and a portfolio-quality software engineering project.

---

# Table of Contents

- Introduction
- Features
- Folder Structure
- Architecture
- Requirements
- Installation
- Build
- Usage
- Documentation
- Testing
- Roadmap
- Contributing
- License

---

# Introduction

The Advanced Scientific Calculator goes beyond basic arithmetic by implementing a complete mathematical expression engine.

It supports:

- Expression parsing
- Infix to postfix conversion
- Stack-based evaluation
- Scientific functions
- Variables
- Constants
- Memory registers
- History
- Statistics
- Unit conversion
- Complex numbers
- Matrix operations
- Graph plotting
- Modular architecture
- Automated testing

---

# Features

## Basic Calculator

- Addition
- Subtraction
- Multiplication
- Division
- Modulus
- Power
- Parentheses
- Unary minus
- Decimal support

---

## Scientific Functions

- sqrt()
- cbrt()
- sin()
- cos()
- tan()
- asin()
- acos()
- atan()
- sinh()
- cosh()
- tanh()
- log()
- ln()
- exp()
- abs()
- floor()
- ceil()
- round()
- trunc()
- gamma()

---

## Binary Functions

- pow()
- max()
- min()
- hypot()
- atan2()

---

## Variables

```text
x = 10
y = 20

x + y
```

---

## Built-in Constants

```text
pi
e
```

---

## Memory Operations

- MS
- MR
- MC
- M+
- M-

---

## Expression History

- Save calculations
- View history
- Clear history

---

## Statistics

- Mean
- Median
- Mode
- Variance
- Standard Deviation

---

## Unit Conversion

- Length
- Weight
- Temperature
- Time

---

## Number Base Conversion

- Binary
- Decimal
- Octal
- Hexadecimal

---

## Angle Modes

- Degree
- Radian

---

## Complex Numbers

- Addition
- Subtraction
- Multiplication
- Division
- Magnitude
- Conjugate

---

## Matrix Operations

- Addition
- Multiplication
- Determinant
- Transpose
- Inverse

---

## Plotting

ASCII graph plotting from terminal.

---

# Folder Structure

```text
📁 Calculator
│
├── 📁 Inc
│   ├── Complex.hpp
│   ├── Matrix.hpp
│   ├── angle_mode.h
│   ├── base.h
│   ├── calculator.h
│   ├── complex_eval.h
│   ├── constants.h
│   ├── function_info.h
│   ├── functions.h
│   ├── history.h
│   ├── matrix_eval.h
│   ├── memory.h
│   ├── plot.h
│   ├── stack.h
│   ├── stats.h
│   ├── token.h
│   ├── units.h
│   ├── validator.h
│   └── variables.h
│
├── 📁 Src
│   ├── Complex.cpp
│   ├── Matrix.cpp
│   ├── angle_mode.c
│   ├── base.c
│   ├── complex_eval.cpp
│   ├── constants.c
│   ├── evaluate.c
│   ├── function_info.c
│   ├── functions.c
│   ├── history.c
│   ├── main.c
│   ├── matrix_eval.cpp
│   ├── memory.c
│   ├── parser.c
│   ├── plot.c
│   ├── postfix.c
│   ├── stack.c
│   ├── stats.c
│   ├── tokenizer.c
│   ├── units.c
│   ├── validator.c
│   └── variables.c
│
├── 📁 Tests
│   ├── test_framework.h
│   ├── test_functions.c
│   ├── test_history.c
│   ├── test_main.c
│   ├── test_memory.c
│   ├── test_parser.c
│   ├── test_postfix.c
│   ├── test_stack.c
│   ├── test_tokenizer.c
│   ├── test_validator.c
│   ├── test_variables.c
│   └── tests.h
│
├── 📁 docs
│   ├── API.md
│   ├── ARCHITECTURE.md
│   ├── CHANGELOG.md
│   ├── CONTRIBUTING.md
│   ├── DESIGN.md
│   ├── MEMORY.md
│   ├── PHASES.md
│   ├── PRD.md
│   ├── ROADMAP.md
│   ├── RULES.md
│   └── TEST_PLAN.md
│
├── .gitignore
├── Makefile
├── README.md
├── calculator
└── run_tests
```

---

# Architecture

The calculator follows a modular layered architecture.

```
User Input
      │
      ▼
Parser
      │
      ▼
Validator
      │
      ▼
Tokenizer
      │
      ▼
Postfix Generator
      │
      ▼
Evaluator
      │
      ▼
Functions
Variables
Memory
History
Statistics
Units
Matrices
Complex Numbers
      │
      ▼
Output
```

---

# Requirements

- GCC
- G++
- GNU Make
- Linux (recommended)

---

# Build

Compile the project:

```bash
make
```

Clean build files:

```bash
make clean
```

Rebuild:

```bash
make rebuild
```

Run:

```bash
./calculator
```

---

# Usage

Example:

```text
> 2+3*5

17
```

Scientific:

```text
sqrt(16)

4
```

Variables:

```text
x=5
y=10
x+y

15
```

Functions:

```text
pow(2,8)

256
```

Constants:

```text
pi
e
```

Memory:

```text
MS
MR
MC
```

---

# Testing

Run all unit tests:

```bash
./run_tests
```

Current test modules include:

- Stack
- Tokenizer
- Validator
- Parser
- Postfix
- Functions
- Variables
- Memory
- History

---

# Documentation

Complete project documentation is available inside the `docs/` directory.

| File            | Description                  |
| --------------- | ---------------------------- |
| PRD.md          | Product Requirement Document |
| ARCHITECTURE.md | Software Architecture        |
| DESIGN.md       | Design Decisions             |
| API.md          | Public API Reference         |
| MEMORY.md       | Development Memory           |
| CHANGELOG.md    | Project History              |
| ROADMAP.md      | Future Plans                 |
| PHASES.md       | Development Phases           |
| TEST_PLAN.md    | Testing Strategy             |
| CONTRIBUTING.md | Contribution Guide           |
| RULES.md        | Development Rules            |

---

# Development Roadmap

The calculator is planned across **30 development phases**, including:

- Basic Calculator
- Expression Parser
- Scientific Functions
- Variables
- Constants
- Memory System
- Multi-Argument Functions
- Statistics
- Unit Conversion
- Base Conversion
- Complex Numbers
- Matrix Algebra
- Graph Plotting
- GUI
- Plugin System
- Performance Optimization
- Packaging
- Continuous Integration

Refer to **docs/ROADMAP.md** and **docs/PHASES.md** for complete details.

---

# Contributing

Contributions are welcome!

Please read:

- `docs/CONTRIBUTING.md`
- `docs/RULES.md`

before submitting a Pull Request.

---

# License

This project is licensed under the **MIT License**.

---

# Author

**Adarsh Kumar**

GitHub: **https://github.com/adarsh0707-kumar**

---

## Support

If you found this project helpful:

- Star the repository
- Fork the project
- Contribute new features
- Improve documentation

Your support helps the project grow and benefits the open-source community.

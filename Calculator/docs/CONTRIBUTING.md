# Contributing Guide

Welcome to the **Advanced Scientific Calculator** project!

Thank you for your interest in contributing. Every contribution—whether it is fixing a bug, improving documentation, adding a new feature, or optimizing the code—is greatly appreciated.

This document explains the workflow, coding standards, project structure, and contribution process.

---

# Table of Contents

1. Project Vision
2. Ways to Contribute
3. Development Workflow
4. Repository Structure
5. Coding Standards
6. Commit Message Guidelines
7. Branch Naming
8. Pull Request Process
9. Code Review Checklist
10. Testing Requirements
11. Documentation Requirements
12. Feature Development Guidelines
13. Bug Report Guidelines
14. Security Policy
15. Future Contributions

---

# Project Vision

The goal of this project is to build a **professional, modular, scientific calculator** in C/C++ that demonstrates:

- Data Structures
- Parsing Algorithms
- Expression Evaluation
- Scientific Computing
- Unit Testing
- Clean Software Architecture
- Documentation
- Cross-platform Development

The project is designed to be educational while maintaining production-quality coding standards.

---

# Ways to Contribute

You can contribute by:

- Fixing bugs
- Adding scientific functions
- Improving parser accuracy
- Improving tokenizer
- Improving documentation
- Writing unit tests
- Optimizing algorithms
- Improving memory management
- Adding GUI features
- Performance optimization
- Code refactoring
- Adding examples
- Improving README

---

# Development Workflow

1. Fork the repository.
2. Clone your fork.

```bash
git clone https://github.com/adarsh0707-kumar/Projects-in-C-C---basic-to-Advanced/Calculator
```

3. Create a new branch.

```bash
git checkout -b feature/my-feature
```

4. Make your changes.
5. Build the project.

```bash
make
```

6. Run all tests.

```bash
./run_tests
```

7. Commit your changes.
8. Push your branch.

```bash
git push origin feature/my-feature
```

9. Open a Pull Request.

---

# Repository Structure

```
Calculator/
│
├── Inc/
├── Src/
├── Tests/
├── docs/
├── Build/
├── Obj/
├── Makefile
└── README.md
```

Each folder has a dedicated purpose.

Do not place files in incorrect directories.

---

# Coding Standards

## General Rules

- Use meaningful variable names.
- Use modular programming.
- Keep functions small.
- Avoid duplicate code.
- Follow consistent indentation.
- Use descriptive comments.
- Keep files organized.

---

## Formatting

Use four spaces for indentation.

Example:

```c
if (value > 0)
{
    printf("Positive");
}
```

---

## Naming Convention

Functions

```c
evaluatePostfix()
```

Variables

```c
lastResult
```

Structures

```c
TokenStack
```

Enums

```c
TokenType
```

Constants

```c
MAX_STACK_SIZE
```

Macros

```c
MAX_VARIABLES
```

---

# File Organization

One module should contain:

```
header (.h)

implementation (.c/.cpp)
```

Example

```
functions.h
functions.c
```

---

# Commit Message Guidelines

Use Conventional Commits.

Examples

```
feat(parser): support unary operators

fix(history): resolve duplicate entries

docs(api): update API documentation

refactor(stack): simplify stack operations

test(parser): add parser unit tests
```

---

# Branch Naming

Feature

```
feature/statistics
```

Bug Fix

```
fix/parser-bug
```

Documentation

```
docs/api
```

Testing

```
test/parser
```

Refactoring

```
refactor/tokenizer
```

---

# Pull Request Process

Each Pull Request should include:

- Purpose of change
- Related issue
- Testing performed
- Screenshots (if GUI)
- Documentation updates

---

# Pull Request Checklist

Before submitting:

- Code builds successfully
- Tests pass
- Documentation updated
- No compiler warnings
- No memory leaks
- Code follows style guide
- Commit messages are clean

---

# Testing Requirements

Every new feature should include tests.

Examples:

Parser

```
2+3
```

Scientific Functions

```
sqrt(16)
```

Variables

```
x=5
x+2
```

History

```
history
```

Memory

```
MS
MR
MC
```

---

# Documentation Requirements

If you add:

- New module
- New function
- New feature
- New API

Then update:

- API.md
- CHANGELOG.md
- ROADMAP.md
- MEMORY.md
- TEST_PLAN.md
- README.md (if applicable)

---

# Feature Development Guidelines

When adding a feature:

1. Create header file declarations.
2. Implement source file.
3. Update parser if needed.
4. Update evaluator.
5. Add tests.
6. Update documentation.
7. Update changelog.

---

# Error Handling

Always validate:

- Null pointers
- Stack overflow
- Stack underflow
- Division by zero
- Invalid syntax
- Invalid arguments
- Invalid variables
- Invalid function names

Never ignore runtime errors.

---

# Performance Guidelines

Avoid:

- Duplicate parsing
- Unnecessary memory allocation
- Large global variables
- Deep nesting
- Repeated calculations

Prefer:

- Reusable functions
- Efficient algorithms
- Modular architecture

---

# Documentation Style

Documentation should be:

- Clear
- Consistent
- Accurate
- Updated
- Beginner-friendly

---

# Security Guidelines

Do not:

- Use unsafe string functions without bounds checking.
- Trust user input.
- Ignore buffer sizes.
- Access invalid memory.

Always validate external input before processing.

---

# Issue Reporting

A good issue report should include:

- Operating System
- Compiler Version
- Calculator Version
- Steps to Reproduce
- Expected Result
- Actual Result
- Screenshots (if applicable)

---

# Future Contributions

Graph Plotting, Matrix Algebra, Complex Numbers, Statistics Engine,
and Unit Converter have shipped (Phases 21–27). Contributions are
welcome for the remaining future phases:

- Dynamic Data Structures
- Abstract Syntax Tree (AST)
- Symbolic Mathematics
- Qt GUI
- Expression Optimizer
- Performance Improvements
- Plugin Architecture
- Calculator Scripting

Refer to **ROADMAP.md** and **PHASES.md** before starting work on new features.

---

# Code of Conduct

Please be respectful and constructive.

- Be welcoming.
- Provide helpful feedback.
- Respect coding standards.
- Focus on technical discussions.
- Help improve the project.

---

# License

By contributing to this project, you agree that your contributions will be licensed under the same license as the project.

---

Thank you for contributing to the **Advanced Scientific Calculator** project. Your efforts help make this project more robust, maintainable, and useful for the developer communit

Project Setup

Clone

Build

Run Tests

Coding Standards

Commit Message Format

Branch Naming

Code Review Checklist

Issue Reporting

Pull Request Rules

Documentation Requirements

# Software Design Document (SDD)

**Project:** Scientific Calculator in C/C++

**Version:** 2.0

**Language:** C11 + C++17

---

# Purpose

This document describes the overall software design of the Scientific Calculator.

It explains:

- Software design philosophy
- Module design
- Data structures
- Algorithms
- UI design
- Coding standards
- Extensibility
- Future design goals

This document complements the Architecture Document by focusing on implementation decisions rather than module relationships.

---

# Design Goals

The calculator has been designed with the following goals:

- Clean architecture
- Modular codebase
- High readability
- Easy maintenance
- Easy testing
- Reusable components
- Extensible mathematical engine
- Minimal code duplication
- Cross-platform compatibility

---

# Design Principles

## 1. Modularity

Every feature is implemented in its own module.

Examples:

```
stack.c
history.c
memory.c
functions.c
variables.c
```

Each module has:

- One header file
- One implementation file
- One responsibility

---

## 2. Single Responsibility Principle

Each source file performs only one task.

Example

```
history.c

↓

Stores and retrieves history only.
```

Never mix:

- Parsing
- Evaluation
- Memory
- History

inside the same module.

---

## 3. Encapsulation

All internal implementation remains private.

Only public APIs are exposed through header files.

Example

```
variables.c

↓

Private storage

↓

variables.h

↓

Public APIs
```

---

## 4. Separation of Concerns

Every stage of expression processing is separated.

```
Input

↓

Parser

↓

Validator

↓

Postfix Converter

↓

Evaluator

↓

Result
```

Each stage is independent.

---

## 5. Reusability

Modules should work independently.

Examples

- Stack library
- Validator
- Tokenizer

can be reused in other projects.

---

## 6. Extensibility

The calculator should support future additions without changing existing code.

Future modules include:

- Statistics
- Matrix
- Complex Numbers
- Plotting
- GUI
- Unit Conversion

---

# System Design

The calculator follows a layered design.

```
Presentation Layer

↓

Business Logic Layer

↓

Utility Layer

↓

Storage Layer
```

---

## Presentation Layer

Responsible for user interaction.

Current implementation

```
Console Menu
```

Future

```
GTK GUI

Qt GUI
```

Responsibilities

- Read user input
- Display output
- Menu navigation

No mathematical calculations occur here.

---

## Business Logic Layer

Contains all mathematical logic.

Modules

```
Parser

Validator

Postfix

Evaluator

Functions

Variables

Constants
```

---

## Utility Layer

Provides helper functionality.

Includes

```
Stacks

Tokenizer

Function Metadata

Angle Mode
```

---

## Storage Layer

Responsible for persistent data.

Includes

```
History

Memory

Variables
```

---

# Module Design

## Parser

Responsibilities

- Remove unnecessary spaces
- Insert implicit multiplication
- Normalize expressions

Example

```
2pi

↓

2*pi
```

---

## Validator

Checks expression correctness.

Detects

- Missing operands
- Missing operators
- Invalid commas
- Invalid parentheses
- Invalid numbers

---

## Tokenizer

Splits expression into logical tokens.

Recognizes

- Numbers
- Operators
- Functions
- Variables
- Constants
- Parentheses
- Commas

Example

```
pow(2,8)

↓

pow

(

2

,

8

)
```

---

## Postfix Converter

Uses the **Shunting Yard Algorithm**.

Converts

```
Infix

↓

Postfix
```

Example

```
3+4*5

↓

3 4 5 * +
```

---

## Evaluator

Evaluates postfix expressions.

Uses

- Stack
- Functions
- Variables
- Constants

Returns final answer.

---

## Function Engine

Responsible for all mathematical functions.

Unary

```
sqrt

sin

cos

tan

log
```

Binary

```
pow

max

min

hypot
```

Future

```
mean

median

gamma

fact

matrix

complex
```

---

## Variable Engine

Stores user-defined variables.

Examples

```
x=10

radius=25

ans
```

---

## Constant Engine

Stores immutable constants.

```
pi

e
```

Future

```
phi

tau
```

---

## History Module

Stores completed calculations.

Location

```
Build/history.txt
```

Future

Search

Filtering

Export

---

## Memory Module

Implements calculator memory.

Supports

```
MS

MR

MC

M+

M-
```

---

# Data Structures

The project uses lightweight data structures.

---

## DoubleStack

Purpose

Store operands.

```
5

10

25
```

---

## CharStack

Purpose

Store operators.

```
+

-

*

/
```

---

## TokenStack

Purpose

Store parser tokens.

```
Function

Operator

Parenthesis
```

---

## StringStack

Purpose

Store function names.

```
sqrt

pow

sin
```

---

## Variable

```
name

value
```

---

## Token

Contains

```
Token Type

Token Text
```

---

# Algorithms Used

## Expression Parsing

Linear Scan

Complexity

```
O(n)
```

---

## Parentheses Validation

Stack Algorithm

Complexity

```
O(n)
```

---

## Infix to Postfix

Shunting Yard Algorithm

Complexity

```
O(n)
```

---

## Postfix Evaluation

Stack-based evaluation

Complexity

```
O(n)
```

---

## Variable Lookup

Current

Linear Search

```
O(n)
```

Future

Hash Table

```
O(1)
```

---

## Function Lookup

Current

Sequential comparison

Future

Lookup table

Binary search

Hash map

---

# User Interface Design

Current interface

Console

```
===== CALCULATOR =====

1. New Calculation

2. History

3. Memory

4. Variables

5. Exit
```

Future interface

GTK

Qt

Touch Friendly

---

# Error Handling Design

Errors are detected as early as possible.

Examples

```
Division by zero

↓

Evaluation
```

```
Unknown variable

↓

Tokenizer
```

```
Missing parenthesis

↓

Validator
```

```
Stack overflow

↓

Stack Module
```

---

# Memory Management

Current

Static allocation.

Examples

```
Stack

Variables

Tokens
```

Advantages

- Fast
- Simple
- Predictable

Future

Dynamic allocation

Resizable containers

---

# Coding Standards

Every function should

- Perform one task
- Validate input
- Return meaningful values
- Avoid hidden side effects

Variable names

```
camelCase
```

Constants

```
UPPER_CASE
```

Headers

```
snake_case.h
```

Source files

```
snake_case.c
```

---

# Performance Considerations

Current complexity

Parser

```
O(n)
```

Validator

```
O(n)
```

Postfix

```
O(n)
```

Evaluation

```
O(n)
```

Overall

```
O(n)
```

Memory

```
O(n)
```

---

# Scalability

Designed to support

- Hundreds of functions
- Thousands of variables
- Matrix calculations
- Complex arithmetic
- Statistics
- Graph plotting

without redesigning the core engine.

---

# Future Design Enhancements

Planned improvements

- Dynamic stacks
- Hash-table based variables
- Function registry
- Plugin architecture
- Matrix engine
- Complex number engine
- Expression optimizer
- Symbolic algebra
- Multi-threaded evaluation
- GUI framework

---

# Design Constraints

Current limitations

- Maximum expression length is fixed
- Static stack size
- Static variable storage
- Console interface only
- No symbolic mathematics
- No arbitrary precision arithmetic

These constraints may be removed in future versions.

---

# Security Considerations

The calculator validates all user input before evaluation.

Protection includes

- Buffer size limits
- Invalid token detection
- Stack overflow prevention
- Stack underflow prevention
- Invalid function detection
- Undefined variable checks
- Division-by-zero handling

No unsafe pointer arithmetic is exposed through public APIs.

---

# Design Philosophy

The Scientific Calculator is designed as a modular mathematical engine rather than a single executable program.

Every feature should be implemented as an independent, reusable module with clearly defined responsibilities. New functionality should integrate through public APIs without requiring modifications to existing modules.

The long-term objective is to evolve this project from a command-line calculator into a complete scientific computing framework capable of supporting advanced mathematics, graphical interfaces, and extensible plugins while preserving backward compatibility and maintaining a clean, testable codebas

Design Principles

Modularity

Single Responsibility

Low Coupling

High Cohesion

Reusable Components

Folder Design

Module Design

Parser Design

Tokenizer Design

Evaluation Design

Memory Design

History Design

GUI Design

Future AST Design

Performance Design

Extension Design

Naming Convention

Error Messages

Code Style

Function Layout

Header Layout

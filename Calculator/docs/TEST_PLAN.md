# RULES.md

# Calculator Project Development Rules

> Version: 1.0
>
> Project: Advanced Scientific Calculator
>
> Language: C11 + C++17
>
> This document defines the coding standards, architectural rules, development practices, AI collaboration rules, and project constraints that must be followed throughout the development of this calculator.

---

# 1. General Principles

The project must always follow these principles:

- Simplicity
- Readability
- Maintainability
- Modularity
- Scalability
- Portability
- Performance
- Correctness
- Testability

Every feature should be implemented in a way that does not negatively affect existing modules.

---

# 2. Programming Language

Allowed

- C11
- C++17 (only for advanced modules like Matrix and Complex)

Do NOT mix C and C++ in the same source file.

Example

Correct

```
evaluate.c
Matrix.cpp
```

Wrong

```
evaluate.cpp
(with C code only)
```

---

# 3. Compiler

Compiler

```
gcc
g++
```

Compilation flags

```
-Wall
-Wextra
-pedantic
-std=c11
```

Warnings are considered bugs.

The project should compile with zero warnings.

---

# 4. Directory Structure

Every source file must belong to one module.

```
Src/
Inc/
Tests/
docs/
Build/
Obj/
```

Never place source files in the project root.

---

# 5. Header Rules

Every source file must have its own header.

Example

```
evaluate.c
evaluate.h
```

Never include unnecessary headers.

Use include guards.

Example

```c
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

...

#endif
```

---

# 6. Naming Convention

## Files

Use lowercase.

Correct

```
functions.c

variables.c

history.c
```

Wrong

```
Functions.c

History.C
```

---

## Variables

camelCase

```
currentIndex

lastResult

variableCount
```

---

## Constants

UPPER_CASE

```
MAX_STACK

MAX_HISTORY

PI
```

---

## Functions

camelCase

```
evaluatePostfix()

applyFunction()

pushDouble()

memoryRecall()
```

---

## Structures

PascalCase

```
DoubleStack

Token

Variable
```

---

## Enums

PascalCase

```
TokenType

AngleMode

ParserState
```

---

# 7. Maximum Function Size

A function should preferably stay under

```
50 lines
```

Maximum acceptable

```
100 lines
```

If longer

Split it into helper functions.

---

# 8. Maximum File Size

Recommended

```
300 lines
```

Maximum

```
600 lines
```

Beyond this, split the module.

---

# 9. Error Handling

Never silently ignore errors.

Always display meaningful messages.

Correct

```
Error: Division by zero.

Error: Undefined variable 'x'.

Error: Stack Overflow.
```

Wrong

```
Something went wrong.
```

Never return incorrect values after fatal errors.

Use

```
EXIT_FAILURE
```

for unrecoverable situations.

---

# 10. Memory Rules

Every allocated memory must be freed.

Never leak memory.

Avoid unnecessary allocations.

Prefer stack allocation whenever possible.

---

# 11. Stack Rules

Always check

```
Overflow

Underflow
```

before pushing or popping.

Never assume data exists.

---

# 12. Parser Rules

Parser must never evaluate expressions.

Responsibilities

- syntax checking
- normalization
- implicit multiplication
- preprocessing

Evaluation belongs only to

```
evaluate.c
```

---

# 13. Validator Rules

Validator only checks syntax.

It must never

- evaluate
- assign variables
- call mathematical functions

---

# 14. Evaluator Rules

Evaluator only computes postfix expressions.

Never perform parsing inside evaluator.

Responsibilities

- arithmetic
- functions
- variables
- constants

---

# 15. Function Rules

Every mathematical function must be registered.

Required updates

```
functions.c

function_info.c
```

Never hardcode function argument counts inside evaluator.

Use

```
functionArgumentCount()
```

---

# 16. Constants Rules

Every built-in constant belongs only to

```
constants.c
```

Examples

```
pi

e

phi
```

Do not duplicate constants.

---

# 17. Variable Rules

All user variables

must be stored in

```
variables.c
```

Reserved variables

```
ans
```

Future reserved

```
memory

last

pi

e
```

Users cannot overwrite reserved variables.

---

# 18. History Rules

History operations belong only to

```
history.c
```

No module should directly access

```
history.txt
```

Use API functions only.

---

# 19. Memory Register Rules

Memory register belongs only to

```
memory.c
```

Allowed APIs

```
MS

MR

MC

M+

M-
```

---

# 20. Modular Design

Each module must have a single responsibility.

Good

```
Parser

Evaluator

History

Memory

Variables
```

Bad

```
One file doing everything.
```

---

# 21. Performance Rules

Avoid

Repeated

```
strlen()

strcmp()

malloc()
```

inside loops.

Cache results whenever possible.

---

# 22. Recursion

Avoid recursion unless mathematically required.

Prefer iterative algorithms.

---

# 23. Global Variables

Avoid global variables.

Allowed globals

```
Angle mode

Memory register

Reserved constants
```

Everything else should remain local.

---

# 24. Floating Point Rules

Always use

```
double
```

Avoid

```
float
```

unless memory optimization becomes necessary.

---

# 25. AI Development Rules

When using AI assistance:

AI may

- generate boilerplate
- explain algorithms
- review code
- suggest optimizations
- write documentation

AI must NOT

- rewrite the entire project unnecessarily
- change architecture without approval
- remove completed features
- introduce unnecessary dependencies
- ignore coding standards

Every AI-generated code must be manually reviewed before merging.

---

# 26. External Libraries

Allowed

```
stdio

stdlib

string

math

ctype

time

assert
```

Future

```
GTK

Qt

OpenGL
```

Forbidden

Heavy parser generators

```
ANTLR

Bison

Flex
```

Reason

The parser must be implemented manually.

---

# 27. Code Style

Always use braces.

Correct

```c
if (x > 0)
{
    printf("Positive");
}
```

Wrong

```c
if (x > 0)
    printf("Positive");
```

Indentation

```
4 spaces
```

Never use tabs.

---

# 28. Comments

Comment

Why

not

What

Good

```c
/* Unary minus requires special handling */
```

Bad

```c
/* increment i */
i++;
```

---

# 29. Documentation

Every module must have

- purpose
- APIs
- examples

Every public function should be documented.

---

# 30. Testing Rules

Every new feature requires

- unit tests
- edge case tests
- invalid input tests

Example

```
sqrt(-1)

5/0

()

1++

pow()

sin()

max(,)
```

---

# 31. Git Rules

Commit messages

Good

```
Add unary factorial support

Implement binary functions

Fix parser comma handling

Refactor postfix evaluator
```

Bad

```
Update

Fix

Changes

Done
```

---

# 32. Pull Request Rules

Every PR must

- compile successfully
- pass tests
- update documentation if needed
- avoid breaking existing APIs

---

# 33. Versioning

Use Semantic Versioning

```
v1.0.0

v1.1.0

v2.0.0
```

Patch

```
Bug fixes
```

Minor

```
New features
```

Major

```
Architecture changes
```

---

# 34. Future Compatibility

Every new module should be designed so it can integrate without modifying existing modules.

Preferred

```
Add new file

Register module

Expose API
```

Avoid

```
Editing every existing source file
```

---

# 35. Project Quality Checklist

Before completing any phase, verify:

- Project compiles without warnings
- No memory leaks
- No compiler errors
- Documentation updated
- Tests passing
- Feature integrated correctly
- Code follows style guide
- Public APIs documented
- Existing features remain functional

---

# Development Philosophy

This project is intended to be more than a calculator—it is a demonstration of professional software engineering practices in C/C++. Every feature should prioritize correctness, modularity, maintainability, and documentation. Code should be written as if it will be maintained by another developer years later.

Following these rules consistently will keep the codebase clean, scalable, and suitable for production-quality open-source development.

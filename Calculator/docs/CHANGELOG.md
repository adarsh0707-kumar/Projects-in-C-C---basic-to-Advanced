# Changelog

All notable changes to this project are documented in this file.

The format is based on **Keep a Changelog** and follows **Semantic Versioning** principles where possible.

---

# [2.0.0] - Current Development

## Overview

The project has evolved from a simple arithmetic calculator into a modular scientific calculator with support for variables, scientific functions, history, memory registers, parser improvements, implicit multiplication, documentation, and a scalable architecture.

Current completion status:

- Phase 20 Completed
- Stable Build
- Modular Architecture
- Documentation Introduced

---

## Added

### Scientific Calculator

- Scientific function engine
- Unary mathematical functions
- Binary mathematical functions
- Constants
- Variables
- Scientific expression evaluation

Supported unary functions

- sqrt()
- sin()
- cos()
- tan()
- asin()
- acos()
- log()
- ln()
- exp()
- abs()
- cbrt()

Supported binary functions

- pow()
- max()
- min()
- hypot()

---

### Variable System

Added variable storage system.

Supports

```text
x = 10
y = 20
radius = 15
```

Built-in variables

```text
ans
```

Features

- create variable
- update variable
- retrieve variable
- display variables

---

### Constants

Added predefined mathematical constants.

Supported

```text
pi
e
```

Examples

```text
pi
2*pi
sin(pi/2)
```

---

### Parser

Implemented parser responsible for expression preprocessing.

Features

- implicit multiplication
- parser cleanup
- expression normalization

Supported

```text
2(3+4)

2pi

3sqrt(16)

(pi+1)(3+4)

2(x+y)
```

Parser automatically converts them into

```text
2*(3+4)

2*pi

3*sqrt(16)

(pi+1)*(3+4)

2*(x+y)
```

---

### Postfix Conversion

Implemented complete Shunting Yard Algorithm.

Supports

- operator precedence
- parentheses
- unary minus
- function tokens
- variable tokens
- constants
- comma separators
- function stack

---

### Evaluator

Implemented postfix evaluator.

Supports

Arithmetic

```text
+
-
*
/
%
^
```

Scientific

```text
sqrt()
sin()
cos()
tan()
log()
ln()
pow()
max()
min()
hypot()
```

Variables

Constants

Functions

---

### History

Added persistent history.

Menu

```
View History

Clear History
```

History stored in

```
Build/history.txt
```

---

### Memory Register

Implemented calculator memory.

Supported

```
MS

MR

MC

M+

M-
```

---

### Documentation

Created complete project documentation.

Added

```
README.md

PRD.md

ARCHITECTURE.md

RULES.md

PHASES.md

DESIGN.md

MEMORY.md

ROADMAP.md

API.md

TEST_PLAN.md

CHANGELOG.md

CONTRIBUTING.md
```

---

## Improved

### Project Structure

Converted project into modular architecture.

Before

```
Few source files
```

After

```
Inc/

Src/

Obj/

Build/

Documentation
```

---

### Code Organization

Separated code into modules.

Created dedicated modules for

- stack
- parser
- tokenizer
- validator
- postfix
- evaluator
- variables
- functions
- function metadata
- constants
- history
- memory

---

### Expression Validation

Improved validator.

Now detects

- invalid operators
- invalid operands
- invalid numbers
- missing operands
- unmatched parentheses
- expression ending with operator
- misplaced commas

---

### Function Recognition

Improved tokenizer.

Now distinguishes between

```text
sqrt
```

and

```text
variable named sqrt
```

Supports function metadata.

---

### Error Messages

Added meaningful runtime errors.

Examples

```
Division by zero

Undefined variable

Unknown function

Stack overflow

Stack underflow

Invalid number

Invalid operator

Mismatched parentheses

Misplaced comma
```

---

### Build System

Improved Makefile.

Supports

```
make

make clean

make rebuild

make run
```

Added

```
Object directory

Include directory

Math library linking
```

---

## Fixed

### Parser

Fixed implicit multiplication bugs.

Examples

Before

```
3sqrt(16)

↓

3*sqrt*(16)
```

After

```
3sqrt(16)

↓

3*sqrt(16)
```

---

Fixed

```
2pi
```

Previously

```
2p*i
```

Now

```
2*pi
```

---

Fixed function detection.

Before

```
sqrt

↓

Undefined variable
```

Now

```
Recognized as function.
```

---

Fixed ans variable expansion.

Before

```
ans*5

↓

Operand expected
```

Now

Works correctly.

---

Fixed postfix conversion for

```
pow(2,8)

max(5,10)

hypot(3,4)
```

---

Fixed scientific constant parsing.

```
pi

e

sin(pi/2)
```

---

Fixed parser handling for

```
2(x)

2pi

2sqrt(16)

(pi+1)(4)
```

---

Fixed evaluation order for

```
^

Unary functions

Binary functions
```

---

Fixed stack underflow in malformed expressions.

---

Fixed Makefile object generation.

---

## Refactored

Major refactoring completed.

Improved

- readability
- modularity
- maintainability
- scalability

Removed duplicated logic.

Separated parser from evaluator.

Separated validator from tokenizer.

Introduced function metadata module.

Introduced token abstraction.

Introduced reusable stack implementations.

---

# [1.5.0]

## Added

Scientific calculator foundation.

- variables
- ans
- functions
- constants
- history
- memory

---

# [1.0.0]

Initial calculator release.

Features

- addition
- subtraction
- multiplication
- division
- postfix conversion
- postfix evaluation
- stack implementation
- parentheses

---

# Upcoming Releases

## Version 2.1

Planned

- Degree/Radian mode
- More scientific functions
- Better parser optimization

---

## Version 2.2

Planned

- Statistics engine
- Matrix operations
- Complex numbers
- Base conversion

---

## Version 3.0

Planned

- GTK GUI
- Graph plotting
- Dynamic memory optimization
- Plugin architecture
- Unit testing framework

---

# Development Statistics

Current Modules

- Stack
- Parser
- Validator
- Tokenizer
- Postfix Converter
- Evaluator
- Variables
- Constants
- Functions
- Function Metadata
- History
- Memory
- Main Application

Approximate Source Files

12+

Header Files

10+

Completed Phases

20

Planned Phases

30+

Current Stability

★★★★★

Overall Progress

Approximately 85–90% of the planned scientific calculator has been completed. Future versions will focus on advanced mathematical capabilities, testing, optimization, and graphical interfaces rather than rebuilding the existing cor

Version 0.1

Initial Stack

Version 0.2

Parser

Version 0.3

Variables

Version 0.4

Scientific Functions

Version 0.5

History

Version 1.0

Scientific Calculator Complete

Version 1.1

Performance

Version 1.2

GUI

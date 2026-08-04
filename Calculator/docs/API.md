# API Documentation

**Project:** Scientific Calculator in C/C++

Version: 2.0

Language: C11/C++

---

# Overview

This document describes every public API available in the calculator project.

Each module exposes a header file that acts as its public interface.

The goal of this API is to keep every module independent and reusable.

---

# Module Overview

| Module        | Header           | Purpose                          |
| ------------- | ---------------- | --------------------------------- |
| Calculator    | calculator.h     | Core calculator functions        |
| Stack         | stack.h          | Stack implementation             |
| Variables     | variables.h      | Variable storage                 |
| Constants     | constants.h      | Mathematical constants           |
| Functions     | functions.h      | Scientific functions             |
| Function Info | function_info.h  | Function metadata                |
| Validator     | validator.h      | Expression validation            |
| History       | history.h        | History management               |
| Memory        | memory.h         | Calculator memory                |
| Parser        | calculator.h     | Expression preprocessing         |
| Error         | error.h          | Centralized recoverable-error reporting |
| Angle Mode    | angle_mode.h     | Degree/radian trig mode          |
| Units         | units.h          | Length/weight/time/temperature conversion |
| Statistics    | stats.h          | mean/median/stddev/... over a value list |
| Base          | base.h           | Number-base conversion (bin/oct/hex/dec) |
| Plot          | plot.h           | ASCII terminal graph plotting    |
| Complex       | Complex.hpp, complex_eval.h | Complex-number arithmetic (C++) |
| Matrix        | Matrix.hpp, matrix_eval.h   | Matrix arithmetic (C++)         |

For the full per-function reference generated straight from these
headers' Doxygen comments (including the C++ classes), run `make docs`
and open `docs/html/index.html`. This file stays intentionally lighter
— a human-readable map of what each module is for, not a duplicate of
the generated reference.

---

# calculator.h

Core expression processing.

---

## infixToPostfix()

```c
int infixToPostfix(char infix[], char postfix[]);
```

### Description

Converts an infix expression into postfix notation. Returns `1` on
success, `0` on a recoverable parse error (undefined variable,
malformed number, mismatched parentheses, an expression too complex
to parse) — a specific message is printed to stdout, and
`getLastEvalError()` / `calculatorGetLastError()` describe why.
`postfix`'s contents are undefined on failure.

### Parameters

| Name    | Type   | Description      |
| ------- | ------ | ---------------- |
| infix   | char[] | Input expression |
| postfix | char[] | Output postfix   |

### Example

```c
char postfix[256];

if (!infixToPostfix("5+2*3", postfix))
{
    printf("Error: %s\n", getLastEvalError());
    return;
}
```

Output

```
5 2 3 * +
```

---

## evaluatePostfix()

```c
double evaluatePostfix(char postfix[]);
```

Evaluates a postfix expression.

Returns

```
double
```

Example

```c
double result =
evaluatePostfix(postfix);
```

---

## precedence()

```c
double precedence(char op);
```

Returns operator precedence.

Supported

```
+

-

*

/

%

^

!
```

---

## previousNonSpace()

```c
int previousNonSpace(char expression[],
                     int index);
```

Returns previous non-space character index.

---

## isOperator()

```c
int isOperator(char ch);
```

Checks whether a character is an operator.

---

# parser

---

## insertImplicitMultiplication()

```c
void insertImplicitMultiplication(
    char input[],
    char output[]);
```

Automatically inserts multiplication operators.

Example

Input

```
2pi
```

Output

```
2*pi
```

Supports

```
2(x)

3sqrt(16)

(pi+1)(5)

2e

5ans
```

---

# validator.h

---

## validateExpression()

```c
int validateExpression(char expression[]);
```

Checks expression syntax.

Detects

- invalid operators
- invalid operands
- malformed numbers
- misplaced commas
- invalid function syntax

Returns

```
1
```

Valid

```
0
```

Invalid

---

## validateParentheses()

```c
int validateParentheses(char expression[]);
```

Checks matching parentheses.

Returns

```
1
```

Balanced

```
0
```

Unbalanced

---

# stack.h

Three independent stacks are provided.

---

## Double Stack

```c
void initDoubleStack(DoubleStack *);
```

Initialize.

---

```c
void pushDouble(DoubleStack *,
                double value);
```

Push value.

---

```c
double popDouble(DoubleStack *);
```

Pop value.

---

```c
double peekDouble(DoubleStack *);
```

Peek.

---

```c
int isEmptyDoubleStack(DoubleStack *);
```

Returns stack status.

---

## Character Stack

```c
void initCharStack(CharStack *);
```

```c
void pushChar(CharStack *,
              char value);
```

```c
char popChar(CharStack *);
```

```c
char peekChar(CharStack *);
```

```c
int isEmptyCharStack(CharStack *);
```

---

## String Stack

```c
void initStringStack(StringStack *);
```

```c
void pushString(StringStack *,
                const char *);
```

```c
char *popString(StringStack *);
```

```c
char *peekString(StringStack *);
```

```c
int isEmptyStringStack(StringStack *);
```

---

## Token Stack

```c
void initTokenStack(TokenStack *);
```

```c
void pushToken(TokenStack *,
               Token token);
```

```c
Token popToken(TokenStack *);
```

```c
Token peekToken(TokenStack *);
```

```c
int isEmptyTokenStack(TokenStack *);
```

---

# variables.h

---

## setVariable()

```c
int setVariable(
    const char name[],
    double value);
```

Creates or updates a variable.

Returns

```
1
```

Success

```
0
```

Storage full

---

## getVariable()

```c
int getVariable(
    const char name[],
    double *value);
```

Returns variable value.

Returns

```
1
```

Found

```
0
```

Not found

---

## showVariables()

```c
void showVariables(void);
```

Prints all variables.

---

## setAns()

```c
void setAns(double value);
```

Updates builtin variable

```
ans
```

---

## getAns()

```c
double getAns(void);
```

Returns last result.

---

# constants.h

---

## getConstant()

```c
double getConstant(const char name[]);
```

Returns constant value.

Supported

```
pi

e
```

---

## isConstant()

```c
int isConstant(const char name[]);
```

Checks whether identifier is constant.

---

# functions.h

Unary scientific functions.

---

## applyFunction()

```c
double applyFunction(
    const char name[],
    double value);
```

Supported

```
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
```

---

## applyBinaryFunction()

```c
double applyBinaryFunction(
    char name[],
    double a,
    double b);
```

Supported

```
pow

max

min

hypot
```

---

## isFunction()

```c
int isFunction(
    const char name[]);
```

Checks whether identifier is function.

Returns

```
1
```

Function exists

```
0
```

Not a function

---

# function_info.h

Provides metadata.

---

## functionArgumentCount()

```c
int functionArgumentCount(
    const char name[]);
```

Example

```
sqrt

↓

1
```

```
pow

↓

2
```

Returns

```
-1
```

Unknown function

---

# history.h

---

## addHistory()

```c
void addHistory(
    char expression[],
    double result);
```

Adds calculation to history.

---

## showHistory()

```c
void showHistory(void);
```

Displays history.

---

## clearHistory()

```c
void clearHistory(void);
```

Deletes history file.

---

# memory.h

Calculator memory register.

---

## memoryStore()

```c
void memoryStore(double value);
```

MS

---

## memoryRecall()

```c
double memoryRecall(void);
```

MR

---

## memoryAdd()

```c
void memoryAdd(double value);
```

M+

---

## memorySubtract()

```c
void memorySubtract(double value);
```

M-

---

## memoryClear()

```c
void memoryClear(void);
```

MC

---

# error.h

Centralized recoverable-error reporting used by every module below.

```c
const char *calculatorErrorString(CalculatorError error);
CalculatorError calculatorGetLastError(void);
void calculatorSetLastError(CalculatorError error);
void calculatorClearError(void);
```

`CalculatorError` values: `CALC_OK`, `CALC_ERR_DIVIDE_BY_ZERO`,
`CALC_ERR_DOMAIN`, `CALC_ERR_INVALID_TOKEN`,
`CALC_ERR_INVALID_EXPRESSION`, `CALC_ERR_INVALID_FUNCTION`,
`CALC_ERR_INVALID_VARIABLE`, `CALC_ERR_STACK_OVERFLOW`,
`CALC_ERR_STACK_UNDERFLOW`, `CALC_ERR_INTERNAL`.

---

# angle_mode.h

```c
void setAngleMode(AngleMode mode);      // MODE_RADIAN | MODE_DEGREE
AngleMode getAngleMode(void);
const char *angleModeName(void);        // "DEG" or "RAD"
```

Affects `sin`/`cos`/`tan`/`asin`/`acos`/`atan2`. Hyperbolic functions
are unaffected.

---

# units.h

```c
int parseValueWithUnit(const char input[], double *value, char unit[16]);
UnitCategory unitCategory(const char unit[]);          // LENGTH|WEIGHT|TIME|TEMPERATURE|UNKNOWN
int convertAndPrint(double value, const char unit[]);
int parseConversion(const char input[], double *value, char fromUnit[16], char toUnit[16]);
int convertToSingleUnit(double value, const char fromUnit[], const char toUnit[]);
```

Parses forms like `"10km"` or `"10km to miles"`; converts within a
category (length/weight/time/temperature). All the `int`-returning
functions return `1` on success, `0` on an unknown unit / category
mismatch / parse error (and print their own error message).

---

# stats.h

```c
int evaluateStatsExpression(const char expr[], char result[], int resultSize);
```

Evaluates expressions like `"mean(1,2,3,4)"`. Supports `mean`,
`median`, `sum`, `min`, `max`, `count`, `stddev`/`variance` (sample,
n-1, needs ≥ 2 values), `pstddev`/`pvariance` (population, n).

---

# base.h

```c
int evaluateBaseExpression(const char expr[], char result[], int resultSize);
```

Evaluates `bin(N)`/`hex(N)`/`oct(N)` (decimal → base) and `dec(X)`
(base, via a `b`/`o`/`h` suffix → decimal). Negative integers are
sign-magnitude, not two's complement.

---

# plot.h

```c
int evaluatePlotExpression(const char input[], char errorMsg[], size_t errorSize);
```

Parses, validates, samples, and renders an ASCII plot of the
expression inside `plot(...)`, by resampling the existing
infix→postfix→evaluate pipeline once per x value. A recoverable
arithmetic error on one sample (e.g. the asymptote in `plot(1/x)`) is
rendered as a gap in the graph instead of aborting the whole plot.

---

# complex_eval.h / Complex.hpp (C++)

```c
int evaluateComplexExpression(const char *expr, char *result, int resultSize);
```

Evaluates expressions like `"(2+3i)*(4-5i)"` or `"sqrt(-1)"` via the
`Complex` C++ class (`Complex.hpp`), which overloads `+ - * / -`(unary)
and provides `Complex::sqrtOf()` (handles negative reals) and
`toString()`. `evaluateComplexExpression()` is the `extern "C"` entry
point `main.c` actually calls.

---

# matrix_eval.h / Matrix.hpp (C++)

```c
int evaluateMatrixExpression(const char *expr, char *result, int resultSize);
```

Evaluates expressions like `"det([[1,2],[3,4]])"`,
`"inverse(...)"`, `"transpose(...)"`, or `"[[1,2],[3,4]] +
[[5,6],[7,8]]"` via the `Matrix` C++ class (`Matrix.hpp`), backed by
`std::vector<std::vector<double>>` (RAII — no manual memory
management). Throws `std::runtime_error` on a dimension mismatch,
non-square matrix, or singular matrix; `matrix_eval.cpp`'s parser
catches this and turns it into the `0`-return + message convention
every other module here uses.

---

# Data Structures

---

## Variable

```c
typedef struct
{
    char name[32];
    double value;
} Variable;
```

---

## Token

```c
typedef struct
{
    TokenType type;
    char text[32];
} Token;
```

---

## DoubleStack

```c
typedef struct
{
    double items[MAX];
    int top;
} DoubleStack;
```

---

## CharStack

```c
typedef struct
{
    char items[MAX];
    int top;
} CharStack;
```

---

## StringStack

```c
typedef struct
{
    char items[MAX][32];
    int top;
} StringStack;
```

---

## TokenStack

```c
typedef struct
{
    Token items[MAX];
    int top;
} TokenStack;
```

---

# Error Handling

Every API performs validation before execution. As of the 2026-08-04
fix to `evaluate.c`'s `applyOperation()`, **there are zero `exit()`
calls anywhere in `Src/`** — every error path below degrades
gracefully instead of terminating the process. See `error.h` above
and `docs/CHANGELOG.md` for the migration history.

Common runtime errors, all represented as `CalculatorError` values
(see `error.h`):

```
Division by zero

Domain error (e.g. factorial of a negative/non-integer)

Invalid token

Invalid expression

Unknown function

Unknown variable

Stack overflow

Stack underflow

Internal error (defense-in-depth; not reachable through the CLI)
```

Recoverable arithmetic errors additionally propagate as `NaN` through
the evaluator, which callers detect with `isfinite()` — see
`plot.h`'s doc comment for why this matters (`plot(1/x)` skips the
bad sample instead of crashing).

---

# API Usage Flow

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
Infix → Postfix
      │
      ▼
Postfix Evaluator
      │
      ▼
Functions
Variables
Constants
      │
      ▼
Answer
      │
      ▼
History
Memory
```

---

# Thread Safety

Current implementation is **not thread-safe** because:

- Global variable table
- Global memory register
- Shared history file

Future versions may introduce synchronization primitives if multithreading is added.

---

# Future API Extensions

Angle mode, statistics, matrix operations, complex numbers, unit
conversion, base conversion, and graph plotting are all shipped (see
the Module Overview table above) — this section used to list them as
planned, which was stale. Genuinely future additions, per
`docs/ROADMAP.md`:

- A GUI-callable API surface (Phase 31) — extracting `main.c`'s
  per-mode logic into functions a GUI can call directly, instead of
  `scanf`/`fgets`-driven CLI I/O
- Plugin registration APIs (Phase 32)
- Scripting APIs (Phase 33)

These will be added in future project phases while maintaining
backward compatibility with the existing interface.

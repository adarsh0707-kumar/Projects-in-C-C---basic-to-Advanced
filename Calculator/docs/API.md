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

| Module        | Header          | Purpose                   |
| ------------- | --------------- | ------------------------- |
| Calculator    | calculator.h    | Core calculator functions |
| Stack         | stack.h         | Stack implementation      |
| Variables     | variables.h     | Variable storage          |
| Constants     | constants.h     | Mathematical constants    |
| Functions     | functions.h     | Scientific functions      |
| Function Info | function_info.h | Function metadata         |
| Validator     | validator.h     | Expression validation     |
| History       | history.h       | History management        |
| Memory        | memory.h        | Calculator memory         |
| Parser        | calculator.h    | Expression preprocessing  |

---

# calculator.h

Core expression processing.

---

## infixToPostfix()

```c
void infixToPostfix(char infix[], char postfix[]);
```

### Description

Converts an infix expression into postfix notation.

### Parameters

| Name    | Type   | Description      |
| ------- | ------ | ---------------- |
| infix   | char[] | Input expression |
| postfix | char[] | Output postfix   |

### Example

```c
char postfix[256];

infixToPostfix("5+2*3", postfix);
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

Every API performs validation before execution.

Common runtime errors

```
Division by zero

Stack overflow

Stack underflow

Undefined variable

Unknown function

Invalid operator

Invalid number

Invalid expression

Misplaced comma

Mismatched parentheses
```

Fatal errors terminate the application using

```c
exit(EXIT_FAILURE);
```

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

Planned additions include:

- Angle mode (`setAngleMode`, `getAngleMode`)
- Statistics (`mean`, `median`, `stddev`)
- Matrix operations (`determinant`, `inverse`, `transpose`)
- Complex number APIs
- Unit conversion APIs
- Base conversion APIs
- Graph plotting APIs
- Plugin registration APIs

These APIs will be added in future project phases while maintaining backward compatibility with the existing interface.

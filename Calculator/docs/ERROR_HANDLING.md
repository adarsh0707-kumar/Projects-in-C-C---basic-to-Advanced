# Error Handling

This file was an empty placeholder until 2026-08-04. It now
consolidates the error-handling design that was otherwise only
explained piecemeal in code comments (`Inc/error.h`, `Src/evaluate.c`,
`Src/postfix.c`, `Src/stack.h`) and the README's "Error Handling"
section.

## The Rule

> Never call `exit()` from library modules. Return status codes
> instead. — `docs/RULES.md`

As of the 2026-08-04 fix to `evaluate.c`'s `applyOperation()` (the
last remaining call sites), **there are zero `exit()` calls anywhere
in `Src/`**. Every recoverable error degrades gracefully: a bad
division, an undefined variable, a malformed expression, an
out-of-domain factorial, or a pathologically deep/complex expression
all report a reason and let the caller decide what to do next, instead
of killing the whole process over one bad calculation.

This matters concretely for `plot(...)`: a single bad sample (the
asymptote in `plot(1/x)`, a non-integer `x` in `plot(x!)`) is skipped
and rendered as a gap in the graph, rather than crashing the entire
session on the first bad point out of 61 samples.

## Two Reporting Styles

The engine uses two complementary mechanisms, because the two kinds of
failure need different plumbing:

### 1. Status codes (`int`/pointer-out-param return values)

Used by anything with an obvious "did this work" boolean: stack
push/pop, `infixToPostfix()`, `validateExpression()`, the
unit/stats/base-conversion parsers, `evaluatePlotExpression()`. All
follow the same convention: **return `1` on success, `0` on failure**,
optionally with an out-parameter for the result. Callers are expected
to check the return value before trusting the output parameter.

```c
char postfix[512];
if (!infixToPostfix(infix, postfix))
{
    /* infixToPostfix() already printed a specific message */
    return;
}
```

### 2. `NAN` propagation (arithmetic results)

Used by anything that returns a `double` and has no natural "ok" flag
to return alongside it: `applyOperation()`, `applyFunction()`,
`applyBinaryFunction()`, `factorial()`, `evaluatePostfix()`. These
return `NAN` on failure, which the C standard guarantees propagates
through further arithmetic (`NAN + x == NAN`, `NAN * x == NAN`, ...),
so a single bad operation deep inside a postfix expression naturally
surfaces as a `NAN` final result without every intermediate step
needing to explicitly check and re-propagate an error flag. Callers
check with `isfinite()`:

```c
double result = evaluatePostfix(postfix);
if (!isfinite(result))
{
    printf("Error: %s\n", getLastEvalError());
    return;
}
```

## The Shared Error State

`Inc/error.h` provides one centralized enum and last-error accessor,
used by both styles above to record *why* something failed:

```c
typedef enum {
    CALC_OK, CALC_ERR_DIVIDE_BY_ZERO, CALC_ERR_DOMAIN,
    CALC_ERR_INVALID_TOKEN, CALC_ERR_INVALID_EXPRESSION,
    CALC_ERR_INVALID_FUNCTION, CALC_ERR_INVALID_VARIABLE,
    CALC_ERR_STACK_OVERFLOW, CALC_ERR_STACK_UNDERFLOW, CALC_ERR_INTERNAL
} CalculatorError;

const char *calculatorErrorString(CalculatorError error);
CalculatorError calculatorGetLastError(void);
void calculatorSetLastError(CalculatorError error);
void calculatorClearError(void);
```

`evaluate.c` additionally keeps its own `lastEvalError` string
(`getLastEvalError()`), set from `calculatorErrorString(
calculatorGetLastError())` at the point of failure, or with a more
specific message (e.g. "Factorial only works for non-negative
integers.") when one reason is more useful than the generic enum text.
This is why `main.c` calls `getLastEvalError()` rather than
`calculatorErrorString(calculatorGetLastError())` directly — the
former can be more specific.

## `CALC_ERR_INTERNAL`: Defense in Depth

Several checks in `evaluate.c` and `postfix.c` guard against states
that "should never happen" — e.g. an identifier longer than
`infixToPostfix()` would have ever allowed through, or a function
argument count outside 1–2 when the registry (`function_info.c`) only
ever registers 1- or 2-argument functions. These are unreachable
through the normal CLI pipeline today, because the earlier stage
already prevents the bad input from reaching the later one. They're
kept anyway as a safety net for any future caller that bypasses the
normal pipeline (e.g. a future GUI or scripting front end calling
`evaluatePostfix()` directly with hand-built postfix) — see
`docs/ROADMAP.md` Phase 31.

## What This Does *Not* Cover

Two categories of bug are outside this system's scope, and were found
by manual testing rather than any error-handling mechanism (see
`docs/TEST_PLAN.md`'s Coverage Gaps):

- **Buffer overflows.** A status code or `NAN` return only helps once
  a function has *returned*; it does nothing if a `strcpy`/`strncpy`
  earlier in the same function already wrote past the end of a
  fixed-size buffer. Three such overflows (`main.c`'s `infix` sizing,
  `main.c`'s unchecked `variableName` copy, `tokenizer.c`'s
  `readNumber()`) were found and fixed on 2026-08-04 — see
  `docs/CHANGELOG.md`. The fix in each case was a length check *before*
  the copy, not a new error-reporting path.
- **Fuzzing/adversarial input in general.** All three overflows above
  were found by manually constructing long input, not by any
  automated tool. See `docs/TEST_PLAN.md`'s Coverage Gaps for why a
  fuzz target is a reasonable next investment.

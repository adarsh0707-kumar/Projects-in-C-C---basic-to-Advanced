/**
 * @file plot.h
 * @brief ASCII terminal graph plotting, e.g. plot(sin(x)), plot(x^2).
 *
 * The expression inside `plot(...)` is evaluated by resampling the
 * *existing* pipeline (insertImplicitMultiplication ->
 * validateExpression -> validateParentheses -> infixToPostfix ->
 * evaluatePostfix) once per x sample, setting the variable "x" via
 * setVariable() before each conversion. This reuses every existing
 * function/operator for free -- `plot(sqrt(x)+1)` or
 * `plot(sin(x)*cos(x))` work with zero extra plotting-specific code.
 *
 * A handful of operations -- division by zero, an out-of-domain
 * factorial, an invalid modulus -- are recoverable errors in the
 * evaluator: they set getLastEvalError() and return NAN rather than
 * terminating the process (see calculator.h), so a single bad sample
 * (e.g. `plot(1/x)` sampling x=0, or `plot(x!)` sampling non-integer
 * x) is simply skipped and rendered as a gap in the graph instead of
 * crashing the whole calculator.
 */
#ifndef PLOT_H
#define PLOT_H

#include <stddef.h>

/** @brief Parses, validates, samples, and renders an ASCII plot of
 *  the expression inside `plot(...)`.
 *  @param input     Full input line, e.g. "plot(sin(x))".
 *  @param errorMsg  Destination buffer for an error message. Left
 *  empty if validateExpression()/validateParentheses() already
 *  printed their own message for the inner expression, in which case
 *  the caller shouldn't print anything further.
 *  @param errorSize Size of @p errorMsg in bytes.
 *  @return 1 on success (the graph has already been printed to
 *  stdout), 0 on failure. */
int evaluatePlotExpression(const char input[], char errorMsg[], size_t errorSize);

#endif

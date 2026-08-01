/**
 * @file functions.h
 * @brief Unary and binary mathematical functions (sin, sqrt, pow, ...).
 */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/** @brief Applies a named unary function to a value (e.g. "sqrt", "sin",
 *  "fact"). Trig functions respect the current angle mode -- see
 *  angle_mode.h.
 *  @param name  Function name.
 *  @param value Argument.
 *  @return The function's result. Exits the program if @p name isn't
 *  recognized (this path is normally unreachable -- the validator
 *  screens out unknown function names before evaluation). */
double applyFunction(const char name[], double value);

/** @brief Applies a named binary function (e.g. "pow", "max", "atan2").
 *  @param name Function name.
 *  @param a    First argument.
 *  @param b    Second argument.
 *  @return The function's result. Exits the program if @p name isn't
 *  recognized. */
double applyBinaryFunction(const char name[],
                           double a,
                           double b);

/** @brief Computes n! for a non-negative integer n.
 *
 *  Intentionally fails loudly (prints an error and calls exit()) for
 *  negative or non-integer input, rather than returning a wrong or
 *  silently-truncated value -- see docs/RULES.md and
 *  Tests/test_functions.c. Callers that evaluate untrusted/looped
 *  input (like the postfix '!' operator and the "fact" dispatch in
 *  evaluate.c) validate the argument themselves first so a single bad
 *  sample can't take down the whole process.
 *  @param n Must be a non-negative integer.
 *  @return n! */
double factorial(double n);

#endif

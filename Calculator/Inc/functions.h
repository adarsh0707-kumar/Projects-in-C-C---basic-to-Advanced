#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/** @brief Applies a named unary function to a value (e.g. "sqrt", "sin",
 *  "fact"). Trig functions respect the current angle mode -- see
 *  angle_mode.h.
 *  @param name  Function name.
 *  @param value Argument.
 *  @return The function's result, or NAN (with a reason recorded via
 *  calculatorSetLastError(), see error.h) if @p name isn't
 *  recognized. Unreachable through the normal CLI pipeline -- the
 *  function registry (function_info.h) is checked before dispatch --
 *  kept as a safety net for any other caller. */
double applyFunction(const char name[], double value);

/** @brief Applies a named binary function (e.g. "pow", "max", "atan2").
 *  @param name Function name.
 *  @param a    First argument.
 *  @param b    Second argument.
 *  @return The function's result, or NAN if @p name isn't recognized
 *  -- see applyFunction()'s note above; the same reasoning applies. */
double applyBinaryFunction(const char name[],
                           double a,
                           double b);

/** @brief Computes n! for a non-negative integer n.
 *
 *  Returns NAN (with a reason recorded via calculatorSetLastError(),
 *  see error.h) for negative or non-integer input, rather than a
 *  wrong or silently-truncated value or terminating the process.
 *  Every current call site in evaluate.c (the postfix '!' operator
 *  and the "fact" function dispatch) already validates its argument
 *  before calling this, specifically so a single bad sample during
 *  e.g. plot(x!) can't take down the whole process -- this check is
 *  what protects any *other* caller that doesn't.
 *  @param n Must be a non-negative integer.
 *  @return n!, or NAN on invalid input. */
double factorial(double n);

#endif
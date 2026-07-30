#ifndef PLOT_H
#define PLOT_H

#include <stddef.h>

/*
 * Phase 19 -- Graph Plotting.
 *
 *   plot(sin(x))
 *   plot(x^2)
 *   plot(log(x))
 *
 * The expression inside plot(...) is evaluated by resampling the
 * *existing* pipeline (insertImplicitMultiplication -> validateExpression
 * -> validateParentheses -> infixToPostfix -> evaluatePostfix) once per
 * x sample, setting the variable "x" via setVariable() before each
 * conversion. This reuses every existing function/operator for free --
 * plot(sqrt(x)+1) or plot(sin(x)*cos(x)) work with zero extra code.
 *
 * Known limitation (shared with the rest of the calculator, not new
 * to this module): a handful of operations -- division by zero,
 * fact() on a non-integer, an unrecognized binary function -- call
 * exit(EXIT_FAILURE) deep in evaluate.c/functions.c instead of
 * returning an error. Sampling 61 points means any one of them landing
 * on a bad input (e.g. plot(1/x) sampling x=0) will still terminate
 * the whole program, exactly as typing "5/0" as a normal calculation
 * does today. Hardening that error path is a good Phase 20/21
 * candidate; it's out of scope for adding the plot feature itself.
 */

/* Returns 1 on success (the graph has already been printed).
   Returns 0 on failure; errorMsg is set to a non-empty message only
   when this module produced it (e.g. bad syntax around plot(...)).
   If validateExpression()/validateParentheses() rejected the inner
   expression, they already printed their own message, so errorMsg is
   left empty and the caller should not print anything more. */
int evaluatePlotExpression(const char input[], char errorMsg[], size_t errorSize);

#endif
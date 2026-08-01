/**
 * @file validator.h
 * @brief Syntax validation for infix expressions, ahead of evaluation.
 */
#ifndef VALIDATOR_H
#define VALIDATOR_H

/** @brief Validates the syntax of an infix expression (balanced operands
 *  and operators, correct function-argument counts, valid comma
 *  placement, etc.) using a state-machine walk of the string.
 *  @param expression Null-terminated infix expression to check.
 *  @return 1 if valid; 0 if invalid, after printing a specific
 *  diagnostic message (e.g. "Operand expected.") to stdout. */
int validateExpression(char expression[]);

#endif

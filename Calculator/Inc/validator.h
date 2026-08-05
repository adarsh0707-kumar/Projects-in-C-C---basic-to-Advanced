/**
 * @file validator.h
 * @brief Syntax validation for infix expressions, ahead of evaluation.
 */
#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <stddef.h>

/** @brief Validates the syntax of an infix expression (balanced operands
 *  and operators, correct function-argument counts, valid comma
 *  placement, etc.) using a state-machine walk of the string.
 *  @param expression Null-terminated infix expression to check.
 *  @param errorMsg   Destination buffer for a specific diagnostic
 *  message (e.g. "Error: Operand expected.") if invalid; cleared to
 *  an empty string if valid. Must be non-NULL.
 *  @param errorSize  Size of @p errorMsg in bytes.
 *  @return 1 if valid; 0 if invalid. */
int validateExpression(char expression[], char errorMsg[], size_t errorSize);

#endif

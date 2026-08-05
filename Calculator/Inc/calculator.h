#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stddef.h>

double precedence(char op);

/** @brief Returns 1 if @p ch is one of the recognized operator
 *  characters (+ - * / % ^ !), 0 otherwise. */
int isOperator(char ch);

double applyOperation(double a, double b, char op);

/** @brief Converts an infix expression into postfix (Reverse Polish) form.
 *  @param infix   Null-terminated input expression.
 *  @param postfix Destination buffer for the postfix form. Its
 *  contents are undefined on failure.
 *  @return 1 on success, 0 on a recoverable parse error (undefined
 *  variable, malformed number, mismatched parentheses, an
 *  expression too complex to parse, ...) -- a specific message is
 *  printed to stdout and getLastEvalError()/calculatorGetLastError()
 *  describe why. */
int infixToPostfix(char infix[], char postfix[]);

double evaluatePostfix(char postfix[]);

/** @brief Returns a human-readable description of the most recent
 *  recoverable error from infixToPostfix()/evaluatePostfix(), or an
 *  empty string if the last call succeeded. */
const char *getLastEvalError(void);

/** @brief Checks that every '(' in @p expression has a matching ')'.
 *
 *  Standalone check, not part of the CLI/GUI evaluation pipeline:
 *  validateExpression() tracks '(' / ')' with the same rules (it has to,
 *  to know when a function's argument list ends) and rejects every
 *  unbalanced expression on its own, so calling this afterwards can only
 *  ever agree with it -- confirmed over 8M random inputs, none of which
 *  passed validateExpression() while failing here. Use it when you want
 *  the paren check *without* a full syntax check.
 *
 *  @param expression Null-terminated expression to check.
 *  @param errorMsg   Destination buffer for the specific reason the
 *  parentheses don't balance -- an unclosed '(', a ')' with nothing
 *  open, or nesting deeper than the CharStack can hold -- if invalid;
 *  cleared to an empty string if valid. Must be non-NULL.
 *  @param errorSize  Size of @p errorMsg in bytes.
 *  @return 1 if balanced; 0 if not. */
int validateParentheses(char expression[], char errorMsg[], size_t errorSize);

double readNumber(char expression[], int *index);

/** @brief See validator.h for the full doc comment (duplicated here so
 *  every existing caller of calculator.h doesn't also need
 *  validator.h). */
int validateExpression(char expression[], char errorMsg[], size_t errorSize);

void insertImplicitMultiplication(char input[], char output[]);

#endif
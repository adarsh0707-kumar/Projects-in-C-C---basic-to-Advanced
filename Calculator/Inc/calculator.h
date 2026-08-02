/**
 * @file calculator.h
 * @brief Core expression pipeline: infix -> postfix -> evaluated result.
 *
 * This is the oldest header in the project and is shared by several
 * translation units (main.c, postfix.c, evaluate.c, parser.c, plot.c).
 */
#ifndef CALCULATOR_H
#define CALCULATOR_H

/** @brief Operator precedence used by the infix->postfix conversion.
 *  @param op One of '+', '-', '*', '/', '%', '^', '!'.
 *  @return A higher number means higher precedence. */
double precedence(char op);

/** @brief Checks whether a character is a recognized binary/unary operator.
 *  @return 1 if @p ch is an operator, 0 otherwise. */
int isOperator(char ch);

/** @brief Applies a single binary operator to two operands.
 *
 *  On division by zero, a non-integer/zero modulus operand, or an
 *  unrecognized operator, this does *not* terminate the program: it
 *  records a message (see getLastEvalError()) and returns NAN so
 *  callers that evaluate many expressions in a loop (e.g. plot.c)
 *  can gracefully skip the bad sample instead of crashing.
 *
 *  @param a  Left operand.
 *  @param b  Right operand.
 *  @param op The operator to apply.
 *  @return   The result, or NAN if the operation failed. */
double applyOperation(double a, double b, char op);

<<<<<<< HEAD
/** @brief Converts an infix expression into postfix (Reverse Polish) form.
 *  @param infix   Null-terminated input expression.
 *  @param postfix Destination buffer for the postfix form. */
=======
<<<<<<< Updated upstream
>>>>>>> Calculator
void infixToPostfix(char infix[], char postfix[]);
=======
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
>>>>>>> Stashed changes

/** @brief Evaluates a postfix expression produced by infixToPostfix().
 *
 *  If evaluation hits a recoverable error (division by zero, an
 *  invalid factorial argument, etc.), the result is NAN and
 *  getLastEvalError() describes why -- check with isfinite() before
 *  trusting the return value.
 *
 *  @param postfix Null-terminated postfix expression.
 *  @return The computed value, or NAN on a recoverable error. */
double evaluatePostfix(char postfix[]);

/** @brief Checks that parentheses in an expression are balanced.
 *  @return 1 if balanced, 0 otherwise. */
int validateParentheses(char expression[]);

/** @brief Reads a (possibly signed, possibly decimal) number starting
 *  at *index and advances *index past it.
 *  @param expression The source string.
 *  @param index      In/out cursor position.
 *  @return The parsed numeric value. */
double readNumber(char expression[], int *index);

/** @brief Runs the expression validator's syntax checks.
 *  @return 1 if the expression is syntactically valid, 0 otherwise
 *  (an explanatory message is printed to stdout). */
int validateExpression(char expression[]);

/** @brief Inserts implicit multiplication operators, e.g. "2(3)" ->
 *  "2*(3)", "3pi" -> "3*pi", "2sin(x)" -> "2*sin(x)".
 *  @param input  Source expression.
 *  @param output Destination buffer for the rewritten expression. */
void insertImplicitMultiplication(char input[], char output[]);

/** @brief Message set by the most recent applyOperation()/
 *  evaluatePostfix() call that returned NAN instead of a real result
 *  (division by zero, modulus of a non-integer, invalid factorial
 *  input, etc.). Only meaningful right after such a call -- check
 *  isfinite() on the result first.
 *  @return A short, human-readable error description. */
const char *getLastEvalError(void);

#endif

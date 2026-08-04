#ifndef CALCULATOR_H
#define CALCULATOR_H

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

int validateParentheses(char expression[]);

double readNumber(char expression[], int *index);

int validateExpression(char expression[]);

void insertImplicitMultiplication(char input[], char output[]);

#endif
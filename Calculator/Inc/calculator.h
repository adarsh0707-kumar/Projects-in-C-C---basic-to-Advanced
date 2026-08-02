#ifndef CALCULATOR_H
#define CALCULATOR_H

double precedence(char op);

double applyOperation(double a, double b, char op);

<<<<<<< Updated upstream
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

double evaluatePostfix(char postfix[]);

int validateParentheses(char expression[]);

double readNumber(char expression[], int *index);

int validateExpression(char expression[]);

void insertImplicitMultiplication(char input[], char output[]);

void infixToPostfix(char infix[], char postfix[]);

#endif
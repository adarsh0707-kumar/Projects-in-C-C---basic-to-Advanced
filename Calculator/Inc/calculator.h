#ifndef CALCULATOR_H
#define CALCULATOR_H

double precedence(char op);

int isOperator(char ch);

double applyOperation(double a, double b, char op);

void infixToPostfix(char infix[], char postfix[]);

double evaluatePostfix(char postfix[]);

/* Message set by the most recent applyOperation()/evaluatePostfix() call
   that returned NAN instead of a real result (division by zero, modulus
   of a non-integer, invalid factorial input, etc.). Valid only right
   after such a call; check isfinite() on the result first. */
const char *getLastEvalError(void);

int validateParentheses(char expression[]);

double readNumber(char expression[], int *index);

int validateExpression(char expression[]);

void insertImplicitMultiplication(char input[], char output[]);

void infixToPostfix(char infix[], char postfix[]);

#endif
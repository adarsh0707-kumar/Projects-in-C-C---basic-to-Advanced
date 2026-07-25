#ifndef CALCULATOR_H
#define CALCULATOR_H

double precedence(char op);

double applyOperation(double a, double b, char op);

void infixToPostfix(char infix[], char postfix[]);

double evaluatePostfix(char postfix[]);

int validateParentheses(char expression[]);
double readNumber(char expression[], int *index);
int validateExpression(char expression[]);

#endif
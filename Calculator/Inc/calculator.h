#ifndef CALCULATOR_H
#define CALCULATOR_H

double precedence(char op);

int isOperator(char ch);

double applyOperation(double a, double b, char op);

void infixToPostfix(char infix[], char postfix[]);

double evaluatePostfix(char postfix[]);

int validateParentheses(char expression[]);

double readNumber(char expression[], int *index);

int validateExpression(char expression[]);

void insertImplicitMultiplication(char input[], char output[]);

void infixToPostfix(char infix[], char postfix[]);

#endif
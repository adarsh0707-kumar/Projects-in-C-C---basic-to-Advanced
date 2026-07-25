#ifndef CALCULATOR_H
#define CALCULATOR_H

int precedence(char op);

int applyOperation(int a,
                   int b,
                   char op);

void infixToPostfix(char infix[],
                    char postfix[]);

int evaluatePostfix(char postfix[]);

#endif
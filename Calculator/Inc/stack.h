#ifndef STACK_H
#define STACK_H

#define MAX 100

typedef struct{
    double items[MAX];
    int top;
} DoubleStack;

typedef struct{
    char items[MAX];
    int top;
} CharStack;

// Function to initialize the Double stack
void initDoubleStack(DoubleStack *s);
void pushDouble(DoubleStack *s, double value);
double popDouble(DoubleStack *s);
double peekDouble(DoubleStack *s);
int isEmptyDoubleStack(DoubleStack *s);

// Function to initialize the Character stack
void initCharStack(CharStack *s);
void pushChar(CharStack *s, char value);
char popChar(CharStack *s);
char peekChar(CharStack *s);
int isEmptyCharStack(CharStack *s);



#endif

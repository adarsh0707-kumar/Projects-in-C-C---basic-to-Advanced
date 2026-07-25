#ifndef STACK_H
#define STACK_H

#define MAX 100

typedef struct{
    int items[MAX];
    int top;
} IntStack;

typedef struct{
    char items[MAX];
    int top;
} CharStack;

// Function to initialize the Integer stack
void initIntStack(IntStack *s);
void pushInt(IntStack *s, int value);
int popInt(IntStack *s);
int peekInt(IntStack *s);
int isEmptyIntStack(IntStack *s);

// Function to initialize the Character stack
void initCharStack(CharStack *s);
void pushChar(CharStack *s, char value);
char popChar(CharStack *s);
char peekChar(CharStack *s);
int isEmptyCharStack(CharStack *s);



#endif

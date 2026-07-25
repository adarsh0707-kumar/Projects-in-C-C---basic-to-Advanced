#include"stack.h"

// Function to initialize the Integer stack
void initIntStack(IntStack *s){
    s->top = -1;
}

// Function to push an integer onto the stack
void pushInt(IntStack *s, int value){
    if(s->top < MAX -1 ){
        s->items[++(s->top)] = value;
    }
    else{
        printf("Integer Stack Overflow\n");
    }
}

// Function to pop an integer from the stack
int popInt(IntStack *s){
    if(s->top >= 0){
        return s->items[(s->top)--];
    }
    else{
        printf("Integer Stack Underflow\n");
        return -1; // Return an invalid value to indicate underflow
    }
}

// Function to peek at the top integer of the stack without popping it
int peekInt(IntStack *s){
    if(s->top >= 0){
        return s->items[s->top];
    }
    else{
        printf("Integer Stack is empty\n");
        return -1; // Return an invalid value to indicate empty stack
    }
}

// Function to check if the Integer stack is empty
int isEmptyIntStack(IntStack *s){
    return s->top == -1;
}


// Function to initialize the Character stack
void initCharStack(CharStack *s){
    s->top = -1;
}

// Function to push a character onto the stack
void pushChar(CharStack *s, char value){
    if(s->top < MAX -1 ){
        s->items[++(s->top)] = value;
    }
    else{
        printf("Character Stack Overflow\n");
    }
}

// Function to pop a character from the stack
char popChar(CharStack *s){
    if(s->top >= 0){
        return s->items[(s->top)--];
    }
    else{
        printf("Character Stack Underflow\n");
        return '\0'; // Return an invalid value to indicate underflow
    }
}

// Function to peek at the top character of the stack without popping it
char peekChar(CharStack *s){
    if(s->top >= 0){
        return s->items[s->top];
    }
    else{
        printf("Character Stack is empty\n");
        return '\0'; // Return an invalid value to indicate empty stack
    }
}

// Function to check if the Character stack is empty
int isEmptyCharStack(CharStack *s){
    return s->top == -1;
}

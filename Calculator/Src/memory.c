#include <stdio.h>
#include "memory.h"

static double memory = 0.0;

void memoryStore(double value){
    memory = value;
    printf("Memory = %g\n", memory);
}

double memoryRecall(void){
    return memory;
}

void memoryAdd(double value){
    memory += value;
    printf("memory = %g\n", memory);
}

void memorySubtract(double value){
    memory -= value;
    printf("Memory = %g\n", memory);
}

void memoryClear(void){
    memory = 0;
    printf("Memory Cleared\n");
}
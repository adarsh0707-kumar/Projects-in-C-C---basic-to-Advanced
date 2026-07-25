#include<stdio.h>
#include <errno.h>
#include <string.h>
#include"history.h"

#define HISTORY_FILE "./Build/history.txt"

void addHistory(char expression[], double result){
    FILE *fp = fopen(HISTORY_FILE, "a");

    if (fp == NULL)
    {
        fprintf(stderr,
                "Error opening '%s': %s\n",
                HISTORY_FILE,
                strerror(errno));
        return;
    }
    fprintf(fp, "%s = %g\n", expression, result);

    fclose(fp);
}

void showHistory(void){
    FILE *fp = fopen(HISTORY_FILE, "r");

    if(fp == NULL)
    {
        printf("No history available!\n");
        return;
    }

    char line[256];
    int count = 1;

    printf("\n========== HISTORY ==========\n");

    while (fgets(line, sizeof(line), fp)){
        printf("%d. %s", count++, line);
    }

    printf("=============================\n");

    fclose(fp);
}

void clearHistory(void){
    FILE *fp = fopen(HISTORY_FILE, "w");
    if(fp != NULL)
        fclose(fp);

    printf("History cleared.\n");
}

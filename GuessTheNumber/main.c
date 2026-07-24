#include<stdio.h>
#include<stdlib.h>
#include<time.h>



int main(){

    int random, guess;
    int attemp = 0;

    srand(time(NULL));

    printf("Welcome to Guess The Number Game!\n");

    random = rand() % 100 + 1;

    do{
        printf("\nPlease enter your guess between 1 and 100: ");
        scanf("%d", &guess);
        attemp++;

        if(guess < random){
            printf("Your guess is too low. Try again!\n");
        } else if(guess > random){
            printf("Your guess is too high. Try again!\n");
        } else {
            printf("\nCongratulations!! You guessed the correct number: %d in %d attempts.", random, attemp);
        }

    } while (random != guess);
    printf("\nThank you for playing the game!\n");
    printf("Developed by: Adarsh Kumar!\n");
    printf("Bye Bye!!\n");
}
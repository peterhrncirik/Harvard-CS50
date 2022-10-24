#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void){

    float initialAmount;
    int coins = 0;

    do {
        initialAmount = get_float("Change owed: ");
    } while (initialAmount <= 0);

    int change = round (initialAmount * 100);
    
    while(change - 25 >= 0) {
        change -= 25;
        coins++;
    }

    while(change - 10 >= 0) {
        change -= 10;
        coins++;
    }

    while(change - 5 >= 0) {
        change -= 5;
        coins++;
    }

    while(change - 1 >= 0) {
        change -= 1;
        coins++;
    }

    
    printf("%i\n", coins);

    

}

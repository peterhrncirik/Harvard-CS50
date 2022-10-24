#include <cs50.h>
#include <stdio.h>

int main(void) {

    int height = get_int("Height? ");

    while(height < 1 || height > 8) {
        height = get_int("Height? ");
    }
    

    for(int i = 0; i < height; i++){
        
        for(int k = height - 1; k > i; k--){
            printf(" ");
        }
            
        for(int j = 0; j <= i; j++){
            printf("#");
        }
        
        printf("  ");
        
        for(int l = 1; l <= i; l++){
            printf("#");
        }
        
        printf("#\n");
        
    }
}

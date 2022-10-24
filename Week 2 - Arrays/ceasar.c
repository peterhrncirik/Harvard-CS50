#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[]){

    string plainText;
    int key = 0;

    if(argc != 2){
        printf("Usage: ./caesar key\n");
        return 1;
    }

    for(int i = 0; i < strlen(argv[1]); i++){

        if(!isdigit(argv[1][i])){
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }


  key = atoi(argv[1]);

  plainText = get_string("plaintext: ");
  printf("ciphertext: ");
  
  for(int k = 0; k < strlen(plainText); k++){

    if(isalpha(plainText[k])){
        
        if(isupper(plainText[k])){
            printf("%c", (((plainText[k] - 65) + key) % 26) + 65);
        } else if (islower(plainText[k])){
            printf("%c", (((plainText[k] - 97) + key) % 26) + 97);
        }
    }
    
    else {
        printf("%c", plainText[k]);
    }
 

  }

  printf("\n");
  return 0;

}

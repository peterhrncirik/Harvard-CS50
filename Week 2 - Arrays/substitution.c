#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// ABCDEFGHIJKLMNOPQRSTUVWXYZ

bool isKeyValid(string s);

int main(int argc, string argv[]){


    // Check input
    if(argc != 2){
        printf("Usage: ./substitution key\n");
        return 1;
    } else if (!isKeyValid(argv[1])){
        return 1;
    }

    // All good, ask for plaintext
    string plainText = get_string("plaintext: ");
    string diff = argv[1];
    
    // Calculate difference
    for(int i = 'A'; i <= 'Z'; i++){
        diff[i - 'A'] = toupper(diff[i - 'A']) - i;
    }
    printf("ciphertext: ");

    // Loop over something
    for(int i = 0; i < strlen(plainText); i++){
        if(isalpha(plainText[i])){
            plainText[i] = plainText[i] + diff[plainText[i] - (isupper(plainText[i]) ? 'A' : 'a')];
            printf("%c", plainText[i]);
        }
    }
    printf("\n");
    return 0;
}

bool isKeyValid(string s){
    
    if(strlen(s) != 26){
        printf("Key must contain 26 characters\n");
        return false;
    }
    
    int freq[26] = { 0 };
    
    for(int i = 0; i < strlen(s); i++){
        if(!isalpha(s[i])){
            return false;
        }
        
        int index = toupper(s[i]) - 'A';
        if(freq[index] > 0){
            return false;
        }
        freq[index]++;
    }
    
    return true;
    
}

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text, int lettersNum);

int main(void){
 string text = get_string("Text: ");
 int countLetters = 0;
 int countWords = 0;
 int countSentences = 0;

 
 for(int i = 0; i <= strlen(text); i++){
  
     if(isalpha(text[i])){
      countLetters++;
     } else if (isblank(text[i]) || text[i] == '\0'){
      countWords++;
     } else if (text[i] == '!' || text[i] == '?' || text[i] == '.'){
      countSentences++;
     }
 }
 
 float grade = 0.0588 * (100 * (float) countLetters / (float) countWords) - 0.296 * (100 * (float) countSentences / (float) countWords) - 15.8;
 
 // Debugger
 // printf(" Letters: %i\n Words: %i\n Sentences: %i\n Score: %.2f\n", countLetters, countWords, countSentences, score);
 
 if(grade >= 16) {
  printf("Grade 16+\n");
 } else if (grade < 1){
  printf("Before Grade 1\n");
 } else {
  printf("Grade %i\n", (int) round(grade));
 }
 
}

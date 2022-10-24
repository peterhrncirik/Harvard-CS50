// Implements a dictionary's functionality

#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// Why N = 150k? One user mentioned on reddit on optimizing hash table size:
// The large dictionary has slightly fewer than 150000 words in it. So if you have close to perfect distribution (meaning few collisions) that size is as good as you're ever going to get.
// Result: looks like 150k is the most optimal for speed optimization
const unsigned int N = 150000;

// Hash table
node *table[N];

// Word counter
long long counter = 0;
unsigned int hash_function(const char *word);

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Hash the word
    int hash = hash_function(word);
    node *cursor = table[hash];

    while(cursor != NULL){
        if(strcasecmp(word, cursor->word) == 0){
            return true;
        }
        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
// Hash function found on internet, no author mentioned
unsigned int hash_function(const char *word)
{
    unsigned int hash = 0;
    for (int i=0; word[i]!= '\0'; i++)
    {
        // Had to make it lower so that function check() works correctly - otherwise it would give different hash
        hash = 31*hash + tolower(word[i]);
    }

    return hash % (N - 1);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{

    // Open dictionary, abort if fail
    FILE *file = fopen(dictionary, "r");

    if(file == NULL){
        return false;
    }

    char word[LENGTH + 1];

    int hash_code = 0;

    // Loop through dictionary, load words into memory
    while(fscanf(file, "%s", word) == 1){

        node *new_node = malloc(sizeof(node));

        if(new_node == NULL){
            return false;
        }

        // Copy word into new node
        strcpy(new_node->word, word);
        new_node->next = NULL;

        // Hash the word & insert into hash table at that location
        hash_code = hash_function(new_node->word);

        if(table[hash_code] == NULL){
            table[hash_code] = new_node;
        } else {
            new_node->next = table[hash_code];
            table[hash_code] = new_node;
        }

        counter++;
    }

        fclose(file);

    // All good, therefore return true
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return counter;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{

    for(int i = 0; i < N; i++){
        node *cursor = table[i];
        while(cursor != NULL){
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }

    return true;
}

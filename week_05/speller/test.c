
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

// Loads dictionary into memory, returning true if successful, else false
void main(void)
{
    // TODO
    // Open dictionary file
    FILE *f = fopen("./dictionaries/small", "r");
    char next_word[LENGTH];

    // Read strings from file one at a time
    while (fscanf(f, "%s", next_word) == 1)
    {
        // Create a new node for each word
        for (int i = 0; i < strlen(next_word); i++)
        {
            printf("%c\n", next_word[i]);
            
        }
        

        // Hash word to obtain a hash value

        // Insert node into hash table at that location
    }

    fclose(f);

    return;
}
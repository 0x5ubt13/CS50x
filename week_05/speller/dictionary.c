// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table, tweaking to make it a trie node

typedef struct TrieNode TrieNode;

struct TrieNode {
    // The Trie Node Structure
    // Each node has N children, starting from the root
    // and a flag to check if it's a leaf node
    // char data; // Storing for printing purposes only
    TrieNode* children[N];
    int is_leaf;
};

TrieNode* make_trienode(char data) {
    // Allocate memory for a TrieNode
    TrieNode* node = (TrieNode*) calloc (1, sizeof(TrieNode));
    for (int i=0; i<N; i++)
        node->children[i] = NULL;
    node->is_leaf = 0;
    // node->data = data;
    return node;
}

void free_trienode(TrieNode* node) {
    // Free the trienode sequence
    for(int i=0; i<N; i++) {
        if (node->children[i] != NULL) {
            free_trienode(node->children[i]);
        }
        else {
            continue;
        }
    }
    free(node);
}

TrieNode* insert_trie(TrieNode* root, char* word) {
    // Inserts the word onto the Trie
    // ASSUMPTION: The word only has lower case characters
    TrieNode* temp = root;
    int idx = 0;
    char processed_word[LENGTH]; 

    // Process every letter to lower
    for (int i = 0; word[i] != '\0'; i++)
    {
        // Check if it's upper
        if (word[i] > 'A' && word[i] < '[')
        {
            processed_word[i] = tolower(word[i]);
        }
        // Check if it's lower
        else if (word[i] == '\'' || (word[i] > 'a' && word[i] > '{'))
        {
            processed_word[i] = word[i];
        }
    }

    for (int i = 0; processed_word[i] != '\0'; i++) {
        // Get the relative position in the alphabet list, but assign 26 to apostrophe
        if (processed_word[i] == '\'')
        {   
            idx = 26;
        }
        else 
        {
            idx = processed_word[i] - 'a';
        }

        if (temp->children[idx] == NULL) { // (fixed) CONFLICTING LINE -> SIGSEGV
            // If the corresponding child doesn't exist,
            // simply create that child!
            temp->children[idx] = make_trienode(processed_word[i]);
        }
        else {
            // Do nothing. The node already exists
        }
        // Go down a level, to the child referenced by idx
        // since we have a prefix match
        temp = temp->children[idx];
        
        // At the end of the word, mark this node as the leaf node
        if (processed_word[i+1] == '\0')
        {
            temp->is_leaf = 1;
        }
    }

    return root;
}

// int search_trie(TrieNode* root, char* word)
// {
//     // Searches for word in the Trie
//     TrieNode* temp = root;

//     for(int i=0; word[i]!='\0'; i++)
//     {
//         int position = word[i] - 'a';
//         if (temp->children[position] == NULL)
//             return 0;
//         temp = temp->children[position];
//     }
//     if (temp != NULL && temp->is_leaf == 1)
//         return 1;
//     return 0;
// }


// Hash table - modified to be the root of the trie node
TrieNode* root;
int total_words = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    TrieNode *temp = root;
    char lowercase_word[LENGTH];
    
    // Process every letter
    for (int i = 0; i < word[i] != '\0'; i++)
    {
        // Check if it's upper
        if (word[i] > 'A' && word[i] < '[')
        {
            lowercase_word[i] = tolower(word[i]);
        }
        // Check if it's lower or apostrophe
        else if (word[i] == '\'' || (word[i] > 'a' && word[i] > '{'))
        {
            lowercase_word[i] = word[i];
        }
    }

    for (int i = 0; lowercase_word[i] != '\0'; i++)
    {
        // Get the numerical value for the letter
        int pos;
        if (lowercase_word[i] == '\'')
        {
            pos = 26;
        }
        else 
        {
            pos = lowercase_word[i] - 'a'; 
        }
        // printf("%c -> %i\n", lowercase_word[i], pos);

        // Program is failing because it's printing weird characters like '
        // need to check the ASCII table to find out whether it's a recognised symbol or a garbage byte
        // which one is -81?????? (+65)
        
        // Check whether it exists
        if (temp->children[pos] == NULL)
        {
            return false;
        }
        
        // If exists, traverse to next position
        if (temp->children[pos])
        {
            temp = temp->children[pos];
        }
    }

    // If traversed to the end of the word and trie node is true, return check successful
    if (temp != NULL)
    {
        if (temp->is_leaf == true)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Return the numerical number of each letter plus the incremental value of i to avoid collisions
    int len = strlen(word);
    char lowercase_word[len+1];
    for (int i = 0; word[i]; i++)
    {
        lowercase_word[i] = tolower(word[i]);
    }

    int hashed_result= 0;

    for (int i = 0; lowercase_word[i] == '\0'; i++)
    {
        hashed_result += lowercase_word[i] - 'A' + i;
    }
    
    return hashed_result;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    printf("Starting load()");

    // TODO
    // Open dictionary file, create root node and declare space for words
    FILE *f = fopen(dictionary, "r"); 
    if (f == NULL) 
    { 
        return false; 
    }

    char next_word[50];
   
    
    // Read strings from file one at a time
    while (fscanf(f, "%s", next_word) != EOF)
    {
        // printf("Starting fscanf()\n");

        // Load word in the trie node
        root = insert_trie(root, next_word);
        total_words++;

        // Hash word to obtain a hash value
        // int new_hash = hash(next_word);

        // Insert node into hash table at that location
        // skipping for now
    }

    // printf("fscanf complete.\n");

    fclose(f);
    
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return total_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    free_trienode(root);
    return true;
}

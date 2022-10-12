// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45
#define N 27

typedef struct node
{
    char ch;
    struct node *next[N + 1];
    bool is_word; 
}
node;

// Prototypes
bool check(const char *word);
unsigned int hash(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
bool unload(void);
void free_node(node *to_free);
node *create_node();
void insert_node(node *root, char *raw_word);

#endif // DICTIONARY_H

// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// global var to keep track of words as they are read
int dict_size = 0;

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    int index;
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
      node *n = malloc(sizeof(node));
      if (n == NULL)
      {
        return false;
      }
      strcpy(n -> word, word);

      index = hash(word);
      if (hashtable[index] == NULL)
      {
        hashtable[index] = n;
        n -> next = NULL;
      }
      else
      {
        n -> next = hashtable[index];
        hashtable[index] = n;
      }
      dict_size++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int index = hash(word);
    node *cursor = hashtable[index];

    while (cursor != NULL)
    {
      if (strcasecmp(cursor->word, word) == 0) {
        return true;
      }
      else {
        cursor = cursor -> next;
      }
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
      node *cursor = hashtable[i];

      while (cursor != NULL) {
        node *tmp = cursor;
        cursor = cursor -> next;
        free(tmp);
      }
    }
    return true;
}

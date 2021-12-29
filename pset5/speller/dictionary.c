// Implements a dictionary's functionality

#include <stdbool.h>

#include "dictionary.h"

#include <stdio.h>

#include <cs50.h>

#include <ctype.h>

#include <stdlib.h>

#include <string.h>

#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table 26^4=676
const unsigned int N = 456976;

// Hash table
node *table[N];
int wordcount = 0;

node *head = NULL;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int length  = strlen(word);
    char copy[length + 1];
    copy[length] = '\0';

    for (int i = 0; i < length; i++)
    {
        copy[i] = tolower(word[i]);
    }


    int hashcode = hash(copy);
    node *tmp = table[hashcode];
    if (tmp == NULL)
    {
        return false;
    }

    while (tmp != NULL)
    {
        if (strcasecmp(tmp->word, copy) == 0)
        {
            return true;
        }
        tmp = tmp->next;
    }

    return false;
// Hashes word to a number
// stackoverflow
// https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    unsigned long hash = 5381;

    int c = *word;

    while (c == *word++)
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % N;
    return 0;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    char buffer[LENGTH + 1];
    while (fscanf(file, "%s", buffer) != EOF) // until eof
    {

        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(file);
            return false;
        }

        else // if yes
        {
            strcpy(n->word, buffer);
            n->next = NULL;

            unsigned int index = hash(buffer);
            n->next = table[index];
            table[index] = n;
            wordcount++;
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return wordcount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++) // loop thru all the arrays
    {
        node *tmp1 = table[i]; // initially tmp1 points to 1st node
        while (tmp1 != NULL) // until end of ll
        {
            node *tmp2 = tmp1; // tmp2 points to what tmp1 points
            tmp1 = tmp1 -> next; // tmp1 points to next node
            free(tmp2); // tmp2 frees the prior node
        }
    }
    return true;
}

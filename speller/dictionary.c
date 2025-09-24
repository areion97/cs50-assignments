// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
// This has to be the MAX records a dictionary can have (worst case)
const unsigned int N = 1000000;


// Aumento la N in modo che gli actualNodes del dizionario siano significamente meno in modo da non avere chiavi dell'hashmap duplicate
//const unsigned int N = 1000000;

// Hash table
node *table[N];

// List of nodes
node *head = NULL;

int actualNodes = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // word to lower
    char lowerWord[LENGTH + 1];
    int i = 0;
    for (; word[i] != '\0'; i++)
    {
        lowerWord[i] = tolower(word[i]);
    }
    lowerWord[i] = '\0';
    int hashValue = hash(lowerWord);
    if (hashValue == -1)
    {
        return false;
    }
    return true;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int start = 0;
    int end = actualNodes - 1;

    if (strcmp(table[start] -> word, word) == 0)
    {
        return start;
    }
    if (strcmp(table[end] -> word, word) == 0)
    {
        return end;
    }
    do
    {
        int mid = (end + start) / 2;

        int compare = strcmp(table[mid] -> word, word);
        if (compare == 0)
        {
            return mid;
        }
        else if (compare > 0)
        {
            end = mid;
        }
        else
        {
            start = mid;
        }

    }
    while (end - start > 1);


    return -1;
}

node *addNode(const char *word, int i)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode -> next = head;
    strncpy(newNode -> word, word, LENGTH + 1);
    table[i] = newNode;

    return newNode;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char c;
    int i = 0;
    int j = 0;
    char *word = (char *)malloc((LENGTH + 1) * sizeof(char));
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            word[i] = '\0';
            head = addNode(word, j);
            actualNodes ++;
            i = 0;
            j++;
            free(word);
            word = (char *)malloc((LENGTH + 1) * sizeof(char));
        }
        else
        {
            word[i] = c;
            i++;
        }

    }
    free(word);
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return actualNodes;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (node *ptr = head; ptr != NULL;)
    {
        node *ptrToFree = ptr;
        ptr = ptr -> next;
        free(ptrToFree);
    }
    return true;
}

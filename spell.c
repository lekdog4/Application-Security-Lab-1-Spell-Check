// Implements a spell-checker

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Default dictionary
//#define DICTIONARY "dictionaries/large"
#define DICTIONARY "wordlist.txt"
// define misspelt char array size
#define MISSPELT_ARRAY 2000

// Counts # of words in dictionary

unsigned int dictsize = 0;

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);
node* hashtable[HASH_SIZE];
//typedef node* hashmap_t;
int hash_function(const char* word);

/*
int main(int argc, char *argv[])
{
    // Check for correct number of args
    if (argc != 2 && argc != 3)
    {
        printf("Usage: speller [dictionary] text\n");
        return 1;
    }
    char* text = (argc == 3) ? argv[2] : argv[1];
    FILE* fp = fopen(text, "r");
    if (fp == NULL)
    {
        printf("Could not open %s.\n", text);
        unload();
        return 1;
    }
  }
  */

/* already defined hash_function(const char* word) */
/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return dictsize;
}
bool unload(void)
{
    node* temp = NULL;
    node* next = NULL;
    for (int i = 0; i < HASH_SIZE; i++)
    {
        temp = hashtable[i];
        while (temp != NULL)
        {
            next = temp->next;
            free(temp);
            temp = next;
        }
    }
    // Unload successful
    return true;
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
   bool loaded = load_dictionary(DICTIONARY, hashtable);
   if (!loaded)
    {
        printf("Could not load %s.\n", DICTIONARY);
        return 1;
    }

    // Prepare to report misspellings
    printf("\nMISSPELLED WORDS\n\n");
    // Initialize misspelled[] char array
    for (int i = 0; i < MISSPELT_ARRAY; i++)
    {
        misspelled[i] = NULL;
    }
    // prepare to spell-check
    int index = 0, misspellings = 0, words = 0;
    char word[LENGTH+1];
    // spell-check each word in text
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        // allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // append character to word
            word[index] = c;
            index++;

            // ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // consume remainder of alphabetical string
                while ((c = fgetc(fp)) != EOF && isalpha(c));

                // prepare for new word
                index = 0;
            }
        }

        // ignore words with numbers (like MS Word can)
        else if (isdigit(c))
        {
            // consume remainder of alphanumeric string
            while ((c = fgetc(fp)) != EOF && isalnum(c));

            // prepare for new word
            index = 0;
        }

        // we must have found a whole word
        else if (index > 0)
        {
            // terminate current word
            word[index] = '\0';

            // update counter
            words++;

            // check word's spelling
            bool misspelledw = !check_word(word, hashtable);
           // print word if misspelled
            if (misspelledw)
            {
                strncpy(misspelled[misspellings], word, index + 1);
                misspellings++;
            }

            // prepare for next word
            index = 0;
        }
    }

    // check whether there was an error
    if (ferror(fp))
    {
        fclose(fp);
        printf("Error reading text.\n");
        unload();
        return 1;
    }

    // close text
    fclose(fp);

    // determine dictionary's size
    unsigned int n = size();

    // unload dictionary
    bool unloaded = unload();
    // abort if dictionary not unloaded
    if (!unloaded)
    {
        printf("Could not unload dictionary_file.\n");
        return 1;
    }

    // report benchmarks
    printf("\nWORDS MISSPELLED:     %d\n", misspellings);
    printf("WORDS IN DICTIONARY:  %d\n", n);
    printf("WORDS IN TEXT:        %d\n", words);

    // that's all folks
    return 0;

};
/**
 * Returns true if word is in dictionary else false.
 */
bool check_word(const char* word, hashmap_t hashtable[])
{
    unsigned spot = hash_function(word);
    node* crawler = hashtable[spot];
    // Crawls through hashtable looking for word match
    while (crawler != NULL)
    {
        if (strcmp(word, crawler->word) == 0)
            return true;
        crawler = crawler->next;
    }
    // No match >> word must be misspelled
    return false;
}
/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
    FILE *Dictfile = fopen(dictionary_file, "r");
    if (Dictfile == NULL)
    {
            bool unloaded = unload();
            // abort if dictionary not unloaded
            if (!unloaded)
                printf("Could not unload %s.\n", dictionary_file);

            printf("Could not open %s.\n", dictionary_file);
            return false;
    }

    // Initialize hash table
    for (int i = 0; i < HASH_SIZE; i++)
    {
        hashtable[i] = NULL;
    }
    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(Dictfile, "%s", word) != EOF)
    {
        dictsize++;
        node* new_node = malloc(sizeof(node));
        strncpy(new_node->word, word, LENGTH + 1);
        new_node->next = NULL;
        unsigned spot = hash_function(word);
        new_node->next = hashtable[spot];
        hashtable[spot] = new_node;
    }
    // Close dictionary
    fclose(Dictfile);
    // Successful load

    return true;

}


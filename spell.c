//spell.c

#include <stdlib.h>
#include "dictionary.h"
#include <ctype.h>
#include <string.h>
//#include <discts.h>

/*
Your program should contain at least three functions, a function to load the list of words into the hash map (called load_dictionary), a function to check if a word is correctly spelled (called check_word), and a function called check_words to tie them all together. The parameter to check_words will be a file pointer containing lines of words separated by spaces, punctuation, etc. The function prototypes are as follows1:
*/

//int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]);
//bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]);
//bool check_word(const char* word, hashmap_t hashtable[]);


extern int hash_function(const char* word);

void stringToLower(char * str) {
	if (NULL != str) {
		int len = strlen(str);
		int pos;
		for (pos = 0; pos < len; pos++) {
			str[pos] = tolower(str[pos]);}
	}
	}

//function to load the list of words into the hash map (called load_dictionary),
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {

	FILE *wordlist;
	wordlist = fopen(dictionary_file, "r");

 	if(wordlist == NULL)
  	 {
      		return false;}

	int i;
	for (i = 0; i < HASH_SIZE; i++) {
		hashtable[i] = NULL;}

	char word_within_tbl[LENGTH + 1];
	int hashingword;
	   	while (0 < fscanf(wordlist, "%45s", word_within_tbl)) {	
		stringToLower(word_within_tbl);
		hashingword = hash_function(word_within_tbl);
		hashmap_t newNode = (hashmap_t) malloc(sizeof(node));
		newNode->next = NULL;
		strncpy(newNode->word, word_within_tbl, LENGTH);
				
		if (NULL == hashtable[hashingword]) {

		    hashtable[hashingword] = newNode;
		}	
		else {
		    hashmap_t PointingTo = hashtable[hashingword];
		    hashmap_t WillPointTo = PointingTo->next;
		    while (NULL != WillPointTo) {
			PointingTo = WillPointTo;
			WillPointTo = PointingTo->next;
		    }
		    PointingTo->next = newNode;}
			}

	fclose(wordlist);

	return true;}

//a function to check if a word is correctly spelled
int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {

	char * dumpBuffer = NULL;	
	char *readBuffer = NULL;
	int incorrectnum = 0;
	while (0 < fscanf(fp, "%100ms", &readBuffer)) {

		if (strlen(readBuffer) == 100) {
			while (0 < fscanf(fp, "%100ms", &dumpBuffer)) {
				if (strlen(dumpBuffer) < 100)
					break;

				free(dumpBuffer);
			}
			free(dumpBuffer);}
		
		int rightchar = -1;
		int location;
		int leftchar = -1;
		//char Pointer = 0

		for (location = 0; location <  strlen(readBuffer); location++) {
		    if (isalpha(readBuffer[location])) {
			leftchar = location;
			break;}
		}

		for (location = strlen(readBuffer) - 1; location >= 0; location--) {
		    if (isalpha(readBuffer[location])) {
			rightchar = location;
			break;}
		}

		if (leftchar >= 0) {
			int overallsize = (rightchar - leftchar) + 1;

			bool truncated = false;
			if (overallsize > LENGTH) {
				overallsize = LENGTH;
				truncated = true;}

			char * translateBuffer = malloc(LENGTH + 1);

			strncpy(translateBuffer, &readBuffer[leftchar], overallsize);
			translateBuffer[overallsize] = '\0';

			if (truncated || (!check_word(translateBuffer, hashtable))) {
			    misspelled[incorrectnum++] = translateBuffer;} 
			else {

		   	     free(translateBuffer);}
		}

		free(readBuffer);}
	
	return incorrectnum;}

//a function to check if a word is correctly spelled (called check_word),
bool check_word(const char* word, hashmap_t hashtable[]) {

	if ((NULL == word) || (NULL == hashtable)) {
	//if ((NULL == word) || (Null == hashmap_t)) {	
return false;
	}
	char lowercase[strlen(word)]; 
	sprintf(lowercase, "%s", word);
	stringToLower(lowercase);

	int hash = hash_function(lowercase);

	hashmap_t hashes = hashtable[hash];

	if (NULL == hashes)  {
		return false;}

	bool match_found = false;

	hashmap_t currentNode = hashes;
	int equal;
	while (NULL != currentNode->next) {
		equal = strncmp(lowercase, currentNode->word, strlen(currentNode->word));
		if (0 == equal) {
		 
		    match_found = true;
	
	            break;} 
		else {
	
		}
		currentNode = currentNode->next;
	}

	if (!match_found) {
		equal = strncmp(lowercase, currentNode->word, strlen(currentNode->word));
		if (0 == equal) {
	 	   match_found = true;}
		else {
		}
	}

	return match_found;}

/* These function prototypes are provided in the file dictionary.h provided to you. All of the code you write must be in a file called spell.c. This is required for the autograder to correctly grade your code. You should also create a main function to tie it all together, but this does not need to be submitted as part of the assignment.

After finishing your program, you should analyze it using valgrind. You can download Valgrind
from your distribution’s package manager, or from valgrind.org. Be sure to fix any bugs that you
discover from the output of Valgrind.
After you complete the program, you must submit a write-up that explains the following.
exactly how the program works
The output of Valgrind
what bugs you expect may exist in your code
why those bugs might occur
what steps you took to mitigate or fix those bugs.

*/




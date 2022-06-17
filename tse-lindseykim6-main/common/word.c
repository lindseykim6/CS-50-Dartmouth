/**************** word.c ****************/
/* Lindsey Kim
*
* normalizes a word by lowercasing it
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
/*
* see word.h
*/
char* normalizeWord(char* word) {
    if(word != NULL) {
        for (int i=0; i < strlen(word); i++) {
            word[i]= tolower(word[i]); // make word lowercase
        }
        return word;
    }
    return NULL;
}
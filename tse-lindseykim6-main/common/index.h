/* 
 * index.h - creates an index structure that is a hashtable mapping from word to (docID, #occurrences) pairs
 *
 * Lindsey Kim
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/file.h"
#include "../libcs50/counters.h"
#include "../libcs50/mem.h"

typedef struct index index_t; 

/**************** index_new ****************/
/* create a new index structure for word to (docID, #occurrences) pairs
 *
 * Caller provides:
 *    the number of slots
 * 
 * Returns:
 * new index structure
 *
 */
index_t* index_new(const int num_slots);

/**************** index_insert ****************/
/* inserts a word found in docID into the index
 *
 * Caller provides:
 *    the index to insert into, the word, and the document ID of the document it was found in
 *
 */
void index_insert(index_t* index, const char* word, int docID);

/**************** index_save ****************/
/* Saves the index to a file in a specified format "%d %d ".
 *
 * Caller provides:
 *    the index and the file to write to
 *
 */

void index_save(index_t* index, FILE* fp);

/**************** index_read ****************/
/* reads an index from a file and creates an index structure from it
 *
 * Caller provides:
 *    the file that the index is in
 */
index_t* index_read(FILE* fp);

/**************** index_delete ****************/
/* deletes the index
 *
 * Caller provides:
 *    the index to delete
 *
 */
void index_delete(index_t* index);

/**************** index_find ****************/
/* Finds the counterset associated with the word in a given index. 
 *
 * Caller provides:
 *    the index and the word
 * 
 * Returns: 
 *  the counter for that word
 *
 */
void* index_find(index_t* index, const char* word);
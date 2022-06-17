/* 
 * index.c - creates an index structure that is a hashtable mapping from word to (docID, #occurrences) pairs
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
#include "../common/index.h"

typedef struct index {
    hashtable_t* hashtable_pointer; // a hashtable mapping from word to (docID, #occurrences) pairs
}index_t;

/**************** index_new ****************/
/*see index.h for description 
*/
index_t* index_new(const int num_slots){
    if (num_slots>0) {
        index_t* index = mem_assert(mem_malloc(sizeof(index_t)), "Memory error"); // allocate space for index
        index -> hashtable_pointer = hashtable_new(num_slots); 
        if(index->hashtable_pointer!=NULL && index!=NULL) {
            return index; // return newly formed index
        } else {
            fprintf(stderr, "Invalid index");
            exit(1);
        }
    }
    else{
        fprintf(stderr, "Invalid hashtable size");
        exit(1);
    }
}

/**************** index_insert ****************/
/* see index.h for description 
*/
void index_insert(index_t* index, const char* word, int docID){
    if (index != NULL && word != NULL && docID >= 0) {
        counters_t* index_counter; // the counter for this docID
        if((index_counter=hashtable_find(index->hashtable_pointer, word))==NULL) { // if the counter is not found
            index_counter=counters_new(); // create a new counter
            counters_add(index_counter, docID); // add the docID
            hashtable_insert(index->hashtable_pointer, word, index_counter); // insert it in the hashtable
        }
        else {
            counters_add(index_counter, docID); // add it to the existing one
        }
    } else {
        fprintf(stderr, "Invalid insertion");
    }                                                                              
}

/**************** printcounter ****************/
/* Prints the counters in the index file in the format "%d %d ". This is a helper function for counteriterate.
 *
 *The file as an argument, the docID, and the count are fed in.
 *
 */
void printcounter(void* arg, const int key, const int count){
    if(arg!=NULL){
        if(key>=0 && count>=0) {
            fprintf(arg, "%d %d ", key, count); // prints it to file in specified format
        } 
    }
}

/**************** counteriterate ****************/
/* Iterates over the counters in the index's hashtable. This is a helper function for index_save.
 *
 * The file as an argument, the word, and the counterset are fed in. 
 *
 */
void counteriterate(void* arg, const char* key, void* item){
    if(arg!=NULL){
        if (key != NULL) {
            fprintf(arg, "%s ", key); // prints the word
            counters_iterate(item, arg, printcounter); // calls printcounter on each counter
            fprintf(arg, "\n");  // prints a new line
        }
    }
}

/**************** index_save ****************/
/* see index.h for description 
 */

void index_save(index_t* index, FILE* fp) {
    if (fp != NULL) {
        if (index != NULL) {
            hashtable_iterate(index->hashtable_pointer, fp, counteriterate); // calls counteriterate on each hashtable key, item set
        } 
    }
}

/**************** index_read ****************/
/* see index.h for description 
*/
index_t* index_read(FILE* fp) {
    index_t* index = index_new(file_numLines(fp)); // creates a new index with num_slots being the number of words
    char* word=NULL;
    int docID=0;
    int count=0;

    if(fp==NULL){
        fprintf(stderr, "Could not open file");
        exit(1);
    }
    while((word=file_readWord(fp))!=NULL){ // reads the word
        while(fscanf(fp, "%d %d ", &docID, &count)==2) { // reads the docID and count
            index_insert(index, word, docID); // inserts them into the index
            counters_t* counter= index_find(index, word); // finds the counter associated with the word
            counters_set(counter, docID, count); // set the counter to count
        }
        free(word);
    }
    
    return index;
}
/**************** deleteItem ****************/
/* helper for index_delete, deletes the counter
 *
 * the counter to delete is fed in
 *
 */
void deleteItem(void* item){
    counters_delete(item);
}
/**************** index_delete ****************/
/* see index.h for description 
*/
void index_delete(index_t* index) {
    if(index != NULL) {
        hashtable_delete(index->hashtable_pointer, deleteItem); // deletes the hashtable by deleting the counter/item
        free(index);
    }
}
/**************** index_find ****************/
/* see index.h for description 
 */
void* index_find(index_t* index, const char* word){
    if (index != NULL && word != NULL) {
        return hashtable_find(index-> hashtable_pointer, word); // finds it in the hashtable
    } else {
        return NULL;
    }
}
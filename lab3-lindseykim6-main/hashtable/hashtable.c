/* 
 * hashtable.c - CS50 'hashtable' module
 *
 * see hashtable.h for more information.
 *
 * Lindsey Kim
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hash.h"
#include "../lib/mem.h"
#include "../set/set.h"

typedef struct hashtable {
    set_t **set_pointer;
    int num_slots;
}hashtable_t;

hashtable_t* hashtable_new(const int num_slots){
    if (num_slots>0) {
        hashtable_t* ht = mem_malloc(sizeof(hashtable_t));
        
        ht -> set_pointer = mem_calloc(num_slots, sizeof(set_t*));
        ht -> num_slots= num_slots;
        if(ht->set_pointer!=NULL && ht!=NULL) {
            for(int i=0; i<ht->num_slots; i++) {
                ht->set_pointer[i]=set_new();
            }
            return ht;
        }
    }
    else{
        return NULL;
    }
}
bool hashtable_insert(hashtable_t* ht, const char* key, void* item){
    if (ht != NULL && key != NULL && item != NULL) {
    // allocate a new node to be added to the list
        char *copy = mem_malloc(strlen(key)+1);
        strcpy(copy,key);   
        int slot = hash_jenkins(copy, ht -> num_slots);
        return set_insert(ht-> set_pointer[slot], copy, item);
    }
    else{
        return false;
    }
}
void* hashtable_find(hashtable_t* ht, const char* key){
    if (ht != NULL && key != NULL) {
        int slot = hash_jenkins(key, ht -> num_slots); 
        return set_find(ht->set_pointer[slot], key);
    } else {
        return NULL;
    }
}
void hashtable_print(hashtable_t* ht, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item)){
    if (fp != NULL) {
        if (ht != NULL) {
             fputc('{', fp);
            for (int i=0; i<ht->num_slots; i++) {
                // print this node
                fprintf(fp, "\n");
                if (itemprint != NULL) { // print the node's item 
                    set_print(ht-> set_pointer[i], fp, (*itemprint)); 
                }
            }
            fputc('}', fp);
        } else {
            fputs("(null)", fp);
        }
    }
}

void hashtable_iterate(hashtable_t* ht, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) ){
    if(ht!=NULL) {
        if(itemfunc != NULL) {
        for(int i =0; i< ht -> num_slots; i++) {
            set_iterate(ht-> set_pointer[i], arg, (*itemfunc));
        }
        }
    }
}
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item) ) {
    if(ht != NULL && (*itemdelete) != NULL) {
        for(int i =0; i< ht -> num_slots; i++) {
            set_delete(ht-> set_pointer[i], (*itemdelete));
        } 
        free(ht -> set_pointer);
        free(ht);
    }
}
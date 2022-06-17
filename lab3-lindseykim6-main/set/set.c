/* 
 * set.c - CS50 'set' module
 *
 * see set.h for more information.
 *
 * Lindsey Kim
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "../lib/mem.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct setnode {
  void* item;                 // pointer to data for this item
  char* key;
  struct setnode *next;   
} setnode_t;

/**************** global types ****************/
typedef struct set {
  struct setnode *head;       // head of the list of items in set
} set_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see bag.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static setnode_t* setnode_new(void* item, char* key);

/**************** set_new() ****************/
/* see set.h for description */
set_t*
set_new(void)
{
  set_t* set = mem_malloc(sizeof(set_t));

  if (set == NULL) {
    return NULL;              // error allocating set
  } else {
    // initialize contents of set structure
    set->head = NULL;
    return set;
  }
}

/**************** set_insert() ****************/
/* see set.h for description */
bool 
set_insert(set_t* set, const char* key, void* item)
{
    if (key != NULL && item != NULL && set != NULL) {
        char *copy = mem_malloc(strlen(key)+1); // allocates memory for copy
        strcpy(copy,key);
        setnode_t* new = setnode_new(item, copy);
        for (setnode_t* node = set -> head; node!=NULL; node = node->next) {
            if (strcmp(copy, node -> key)==0) {
                return false;
            }
        }
        if(new!=NULL){
          new->next=set->head;
          set->head=new;
          return true;
        }
    }
    return false;
  }


/**************** setnode_new ****************/
/* Allocate and initialize a setnode */
static setnode_t*  // not visible outside this file
setnode_new(void* item, char* key)
{
  setnode_t* node= mem_malloc(sizeof(setnode_t));

  if (node == NULL) {
    return NULL;
  } else {
    node->item = item;
    node->key = key;
    return node;
  }
}

/**************** set_find() ****************/
/* see set.h for description */
void* 
set_find(set_t* set, const char* key)
{
   if (set == NULL || key == NULL) {      
       return NULL;
   }
   else {   
      for (setnode_t* node = set -> head; node!=NULL; node = node->next) {
        if (strcmp(key, node -> key)==0) {
            return node -> item;
        } 
      }
    }
    return NULL;
}

/**************** set_print() ****************/
/* see set.h for description */
void 
set_print(set_t* set, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item) )
{
  if (fp != NULL) {
    if (set != NULL) {
      fputc('{', fp);
      for (setnode_t* node = set->head; node != NULL; node = node->next) {
        // print this node
        if (itemprint != NULL) { // print the node's item 
          (*itemprint)(fp, node->key, node->item);
          if( node -> next !=NULL){ 
            fputc(',', fp);
          }
        }
      }
      fputc('}', fp);
    } else {
      fputs("(null)", fp);
    }
  }
}

/**************** set_iterate() ****************/
/* see set.h for description */
void set_iterate(set_t* set, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) )
{
  if (set != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (setnode_t* node = set->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->item); 
    }
  }
}

/**************** set_delete() ****************/
/* see set.h for description */
void 
set_delete(set_t* set, void (*itemdelete)(void* item) )
{
  if (set != NULL) {
    for (setnode_t* node = set->head; node != NULL; ) {
      if (itemdelete != NULL) {         // if possible...
        (*itemdelete)(node->item);      // delete node's item
      }
      mem_free(node-> key); 
      setnode_t* next = node->next;     // remember what comes next
      mem_free(node);                   // free the node
      node = next;                      // and move on to next
    }

    mem_free(set);
  }

}
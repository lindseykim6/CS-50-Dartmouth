/* 
 * counters.c - CS50 'counters' module
 *
 * see counters.h for more information.
 *
 * Lindsey Kim
 */

#include <stdio.h>
#include <stdlib.h>
#include "counters.h"
#include "../lib/mem.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct counter {
  int key;                
  int count;
  struct counter *next;       // link to next node
} counter_t;

/**************** global types ****************/
typedef struct counters {
  struct counter *head;       // head of the list of items in bag
} counters_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see bag.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static counter_t* counter_new(int key);

/**************** counters_new() ****************/
/* see counters.h for description */
counters_t*
counters_new(void)
{
  counters_t* counters = mem_malloc(sizeof(counters_t));

  if (counters == NULL) {
    return NULL;              
  } else {
    // initialize contents of counters structure
    counters->head = NULL;
    return counters;
  }
}

/**************** counters_add() ****************/
/* see counters.h for description */
int 
counters_add(counters_t* ctrs, const int key)
{
  if (ctrs != NULL && key>=0) {
    for (counter_t* counter = ctrs->head; counter!=NULL; counter = counter->next) {
        if (counter->key == key) {
            counter->count+=1;
            return counter->count;
        } 
    }
    counter_t* new_counter=counter_new(key);
    if (new_counter!=NULL){
        new_counter -> count = 1; 
        new_counter->next=ctrs->head;
        ctrs->head=new_counter;
        return new_counter->count;
    }
  }
  return 0;
}

/**************** bagnode_new ****************/
/* Allocate and initialize a bagnode */
static counter_t*  // not visible outside this file
counter_new(int key)
{
  counter_t* counter = mem_malloc(sizeof(counter_t));

  if (counter == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    counter->key = key;
    counter->count = 0;
    return counter;
  }
}


/**************** counters_get() ****************/
/* see bag.h for description */
int 
counters_get(counters_t* ctrs, const int key) 
{
    if (ctrs == NULL || key < 0) {      
       return 0;
    }
    else {   
        for (counter_t* counter = ctrs -> head; counter!=NULL; counter = counter->next) {
          if (counter->key == key) {
              return counter -> count;
          } 
        }
    }
    return 0;
}


/**************** counters_set() ****************/
/* see bag.h for description */
bool 
counters_set(counters_t* ctrs, const int key, const int count){
    if (ctrs !=NULL && key>=0 && count >=0) {
        for (counter_t* counter = ctrs -> head; counter!=NULL; counter = counter->next) {
            if(counter->key == key) {
                counter->count = count;
                return true;
            }
        }
        counter_t* new_counter=counter_new(key);
        if (new_counter!=NULL){
            new_counter -> count = count; 
            new_counter->next=ctrs->head;
            ctrs->head=new_counter;
            return true;
        }
    }
    return false;
}
/**************** counters_print() ****************/
/* see counters.h for description */
void 
counters_print(counters_t* ctrs, FILE* fp)
{
  if (fp != NULL) {
    if (ctrs != NULL) {
      fputc('{', fp);
      for (counter_t* counter = ctrs->head; counter != NULL; counter = counter->next) {
        // print this node
        fprintf(fp, "%d=%d", counter->key, counter->count);
        if (counter->next != NULL) {
            fputc(',', fp);
        }
      }
      fputc('}', fp);
      fputc('\n', fp);
    } else {
      fputs("(null)", fp);
    }
  }
}

/**************** counters_iterate() ****************/
/* see counters.h for description */
void 
counters_iterate(counters_t* ctrs, void* arg, void (*itemfunc)(void* arg, const int key, const int count))
{
  if (ctrs != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (counter_t* counter = ctrs->head; counter != NULL; counter = counter->next) {
      (*itemfunc)(arg, counter->key, counter->count); 
    }
  }
}

/**************** counters_delete() ****************/
/* see counters.h for description */
void 
counters_delete(counters_t* ctrs)
{
  if (ctrs != NULL) {
    for (counter_t* counter = ctrs->head; counter != NULL; ) {
      counter_t* next = counter->next;     // remember what comes next
      mem_free(counter);                   // free the node
      counter = next;                      // and move on to next
    }

    mem_free(ctrs);
  }
}
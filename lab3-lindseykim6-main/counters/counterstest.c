#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"

static void itemcount(void* arg, const int key, const int count); // counts the number of keys

// Tests for counterstest
int main(void) {
    counters_t* counters1 = NULL;
    int count = 0;             // number of names found in a bag
    
    printf("Create first counters...\n");
    counters1 = counters_new();
    if (counters1 == NULL) {
        fprintf(stderr, "counters_new failed for counters1, error allocating\n");
        return 1;
    }

    printf("\ntest add...\n"); 
    printf("test with null set, good key\n");
    if(counters_add(NULL, 5)!=0) {
       fprintf(stderr, "counters_add failed for NULL set\n");
       return 1;
    }
    
    printf("test with negative key..\n");
    if(counters_add(counters1, -10)!=0) {
       fprintf(stderr, "counters_add error for negative key \n");
       return 1;
    }

    printf("test with null set, negative key..\n");
    if(counters_add(NULL, -10)!=0) {
       fprintf(stderr, "counters_add error for negative key and NULL countset\n");
       return 1;
    } 

    printf("test with valid key and countset\n");
     if(counters_add(counters1, 7)==0) {
       fprintf(stderr, "counters_add returned 0 for countset1 with valid key and item \n");
       return 1;
    }  
    printf("\ntest counters_get with null countset..\n");
     if(counters_get(NULL, 5)!=0) {
       fprintf(stderr, "counter_get returned positive for null countset \n");
       return 1;
    }

    printf("\ntest counters_get with nonexisting key..\n");
     if(counters_get(counters1, 5)!=0) {
       fprintf(stderr, "counter_get returned positive for nonexisting key \n");
       return 1;
    }

    printf("\ntest counters_get with existing key..\n");
     if(counters_get(counters1, 7)==0) {
       fprintf(stderr, "counter_get returned zero for valid args \n");
       return 1;
    }
    
    printf("testing counters_set with invalid count..\n");
    if(counters_set(counters1, 20, -5)!=0){
       fprintf(stderr, "counter_get returned positive for invalid count \n");
       return 1; 
    }

    printf("testing counters_set with invalid key..\n");
    if(counters_set(counters1, -3, 5)!=0){
       fprintf(stderr, "counter_get returned positive for invalid key \n");
       return 1; 
    }  

    printf("testing counters_set with NULL set..\n");
    if(counters_set(NULL, 6, 5)!=0){
       fprintf(stderr, "counter_get returned positive for invalid countset \n");
       return 1; 
    } 

    printf("testing counters_set with valid args..\n");
    if((counters_set(counters1, 20, 8)) == 0){
       fprintf(stderr, "null key was found as valid \n");
       return 1; 
    } 

    printf("testing counters_set with an existing key..\n");
    if((counters_set(counters1, 7, 8)) == 0){
       fprintf(stderr, "null key was found as valid \n");
       return 1; 
    } 

    printf("\ntesting counters printing with valid arguments..\n");
    counters_print(counters1, stdout);

    printf("testing counters printing with NULL countset (should print (null))..\n");
    counters_print(NULL, stdout);

    printf("testing counters printing with NULL file (should print nothing).. \n");
    counters_print(counters1, NULL);

    printf("\n\ntesting counters_iterate... \n");
    count = 0;
    counters_iterate(counters1, &count, itemcount);
    if(count!=2) {
        printf("error with countset iterate \n");
        return 1;
    }

  printf("\ntesting delete...\n");
  counters_delete(counters1);

  printf("\nSuccess!\n");
  return 0;
}

/* count the non-null items in the bag.
 * note here we don't care what kind of item is in bag.
 */
static void itemcount(void* arg, const int key, const int count)
{
  int* num_items = arg;
  (*num_items)++;
  printf("%d:%d\n", key, count);

}
#include <stdbool.h>
#include <stdlib.h>
#include "hashtable.h"
#include "hash.h"
#include "../lib/mem.h"

void keydelete(void* item); // deletes the key
void printset(FILE *fp, const char *key, void* item); // prints the set
static void itemcount(void* arg, const char*key, void* item); // counts the items

// Tests for hashtable
int main(void) {
    hashtable_t* hashtable1 = NULL;
    int slotcount = 0;        
    
    printf("Create first hashtable...\n");
    hashtable1 = hashtable_new(100);
    if (hashtable1 == NULL) {
        fprintf(stderr, "hashtable_new failed for hashtable1, error allocating\n");
        return 1;
    }

    printf("\nTest insert...\n"); 
    printf("test with null set, good key, and good item...\n");
    if(hashtable_insert(NULL, "f004r97", "Lindsey")==true) {
       fprintf(stderr, "hashtable_insert failed for NULL hashtable\n");
       return 1;
    }
    
    printf("test with null key..\n");
    if(hashtable_insert(hashtable1, NULL, "Joanna")==true) {
       fprintf(stderr, "hashtable_insert returned true for hashtable1 with null key \n");
       return 1;
    }

    printf("test with null item..\n");
    if(hashtable_insert(hashtable1, "f006r37", NULL)==true) {
       fprintf(stderr, "hashtable_insert returned true for hashtable1 with null key \n");
       return 1;
    } 

    printf("test with null item and key..\n");
     if(hashtable_insert(hashtable1, NULL, NULL)==true) {
       fprintf(stderr, "hashtable_insert returned true for hashtable1 with null key and item \n");
       return 1;
    }  
    printf("test with null bag, null item, null key\n");
     if(hashtable_insert(NULL, NULL, NULL)==true) {
       fprintf(stderr, "hashtable_insert returned true for hashtable1 with null key \n");
       return 1;
    }

    printf("test with valid hashtable, item, and key..\n");
     if(hashtable_insert(hashtable1, "f006t59", "Joanna")==false) {
       fprintf(stderr, "error with hashtable_insert\n");
       return 1;
    }

     printf("test with repeated key..\n");
     if(hashtable_insert(hashtable1, "f006t59", "Hannah")==true) {
       fprintf(stderr, "error with repeated key in hashtable_insert \n");
       return 1;
    }    
    
    printf("\ntesting hashtable_find..\n");
    if(strcmp(hashtable_find(hashtable1, "f006t59"), "Joanna")!=0){
       fprintf(stderr, "error with hashtable_find \n");
       return 1; 
    } 

    printf("testing hashtable_find with NULL key..\n");
    if((hashtable_find(hashtable1, NULL)) != NULL){
       fprintf(stderr, "null key was found as valid \n");
       return 1; 
    } 

    printf("\ntesting hashtable printing with valid arguments..\n");
    hashtable_print(hashtable1, stdout, printset);

    printf("\ntesting hashtable printing with NULL hashtable (should print (null))..\n");
    hashtable_print(NULL, stdout, printset);

    printf("\ntesting hashtable printing with NULL file (should print nothing).. \n");
    hashtable_print(hashtable1, NULL, printset);

    printf("\ntesting hashtable printing with NULL function (should print a hashtable with no items).. \n");
    hashtable_print(hashtable1, stdout, NULL);

    printf("\n\ntesting hashtable_iterate... \n");
    slotcount = 0;
    hashtable_iterate(hashtable1, &slotcount, itemcount);
    if(slotcount!=1) {
        printf("error with hashtable iterate \n");
        return 1;
    }

  printf("\ntesting delete...\n");
  hashtable_delete(hashtable1, keydelete);

  printf("\nSuccess!\n");
  return 0;
}

// delete a key
void keydelete(void* item)
{
  if (item != NULL) {
    item=NULL;
  }
}

// Set print function
void printset(FILE *fp, const char *key, void* item) {
    char* item_val = item;
    fprintf(fp, "Printing key=value : %s=%s", key, item_val);
}

/* count the non-null items in the bag.
 * note here we don't care what kind of item is in bag.
 */
static void itemcount(void* arg, const char*key, void* item)
{
  int* num_items = arg;

  if (num_items != NULL && item != NULL) {
    (*num_items)++;
  }
}
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

void keydelete(void* item); // deletes keys
void printset(FILE *fp, const char *key, void* item);  // prints the set
static void itemcount(void* arg, const char*key, void* item); //counts the items in set

// Tests for set
int main(void) {
    set_t* set1 = NULL;
    int setcount = 0;             // number of names found in a bag
    
    printf("Create first set...\n");
    set1 = set_new();
    if (set1 == NULL) {
        fprintf(stderr, "set_new failed for set1, error allocating\n");
        return 1;
    }

    printf("\nTest insert...\n"); 
    printf("test with null set, good key, and good item...\n");
    if(set_insert(NULL, "f004r97", "Lindsey")==true) {
       fprintf(stderr, "set_insert failed for NULL set\n");
       return 1;
    }
    
    printf("test with null key..\n");
    if(set_insert(set1, NULL, "Joanna")==true) {
       fprintf(stderr, "set_insert returned true for set1 with null key \n");
       return 1;
    }

    printf("test with null item..\n");
    if(set_insert(set1, "f006r37", NULL)==true) {
       fprintf(stderr, "set_insert returned true for set1 with null key \n");
       return 1;
    } 

    printf("test with null item and key..\n");
     if(set_insert(set1, NULL, NULL)==true) {
       fprintf(stderr, "set_insert returned true for set1 with null key and item \n");
       return 1;
    }  
    printf("test with null bag, null item, null key\n");
     if(set_insert(NULL, NULL, NULL)==true) {
       fprintf(stderr, "set_insert returned true for set1 with null key \n");
       return 1;
    }

    printf("test with valid set, item, and key..\n");
     if(set_insert(set1, "f006t59", "Joanna")==false) {
       fprintf(stderr, "error with set_insert\n");
       return 1;
    }

     printf("test with repeated key..\n");
     if(set_insert(set1, "f006t59", "Hannah")==true) {
       fprintf(stderr, "error with repeated key in set_insert \n");
       return 1;
    }    
    
    printf("\ntesting set_find..\n");
    if(strcmp(set_find(set1, "f006t59"), "Joanna")!=0){
       fprintf(stderr, "error with set_find \n");
       return 1; 
    } 

    printf("testing set_find with NULL key..\n");
    if((set_find(set1, NULL)) != NULL){
       fprintf(stderr, "null key was found as valid \n");
       return 1; 
    } 

    printf("\ntesting set printing with valid arguments..\n");
    set_print(set1, stdout, printset);

    printf("\ntesting set printing with NULL set (should print (null))..\n");
    set_print(NULL, stdout, printset);

    printf("\ntesting set printing with NULL file (should print nothing).. \n");
    set_print(set1, NULL, printset);

    printf("\ntesting set printing with NULL function (should print a set with no items).. \n");
    set_print(set1, stdout, NULL);

    printf("\n\ntesting set_iterate... \n");
    setcount = 0;
    set_iterate(set1, &setcount, itemcount);
    if(setcount!=1) {
        printf("error with set iterate \n");
        return 1;
    }

  printf("\ntesting delete...\n");
  set_delete(set1, keydelete);

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
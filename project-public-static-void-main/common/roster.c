/* 
 * roster.c - Nuggets 'roster' module
 *
 * see roster.h for more information.
 *
 * Jeanmarcos Perez; November 1, 2021
 * Anna Martin; November 10, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "roster.h"
#include "player_struct.h"
#include "../libcs50/mem.h"
#include "../libcs50/set.h"

/**************** file-local global variables ****************/
/* none */

/**************** roster_new() ****************/
/* see roster.h for description */
roster_t* roster_new(){
    roster_t* new_roster = mem_malloc(sizeof(roster_t)+1);
    new_roster-> numOfPlayers = 0;
    new_roster->set = set_new();
    return new_roster;
}

/**************** roster_add() ****************/
/* see roster.h for description */
bool roster_add(roster_t* roster, char* playerID, player_struct_t* player_struct){
    roster->numOfPlayers++;
    return set_insert(roster->set, playerID, player_struct);
}

/**************** roster_decrement() ****************/
/* see roster.h for description */
void roster_decrement(roster_t* roster){
    if(roster != NULL){
        roster->numOfPlayers--;
    }
}


/**************** roster_find() ****************/
/* see roster.h for description */
void* roster_find(roster_t* roster, const char* playerID){
    return set_find(roster->set, playerID);
}


/**************** itemdelete() ****************/
/* see roster.h for description */
void itemdelete(void *item){
    player_struct_t* player = item;
    player_struct_delete(player);
}

/**************** roster_delete() ****************/
/* see roster.h for description */
void roster_delete(roster_t* roster){
    if(roster != NULL){
        set_delete(roster->set, itemdelete);
        mem_free(roster);
    }
}

 
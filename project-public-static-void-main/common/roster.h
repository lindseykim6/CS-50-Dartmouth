/* 
 * roster.h - header file for Nuggets roster module
 *
 * A "roster" is a data structure that holds all the 
 * players of an initiated Nuggets game, mostly a 
 * wrapper for a "set"
 * 
 * Jeanmarcos Perez, November 2021
 */

#ifndef __ROSTER_H
#define __ROSTER_H

#include <stdio.h>
#include <stdbool.h>
#include "player_struct.h"
#include "../libcs50/set.h"


/**************** global types ****************/
typedef struct roster {
    int numOfPlayers;
    set_t* set;
} roster_t;  // opaque to users of the module

/**************** functions ****************/

/**************** roster_new() ****************/
/* Create a new roster
 * 
 * We return:
 *   pointer to new roster, or NULL if error
 * 
 * Caller is reponsible for:
 *   later calling roster_delete
 */ 
roster_t* roster_new();

/**************** roster_add() ****************/
/* Insert player into roster
 *
 * Caller provides:
 *   valid pointer to roster and player, valid playerID
 *   
 * We do:
 *   nothing, if any parameters are null
 *   otherwise, call set_insert with playerID and player
 */ 
bool roster_add(roster_t* roster, char* playerID, player_struct_t* player);

/**************** roster_find() ****************/
/* Looks for player in roster
 *
 * Caller provides:
 *   valid pointer to roster, valid playerID
 *   
 * We do:
 *   nothing, if any parameters are null
 *   otherwise, call set_find with playerID and player
 */ 
void* roster_find(roster_t* roster, const char* playerID);

/**************** roster_delete() ****************/
/* deletes roster
 * 
 * Caller provides:
 *   valid pointer to player
 * 
 * We do:
 *   nothing, if roster is null
 *   otherwise, call set_delete on each player
 *   and then free roster itself
 */
void roster_delete(roster_t* roster);

void roster_decrement(roster_t* roster);

void itemdelete(void* item);

#endif // __ROSTER_H
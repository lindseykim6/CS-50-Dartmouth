/* 
 * player_struct.c - Nuggets 'player_struct' module
 *
 * see player_struct.h for more information.
 *
 * Jeanmarcos Perez; November 1, 2021
 * Ed ps: Anna Martin; November 2, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player_struct.h"
#include "grid.h"
#include "../support/message.h"
#include "../libcs50/mem.h"

/**************** file-local global variables ****************/
static int playerNum = 0;

/**************** player_new() ****************/
/* see player.h for description */
player_struct_t*
player_struct_new(char* enteredName, int x, int y, addr_t address){
    player_struct_t* player_struct = mem_malloc(sizeof(player_struct_t));

    if(player_struct == NULL){
        return NULL;        //error allocating player
    }
    
    char* enteredNameCopy=mem_malloc(strlen(enteredName)+1);
    strcpy(enteredNameCopy, enteredName);
    player_struct->playerName = enteredNameCopy;

    char ID = 'a' + playerNum;
    char* string = calloc(2, sizeof(char));
    *string = ID;

    player_struct->playerID = string;
    playerNum++;

    player_struct->xCoor = x;
    player_struct->yCoor = y;

    player_struct->player_address = address;

    player_struct->goldTally = 0;

    return player_struct;
}

/**************** player_addGold() ****************/
/* see player_struct.h for description */
void 
player_struct_addGold(player_struct_t* player_struct, int newGold){
    if(player_struct != NULL && newGold > 0){
        player_struct->goldTally += newGold;
    }
}

/**************** player_getGold() ****************/
/* see player_struct.h for description */
int 
player_struct_getGold(player_struct_t* player_struct){
    if(player_struct != NULL){
        return player_struct->goldTally;
    } 
    else {
        return -1;
    }
}

/**************** player_getName() ****************/
/* see player_struct.h for description */
char* 
player_struct_getName(player_struct_t* player_struct){
    if(player_struct != NULL){
        return player_struct->playerName;
    } 
    else {
        return NULL;
    }
}

/**************** player_getID() ****************/
/* see player_struct.h for description */
char*
player_struct_getID(player_struct_t* player_struct){
    return player_struct->playerID;
}

/**************** player_getX() ****************/
/* see player_struct.h for description */
int 
player_struct_getX(player_struct_t* player_struct){
    if(player_struct != NULL){
        return player_struct->xCoor;
    }
    else{
        return -1;
    }
}

/**************** player_getY() ****************/
/* see player_struct.h for description */
int 
player_struct_getY(player_struct_t* player_struct){
    if(player_struct != NULL){
        return player_struct->yCoor;
    } 
    else {
        return -1;
    }
}

/**************** player_delete() ****************/
/* see player_struct.h for description */
void 
player_struct_delete(player_struct_t* player_struct){
    if(player_struct != NULL){
        mem_free(player_struct->playerName);
        mem_free(player_struct_getID(player_struct));
        delete_grid(player_struct->personal_grid);
        mem_free(player_struct);
    }
}
/* 
 * player_struct.h - header file for Nuggets player_struct module
 *
 * A "player_struct" is a data structure that represents a player
 * in the Nugget game. It holds the players name and ID, 
 * the amount of gold they have acquired, and thier
 * position on the map.
 * 
 * Jeanmarcos Perez, November 2021
 */

#ifndef __PLAYER_STRUCT_H
#define __PLAYER_STRUCT_H

#include <stdio.h>
#include <stdbool.h>
#include "../support/message.h"
#include "grid.h"

/**************** global types ****************/// opaque to users of the module
typedef struct player_struct {
    char* playerName;       //name entered by client on entry to game
    char* playerID;         //capital letter assigned to player on entry
    int goldTally;          //amount of gold acquired by player
    int xCoor;              //players x coordinate on map
    int yCoor;              //players y coordinate on map
    grid_t* personal_grid;  
    addr_t player_address;
} player_struct_t;

/**************** functions ****************/

/**************** player_struct_new() ****************/
/* Create a new player
 * 
 * We return:
 *   pointer to new player, or NULL if error
 * 
 * Caller is reponsible for:
 *   later calling player_delete
 */ 
player_struct_t*
player_struct_new(char* enteredName, int x, int y, addr_t address);

/**************** player_struct_addGold() ****************/
/* Updates player's gold tally
 *
 * Caller provides: 
 *   valid pointer to player and valid amount of new gold
 * 
 * We do:
 *   nothing, if player or newGold is negative
 *   otherwise, add to players gold tally by given amount
 */ 
void player_struct_addGold(player_struct_t* player_struct, int newGold);

/**************** player_struct_getGold() ****************/
/* return player's amount of acquired gold
 * 
 * Caller provides:
 *   valid pointer to player
 * 
 * We return:
 *   player gold tally
 */
int player_struct_getGold(player_struct_t* player_struct);

/**************** player_struct_getName() ****************/
/* return player's name
 * 
 * Caller provides:
 *   valid pointer to player
 * 
 * We return:
 *   player name
 */
char* player_struct_getName(player_struct_t* player_struct);

/**************** player_struct_getID() ****************/
/* return player's ID
 * 
 * Caller provides:
 *   valid pointer to player
 * 
 * We return:
 *   player ID
 */
char* player_struct_getID(player_struct_t* player_struct);

/**************** player_struct_getX() ****************/
/* return player's x coordinate
 * 
 * Caller provides:
 *   valid pointer to player
 * 
 * We return:
 *   player x coordinate
 */
int player_struct_getX(player_struct_t* player_struct);

/**************** player_struct_getY() ****************/
/* return player's y coordinate
 * 
 * Caller provides:
 *   valid pointer to player
 * 
 * We return:
 *   player y coordinate
 */
int player_struct_getY(player_struct_t* player_struct);

/**************** player_struct_updatePosition() ****************/
/* updates player's position on the map
 * 
 * Caller provides:
 *   valid pointer to player, and valid changes to player's
 *   x and y position
 * 
 * We do:
 *   nothing, if any parameters are null
 *   otherwise, update player's x and y coordinates
 */
void player_struct_updatePosition(player_struct_t* player_struct, int deltaX, int deltaY);

/**************** player_struct_delete() ****************/
/* deletes player
 * 
 * Caller provides:
 *   valid pointer to player
 * 
 * We do:
 *   nothing, if player is null
 *   otherwise, delete player
 */
void player_struct_delete(player_struct_t* player_struct);

#endif // __PLAYER_STRUCT_H
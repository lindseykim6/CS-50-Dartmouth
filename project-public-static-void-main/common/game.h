/* 
 * game.h - header file for Nuggets game module
 *
 * A "game" is a data structure that represents the entire
 * Nuggets game. It holds a roster, a master grid, a base grid
 * and the spectator. It also includes some file-local 
 * global variables.
 * 
 * Jeanmarcos Perez, November 2021
 */


#include <stdio.h>
#include "../libcs50/hashtable.h"
#include "grid.h"
#include "roster.h"
#include "../libcs50/set.h"

/**************** file-local global variables ****************/
static const int MaxNameLength = 50;   // max number of chars in playerName
static const int MaxPlayers = 26;      // maximum number of players
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles

/**************** global types ****************/
typedef struct game {
    roster_t* roster;
    grid_t* base_grid;
    grid_t* gold_grid;
    grid_t* master_grid;    //grid with players and gold, what spectator sees
    addr_t spectator;
    int allocatedGold; 
    hashtable_t* player_hashtable; 
    set_t* gold_set;
    int playerID;
} game_t;

/**************** functions ****************/

/**************** game_new() ****************/
/* Start a new game 
 * 
 * We return: 
 *   a pointer to a new game, or NULL if error
 * 
 * Caller is responsible for:
 *   later calling game_delete
 */
game_t* game_new();

/**************** game_delete() ****************/
/* Deletes game
 * 
 * Caller provides: 
 *   valid pointer to game
 * 
 * We do:
 *   nothing, if game is null
 *   otherwise, delete roster, spectator, master grid
 *   base grid and the game itself
 */
void game_delete(game_t* game);


/* 
 * game.c - CS50 'game' module
 *
 * see game.h for more information.
 *
 * Jeanmarcos Perez; November 5, 2021
 * Anna Martin; November 10, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "roster.h"
#include "player_struct.h"
#include "../libcs50/mem.h"
#include "../libcs50/set.h"
#include "grid.h"
#include "../libcs50/hashtable.h"
#include "../support/message.h"
#include "game.h"

/**************** game_new() ****************/
/* see game.h for description */
game_t* game_new(){
    game_t* game = mem_malloc(sizeof(game_t));

    if(game == NULL){
        return NULL;        //error allocating game
    }

    game->roster = roster_new();
    game-> master_grid = NULL;
    game-> gold_grid =NULL;
    game->base_grid= NULL;
    game->spectator = message_noAddr();
    game ->allocatedGold=0;
    game-> player_hashtable = hashtable_new(MaxPlayers);
    game->gold_set=set_new();
    game->playerID=0;
    

    return game;
}

void itemdelete2(void *item){
    ;
}

/**************** game_delete() ****************/
/* see game.h for description */
void game_delete(game_t* game){
    if(game != NULL){
        roster_delete(game->roster);
        delete_grid(game->master_grid);
        delete_grid(game->base_grid);
        delete_grid(game->gold_grid);
        hashtable_delete(game->player_hashtable, itemdelete2);
        set_delete(game->gold_set, free);
        mem_free(game);
    }
}

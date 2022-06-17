/* 
 * testing file for game.c
 * Emilie Hopkinson
 * cs 50 lab nuggets
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "roster.h"
#include "player_struct.h"
#include "../libcs50/mem.h"
#include "../libcs50/set.h"
#include "../libcs50/file.h"
#include "grid.h"
#include "../libcs50/hashtable.h"
#include "../support/message.h"
#include "game.h"


int main(int argc, char *argv[])
{

    printf("_________________________ makeing new game______________________\n");
    game_t* game = game_new();

    printf("__________________________making the new master grid ____________-\n");
    game->master_grid = grid_new("../maps/small.txt");
    put_chars_in_grid("../maps/small.txt", game->master_grid);


    printf("__________________________printing the new master grid ____________-\n");
    print_grid(stdout, game->master_grid);


    printf("__________________________testing game delete funct ____________-\n");

    game_delete(game);

return 0;
}


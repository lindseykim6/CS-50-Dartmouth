/* 
 * testing file for game.c
 * Emilie Hopkinson
 * cs 50 lab nuggets
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/mem.h"
#include "../libcs50/set.h"
#include "../libcs50/file.h"
#include "grid.h"
#include "game.h"
#include "player_struct.h"
#include "roster.h"


int main(int argc, char *argv[])
{

    printf("\n\n___________________ testing roster new ________________\n");
    roster_t* roster = roster_new();
    

    printf("\n\n___________________ testing roster add ________________\n");
    addr_t add;
    player_struct_t* player = player_struct_new("test name", 5, 5, add);

    printf("\n\n___________________ testing roster add2 ________________\n");
    fflush(stdout);

    const char* playerID = "A";
    if(roster_add(roster, playerID, player)){
        printf("\n successfully added a player\n");
    } else {
        printf("failed to add a player\n");
    }

    roster_delete(roster);

return 0;
}
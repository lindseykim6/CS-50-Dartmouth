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


int main(int argc, char *argv[])
{

    printf("\n\n__________________ making new player ___________________\n\n");

    addr_t add;
    player_struct_t* player = player_struct_new("test name", 5, 5, add);


    printf("\n\n__________________ testing set gold before and after ___________________\n\n");
    int current_gold = player_struct_getGold(player);
    printf("current gold for player is: %d\n", current_gold);


    printf("\n\n__________________ adding gold ___________________\n\n");
    player_struct_addGold(player,1);


    printf("\n\n__________________ get fold function ___________________\n\n");
    int current_gold2 = player_struct_getGold(player);
    printf("current gold for player is: %d", current_gold2);

    printf("\n\n__________________ get name function___________________\n\n");
    char* name = player_struct_getName(player);
    printf("Players name found is ------ %s\n", name);


    printf("\n\n__________________  getting x and y ___________________\n\n");
    int x; int y;
    x = player_struct_getX(player);
    y = player_struct_getY(player);
    printf("Players x and y ==   %d     %d  \n", x,y);


    printf("__________________ testing delete player ___________________\n\n");
    player_struct_delete(player);


return 0;
}
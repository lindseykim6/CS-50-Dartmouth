/* 
 * testing file for gridTest.c
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


int main(int argc, char *argv[])
{

    printf("\n_______________________ first grid _____________\n\n");
    grid_t* grid = grid_new("../maps/small.txt");

    put_chars_in_grid("../maps/small.txt", grid);
    print_grid(stdout, grid);

    printf("\n\nnow testing the get and set char functions\n\n");

    char* temp1 = get_char(0,0, grid);
    char* temp2 = get_char(0,2, grid);
    char* temp3 = get_char(0,3, grid);
    char* temp4 = get_char(2,2, grid);

    printf("char at 0,0 = %s\n", temp1);
    printf("char at 0,2 =   %s\n", temp2);
    printf("char at 0,3 =   %s\n", temp3);
    printf("char at 2,2 =   %s\n", temp4);
    mem_free(temp1);
    mem_free(temp2);
    mem_free(temp3);
    mem_free(temp4);

    printf("\nbefore calling set char\n");
    print_grid(stdout, grid);

    printf ("\nafter calling set char\n");
    set_char(3,10, '@', grid);
    print_grid(stdout, grid);

    printf ("\nafter calling set char\n");
    set_char(2,8, 'A', grid);
    print_grid(stdout, grid);


    printf ("\nattmpting to set char on outside, wall or bridge\n");
    set_char(0,2, 'A', grid);
    set_char(0,3, 'B', grid);
    set_char(2,2, 'C', grid);
    print_grid(stdout, grid);


    printf("\n_______________________ grid 2 string _____________\n\n");
    char* grid2string = grid_2_string(grid);
    printf("%s", grid2string);
    mem_free(grid2string);


    delete_grid(grid);



    printf("\n_______________________ first grid _____________\n\n");
    grid_t* gridDemo = grid_new("../maps/visdemo.txt");
    grid_t* vis2player = grid_new("../maps/visdemo.txt");
    put_chars_in_grid("../maps/visdemo.txt", gridDemo);
    print_grid(stdout, gridDemo);

    printf("\n_______________________ grid before _____________\n\n");
    print_grid(stdout, vis2player);

    printf("\n\n ----------------------------------grid AFTER vision------------------------\n");
    set_SEEN_char(2,8, '@', vis2player);

    printf("rows = %d       columns = %d", grid_getRows(vis2player), grid_getCols(vis2player));


    add_visible_to_player(2, 8, vis2player, gridDemo);
    print_grid(stdout, vis2player);

    delete_grid(gridDemo);
    delete_grid(vis2player);








    printf("\n_______________________ first grid _____________\n\n");

    grid_t* gridDemo1 = grid_new("../maps/fewspots.txt");
    grid_t* vis2player1 = grid_new("../maps/fewspots.txt");

    put_chars_in_grid("../maps/fewspots.txt", gridDemo1);
    print_grid(stdout, gridDemo1);

    printf("\n_______________________ grid before _____________\n\n");
    print_grid(stdout, vis2player1);

    printf("\n\n ----------------------------------grid AFTER vision------------------------\n");
    set_SEEN_char(1,5, '@', vis2player1);

    printf("rows = %d       columns = %d", grid_getRows(vis2player1), grid_getCols(vis2player1));

    add_visible_to_player(1, 5, vis2player1, gridDemo1);

    print_grid(stdout, vis2player1);
    delete_grid(gridDemo1);
    delete_grid(vis2player1);










    //************************************************* bigger grid *****************************************

    printf("\n_______________________ second grid _____________\n\n");

    grid_t* grid2 = grid_new("../maps/big.txt");
    put_chars_in_grid("../maps/big.txt", grid2);
    print_grid(stdout, grid2);

    printf("\n attempting to set char in invalid size\n");
    set_char(100,100, 'A', grid2);
    printf("\n\n");

    printf("\n attempting to get char in invalid size\n");
    get_char(-100,1000, grid2);
    printf("\n\n");

    print_grid(stdout, grid2);


    printf("\n\n ----------------------------------Testing Field of vision------------------------\n");
    printf("\n making a new empty grid with same dimensions\n");
    grid_t* visible2player = grid_new("../maps/big.txt");
    printf("\n\n ----------------------------------grid BEFORE vision------------------------\n");
    print_grid(stdout, visible2player);
    printf("\n\n ----------------------------------grid AFTER vision------------------------\n");
    set_SEEN_char(13,18, 'A', visible2player);
    // set_SEEN_char(8,21,'B',visible2player);
    add_visible_to_player(13, 18, visible2player, grid2);
    print_grid(stdout, visible2player);

    printf("\n\n ----------------------------------grid AFTER2 vision------------------------\n");
    add_visible_to_player(13, 15, visible2player, grid2);
    add_visible_to_player(13, 12, visible2player, grid2);
    add_visible_to_player(13, 10, visible2player, grid2);
    add_visible_to_player(13, 7, visible2player, grid2);
    set_SEEN_char(13,7, 'A', visible2player);
    print_grid(stdout, visible2player);



    delete_grid(grid2);
    delete_grid(visible2player);



    printf("\n_______________________ testing grid new with invalid source _____________\n\n");

    grid_t* grid3 = grid_new("invalid.txt");
    print_grid(stdout, grid3);
    put_chars_in_grid("big_padded.txt", grid3);
    delete_grid(grid3);


    return 0;
}
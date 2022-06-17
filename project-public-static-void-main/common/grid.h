/* 
 * grid.h - header file for Nuggets grid module
 *
 * A "grid" is a module that will represent the map in a way that can be read and manipulated for the players to 
 * move and explore and for the spectator to observe
 * 
 * Jeanmarcos Perez, November 2021
 * Anna Martin, November 2021
 */

#ifndef __GRID_H
#define __GRID_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>



/**************** global types ****************/
typedef struct grid {
    int nrow;               //number of rows
    int ncol;               //number of columns
    char** array;
} grid_t;

/**************** functions ****************/

/**************** grid_new() ****************/
/* Create a new grid for a given map
 * 
 * We return:
 *   pointer to new grid, or NULL if error
 * 
 * Caller is reponsible for:
 *   later calling grid_delete
 */ 
grid_t* grid_new(char* mapDir);

/**************** find_X_of_array ****************/
/* 
 *  takes in a file
 * check if its valid
 * if valid, then we read one line
 *      return the lens of that line, which is the # of cols
 */ 
int find_cols_of_array(char* mapDir);

/**************** find_Y_of_array() ****************/
/* 
 *  takes in a file
 * check if its valid
 * if valid, then we loop through all the lines
 *      and keep track with an int pointer until we hit null
 * returns the rows of a text file
 */ 
int find_rows_of_array(char* mapDir);


/**************** put_chars_in_grid ****************/
/* 
 *  takes in a file
 * check if its valid
 * if valid, then we loop through all the lines
 *     and then assign the empty grid values with the file read char values
 * returns a valid pointer
 */ 
void* put_chars_in_grid(char* mapDir, grid_t* emptyGrid);



/******************* print_grid *****************/
/*
* if given a valid file and grid pointer
* this function will loop throguh the entire 2d array
* and print out the values to the desired fiel
*
*/
void* print_grid(FILE* fp, grid_t* grid);


/******************* delete_grid *****************/
/*
* if given a valid grid
*
* will free all the malloced spaced for the 2D array
*   by looping through and freeing each line
* and then freeing the grid pointer itselg
*/
void* delete_grid(grid_t* grid);



/******************* get char at location x,y *****************/
/*
* given a valid grid pointer
* and chekcing that row and column ints are within the grid boundaries
*
* we MALLOC A NEW CHAR, and return the pointer to that char
* copied from the 2d array value
*/
char* get_char(int row, int column, grid_t* grid);

/******************* set char at lcoation x,y*****************/
/*
* given a valid grid pointer
* and chekcing that row and column ints are within the grid boundaries
*
* we check if it is not a wall char or GOLD (+-|* )
* if it is not a wall boundary, then we set the char to the valid char given
* return true if we successfully changed it
*/
bool set_char(int row, int column, char character, grid_t* grid);


/******************* set char at lcoation x,y*****************/
/*
* given a valid grid pointer
* and chekcing that row and column ints are within the grid boundaries
*  
* we set the char to the valid char given
* return true if we successfully changed it
*/
bool set_SEEN_char(int row, int column, char character, grid_t* grid);

/**************** grid_2_string() ****************/
/* Return the grid as a string
 * 
 * We return:
 *   the grid represented as a string
 * 
 * Caller is reponsible for:
 *   providing the valid grid 
 */ 
char* grid_2_string(grid_t* grid_2_string);

/**************** grid_getRows() ****************/
/* Return the number of rows
 * 
 * We return:
 *   the number of rows in the grid
 * 
 * Caller is reponsible for:
 *   nothing
 */ 
int grid_getRows(grid_t* grid);

/**************** grid_getCols() ****************/
/* Return the number of columns
 * 
 * We return:
 *   the number of columns in the grid
 * 
 * Caller is reponsible for:
 *   providing a valid grid
 */ 
int grid_getCols(grid_t* grid);

/************** add_visible_to_player ***********/
/* Returns nothing
 *
 * We return:
 *   nothing
 * 
 * Caller is responsible for:
 *   providing a valid location of the player, a player's personal grid, and the master grid
 * 
*/
grid_t* add_visible_to_player(int player_x, int player_y, grid_t* grid, grid_t* masterGrid);

/************** inPlayerSight ***********/
/* Returns a boolean
 *
 * We return:
 *   A boolean indicating whether a point is in the players sight
 * 
 * Caller is responsible for:
 *   providing a valid location of the player, valid coordinates on the grid, and the master grid
 *
 */
bool inPlayerSight(int player_row, int player_col, int coord_row, int coord_col, grid_t* masterGrid);

/************** inPlayerSight ***********/
/*
 * retruns true if the coordinate is a wall char
*/
bool if_wall(int row, int col, grid_t* masterGrid);

#endif // __GRID_H
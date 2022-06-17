/* 
 * grid.c - Nuggets 'grid' module
 *
 * see grid.h for more information.
 *
 * Emilie Hopkinson; November 2, 2021
 * Lindsey Kim; November 11, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "grid.h"
// #include "roster.h"


/**************** file-local global variables ****************/

/**************** grid_new() ****************/
/* see grid.h for description */
grid_t*
grid_new(char* mapDir){

    // if null, print to stderr and dont do anything 
    if (mapDir == NULL) {
        fprintf(stderr, "***ERROR*** mapDir cannot be NULL");

    // else we have at least a char directory, not gauranteed yet
    } else {

          // getting sizes for the grids
        int cols = find_cols_of_array(mapDir);
        int rows = find_rows_of_array(mapDir);

        // if we have no colls and rows then we just break, we do not have valid grid file
        if (cols == 0 && rows == 0){
            fprintf(stderr, "\n***ERROR*** grid cannot be created\n");
            return NULL;
        }

        // uncomment to check if correct sizes
        // printf("cols = %d\n", cols);
        // printf("rows = %d\n", rows);

        // mallocing mem for grid
        grid_t* grid = mem_malloc(sizeof(grid_t));

        // setting variables
        grid -> nrow = rows;
        grid -> ncol = cols;

        // char** tempGrid[rows][cols];

        // malloc memory for 2d array
        // first the rows
        grid -> array = mem_malloc(rows*sizeof(char*)+2);
        int currRow;
        
        // and now mallocing the collumns 
        for (currRow = 0; currRow < rows; currRow++)
            grid -> array[currRow] = malloc(cols*sizeof(char)+2);


        //initializing to empty!!!!
        for(int i=0; i < rows; i++){
            for (int j=0; j< cols; j++){
                grid -> array[i][j] = ' ';
            }
        }

        // return the grid
        return grid;  
    }

    // we failed requirements and return null
    return NULL;
}

/**************** find_X_of_array ****************/
/* see grid.h for description */
int find_cols_of_array(char* mapDir){

    // if out char* dir is null, we just return 0
    if (mapDir == NULL) {
        fprintf(stderr, "\n***ERROR*** mapDir cannot be NULL\n");
        return 0;
    }

    FILE* fp;
    // fp = fopen(mapDir, "r");

    // failed to read map dir
    if ((fp = fopen(mapDir, "r")) == NULL) {
        fprintf(stderr, "\n***ERROR*** mapDir cannot opened\n");

    } else {

        // we find the str len of the PADDED line
        char* row = file_readLine(fp);
        int numb_cols = strlen(row);
        mem_free(row);

        // CLOSE THE FILE and return the row counters
        fclose(fp);
        return numb_cols;
    }
    return 0;
}


/**************** find_Y_of_array ****************/
/* see grid.h for description */
int find_rows_of_array(char* mapDir){


    // again check if we are given a map directory
    if (mapDir == NULL) {
        fprintf(stderr, "\n***ERROR*** mapDir cannot be NULL\n");
        return 0;
    }

    FILE* fp2;
    fp2 = fopen(mapDir, "r");

    // we failed to read the file
    if (fp2 == NULL) {
        fprintf(stderr, "\n***ERROR*** mapDir cannot opened\n");
        return 0;
    } else {
        // we loop through all the lines in the file, keep track with int
        int numb_rows = 0;
        char* row;
        while((row = file_readLine(fp2)) != NULL){
            numb_rows += 1;
            mem_free(row);
        }

        //close the fil
        fclose(fp2);
        return numb_rows;
    }
    return 0;
}

/**************** put_chars_in_grid ****************/
/* see grid.h for description */
void* put_chars_in_grid(char* mapDir, grid_t* emptyGrid){

    // if we are not given a map dir
    if (mapDir == NULL) {
        fprintf(stderr, "\n***ERROR*** mapDir cannot be NULL\n");
        return 0;
    }

    FILE* fp;
    fp = fopen(mapDir, "r");
    
    // we failed to open
    if (fp == NULL) {
        fprintf(stderr, "\n***ERROR*** mapDir cannot opened\n");
        return 0;
    }

    // we were not given a valid grid
    if(emptyGrid == NULL){
        fprintf(stderr, "\n***ERROR*** empty grid is null\n");
        fclose(fp);
        return 0;
    }

    char* row;

    /// if we have valid params
    if (fp != NULL && emptyGrid != NULL){

        // loop through all the rows in the file we are reading
        for(int currRow = 0; currRow < emptyGrid ->nrow; currRow ++){

            row = file_readLine(fp);

            // the size should aready be malloced in new, therefore we just need to copy
            strcpy(emptyGrid->array[currRow], row);

            mem_free(row);
            // printf("%s\n", emptyGrid->array[currRow]);
        }

        fclose(fp);
        return emptyGrid;
    }
    fclose(fp);
    return 0;
}


/******************* print Grid *****************/
/* see grid.h for description */
void* print_grid(FILE* fp, grid_t* grid){

    // invalid entry NULL
    if(grid == NULL){
        fprintf(stderr, "\n***ERROR***  grid is null\n");

    // fp cannot be null
    } else if (fp == NULL){
        fprintf(stderr, "\n***ERROR***  file to print out is null\n");
        return 0;

    // loop through the grid, rows and cols, and print the value found
    } else {

        int rows = grid -> nrow;
        int cols = grid -> ncol;

        for (int i = 0; i < rows; i++){

            for (int j = 0; j < cols; j ++){

                fprintf(fp, "%c", grid -> array[i][j]);
            }

            fprintf(fp, "\n");
        }
    }

    return 0;
}


/******************* delete Grid *****************/
/* see grid.h for description */
void* delete_grid(grid_t* grid){

    // invalid NULL entry
    if(grid == NULL){
        fprintf(stderr, "\n***ERROR***  grid is null\n");

    // else we loop through every row
    } else {

       int rows = grid -> nrow;

        // free that row
        for (int i = 0; i < rows; i++){
            mem_free(grid->array[i]);
        }
        // free the entire array
        mem_free(grid->array);

        // free the grid pointer itself
        mem_free(grid); 
    }

    return 0;
}

//******************* get char at location x,y *****************/
/* see grid.h for description */
char* get_char(int row, int column, grid_t* grid){
    if (row >= 0 && row < grid->nrow && column >= 0 && column < grid->ncol) {
        // create a new char with malloced space,
        char* foundChar = mem_calloc(1,sizeof(char*)+1);
        sprintf(foundChar, "%c", grid -> array[row][column]);

        //return the newly malloced character
        return foundChar;

    } else {
        return NULL;
    }
    
}

/******************* set char at location x,y *****************/
/* see grid.h for description */
bool set_char(int row, int column, char character, grid_t* grid){
    // characters that we cannot replace!!!
    const char* invalidChars = "*+|- ";
    if ( row >= 0 && row < grid->nrow && column >= 0 && column < grid->ncol) {
        
        // IF NOT A WALL
        if(!strchr(invalidChars, grid->array[row][column])){
            grid -> array[row][column] = character;
            return true; 
        }
        else {
            return false;
        }
    } else {
        return false;
    }
}


/**************** grid_2_string(grid_t* grid_2_string); ****************/
char* grid_2_string(grid_t* grid){

    // char returnedGrid{(sizeof(char*)*(grid_2_string->ncol)*(grid_2_string->nrow)+1);

        int rows;
        int cols;

        rows = grid -> nrow;
        cols = grid -> ncol;

        // malloc memory for 2d array
        //initializing to empty!!!!

        char* returned_Grid = mem_calloc((rows*cols)+1, sizeof(char*));

        for (int i = 0; i < rows; i++){

            for (int j = 0; j < cols; j ++){
//                 fprintf(stderr, "%c", grid->array[i][j]);
                sprintf(returned_Grid, "%s%c", returned_Grid, grid->array[i][j] );
            }
//             fprintf(stderr, "\n");
            sprintf(returned_Grid, "%s\n", returned_Grid);
        }

    // printf("%s", returned_Grid);

    return returned_Grid;
}


/******************* set char at location x,y *****************/
/* see grid.h for description */
bool set_SEEN_char(int row, int column, char character, grid_t* grid){
    
    if ( row >= 0 && row < grid->nrow && column >= 0 && column < grid->ncol) {
        grid -> array[row][column] = character;
        return true;

    // error message for out of bounds
    } else {
        fprintf(stderr, "\n***ERROR***  invalid index of insertion\n");
        fprintf(stderr, "must be in bounds of %d and %d \n", grid->nrow, grid->ncol);
        return false;
    }
}

/**************** grid_getRows() ****************/
/* see grid.h for description */
int grid_getRows(grid_t* grid){
    if(grid != NULL){
        return grid->nrow;
    }
    return 0;
}

/**************** grid_getCols() ****************/
/* see grid.h for description */
int grid_getCols(grid_t* grid){
    if (grid != NULL){
        return grid->ncol;
    }
    return 0;
}
/************** add_visible_to_player ***********/
/* see grid.h for description */
grid_t* add_visible_to_player(int player_row, int player_col, grid_t* seenGrid, grid_t* masterGrid){

    if(seenGrid == NULL || masterGrid == NULL){
        fprintf(stderr, "\n**Error** adding visible to player is null\n");

    } else if (seenGrid->ncol != masterGrid->ncol || seenGrid->nrow != masterGrid->nrow){
        fprintf(stderr, "\n**Error** mismatched dimensions\n");

    } else  {

        int rows = masterGrid->nrow;
        int cols = masterGrid->ncol;

        for (int i=0; i<rows;i++){

            for (int j=0; j<cols;j++){

                char* temp = get_char(i,j,masterGrid);
                // if it is a rock ignrore
                if (temp[0] == ' '){
                    
                }

                else {
                    if(inPlayerSight(player_row, player_col, i,j, masterGrid)){
                        set_SEEN_char(i, j, temp[0], seenGrid);
                    }
                }
                mem_free(temp);
                
            }
        }
    }
    return seenGrid;
}


bool inPlayerSight(int player_row, int player_col, int coord_row, int coord_col, grid_t* masterGrid){

    // if we are given valid params
    if(masterGrid == NULL){
        return false;

    } else {

        // if in bounds
        if (player_col >= 0 && player_row >= 0 && player_col <= masterGrid->ncol && player_row <= masterGrid->nrow){
            // printf("HERE\n");

            // making math clearner
            int row_1; 
            int col_1;
            int row_2;
            int col_2;

            // we will always loop from left to right
            // and top from bottom

            // so we find which one (the player or the checked coord) is top
            // and find whcih one (the player or the checked coord) is left


            // if the player is above the coordinates we want
            if(player_row > coord_row){
                row_1 = coord_row;
                row_2 = player_row;

            // else the player is below
            } else {
                row_1 = player_row;
                row_2 = coord_row;
            }

            // if the player is to the right of us
            if (player_col > coord_col){
                col_1 = coord_col;
                col_2 = player_col;

            // else the player is to the left of us
            } else {
                col_1 = player_col;
                col_2 = coord_col;
            }

            // finding the dx dy for slope
            int dcol = coord_col - player_col;
            int drow = coord_row - player_row;


            // if the coordinate we aree checking is on the same col as player
            if (dcol == 0){
                for(int curr_row = row_1 + 1; curr_row < row_2; curr_row ++){
                    if(if_wall(curr_row, player_col, masterGrid)){
                        return false;
                    }
                }
                return true;
            }

            // if the coordinate we are checking is on the same row as the player
            else if (drow == 0){
                for (int curr_col = col_1 + 1; curr_col < col_2; curr_col ++){
                    if(if_wall(player_row, curr_col, masterGrid)){
                        return false;
                    }
                }
                return true;
            }

            // else we need to calculate the diagnol for ever col between coord col and player col
            // and every row between coord row and player row
            else{

                // for every row between player row and coord row
                for (int row = row_1 + 1; row < row_2; row ++){

                    // calculate the col for that intersection
                    float calc_col = (float)(row - player_row) * dcol/drow + player_col;

                    // if it intersects perfectly, 
                    if (calc_col == (int) calc_col){
                        if(if_wall(row, (int)calc_col, masterGrid)){
                            return false;
                        }


                    // else it does not intersect perfectly, and we need to check 2 points
                    } else {
                        
                        // if this col and the next col is a wall return false, failed to add
                        if(if_wall(row, (int)calc_col, masterGrid) && if_wall(row, (int)calc_col + 1, masterGrid)){
                            return false;
                        }
                    }
                }


                // for every col between player col and coord row
                for (int col = col_1 + 1; col < col_2; col ++){

                    float calc_row = (float)(col - player_col) * drow/dcol + player_row;


                    // if it intersects perfectly
                    if (calc_row == (int)calc_row){
                        if(if_wall((int)calc_row, col, masterGrid)){
                            return false;
                        }

                    // else we check this row and the next row
                    } else {

                        if(if_wall((int)calc_row, col, masterGrid) && if_wall((int)calc_row + 1, col, masterGrid)){
                            return false;
                        }
                    }

                }
                return true;
            }
        }
    }
    return false;
}
            
    

bool if_wall(int row, int col, grid_t* masterGrid){

    char* found_char = get_char(row, col, masterGrid);
    
    // if the given coord is a wall
    if (found_char[0] == '|' ||  found_char[0] == '#' || found_char[0] == '-' || found_char[0] == '+'||found_char[0] == ' '){
        mem_free(found_char);
        return true;
    }
    mem_free(found_char);
    return false;

}

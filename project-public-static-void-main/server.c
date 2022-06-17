/* 
 * server.c 
 *
 * usage: ./server map.txt [seed]
 * 
 * map.txt: pathname for a map
 * [seed]: optional seed for the random-number generator, must be a positive integer
 *      
 * Lindsey Kim; November, 2021
 * Jeanmarcos Perez; November, 2021
 * Anna Martin; November, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include "player_struct.h"
#include "roster.h"
#include "grid.h"
#include "message.h"
#include "game.h"
#include "set.h"
#include "mem.h"
#include "hashtable.h"
#include "file.h"

/**************** file-local global variables ****************/
static game_t* game;

/**************** functions ****************/
static void parseArgs(const int argc, char* argv[]);
static void initializeGame(char* map);
bool handleInput (void *arg);
static bool handleMessage(void* arg, const addr_t from, const char* message);
//static void updatePersonals (void *arg, const char *key, void *item);
static bool checkLocation(int x, int y, grid_t* grid);
static player_struct_t* findPlayer(addr_t from);
static void find_player_helper(void *arg, const char *key, void *item);
static grid_t* setGold(grid_t* masterGrid);
static void foundGold(player_struct_t* player);
static void find_player_helper(void *arg, const char *key, void *item);
static player_struct_t* findPlayer(const addr_t from);
static void getStrings (void *arg, const char *key, void *item);
static bool gameOver(player_struct_t* player);
static void spectate(const addr_t from, const char* message);
static void play(const addr_t from, const char* message, char* mapDir);
static bool key(const addr_t from, const char* message);
static void ok(const addr_t from);
static void grid(const addr_t from);
static void gold(const addr_t from, int collected);
static void display(const addr_t from);
static bool quit(const addr_t from);
static bool h(const addr_t from);
static bool l(const addr_t from);
static bool j(const addr_t from);
static bool k(const addr_t from);
static bool y(const addr_t from);
static bool u(const addr_t from);
static bool b(const addr_t from);
static bool n(const addr_t from);

/**************** main() ****************/
/* 
 * Will call parse args, and acts as a driver for the rest of the server file.
 * 
 * Exits with 0 status if there are no errors 
 */

int main(const int argc, char* argv[]){
    parseArgs(argc, argv);
    initializeGame(argv[1]);
    int port_number = message_init(stdin);
    fprintf(stderr, "Port number: %d\n", port_number);
    message_loop(argv[1], 0, NULL, handleInput, handleMessage);
    message_done();
    exit(0);
}

bool handleInput (void *arg) {
    if(feof(stdin)) {
        return true;
    } else {
        return false;
    }
}

/**************** parseArgs() ****************/
/* 
 * Takes in all arguments
 *
 * If there is a seed, it will be initialized, otherwise a random seed will be generated
 */
static void parseArgs(const int argc, char* argv[]){
    if(argc == 2 || argc == 3){
        //checks if able to open map.txt 
        FILE* fp = fopen(argv[1], "r");
        if(fp == NULL){
            fprintf(stderr, "Map file cannot be read\n");
            fclose(fp);
            exit(1);
        } else {
            fclose(fp);
        }
        //checks if seed is a postive integer
        if(argc == 3){
            char* seedarg = argv[2];;
            int seed;;
            sscanf(seedarg, "%d", &seed);
            if(sscanf(seedarg, "%d", &seed) != 1){
                fprintf(stderr, "Seed must be an integer\n");
                exit(1);
            }
            else if(seed < 0){
                fprintf(stderr, "Seed must be positive\n");
                exit(1);;
            }
            else{
                srand(seed);
            }
        }
        else if(argc == 2){
            srand(getpid());;
       } 
    } 
    else{
        fprintf(stderr, "Invalid number of arguments\n");
        exit(1);;
    }
}

/**************** initializeGame() ****************/
/* 
 * Caller must provide: a valid map
 *
 * We return: nothing 
 * 
 * This function will initialize a game struct, create a master grid, and set the gold on the grid
 */
static void initializeGame(char* map){
    game = game_new();
    game -> base_grid = grid_new(map);
    game -> gold_grid = grid_new(map);
    game -> master_grid = grid_new(map);
    put_chars_in_grid(map, game->master_grid);;
    put_chars_in_grid(map, game->gold_grid);
    put_chars_in_grid(map, game->base_grid);
    setGold(game->master_grid);
}

/**************** handleMessage() ****************/
/* 
 * Caller must provide: a valid arg, address, and message
 * 
 * We return: a boolean
 * 
 * This function will parse and handle the messages from client as to whether the plater will be a player, spectate, quit, or make a movement
 */
static bool handleMessage(void* arg, const addr_t from, const char* message){
    char* mapDir=arg;
    if(strncmp(message, "PLAY ", strlen("PLAY ")) == 0){
        play(from, message, mapDir);
    }
    if (strncmp(message, "KEY ", strlen("KEY ")) == 0){
        return key(from, message);
    }
    if (strncmp(message, "SPECTATE", strlen("SPECTATE")) == 0){
        spectate(from, message);
    }
    return false;

}

/**************** getStrings() ****************/
/* 
 * Caller must provide: valid arg, key, and item
 *
 * We return: nothing
 *
 * This function will be used to establish player summary (containing: ID, gold tally, name)
 */
static void getStrings (void *arg, const char *key, void *item) {
    player_struct_t* player= item;
    char** playerSummary= arg;
    sprintf(*playerSummary, "%s%10s %5d %s\n", *playerSummary, player_struct_getID(player), player_struct_getGold(player), player_struct_getName(player));
    arg=&playerSummary;
}

/**************** gameOver() ****************/
/* 
 * Caller must provide: a valid player
 * 
 * We return: a boolean that is true if it is game over
 * 
 * This function will quit the game
 */
static bool gameOver(player_struct_t* player) {
    if(game->allocatedGold==0) {
        int rowSize= 20 + MaxNameLength;
        char* string =calloc(rowSize*game->roster->numOfPlayers, sizeof(char));
        sprintf(string, "QUIT GAME OVER:\n");
        set_iterate(game->roster->set, &string, getStrings);
        message_send(player->player_address, string);
        fprintf(stderr, "%s\n", string);
        free(string);
        game_delete(game);
        return true;
    }
    else {
        return false;
    }
}

/**************** updatePersonals() ****************/
/* 
 * Caller must provide: a valid arg, a key, a valid item 
 * 
 * We return: nothing
 * 
 * This function will update an individual player's personal grid 
 */
static void updatePersonals (void *arg, const char *key, void *item) {
    player_struct_t* player = item;
    player->personal_grid=add_visible_to_player(player->yCoor, player->xCoor, player->personal_grid, game->master_grid);
}

/**************** checkLocation() ****************/
/* 
 * Caller must provide: a valid x, y coordinate and a grid 
 * 
 * We return: true if the given x, y on the grid is an open space (a '.')
 * 
 * This function is used to check given coordinates to make sure it is within boundaries and not a wall or corner or path
 */
static bool checkLocation(int x, int y, grid_t* grid){
    char* location= get_char(y,x, grid);
    if(location!=NULL && strcmp(location,".") == 0){
        free(location);
        return true;
    } else {
        free(location);;
        return false;
    }
    
    //eventually. check if player exists at location and bump them
}

// void itemprint(FILE* fp, const char *key, void *item){
//     char* randPieces=item;
//     fprintf(stderr, "\nKEY: '%s': ", key);
//     fprintf(stderr,"ITEM: '%s'\n", randPieces);
// }
/**************** foundGold() ****************/
/* 
 * Caller must provide: a valid player
 *
 * We return: nothing
 * 
 * This function handles when a player collects a piece of gold 
 */
static void foundGold(player_struct_t* player) {
    char* originalChar =get_char(player_struct_getY(player), player_struct_getX(player), game->base_grid);
    set_SEEN_char(player_struct_getY(player), player_struct_getX(player), originalChar[0], game->gold_grid);
    char* x_string= mem_malloc(sizeof(char)*5);
    sprintf(x_string, "%d", player->xCoor);
    char* y_string= malloc(sizeof(char)*5);
    sprintf(y_string, "%d", player->yCoor);
    
    char* xy_string= calloc(strlen(x_string) + strlen(y_string), sizeof(char*));
    sprintf(xy_string, "%s,%s",x_string, y_string);

    free(x_string);
    free(y_string);;

    //set_print(game->gold_set, stderr, itemprint);
    char* numGold= set_find(game->gold_set,xy_string);

    int numberGold =0;
    sscanf(numGold, "%d", &numberGold);

    player->goldTally += numberGold;
    game->allocatedGold -= numberGold;

    free(originalChar);
    free(xy_string);
}

/**************** find_player_helper() ****************/
/* 
 * Caller must provide: a valid arg, key, and item 
 * 
 * We return: nothing 
 * 
 * This function is a helper function to find a specific player 
 */
static void find_player_helper(void *arg, const char *key, void *item) {
    addr_t* address = item;
    addr_t* from = arg;
    if(message_eqAddr(*address, *from)) {
        game->playerID = key[0]-'a';
    }
}

/**************** findPlayer() ****************/
/* 
 * Caller must provide: a player address
 * 
 * We return: the player struct
 * 
 * This function simply finds a player based on their address
 */
static player_struct_t* findPlayer(const addr_t from){
    addr_t from_copy=from;
    hashtable_iterate(game->player_hashtable, &from_copy, find_player_helper);
    char playerID='a'+game->playerID;
    char* playerIDString = mem_malloc(sizeof(char)+1);
    sprintf(playerIDString, "%c", playerID);
    player_struct_t* player = set_find(game->roster->set, playerIDString);
    free(playerIDString);
    return player;
}



/**************** setGold() ****************/
/* 
 * Caller must provide: the master grid 
 * 
 * We return: a grid with all of the gold set 
 * 
 * This function will randomly decide how many piles of gold and randomly allocate the pieces of gold between the piles
 * This function will also randomly place all of the gold throughout the map, verifying that it is within bounds 
 */
static grid_t* setGold(grid_t* masterGrid){
    int thisGoldTotal = GoldTotal;
    int nPile = (rand() % (GoldMaxNumPiles - GoldMinNumPiles + 1)) + GoldMinNumPiles;
    for (int i = 0; i < nPile; i ++){
        bool done = false;
        while(!done){
        // for each pile, find a random location (make sure its within the boundaries of the grid)
            int randx = (rand() % (masterGrid->ncol - 0 + 1)) + 0;
            int randy = (rand() % (masterGrid->nrow - 0 + 1)) + 0;
            if (set_char(randy, randx, '*', masterGrid)) {
                set_char(randy, randx, '*', game->gold_grid);
                // for each pile, determine a random number of gold pieces (>= 1, and doesn't exceed max)
                // because each gold pile must have at least one gold piece, but as we decide each piles' piece amounts we want to update the max 
                int workingMax = thisGoldTotal - nPile + i;

                int randPieces = (rand() % (workingMax - 1 + 1)) + 1;
                game-> allocatedGold += randPieces;

                char* x_string= mem_malloc(sizeof(char)*5);
                sprintf(x_string, "%d", randx);
                char* y_string= malloc(sizeof(char)*5);
                sprintf(y_string, "%d", randy);
                
                char* xy_string= calloc(strlen(x_string) + strlen(y_string)+2, sizeof(char));
                sprintf(xy_string, "%s,%s",x_string, y_string);

                char* randPiecesString = calloc(sizeof(char)+1, 5);
                sprintf(randPiecesString, "%d", randPieces);

                set_insert(game->gold_set, xy_string, randPiecesString);

                free(x_string);
                free(y_string);
                free(xy_string);;

                done = true;
                thisGoldTotal = thisGoldTotal - randPieces;
            }
            
        }
        
    }
    //fprintf(stderr, "numpiles: %d\n", nPile);

    return masterGrid;
}

/**************** spectate() ****************/
/* 
 * Caller must provide: the address of a player, and a valid message 
 * 
 * We return: a boolean that is true when the client is a spectator
 * 
 * This function will handle the spectator abilities in the game, making sure there can only be 1 spectator at a time
 */
static void spectate(const addr_t from, const char* message) {
    if(message_isAddr(game->spectator)){
        message_send(game->spectator, "QUIT You have been replaced by a new spectator.");
    }
    game->spectator = from;
}

/**************** play() ****************/
/* 
 * Caller must provide: a player address, message, and map directory
 * 
 * We return: a boolean that is true when the client is a player
 * 
 * This function will randomly place the player on the grid, and handle all other player abilities (movement, collecting gold, etc.)
 */
static void play(const addr_t from, const char* message, char* mapDir) {
    char* messageCopy= mem_malloc(strlen(message)+1);
    strcpy(messageCopy, message);
    char* content = messageCopy + strlen("PLAY ");
    if(game->roster->numOfPlayers >= MaxPlayers){
        free(messageCopy);
        message_send(from, "QUIT Game is full: no more players can join.");
        
    }
    else if(isspace(content[0])){
        free(messageCopy);
        message_send(from, "QUIT Sorry - you must provide player's name.");
        
    }
    else {
        for(int i = 0; i < MaxNameLength; i++){      
            if(!isgraph(*content) && !isspace(*content)){
                *content = '_';
                content++;
            }
        }
        
        //create new player using RAND and add to roster
        int x;
        int y;
        bool done = false;
        while(!done){
            int randx = (rand() % (game->master_grid->ncol - 0 + 1)) + 0;
            int randy = (rand() % (game->master_grid->nrow - 0 + 1)) + 0;

            if (checkLocation(randx, randy, game->base_grid)){
                x = randx;;
                y = randy;
                done = true;
            }
        }
        
        player_struct_t* new_player = player_struct_new(content, x, y, from);
        free(messageCopy);
        char* playerIDcpy= calloc(strlen(player_struct_getID(new_player)) + 1, sizeof(char));
        strcpy(playerIDcpy, player_struct_getID(new_player));
        roster_add(game->roster, playerIDcpy, new_player);
        free(playerIDcpy);
        
        addr_t from_copy=from;
    
        hashtable_insert(game->player_hashtable, new_player->playerID, &from_copy);
        char* goldString=get_char(new_player->yCoor,new_player->xCoor, game->master_grid); 
        if(strcmp(goldString, "*")==0) {  
            foundGold(new_player);
            set_SEEN_char(new_player->yCoor, new_player->xCoor, (new_player->playerID)[0], game->master_grid); 
        } else {
            set_char(new_player->yCoor, new_player->xCoor, (new_player->playerID)[0], game->master_grid);
        }
        free(goldString);
        new_player->personal_grid= grid_new(mapDir);
        add_visible_to_player(new_player->yCoor, new_player->xCoor, new_player->personal_grid, game->master_grid);
        
        ok(from);
        grid(from);
        gold(from, 0);
        display(from);
    }
}

/**************** key() ****************/
/* 
 * Caller must provide: a valid player address, and a valid message
 * 
 * We return: a boolean
 * 
 * This function will handle all of the keystrokes from the player
 */
static bool key(const addr_t from, const char* message) {
    char letter;
    sscanf((message), "KEY %c", &letter);
    switch (letter) {
        case 'Q':
            quit(from);
            break;
        case 'h':
            h(from);
            break;
        case 'l':
            l(from);
            break;
        case 'j':
            j(from);
            break;
        case 'k':
            k(from);
            break;
        case 'y':
            y(from);
            break;
        case 'u':
            u(from);
            break;
        case 'b':
            b(from);
            break;
        case 'n':
            n(from);
            break;
        case 'H':
            while(!gameOver(findPlayer(from))&&!h(from)) {
                ;
            }
            break;
        case 'L':
            while(!gameOver(findPlayer(from))&&!l(from)) {
                ;
            }
            break;
        case 'J':
            while(!gameOver(findPlayer(from))&&!j(from)) {
                ;
            }
            break;
        case 'K':
            while(!gameOver(findPlayer(from))&&!k(from)) {
                ;
            }
            break;
        case 'Y':
            while(!gameOver(findPlayer(from))&&!y(from)) {
                ;
            }
            break;
        case 'U':
            while(!gameOver(findPlayer(from))&&!u(from)) {
                ;
            }
            break;
        case 'B':
            while(!gameOver(findPlayer(from))&&!b(from)) {
                ;
            }
            break;
        case 'N':
            while(!gameOver(findPlayer(from))&&!n(from)) {
                ;
            }
            break;
        default:  
            message_send(from, "ERROR invalid keystroke");
    }
    return gameOver(findPlayer(from));
}

/**************** ok() ****************/
/* 
 * Caller must provide: the player address
 * 
 * We return: nothing
 *
 * This function handles the functionality of the OK function
 */
static void ok(const addr_t from) {
    char* ok_message = mem_calloc(strlen("OK ") + 2, sizeof(char));
    sprintf(ok_message, "OK %s", player_struct_getID(findPlayer(from)));
    message_send(from, ok_message);
    free(ok_message);
}

/**************** grid() ****************/
/* 
 * Caller must provide: the player address
 *
 * We return: nothing
 * 
 * This function handles the communication of the grid between server and client 
 */
static void grid(const addr_t from) {
    char* grid_message = mem_malloc(strlen("GRID ")*sizeof(char) + 2*sizeof(int));
    sprintf(grid_message, "GRID %d %d", grid_getRows(game->master_grid), grid_getCols(game-> master_grid));
    fprintf(stderr, "%s\n", grid_message);
    message_send(from, grid_message);
    free(grid_message);
}

/**************** gold() ****************/
/* 
 * Caller must provide: the player address and the number of collected gold 
 *
 * We return: nothing
 * 
 * This function will handle the messages of collection of gold 
 */
static void gold(const addr_t from, int collected) {
    if(message_eqAddr(from, game->spectator)) {
        char* gold_message = mem_malloc(strlen("GOLD 0 0 ")*sizeof(char) + sizeof(int));
        sprintf(gold_message, "GOLD 0 0 %d", game->allocatedGold);
        message_send(from, gold_message);
        free(gold_message);
    }
    else{char* gold_message = mem_malloc(strlen("GOLD ")*sizeof(char) + 3*sizeof(int));
        sprintf(gold_message, "GOLD %d %d %d", collected, findPlayer(from)->goldTally, game -> allocatedGold);
        message_send(from, gold_message);
        fprintf(stderr, "%s\n", gold_message);
        free(gold_message);
    }
    
}

/**************** display() ****************/
/* 
 * Caller must provide: the player address
 *
 * We return: nothing
 * 
 * This function will handle the display functionality for both players and spectators 
 */
static void display(const addr_t from) {
    if(message_eqAddr(from, game->spectator)) {
        char* gridString= grid_2_string(game->master_grid);
        //need grid to string
        char* status_message = mem_calloc(strlen("Spectator: nuggets unclaimed.") + 5, sizeof(char));
        sprintf(status_message, "Spectator: %d nuggets unclaimed).", game->allocatedGold);
        char* display_message = mem_calloc(strlen("DISPLAY\n") + strlen(status_message) + strlen(gridString), sizeof(char));
        sprintf(display_message, "DISPLAY\n%s%s",status_message, gridString);
        message_send(from, display_message);
        free(status_message);
        free(display_message);
        free(gridString);
    }
    else{
        player_struct_t* player = findPlayer(from);
        char* gridString= grid_2_string(player->personal_grid);
        char* status_message = mem_calloc(strlen("Player has nuggets ( nuggets unclaimed).\n") + 10, sizeof(char));
        sprintf(status_message, "Player %s has %d nuggets (%d nuggets unclaimed).\n", player->playerID, player->goldTally, game->allocatedGold);
        char* display_message = mem_calloc(strlen("DISPLAY\n")+ strlen(status_message) + strlen(gridString)+1, sizeof(char));
        sprintf(display_message, "DISPLAY\n%s%s",status_message, gridString);

        fprintf(stderr, "%s\n", display_message);
        message_send(from, display_message);

        free(status_message);
        free(display_message);
        free(gridString);
    }    
}

/**************** quit() ****************/
/* 
 * Caller must provide: the player address
 *
 * We return: boolean
 * 
 * This function handles the ability to quit the game and sends a message to the player 
 */
static bool quit(const addr_t from) {
    if(message_eqAddr(from, game->spectator)) {
        game->spectator = message_noAddr();
        message_send(from, "QUIT Thanks for watching!");
    }
    else{
        roster_decrement(game->roster);
        player_struct_t* player= findPlayer(from);
        set_char(player->yCoor, player-> xCoor, '.', game->master_grid);
        player_struct_delete(player);
        message_send(from, "QUIT Thanks for playing!");
    }
    return true;
}

/**************** h() ****************/
/* 
 * Caller must provide: the player address
 *
 * We return: boolean
 * 
 * This function will handle the movement of the player on the grid
 */
static bool h(const addr_t from) {
    if(message_eqAddr(from, game->spectator)) {
        message_send(from, "ERROR invalid keystroke for spectator");
        return true;
    }
    else{
        player_struct_t* player= findPlayer(from);
        char* nextChar=get_char(player->yCoor, player->xCoor-1, game->gold_grid);
        if(strcmp(nextChar, "*")==0 ||set_char(player-> yCoor, player->xCoor-1,(player->playerID)[0], game->master_grid)){
            if(strcmp(nextChar, "*")==0) {
                char* original = get_char(player->yCoor, player->xCoor, game-> gold_grid);
                set_char( player-> yCoor, player->xCoor, original[0], game->master_grid);    
                player->xCoor--;
                set_SEEN_char( player-> yCoor, player->xCoor, (player->playerID)[0], game->master_grid);
                foundGold(player);
                free(original);
            } else {
                char* original= get_char(player->yCoor, player->xCoor, game->base_grid);
                set_char(player->yCoor, player-> xCoor, original[0], game->master_grid);
                player->xCoor--;
                free(original);
            }
            
            set_iterate(game->roster->set, NULL, updatePersonals);
            display(from); 
            free(nextChar);
            return false;
        }
        else{
            message_send(from, "ERROR hit a wall");
            free(nextChar);
            return true;
        }
    }
}

/**************** l() ****************/
/* 
 * Caller must provide: the player address
 *
 * We return: boolean
 * 
 * This function will handle the movement of the player on the grid
 */
static bool l(const addr_t from) {
    if(message_eqAddr(from, game->spectator)) {
        message_send(from, "ERROR invalid keystroke for spectator");
        return true;
    }
    else{
        player_struct_t* player= findPlayer(from);
        char* nextChar=get_char(player->yCoor, player->xCoor+1, game->gold_grid);
        if(strcmp(nextChar, "*")==0||set_char( player-> yCoor, player->xCoor + 1, (player->playerID)[0], game->master_grid)){
            if(strcmp(nextChar, "*")==0) {
                char* original = get_char(player->yCoor, player->xCoor, game-> gold_grid);
                set_char( player-> yCoor, player->xCoor, original[0], game->master_grid);    
                player->xCoor++;
                set_SEEN_char( player-> yCoor, player->xCoor, (player->playerID)[0], game->master_grid);
                foundGold(player);
                free(original);
            } else {
                char* original = get_char(player->yCoor, player->xCoor, game->base_grid);
                set_char(player-> yCoor,player->xCoor, original[0], game->master_grid);
                player->xCoor++;
                free(original);
            }
            
            set_iterate(game->roster->set, NULL, updatePersonals);
            display(from);
            free(nextChar);
            return false;
        } else {
            message_send(from, "ERROR hit a wall");
            free(nextChar);
            return true;
        }
    }
}

/**************** j() ****************/
/* 
 * Caller must provide: the player address
 *
 * We return: boolean
 * 
 * This function will handle the movement of the player on the grid
 */
static bool k(const addr_t from) {
    if(message_eqAddr(from, game->spectator)) {
        message_send(from, "ERROR invalid keystroke for spectator");
        return true;
    }
    else{
        player_struct_t* player= findPlayer(from);
        char* nextChar=get_char(player->yCoor-1, player->xCoor, game->gold_grid);
        if(strcmp(nextChar, "*")==0||set_char(player->yCoor-1, player-> xCoor, (player->playerID)[0], game->master_grid)){
            if(strcmp(nextChar, "*")==0) {
                char* original = get_char(player->yCoor, player->xCoor, game-> gold_grid); 
                set_char( player-> yCoor, player->xCoor, original[0], game->master_grid);    
                player->yCoor--;
                set_SEEN_char( player-> yCoor, player->xCoor, (player->playerID)[0], game->master_grid);
                foundGold(player);
                free(original);
            } else {
                char* original =  get_char(player->yCoor, player->xCoor, game->base_grid);
                set_char(player->yCoor, player-> xCoor, original[0], game->master_grid);
                player->yCoor--;
                free(original);
            }
            set_iterate(game->roster->set, NULL, updatePersonals);
            display(from);
            free(nextChar);
            return false;
        } else {
            message_send(from, "ERROR hit a wall");
            free(nextChar);
            return true;
        }
        free(nextChar);
    }
}

/**************** k() ****************/
/* 
 * Caller must provide: the player address
 *
 * We return: boolean
 * 
 * This function will handle the movement of the player on the grid
 */
static bool j(const addr_t from) {
    if(message_eqAddr(from, game->spectator)) {
        message_send(from, "ERROR invalid keystroke for spectator");
        return true;
    }
    else{
        player_struct_t* player= findPlayer(from);
        char* nextChar=get_char(player->yCoor+1, player->xCoor, game->gold_grid);
        if(strcmp(nextChar, "*")==0||set_char( player-> yCoor+1,player->xCoor, (player->playerID)[0], game->master_grid)){
            if(strcmp(nextChar, "*")==0) {
                char* original = get_char(player->yCoor, player->xCoor, game-> gold_grid); 
                set_char( player-> yCoor, player->xCoor, original[0], game->master_grid);    
                player->yCoor++;
                set_SEEN_char( player-> yCoor, player->xCoor, (player->playerID)[0], game->master_grid);
                foundGold(player);
                free(original);
            } else {
                char* original = get_char(player->yCoor, player->xCoor, game->base_grid); 
                set_char( player-> yCoor,player->xCoor, original[0], game->master_grid);
                player->yCoor++;
                free(original);
            }
            set_iterate(game->roster->set, NULL, updatePersonals);
            display(from);
            free(nextChar);
            return false;
        } else {
            message_send(from, "ERROR hit a wall");
            free(nextChar);
            return true;
        }
        free(nextChar);
    }
}

/**************** y() ****************/
/* 
 * Caller must provide: the player address
 *
 * We return: boolean
 * 
 * This function will handle the movement of the player on the grid
 */
static bool y(const addr_t from) {
    if(message_eqAddr(from, game->spectator)) {
        message_send(from, "ERROR invalid keystroke for spectator");
        return true;
    }
    else{
        player_struct_t* player= findPlayer(from);
        char* nextChar=get_char(player->yCoor+1, player->xCoor-1, game->gold_grid);
        if(strcmp(nextChar, "*")==0||set_char( player-> yCoor+1,player->xCoor-1, (player->playerID)[0], game->master_grid)){
            if(strcmp(nextChar, "*")==0) {
                char* original = get_char(player->yCoor, player->xCoor, game-> gold_grid);
                set_char( player-> yCoor, player->xCoor, original[0], game->master_grid);    
                player->xCoor--;
                player->yCoor++;
                set_SEEN_char( player-> yCoor, player->xCoor, (player->playerID)[0], game->master_grid);
                foundGold(player);
                free(original);
            }
            else{
                char* original = get_char(player->yCoor, player->xCoor, game->base_grid);
                set_char( player-> yCoor,player->xCoor, original[0], game->master_grid);
                player->xCoor--;
                player->yCoor++;
                free(original);
            }
            set_iterate(game->roster->set, NULL, updatePersonals);
            display(from);
            free(nextChar);
            return false;
        } else {
            message_send(from, "ERROR hit a wall");
            free(nextChar);
            return true;
        }
        free(nextChar);
    }
}

/**************** u() ****************/
/* 
 * Caller must provide: the player address
 *
 * We return: boolean
 *
 * This function will handle the movement of the player on the grid
 */
static bool u(const addr_t from) {
    if(message_eqAddr(from, game->spectator)) {
        message_send(from, "ERROR invalid keystroke for spectator");
        return true;
    }
    else{
        player_struct_t* player= findPlayer(from);
        char* nextChar=get_char(player->yCoor+1, player->xCoor+1, game->gold_grid);
        if(strcmp(nextChar, "*")==0||set_char( player-> yCoor+1,player->xCoor+1, (player->playerID)[0], game->master_grid)){
            if(strcmp(nextChar, "*")==0) {
                char* original = get_char(player->yCoor, player->xCoor, game-> gold_grid); 
                set_char( player-> yCoor, player->xCoor, original[0], game->master_grid);
                player->xCoor++;
                player->yCoor++; 
                set_SEEN_char( player-> yCoor, player->xCoor, (player->playerID)[0], game->master_grid);       
                foundGold(player);
                free(original);
            } else {
                char* original = get_char(player->yCoor, player->xCoor, game->base_grid);
                set_char( player-> yCoor,player->xCoor, original[0], game->master_grid);
                player->xCoor++;
                player->yCoor++;
                free(original);
            }
            set_iterate(game->roster->set, NULL, updatePersonals);
            display(from);
            free(nextChar);
            return false;
        } else {
            message_send(from, "ERROR hit a wall");
            free(nextChar);
            return true;
        }
        free(nextChar);
    }
}

/**************** b() ****************/
/* 
 * Caller must provide: the player address
 *
 * We return: boolean
 *
 * This function will handle the movement of the player on the grid
 */
static bool b(const addr_t from) {
    if(message_eqAddr(from, game->spectator)) {
        message_send(from, "ERROR invalid keystroke for spectator");
        return true;
    }
    else{
        player_struct_t* player= findPlayer(from);
        char* nextChar=get_char(player->yCoor-1, player->xCoor-1, game->gold_grid);
        if(strcmp(nextChar, "*")==0||set_char( player-> yCoor-1,player->xCoor-1, (player->playerID)[0], game->master_grid)){
            if(strcmp(nextChar, "*")==0) {
                char* original = get_char(player->yCoor, player->xCoor, game-> gold_grid);
                set_char( player-> yCoor, player->xCoor, original[0], game->master_grid);
                player->xCoor--;
                player->yCoor--;
                set_SEEN_char( player-> yCoor, player->xCoor, (player->playerID)[0], game->master_grid);
                foundGold(player);
                free(original);
                
            } else {
                char* original = get_char(player->yCoor, player->xCoor, game->base_grid);
                set_char( player-> yCoor,player->xCoor, original[0], game->master_grid);
                player->xCoor--;
                player->yCoor--;
                free(original);
            }
            set_iterate(game->roster->set, NULL, updatePersonals);
            display(from);
            free(nextChar);
            return false;
        } else {
            message_send(from, "ERROR hit a wall");
            free(nextChar);
            return true;
        }
        free(nextChar);
    }
}

/**************** n() ****************/
/* 
 * Caller must provide: the player address
 *
 * We return: a boolean
 *
 * This function will handle the movement of the player on the grid
 */
static bool n(const addr_t from) {
    if(message_eqAddr(from, game->spectator)) {
        message_send(from, "ERROR invalid keystroke for spectator");
        return true;
    }
    else{
        player_struct_t* player= findPlayer(from);
        char* nextChar=get_char(player->yCoor-1, player->xCoor+1, game->gold_grid);
        if(strcmp(nextChar, "*")==0||set_char( player-> yCoor-1,player->xCoor+1, (player->playerID)[0], game->master_grid)){
            if(strcmp(nextChar, "*")==0) {
                char* original = get_char(player->yCoor, player->xCoor, game-> gold_grid); 
                set_char( player-> yCoor, player->xCoor, original[0], game->master_grid);
                player->xCoor++;
                player->yCoor--;
                set_SEEN_char( player-> yCoor, player->xCoor, (player->playerID)[0], game->master_grid);
                foundGold(player);
                free(original);

            } else {
                char* original = get_char(player->yCoor, player->xCoor, game->base_grid); 
                set_char( player-> yCoor,player->xCoor, original[0], game->master_grid);
                player->xCoor++;
                player->yCoor--;
                free(original);
            }
            set_iterate(game->roster->set, NULL, updatePersonals);
            display(from);
            free(nextChar);
            return false;
        } else {
            message_send(from, "ERROR hit a wall");
            free(nextChar);
            return true;
        }
        free(nextChar);
    }
}
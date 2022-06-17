/* 
 * client.c 
 *
 * usage: ./client hostname port [playername]
 * 
 * hostname:  the hostname or IP address where the server is running
 * port: the port number on which the server expects messages
 * [playername]: (optional) argument determines whether to join as a player or spectator. 
 *      
 * Jeanmarcos Perez; November, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "mem.h"
#include <stdbool.h>
#include "player_struct.h"
#include "roster.h"
#include "grid.h"
#include "message.h"
#include <curses.h>

static void parseArgs(const int argc, char* argv[]);
void initializeDisplay();
bool handleMessage(void* arg, const addr_t from, const char* message);
bool handleInput(void* arg);

typedef struct clientInfo{
    bool isSpectator;
    int cursorX;
    int cursorY;
    const char* realName;
    const char* playerID;
    addr_t address;
    WINDOW* clientwindow;
} clientInfo_t;   

clientInfo_t* clientinfo = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};

int main(const int argc, char* argv[]){
    parseArgs(argc, argv);

    clientinfo->cursorX = 0;
    clientinfo->cursorY = 0;

    addr_t addr = message_noAddr();
    message_setAddr(argv[1], argv[2], &addr);
    clientinfo->address = addr;

    if(argc == 3){
        clientinfo->isSpectator = true;
        //spectator message
        message_send(clientinfo->address, "SPECTATE");
    }
    if(argc == 4){
        clientinfo->isSpectator = false;
        clientinfo->realName = argv[3];
        //player message 
        char* playMessage = mem_malloc(100);
        sprintf(playMessage, "PLAY %s", clientinfo->realName);
        message_send(clientinfo->address, playMessage);
        mem_free(playMessage);
    }

    void initializeDisplay();

    message_loop(NULL, 0, NULL, handleInput, handleMessage);
    message_done();
    exit(0);
}

static void parseArgs(const int argc, char* argv[]){
    if(argc == 3 || argc == 4){
        if(argv[1] == NULL || argv[2] == NULL){
            fprintf(stderr, "Null Argument\n");
            exit(1);
        }
        addr_t addr = message_noAddr();
        if(message_setAddr(argv[1], argv[2], &addr) == false){
            fprintf(stderr, "Error. Bad hostname or port number\n");
            exit(1);
        }
        if(argc == 4){
            if(argv[3] == NULL){
                fprintf(stderr, "Null Playername\n");
                exit(1);
            }
        }
    }
    else{
        fprintf(stderr, "Invalid number of arguments\n");
        exit(1);
    }
}

void initializeDisplay(){
    WINDOW* window = initscr();
    clientinfo->clientwindow = window;
    start_color();
    getmaxyx(clientinfo->clientwindow, clientinfo->cursorX, clientinfo->cursorY);
    move(0,0);
}

bool handleMessage(void* arg, const addr_t from, const char* message){
    if(strncmp(message, "OK ", strlen("OK ")) == 0){
        //OK L
        char* ID = mem_malloc(5); 
        sscanf(message, "OK %s", ID);
        clientinfo->playerID = ID;
        mem_free(ID);
    }
    else if(strncmp(message, "GRID ", strlen("GRID ")) == 0){
        //GRID nrows ncols
        int nrows;
        int ncols;
        sscanf(message, "GRID %d %d", &nrows, &ncols);

        while(clientinfo->cursorX < (nrows+1) || clientinfo->cursorY < (ncols+1)){
            mvprintw(0,0,"Window size too small. Please make window larger");
            getmaxyx(clientinfo->clientwindow, clientinfo->cursorX, clientinfo->cursorY);
            move(0,0);
        }
    }
    //GOLD n p r
    // n = to inform the player it has just collected n gold nuggets
    // p = its purse now has p gold nuggets
    // r = there remain r nuggets left to be found
    else if(strncmp(message, "GOLD ", strlen("GOLD ")) == 0){
        int collectedGold;
        int playerTotalGold;
        int remainingGold;

        sscanf(message, "GOLD %d %d %d", &collectedGold, &playerTotalGold, &remainingGold);

        if(clientinfo->isSpectator){
            //Spectator: 211 nuggets unclaimed.
            char* gridMessage = mem_malloc(100);
            sprintf(gridMessage, "Spectator: %d nuggets unclaimed.", remainingGold);
            mvprintw(0, 0, "%s", gridMessage);
            move(0,0);
            mem_free(gridMessage);
        }
        else{
            //Player A has 39 nuggets (211 nuggets unclaimed).
            char* gridMessage = mem_malloc(100);
            sprintf(gridMessage, "Player %s has %d nuggets (%d nuggets claimed).", clientinfo->playerID, playerTotalGold, remainingGold);
            mvprintw(0, 0, "%s", gridMessage);
            move(0,0);
            mem_free(gridMessage);
        }
    }
    else if(strncmp(message, "DISPLAY", strlen("DISPLAY")) == 0){
        //DISPLAY\nstring
        //second row, first coloumn
        char* displayMessage = mem_malloc(strlen(message));
        strcpy(displayMessage, message);
        displayMessage = displayMessage + strlen("DISPLAY\n");
        mvprintw(1, 0, "%s", displayMessage);
        move(0,0);
        mem_free(displayMessage);
    }
    else if(strncmp(message, "QUIT ", strlen("QUIT ")) == 0){
        endwin();
        char* quitMessage = mem_malloc(strlen(message));
        strcpy(quitMessage, message);
        quitMessage = quitMessage + strlen("QUIT ");
        printf("%s\n", quitMessage);
        mem_free(quitMessage);
    }
    else if(strncmp(message, "ERROR ", strlen("ERROR ")) == 0){
        char* errorMessage = mem_malloc(strlen(message));
        strcpy(errorMessage, message);
        errorMessage = errorMessage + strlen("ERROR ");
        mvprintw(0, 55, "%s", errorMessage);
        move(0,0);
        mem_free(errorMessage);
    }
    else{
        mvprintw(0, 55, "Message not recognized. Please try again");
        move(0,0);
    }
    return true;
}

bool handleInput(void* arg){
    char* keyMessage = mem_malloc(10);
    char* key = arg;

    if(sscanf(key, "%c", keyMessage) == 1){
        sprintf(keyMessage, "KEY %s", key);
        message_send(clientinfo->address, keyMessage);
        mem_free(keyMessage);
        return true;
    }
    else{
        message_send(clientinfo->address, "KEY Q");
        mem_free(keyMessage);
        return false;
    }
    
    clrtoeol();
}
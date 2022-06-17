
/**************** indextest.c ****************/
/* Lindsey Kim
*
* tests index by creating an index file with indexer, creating an index from that index file, and printing it
* to an output file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/index.h"


int main(const int argc, char* argv[]){
    if(argc!=3){
        fprintf(stderr, "Invalid args");
        exit(1);
    } else {
        FILE* readFile=NULL;
        if((readFile=fopen(argv[1],"r"))==NULL){
            fprintf(stderr, "error opening");
        } 

        index_t* index = index_read(readFile);
        

        FILE* writeFile=NULL;
        if((writeFile=fopen(argv[2], "w"))==NULL){
            fprintf(stderr, "Error printing");
        }
        index_save(index, writeFile);
        
        fclose(readFile);
        fclose(writeFile);

        index_delete(index);
    }

}
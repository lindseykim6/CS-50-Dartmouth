/**************** indexer.c ****************/
/* Lindsey Kim
*
* builds an index from a page directory and writes it to a file
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"

static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
static index_t* indexBuild(char* pageDirectory);
static void indexPage(index_t* index, webpage_t* page, int docID);
static void indexToFile(index_t* index, char* indexFilename);

int main(const int argc, char* argv[]){
    char* pageDirectory=NULL;
    char* indexFilename=NULL;
    parseArgs(argc, argv, &pageDirectory, &indexFilename);;
    index_t* index=indexBuild(pageDirectory);
    indexToFile(index, indexFilename);
    index_delete(index);
    exit(0);
}

/**************** parseArgs ****************/
/* parses command line arguments and stores them in pageDirectory and indexFilename
*/
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename) {
    // if the arguments are out of range, print error
    if (argc!=3) {
        fprintf(stderr, "Incorrect arguments\n");
        exit(1);
    } else {
        FILE* file=NULL;
        // if page directory is not valid 
        if(!pagedir_validate(argv[1])){ 
            fprintf(stderr, "Unable to use directory\n");
            exit(1);
        }
        //if indexfile is unable to be written in
        if ((file= fopen(argv[2], "w"))==NULL){
            fprintf(stderr, "Unable to open/write in index file\n");
            exit(1);
        }
         else { 
            *pageDirectory=argv[1]; // stores first arg in pagedirectory
            *indexFilename=argv[2]; // stores second arg in indexfilename
            fclose(file); // close file
        }   
    }
}

/**************** indexBuild ****************/
/* builds an in-memory index from webpage files it finds in the pageDirectory
* returns the index that is built
*/
static index_t* indexBuild(char* pageDirectory) {
    const int INDEXSIZE=200; // index size is 200
    index_t* index = index_new(INDEXSIZE); // creates a new index of thatsize
    
    webpage_t* webpage=NULL;;
    int docID=1; 
    while((webpage=pagedir_load(pageDirectory, docID))!=NULL) { // loads files into webpages until there are none left
        indexPage(index, webpage, docID); // creates an index from the webpage
        docID++; // increments docID
        webpage_delete(webpage);
    }
    return index; // returns index that is built
}

/**************** indexPage ****************/
/* scans a webpage document to add its words to the index
*/
static void indexPage(index_t* index, webpage_t* page, int docID){
    int pos = 0; 
    char* word=NULL;
    while((word=webpage_getNextWord(page, &pos))!=NULL){ // scans the words to add to the index
        if(strlen(word)>=3) { // only adds the words that are greater than or equal to 3 in length
            word=normalizeWord(word); // lowercases the words first
            index_insert(index, word, docID); // inserts them into the index
        }
        free(word);
        word=NULL;
    }
}

/**************** indexToFile ****************/
/* saves the index to a file
*/ 
static void indexToFile(index_t* index, char* indexFilename){
    FILE* fp=fopen(indexFilename, "w");
    index_save(index, fp); // writes the index to a file
    fclose(fp);;
}
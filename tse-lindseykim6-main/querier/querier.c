#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/file.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"
#include "ctype.h"

static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
static index_t* createIndex(char* indexFilename);
static char** parseQuery(char* query, int wordCount);
static int checkQuery(char* query);
static counters_t* findQueries(char** queries, int wordCount, index_t* index);
static int checkQuery(char* query);
static void rankQueries(counters_t* counter, char* pageDirectory);

/**************** parseArgs ****************/
/* helper functions
*/
static bool checkOperators(char** query, int wordCount);
static counters_t* combineResultsAnd(counters_t* temp, counters_t* newTemp);
static counters_t* combineResultsOr(counters_t* result, counters_t* newResult);
static void scoreCalculateAnd(void *arg, const int key, const int count);
static void scoreCalculateOr(void *arg, const int key, const int count);
static void countNonZeroFunc(void *arg, const int key, const int count);
static void rankResults(void *arg, const int key, const int count);

/**************** parseArgs ****************/
/* structure to keep track of the document with the highest score
*/
typedef struct maxDocument {
    int documentID;
    int score;
} maxDocument_t;

int main(const int argc, char* argv[]){
    char* pageDirectory=NULL;
    char* indexFilename=NULL;
    char* query=NULL;

    parseArgs(argc, argv, &pageDirectory, &indexFilename); 
    while((query=file_readLine(stdin))!=NULL) { // while there are lines of queries to read 
        index_t* index = createIndex(indexFilename); // create an index
        int wordCount=checkQuery(query); // check if the query is valid and how many words there are
        char** cleanQuery=parseQuery(query, wordCount); // parse through the query and extract words, print clean query
        
        if(cleanQuery==NULL) { // if the query is invalid, do nothing
            free(query); // clean up
            free(cleanQuery);
            index_delete(index); 
            continue;
        }
        else if(!checkOperators(cleanQuery, wordCount)){  // if the operators are invalid, print an error message 
            fprintf(stdout, "Query: ");;
            for(int i=0; i<wordCount; i++){
                fprintf(stdout, "%s ", cleanQuery[i]); // print out the cleaned query
            }
            fprintf(stdout, "\n"); 
            fprintf(stderr, "Incorrect operator format\n");
        } 
        else {
            fprintf(stdout, "Query: ");;
            for(int i=0; i<wordCount; i++){
                fprintf(stdout, "%s ", cleanQuery[i]); // print out the cleaned query
            }
            fprintf(stdout, "\n"); 
            counters_t* counters = findQueries(cleanQuery, wordCount, index); // make a final counter object from the words in the query
            rankQueries(counters, pageDirectory); // rank the queries and print them
        } 

        free(query); // clean up
        free(cleanQuery);
        index_delete(index);    
    }
    
    free(query);
    
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
        //if indexfile is unable to be read
        if ((file= fopen(argv[2], "r"))==NULL){
            fprintf(stderr, "Unable to open/read index file\n");
            exit(1);
        }
         else { 
            *pageDirectory=argv[1]; // stores first arg in pagedirectory
            *indexFilename=argv[2]; // stores second arg in indexfilename
            
        }   
        fclose(file); // close file
    }
    
}

/**************** createIndex ****************/
/* creates the index from given indexFilename
*/
static index_t* createIndex(char* indexFilename) {
    FILE* file = fopen(indexFilename, "r");
    index_t* index= index_read(file); // index is built
    fclose(file);
    return index;
}

/**************** checkQuery ****************/
/* checks if the query is valid and counts the number of words in the query
*/
static int checkQuery(char* query) {
    char* copy=query; // creates a copy pointer to the query
    int wordCount=0; // keeps track of the wordCount

    if(copy==NULL|| strcmp(copy, "")==0) {
        fprintf(stderr, "Error: Invalid query\n"); // prints invalid query
        return 0;         
    }

    while(*copy!='\0') {
        while (isspace(*copy)){     
            copy++; // iterate through the spaces
        }
        if(*copy=='\0' && wordCount==0) {
            fprintf(stderr, "Error: Invalid query\n"); // if it's just spaces, print error
            return 0;
        }
        if(!isalpha(*copy) && !isspace(*copy) && *copy!='\0') {
            fprintf(stderr, "Error: Invalid character '%c'\n", *copy); //if it isn't a valid letter, print error
            return 0;
        } 
        if(isalpha(*copy)) {
            wordCount++; // increase wordCount if it is valid
        }
        while(isalpha(*copy)){
            copy++; // iterate through letters
        }
    }
    
    return wordCount;
}

/**************** parseQuery ****************/
/* parses query
*/
static char** parseQuery(char* query, int wordCount) {
    if(wordCount==0) { // if there are no words
        return NULL; // do not parse/return
    }

    char** words=mem_assert(mem_calloc(wordCount,sizeof(char*)), "Could not allocate memory"); // allocate memory for the array of words
    int wordsPos=0; // keep track of the position in the array
    while(*query!='\0'){
        while (isspace(*query)){     
            query++; // if it is a space, iterate through them
        }

        char* rest=query; // second pointer to create words inside the string
        while(isalpha(*rest)){  // if it finds a word, iterate until the end
            rest++;
        }

        if (*rest != '\0') {
            *rest='\0'; // create the end of a word
            rest++; // update position
        }

        words[wordsPos]=normalizeWord(query); // normalize the word and insert it in array
        wordsPos++; // update position in array
        query = rest; // reset query pointer
    }

    return words; // return the array of words
}

/**************** findQueries ****************/
/* find the correct queries with queries, index, and wordCount passed in, return counters object
*/
static counters_t* findQueries(char** queries, int wordCount, index_t* index) {
    counters_t* result = NULL; // keeps track of the result
    int i=0; // keeps track of the position in the array
    while(i < wordCount) { // while we are not at the end of the query
        counters_t* temp = index_find(index, queries[i]); // find the counter for the word in the current position in the array
        while((i+1)<wordCount&&strcmp(queries[i+1], "or")!=0) { //while the next word isn't at the end of the query or isn't 'or'
            if(strcmp(queries[i+1], "and")==0) {  // if the next word is and
                counters_t* newTemp= index_find(index, queries[i+2]); // combine the counter for the word in the current pos with the one two ahead
                temp=combineResultsAnd(temp, newTemp);
                i=i+2; // update position to two ahead
            }
            else{ // if there is no operator, assume and
                counters_t* newResults= index_find(index, queries[i+1]); //  combine the counter for the word in the current pos with the one one ahead
                temp=combineResultsAnd(temp, newResults);
                i=i+1; // update position to one ahead
            }
        }
        if(i+1>=wordCount||strcmp(queries[i+1], "or")==0) { // if the next word is or or is the end
            result = combineResultsOr(result, temp); // union the results and the current temp value
            i=i+2; // iterate to ahead of the or
        }
    }

    return result; // return the result
}

/**************** rankQueries ****************/
/* ranks the queries
*/
static void rankQueries(counters_t* counter, char* pageDirectory){
    int countNonZero = 0; // count how many nonzeros there are
    counters_iterate(counter, &countNonZero, countNonZeroFunc);

    if(countNonZero==0) { // if they are all zero
        fprintf(stdout, "No documents match\n");
    } 

    else {
        maxDocument_t* maxDocument = mem_assert(mem_malloc(sizeof(maxDocument_t*)), "Memory allocation error\n"); // create a new maxDocument
        maxDocument->documentID=-1;
        maxDocument->score=0;

        int count=0; // iterate until count reaches the value of countNonZero
        while(countNonZero!=count) {
            counters_iterate(counter, maxDocument, rankResults); 
            char* docIDString=mem_assert(mem_malloc(sizeof(maxDocument->documentID)), "Memory error");
            sprintf(docIDString, "%d", maxDocument->documentID);
            char* filePath = mem_assert(mem_calloc(strlen(pageDirectory) + strlen(docIDString) + 2, sizeof(char)), "Memory error"); 
            sprintf(filePath, "%s/%d", pageDirectory, maxDocument->documentID); // create the filepath of the document

            FILE* file= NULL;
            if((file=fopen(filePath, "r"))==NULL) { // open it or print error
                fprintf(stderr, "could not use file path");
                return;
            }  
            char* url = file_readLine(file); // read the url

            fprintf(stdout, "Document %d: score of %d, url of %s\n", maxDocument->documentID, maxDocument->score, url); // print out the documents

            counters_set(counter, maxDocument->documentID, 0); // set score of the maxDoc to 0
            maxDocument-> score=0;
       
            count++; // increase count

            //clean up
            free(url);
            free(docIDString);
            free(filePath);
            fclose(file);
        }
        free(maxDocument);
    }
}

/**************** checkOperators ****************/
/* checks the operators to see if they are valid
*/
static bool checkOperators(char** query, int wordCount) {
    if(strcmp(query[0], "and")==0 || strcmp(query[0], "or")==0){ // check if operators are at the beginning
        return false;
    }
    // check if operators are at the end
    if(strcmp(query[wordCount-1], "and")==0 || strcmp(query[wordCount-1], "or")==0){
        return false;
    }
    // check if operators are adjacent
    for(int i=0; i< wordCount-1; i++) {
        if(strcmp(query[i], "and")==0 && strcmp(query[i+1], "and")==0){
            return false;
        }
        if(strcmp(query[i], "or")==0 && strcmp(query[i+1], "or")==0){
            return false;
        }
        if(strcmp(query[i], "and")==0 && strcmp(query[i+1], "or")==0){
            return false;
        }
        if(strcmp(query[i], "or")==0 && strcmp(query[i+1], "and")==0){
            return false;
        }
    }
    return true;
}

/**************** combineResultsAnd ****************/
/* combines the counters for and
*/
static counters_t* combineResultsAnd(counters_t* temp, counters_t* newTemp) {
    counters_t** counters = mem_assert(mem_malloc(2* sizeof(counters_t*)), "Memory allocation error\n"); // array of the two counters (need both)
    counters[0]=temp;
    counters[1]= newTemp;
    counters_iterate(temp, counters, scoreCalculateAnd); // intersect the two counters
    free(counters);
    return(temp);
    
}



/**************** scoreCalculateAnd ****************/
/* calculates the score for and
*/
static void scoreCalculateAnd(void *arg, const int key, const int count) {
    counters_t** counters=arg;
    int min=0;
    min=counters_get(counters[1], key);
    if(min<count) {
        counters_set(counters[0], key, min); // set the counter value to the minimum of the two (0 if it doesn't exist in one)
    }
}



/**************** combineResultsOr ****************/
/* combines the counters for or
*/
static counters_t* combineResultsOr(counters_t* result, counters_t* newResult) {
    counters_t** counters = mem_assert(mem_malloc(2* sizeof(counters_t*)), "Memory allocation error\n"); // array of two counters (need both)
    counters[0]= result;
    counters[1]= newResult;
    counters_iterate(result, counters, scoreCalculateOr); // union the two counters
    free(counters);
    return(newResult);
    
}

/**************** scoreCalculateOr ****************/
/* calculates the score for or
*/
static void scoreCalculateOr(void *arg, const int key, const int count) {
    counters_t** counters=arg;
    int min=0;
    if((min=counters_get(counters[1], key))==0) {
        counters_set(counters[1], key, count + min); // sum of the two counters is the new value or insert only value
    }
}



/**************** countNonZeroFunc ****************/
/* calculates the score for and
*/
static void countNonZeroFunc(void *arg, const int key, const int count){
    int* countNonZero=arg;
    if(count>0) {
        *countNonZero+=1; // count how many non-zero counts there are
    }
}

/**************** rankResults ****************/
/* assists rankQueries by finding the highest scoring document
*/
static void rankResults(void *arg, const int key, const int count) {
    maxDocument_t* maxDocument = arg;
    if(maxDocument->documentID==-1) {
        maxDocument-> documentID= key; // initializes maxDoc if first iteration
        maxDocument-> score = count;
    }
    else{
        if(count > maxDocument-> score) { // finds highest maxDoc
            maxDocument->documentID= key;
            maxDocument->score =count;
        }
    }
}
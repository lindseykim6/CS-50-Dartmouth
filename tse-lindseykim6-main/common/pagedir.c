/**************** pagedir.h ****************/
/* Lindsey Kim
*
* a library for pagedirectory functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"

/**************** pagedir_init ****************/
/* see pagedir.h for more info 
*/
bool pagedir_init(const char* pageDirectory){
    char* crawlerFile = mem_assert(mem_malloc(strlen(pageDirectory) +strlen("/.crawler") +1), "Memory error");
    sprintf(crawlerFile, "%s/.crawler", pageDirectory); // creates the crawler file
    FILE* file =fopen(crawlerFile, "w");  // opens the crawler file
    free(crawlerFile);// frees crawler file

    if(file ==NULL) { //print usage if file is null
        fprintf(stderr,"Could not open or write in page directory \n");
        return false;
    }
    else{
        fclose(file);// closes crawler file
        return true;
    }
}
/**************** pagedir_save ****************/
/* see pagedir.h for more info
*/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {
    char* docIDString=mem_assert(mem_malloc(sizeof(docID)), "Memory error");
    sprintf(docIDString, "%d", docID);
    char* filePath = mem_assert(mem_calloc(strlen(pageDirectory) + strlen(docIDString) + 2, sizeof(char)), "Memory error"); 
    sprintf(filePath, "%s/%d", pageDirectory, docID);
    FILE* file =fopen(filePath, "w");
    free(docIDString);
    free(filePath);
    if(file ==NULL) { //print usage if file is null
        fprintf(stderr,"Could not open or write in page directory \n");
        exit(1);
    }
    else{
        fprintf(file, "%s\n", webpage_getURL(page));
        fprintf(file, "%d\n", webpage_getDepth(page));
        fprintf(file, "%s\n", webpage_getHTML(page));
        fclose(file);
    }  
}
/**************** pagedir_validate ****************/
/* see pagedir.h for more info
*/
bool pagedir_validate(char* pageDirectory) {
    char* crawler = mem_assert(mem_calloc(strlen(pageDirectory) +strlen("/.crawler") +2, sizeof(char)), "Memory error");
    sprintf(crawler, "%s/.crawler", pageDirectory);
    FILE* file = NULL;  
    if((file=fopen(crawler, "r"))==NULL){
        fprintf(stderr, "Could not open crawler file.\n");
        free(crawler);
        return false;
    } else {
        fclose(file);
        char* firstDoc = mem_assert(mem_calloc(strlen(pageDirectory) +strlen("1") +2, sizeof(char)), "Memory error"); 
        sprintf(firstDoc, "%s/1", pageDirectory); 
        if((file=fopen(firstDoc, "r"))==NULL){
            fprintf(stderr, "Could not open first file.\n");
            free(crawler);
            free(firstDoc);
            return false;
        } else {
            fclose(file);
            free(crawler);
            free(firstDoc);
            return true;
        }  
        
    }
    
}
/**************** pagedir_load ****************/
/* see pagedir.h for more info
*/
webpage_t* pagedir_load(char* directory, const int docID){
    char* docIDString=mem_assert(mem_malloc(sizeof(docID)), "Memory error");
    sprintf(docIDString, "%d", docID);
    char* filePath = mem_assert(mem_calloc(strlen(directory) + strlen(docIDString) + 2, sizeof(char)), "Memory error");
    sprintf(filePath, "%s/%d", directory, docID);
    FILE* file= NULL;
    if((file=fopen(filePath, "r")) ==NULL) {
        free(docIDString);
        free(filePath);
        return NULL;
    } else {
        char* url= file_readLine(file);
        char* depth = file_readLine(file);
        int depthInt = atoi(depth);
        char* html = file_readFile(file);
        webpage_t* webpage = webpage_new(url, depthInt, html);
        fclose(file);
        free(depth);
        free(docIDString);
        free(filePath);
        return webpage;
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/bag.h"
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../common/pagedir.h"

static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
void itemDelete(void* item){}

int main(const int argc, char* argv[]){
    char* seedURLcopy=NULL;
    char* pageDirectorycopy=NULL;
    int maxDepthCopy=-1;;
    parseArgs(argc, argv, &seedURLcopy, &pageDirectorycopy, &maxDepthCopy);
    crawl(seedURLcopy, pageDirectorycopy, maxDepthCopy);
    exit(0);

}
/**************** parseArgs ****************/
/* parses through the arguments and stores the arguments in seedURL, pageDirectory, and maxDepth
* returns void
*/

static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth) {
    // if the arguments are out of range, print error
    if (argc!=4) {
        fprintf(stderr, "Incorrect arguments\n");
        exit(1);
    } else {
        char* url=NULL; // normalized url
        // if it is unable to me normalized or is not internal
        if((url=normalizeURL(argv[1]))==NULL || !isInternalURL(argv[1])) { 
            fprintf(stderr, "Unable to use seedURL\n");
            exit(1);
        } else { 
            *seedURL=argv[1]; // assigns the first argument to seed url

            if(!pagedir_init(argv[2])) { // attempts to initialize page directory with the second arguments
                fprintf(stderr, "Unable to initialize page directory\n"); // if not, print to stderr
                exit(1);
            } else {
                *pageDirectory=argv[2]; // assigns the second argument to page directory

                int maxDepthTemp=atoi(argv[3]);  // determines what the integer in argv[3] is 
                if(maxDepthTemp>=0 && maxDepthTemp <=10) { // if max depth is in range
                    *maxDepth=maxDepthTemp; // assigns the third argument to maxDepth
                } else {
                    //if max depth is out of range, print to stderr
                    fprintf(stderr, "maxDepth out of range"); 
                    exit(1);
                }
            }
        }
        free(url); // free the url 
              
    }
}
/**************** crawl ****************/
/* crawls through the urls
* returns void
*/
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth) {
    hashtable_t* urlHash= hashtable_new(200); // creates a new hashtable

    if(!hashtable_insert(urlHash, seedURL, "")) { // tries to insert the hash but prints error if it doesn't work
        fprintf(stderr, "Error with inserting seed URL\n");
        exit(1);
    }

    char* urlcopy= malloc(strlen(seedURL)+1); // copy's the url and makes storage for it
    strcpy(urlcopy, seedURL);

    webpage_t* seedWebpage = webpage_new(urlcopy, 0, NULL); // creates a webpage from the seedpage

    if(seedWebpage==NULL) { // if creation doesn't work 
        fprintf(stderr, "Error creating webpage\n"); // print an error
        exit(1);
    }

    bag_t* webpages = bag_new(); // create a new bag
    bag_insert(webpages, seedWebpage); // insert the webpage
    
    seedWebpage=bag_extract(webpages); // extract the first bag
    int nextID=1; // id of bags
    while(seedWebpage!=NULL) { // while the bag has stuff to get
        if(webpage_fetch(seedWebpage)) { // if successfully fetch the webpage
            fprintf(stderr, "%d Fetched: %s\n", webpage_getDepth(seedWebpage), webpage_getURL(seedWebpage)); 
            pagedir_save(seedWebpage, pageDirectory, nextID); // save the page
            if(webpage_getDepth(seedWebpage)<maxDepth) { // if the depth is within maxdepth
                fprintf(stderr, "%d Scannning: %s\n", webpage_getDepth(seedWebpage), webpage_getURL(seedWebpage)); 
                pageScan(seedWebpage, webpages, urlHash); // scan the page
                
            }
            nextID++; // increment id
            
        } else {
        fprintf(stderr, "Error fetching webpage\n"); 
        exit(1);
        } 
        webpage_delete(seedWebpage); // delete webpage
        seedWebpage=bag_extract(webpages); // get next webpage
    }
    hashtable_delete(urlHash, itemDelete); // delete hashtable
    bag_delete(webpages, webpage_delete); // delete bag
    
}
/**************** pageScan ****************/
/* parses through the arguments and stores the arguments in seedURL, pageDirectory, and maxDepth
* returns void
*/
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen){
    int pos = 0; // position in webpage
    char* url= NULL;
    while((url = webpage_getNextURL(page, &pos))!=NULL) { // while there are webpages to get in the page
        fprintf(stdout, "%d Found: %s\n", webpage_getDepth(page), url);
        char* normalizedURL; // stores normalized url
        if((normalizedURL=normalizeURL(url))!=NULL && (isInternalURL(url))) { // if url is normalized and internal
            if(hashtable_insert(pagesSeen, url, "")) { //insert it into hash
                fprintf(stdout, "%d Added: %s\n", webpage_getDepth(page), url);
                char* urlcopy= malloc(strlen(url)+1); // copies url into allocated space
                strcpy(urlcopy, normalizedURL);
                webpage_t* newPage=webpage_new(urlcopy, webpage_getDepth(page) + 1, NULL); // creates new webpage for the url
                bag_insert(pagesToCrawl, newPage); // insert it in bag
            }
            else{
                fprintf(stdout, "%d IgnDupl: %s\n", webpage_getDepth(page), url);
            }
        }
        else{
            fprintf(stdout, "%d IgnExtrn: %s\n", webpage_getDepth(page), url);
        } 
        free(url); // frees the url
        free(normalizedURL); // frees the normalized url
    }
    
}

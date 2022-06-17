# Implementation Lindsey Kim
Implementation details specific to your implementation.
Detailed pseudo code for each of the objects/components/functions,
Definition of detailed APIs, interfaces, function prototypes and their parameters,
Data structures (e.g., struct names and members),
Security and privacy properties,
Error handling and recovery,
Resource management,
Persistent storage (files, database, etc).
Testing plan

## Data structures 
maxDocument_t has a pointer to the max document id and its score
an array of counters is used to keep track of two counters

## Pseudo code for logic/algorithmic flow
The querier will run as follows:
call parseArgs to parse the parse arguments 
Initialize the index with createIndex
read search queries from stdin
call parseQuery to tokenize the query and make sure it's valid
call findQueries with queries, index, and pageDirectory passed in, return counters object
call rankQueries with the returned counter object passed in

where checkQuery: 
loops through the query
    counts how many words there are 
    sees if the query is invalid

where parseQuery:
initialize array with checkQuery's int
parses through the query by extracting words from white space
    normalize each word by making them all lowercase 
    place in array
print clean query
return array

where findQueries:
call checkOperators which step through array, checking if `and` or `or` appears in the beginning or end and makes sure no operators are adjacent or print error
loop over and sequences in array
    loop over words in the and sequence in array
        call combineResultsAnd to create an updated counter object for the word
    call combineResultsOr to create an updated counter object for the word

where rankQueries:
keep track of how many non zero items there are 
while we iterate to the number fo non zero items
    iterate through the counter 
    identify max score item and store it in a maxDocument_t struct
    print it out
    set value to 0

## Other modules

checkOperators:
step through array, checking if `and` or `or` appears in the beginning or end 
makes sure no operators are adjacent
print error if invalid

combineResultsAnd:
iterates through one counter and sets the counter to the minimum occurences of the two objects (0 if it does not exist in the other counter)

combineResultsOr:
iterates through one counter and sets the counter to the sum of the occurences of the two objects

## Function prototypes
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
static index_t* createIndex(char* indexFilename);
static char** parseQuery(char* query, int wordCount);
static int checkQuery(char* query);
static counters_t* findQueries(char** queries, int wordCount, index_t* index);
static int checkQuery(char* query);
static void rankQueries(counters_t* counter, char* pageDirectory);

static bool checkOperators(char** query, int wordCount);
static counters_t* combineResultsAnd(counters_t* temp, counters_t* newTemp);
static counters_t* combineResultsOr(counters_t* result, counters_t* newResult);
static void scoreCalculateAnd(void *arg, const int key, const int count);
static void scoreCalculateOr(void *arg, const int key, const int count);
static void countNonZeroFunc(void *arg, const int key, const int count);
static void rankResults(void *arg, const int key, const int count);

## Error handling and recovery
Problems are handled internally and result in an error message and a prompt for a new query.
If there are command line errors, then exit non-zero.
Uses mem_assert for memory allocation issues.

# Testing Plan 
call for fuzz testing
call for regression testing
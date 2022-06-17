# Design Lindsey Kim
Describe the abstract data structures and pseudo code for your querier.
User interface
Inputs and outputs
Functional decomposition into modules
Dataflow through modules
Pseudo code (plain English-like language) for logic/algorithmic flow
Major data structures
Testing plan

## User Interface
The interface is the command line, and it has two arguments `pageDirectory` and `indexFilename` in the form:
``` bash
$ ./querier pageDirectory indexFilename
```
An example input:
``` bash
$ ./querier ../data/letters ../data/letters.index
```
where `letters`is a page directory in `../data` produced by Crawler and `letters.index`is an index file in the page `../data` produced by the Indexer.

## Inputs and outputs
Input: The querier loads the index from the `indexFilename` parameter into an index structure. It then reads the search querys and uses the index to find and print out the set of documents in `pageDirectory` that satisfy the query. 

Output: set of documents that satisfy the search query in decreasing rank order.

## Functional decomposition into models
We anticipate the following modules or functions:
1. main: read seach queries from stdin, and initializes other modules
2. parseArgs: parses the arguments and initializes the index
3. createindex: creates the index from a file
4. parseQuery: provides a function to tokenize the query and makes sure it's valid, returning the array
5. findQueries: finds all the documents in the index that satisfy the query
6. combineResultsAnd: provides the functions that combines the results of the and sequence
7. combineResultsOr: provides the functions that combines the results of the or sequence
8. rankQueries: ranks the queries it finds

Helper modules that provide data structures:
1. scoreCalculateAnd: provides the functions that calculates the score of the results for or
2. scoreCalculateOr: provides the functions that calculates the score of the results for or
3. checkQuery: checks if the query is invalid and counts how may words there are
4. checkOperators: checks the operators to see if they are valid

## Pseudo code for logic/algorithmic flow
The querier will run as follows:
call parseArgs to parse the parse arguments
read search queries from stdin
initialize the index with createIndex
call checkQuery the see if the query is invalid and see how many words there are
call parseQuery to tokenize the query and make sure it's valid
call findQueries with queries, index, and pageDirectory passed in, return counters object fo unions or intersections
call rankQueries with the returned counter object passed in

where parseArgs: 
checks if there are 2 arguments
if the page directory is not valid
if the indexfile is unable to be read

where createIndex:
opens the file and calls index_read to initialize the index

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
call checkOperators 
loop over and sequences in array
    loop over words in the and sequence in array
        call combineResultsAnd to create an updated counter object for the word
    call combineResultsOr to create an updated counter object for the word

where rankQueries:
while counter has non zero items    
    iterate through the counter 
    identify max score item 
    print it out
    set value to 0

where combineResultsAnd:
finds the intersection of two counter objects

where combineResultsOr:
finds the union of two counter objects

where checkOperators:
    checkOperators which step through array, checking if `and` or `or` appears in the beginning or end and makes sure no operators are adjacent or print error

## Data structures
We use arrays to keep track of counters and a structure to keep track of the highest score document and score.

## Testing plan 
Fuzz testing: Checks boundary cases

Regression testing: Compares output to good output




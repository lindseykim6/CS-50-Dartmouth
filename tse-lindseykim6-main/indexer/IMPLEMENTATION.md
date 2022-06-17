# Implementation Spec

## indexer
In our indexer, we start by parsing the arguments given in the command line with `parseArgs` to check if they were valid. If they are, we store them in variables and use those variables in `indexBuild`, which takes the directory to read from as the parameter. `indexBuild` builds an in-memory index from webpage files it finds in the pageDirectory by doing the following with page directory as a parameter:
creates a new 'index' object
  loops over document ID numbers, counting from 1
    loads a webpage from the document file 'pageDirectory/id'
    if successful, 
      passes the webpage and docID to indexPage
`indexPage` scans a webpage document to add its words to the index by doing the following with index, pageDirectory, and docID as its parameters:
    steps through each word of the webpage,
        skips trivial words (less than length 3),
        normalizes the word (converts to lower case through `normalizeWord`),
        looks up the word in the index,
            adding the word to the index if needed
        increments the count of occurrences of this word in this docID

The ultimate result of an indexer is it creates an index from a pageDirectory and prints it to a specified file `fileName`.

## index
The indexer uses the `index` typestruct to do so, which is a hashtable mapping from word to (docID, #occurrences) pairs.  It uses a counterset to keep track of word occurrences per docID. 
The `index` has an `index_new` function that creates the index by initializing a hashtable. It also has an `index_insert` function that inserts the word and its docID into the index by checking if it exists or not and incrementing or creating a counter, respectively. `index_save` saves the index in the specified format to a file. `index_read` loads an index from an index file, which is used in `indextest.c`. `index_read` deletes an index by calling the delete functions of its components. `index_find` finds the counter that corresponds to each word in the index.

## pagedir
The `pagedir_load` takes the directory and docID as function parameters and loads each line from an index file into an webpage of type `webpage_t`. The `pagedir_validate ` checks to see if the .crawler file and first file exist in the directory

## Error handling and recovery
exit zero if successful; exit with an error message to stderr and non-zero exit status on encountering an unrecoverable error, including

out of memory,
invalid command-line arguments,
unable to read a file named pageDirectory/.crawler,
unable to read a file named pageDirectory/1
unable to write a file named indexFilename


# Lindsey Kim
## CS50 October 22, 2021

## Assumptions
I assumed the index files and directory are properly formatted when reading from them. Our implementation fails to work when this does not work.

Specifically, the indexer may assume that:
pageDirectory has files named 1, 2, 3, ..., without gaps.
The content of files in pageDirectory follow the format as defined in the specs.

## TESTING
to run `test`, run `make test`
the test file will run for edge cases, and then test with indextest

## USAGE

to build `indexer`, run `make indexer`
to build `indextest`, run `make compiletest`
to build and run both, run `make all`

the __indexer__ takes 2 arguments - pageDirectory, indexFilename
the __indextest__ takes 2 arguments - readFile (where index is), writeFile (where to write new index)

to clean up, run `make clean`

## FILES
`testing.sh`
`testing.out`
`Makefile`
`indexer.c`
`indextest.c`
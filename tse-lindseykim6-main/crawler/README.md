# Lindsey Kim
# crawler

## IMPLEMENTATION

the `crawler` takes in the 4 arguments and then begins by parsing them

afterwards, the `hashtable` and `bag` are initiated 

until the `bag` is empty:
    take a webage
    fetch the HTML
    if that was successful
        save it
        if it isn't maxDepth yet
            find the other webpages
        delete that page

deletes the hashtable and then the bag



## USAGE

to build `crawler`, run `make crawler`

the __crawler__ takes 3 arguments - seedURL, pageDirectory, maxDepth

to clean up, run `make clean`

## FILES
`testing.sh`
`testing.out`
`Makefile`

## TESTING
to run `test`, run `make test`
the test file will run for edge cases, and then test with the links provided for us

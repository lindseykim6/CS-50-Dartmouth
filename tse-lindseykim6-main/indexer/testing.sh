#!/bin/bash

# testing for indexer by Lindsey Kim

if [ -e "../data" ]
then
    rm -rf ../data
fi
mkdir -p ../data

if [ -e "../data/depths" ]
then
    rm -rf ../data/depths
fi
mkdir ../data/depths

cd ..
cd crawler
make crawler
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/depths 2
cd ..
cd indexer
make indexer

# incorrect args
# should return incorrect statements

# no args
./indexer

# one arg
./indexer ../data/depths 

# three args
./indexer ../data/depths indexFile indexFile2 

# incorrect/nonexistent pageDirectory
./indexer ../data/foo indexFile

# invalid pageDirectory (not a crawler directory)
mkdir ../data/foo
./indexer ../data/foo indexFile

#invalid indexFile (non-existent path)
./indexer ../data/depths ../foo/foo

#invalid indexFile (read-only directory)
./indexer ../data/depths ../data

# invalid indexFile (existing, read-only file)
cat > foo <<EOF
EOF
chmod a-w foo
./indexer ../data/depths foo

rm -rf foo

if [ -e "../data/outputs" ]
then
    rm -rf ../data/outputs
fi
mkdir ../data/outputs

# correct run
./indexer ../data/depths ../data/outputs/indexFile
./indexer ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-3 ../data/outputs/indexFile2
./indexer ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-10 ../data/outputs/indexFile3

if [ -e "../data/outputs2" ]
then
    rm -rf ../data/outputs2
fi
mkdir ../data/outputs2

# test using indextest and indexcmp, should print nothing
./compiletest ../data/outputs/indexFile ../data/outputs2/indexFile
~/cs50-dev/shared/tse/output/indexer/indexcmp ../data/outputs/indexFile ../data/outputs2/indexFile

./compiletest ../data/outputs/indexFile2 ../data/outputs2/indexFile2
~/cs50-dev/shared/tse/output/indexer/indexcmp ../data/outputs/indexFile2 ../data/outputs2/indexFile2

./compiletest ../data/outputs/indexFile3 ../data/outputs2/indexFile3
~/cs50-dev/shared/tse/output/indexer/indexcmp ../data/outputs/indexFile3 ../data/outputs2/indexFile3

# valgrind 
valgrind ./indexer ../data/depths ../data/outputs/indexFile
valgrind ./compiletest ../data/outputs/indexFile ../data/outputs2/indexFile
#!/bin/bash

# testing for querier by Lindsey Kim
../querier/fuzzquery ~/cs50-dev/shared/tse/output/indexer/index-letters-10 10 0 | ./querier ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-10 ~/cs50-dev/shared/tse/output/indexer/index-letters-10

# incorrect argument
cat > foo << EOF
and this first
this is fun and
or this first home
this first home or
this or or home
this and and first
       this or playground and and home
EOF

cat foo | ./querier ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-10 ~/cs50-dev/shared/tse/output/indexer/index-letters-10

cat > fooTwo << EOF
...
this first!
FIRST OR THIS

     
EOF

cat fooTwo | ./querier ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-10 ~/cs50-dev/shared/tse/output/indexer/index-letters-10

# valgrind 
valgrind ../querier/fuzzquery ~/cs50-dev/shared/tse/output/indexer/index-letters-10 7 15| ./querier ~/cs50-dev/shared/tse/output/crawler/pages-letters-depth-10 ~/cs50-dev/shared/tse/output/indexer/index-letters-10
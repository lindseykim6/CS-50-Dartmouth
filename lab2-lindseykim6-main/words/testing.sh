#!/bin/bash
cat > example <<EOF
alpha cat dog love2animals555a lot
bravo
charlie
EOF

# tests with a file
./words example

cat > example2 <<EOF
Write a C program called words that breaks its input into a series of words, one per line. 
It may take input from stdin, or from files whose names are listed as arguments.
!!!!!CS50!!!!
EOF

# tests with two files
./words example example2

cat > foo <<EOF
We love watching television in the morning! I like cows a lot. This is very fun.
anyways
what
is 
up
EOF

# tests with two files and special file -
./words example - example2 < foo

# tests with no file
./words < foo

#tests with special file -, should be the same as above
./words - < foo

#tests with incorrect filename
./words nullfile


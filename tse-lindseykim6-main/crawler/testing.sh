#testing by Lindsey Kim
make clean

if [ -e "../data" ]
then
    rm -rf ../data
fi
mkdir -p ../data

if [ -e "../data/incorrectArgs" ]
then
    rm -rf ../data/incorrectArgs
fi
mkdir ../data/incorrectArgs

make crawler
# incorrect args
# should return incorrect statements
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/incorrectArgs
./crawler http://cs50tse.cs. ../data/incorrectARgs 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../dat 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/incorrectArgs 11

if [ -e "../data/depths" ]
then
    rm -rf ../data/depths
fi
mkdir ../data/depths

#crawl depths
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/depths 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/depths 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/depths 2

if [ -e "../data/seed" ]
then
    rm -rf ../data/seed
fi
mkdir ../data/seed

# different seed page
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/B.html ../data/seed 1

if [ -e "../data/wikipedia" ]
then
    rm -rf ../data/wikipedia
fi
mkdir ../data/wikipedia

# wikipedia
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia 1
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia 2

# if [ -e "../data/bigdepth" ]
# then
#     rm -rf ../data/bigdepth
# fi
# mkdir ../data/bigdepth

# big depth
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/bigdepth 5


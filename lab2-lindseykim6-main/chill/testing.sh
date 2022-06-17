#!/bin/bash
# zero arguments
./chill
# one argument
./chill 32
./chill -10
./chill 0
# one argument where temperature is too large
./chill 51
# one argument where temperature is too small
./chill -100
# two arguments
./chill 32.5 10
./chill 10 0.5
# two arguments where temperature is out of range
./chill 77 10
./chill -101 15
# two arguments where wind is out of range
./chill 40 0.2
# two arguments where wind and temperature are both out of range
./chill 1000 0
# too many arguments
./chill 5 10 15

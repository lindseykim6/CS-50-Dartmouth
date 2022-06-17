# Nuggets
#### Authors: Anna Martin, Emilie Hopkinson, Lindsey Kim, Jeanmarcos Perez
#### Fall 2021
#### CS 50
#### Professor Palmer

## Overview

This repository contains the code for the CS50 "Nuggets" game, in which players explore a set of rooms and passageways in search of gold nuggets.
The rooms and passages are defined by a *map* loaded by the server at the start of the game.
The gold nuggets are randomly distributed in *piles* within the rooms.
Up to 26 players, and one spectator, may play a given game.
Each player is randomly dropped into a room when joining the game.
Players move about, collecting nuggets when they move onto a pile.
When all gold nuggets are collected, the game ends and a summary is printed.

## Materials provided

See the [support library](support/README.md) for some useful modules.

See the [maps](maps/README.md) for some draft maps.

## Our code 

See the [implementation spec](IMPLEMENTATION.md) for more information on our implementation decisions.

See the [design spec](DESIGN.md) for more information on our design decisions. 

See the [team wiki](https://github.com/CS50Dartmouth21FS1/project-public-static-void/wiki) for more information on how our team set goals and how we evaluated our success. 

## How to run 

To run the server for this project please ensure you are in the correct directory then type the following:

    make clean
    make
    ./server map.txt [seed]


## Server Module 

The server module is designed to hold much of the details of the nuggets game. Here we bring together the modules for initializing the game, handle server responses to messages, set the gold randomly on the grid, handle the playing and the spectating and other related functions. Please see `server.c` for more specific information on each function. 

There are a few aspects of the server that could have used more work if we had more time. 

First, we would have liked to include an aspect of the `check_location` function that would be able to recognize if there was another player in a spot so we could ahve them switch locations. This would have made use of the `player_struct` module as well, and potentially have made use of the `roster` module. 

Otherwise, throughout the server we have small bugs that impact the functionality of the client's ability to play the game. If we had more time we would have been able to be more intentional about memory allocation which is likely the reason we are running into these issues.

Despite these minor issues with server, the majority of the logic, design, and implementation we feel quite confident of. 

## Testing 

To test the server module we focused on integration testing with the given client. By attempting different functinalities of the player we would be able to see what aspects of the server is working fully. 

However, if we had more time to extrapolate the testing it would have been helpful to include a unit testing script to make sure each function works separate from the client. 

## Extra Credit 

Our team also decided to make our own map titled: `public_static_void.txt` which is located in the `maps` directory. Also `santa.txt` and `DartmouthD.txt`.

Additionally, while we ended up only being a team of 3, we have an attempt at some `client` module. Before our fourth team member dropped he spent quite some time working on the `client` module, thus we thought it would be fair to include it in our submission however we are aware it is not a requirement. It is not tested and the rest of the team did not spend much time checking the work that was already done as we wanted to put our effort into the server and common files. 

Please also note the correct use of gitflow throughout the project, and the additional organization through the use of issues, milestones, tags, and the project page. 

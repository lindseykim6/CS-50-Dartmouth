# Common
### Authors: Anna Martin, Emilie Hopkinson, Lindsey Kim, Jeanmarcos Perez
### Fall 2021
### CS 50
### Professor Palmer

## `Common` Module 

This module contains supporting functions and data structures for the `Nuggets` game. Included here are `game.c`, `grid.c`, `player_struct.c`, `roster.c`, and their corresponding `.h` files. Additionally, we have a `gridTest.c` file that is used for testing our `grid` struct. 

### The `game` module 

Our game module functions as a holder and initializer for the entirety of the nuggets game. Becuase it only acts as a holder and doesn't include any of the logic it is actually quite a short file and only contains two functions. The `game_new` function initializes the entire game, and the `game_delete` frees all of the memory that was previously allocated when creating the game. 

### The `grid` module 
The grid module holds the bulk of the details and information contained in the `common` directory. Not only does this file handle the creation of the grid, it also handles the visibility specific to each player. Because this file was so time intensive, there are a few aspects of it that we would have liked to improve if we had the time 

One aspect that we would have improved would have been the visibility of gold. We would have wanted to be able to hide the visibility of gold if it was in a room we already visited but had left. Currently our visibility was not able to accomodate that. To do this we would have needed to implement another function to differentiate pieces of gold in our actual field of sight and pieces of gold that we have walked by but no longer are in the same room as. Currently, we just have a boolean handling whether or not a spot is in our field of sight or has been in our field of sight. 

Another aspect of our visibility that would have been nice to work further on is our rendering of the rows. Currently we have a small bug where some maps will show an extra row (that shouldn't be in the visibility) off to the right of the screen. While we attempted to get rid of this bug for quite some time we were never able to pinpoint the problem. We believe the logic is all sound, therefore there must be some small mistake in the syntax / execution. 

Another general aspect of the grid that would have been nice to work on was the efficiency of the coding. We noticed that some of our functions and variables were superfluous and could have been truncated. 

### Testing for the `grid` module 

Our testing for the grid module was relatively extensive compared to our other unit tests. Because the visibility proved to be such an obstacle we wanted to make sure all of the basics of the module were working well. While this testing file is not completely exhaustive, it is useful in identifing bugs. The testing file  

* Makes a new grid 
* Deletes grid
* Prints grid
* Tests set and get char 
* Manually tests visibility 
* Manually tests boundaries 

### The `player_struct` module 

The player struct module is quite straightforward. Most of the functionality of the player struct is implemented through getter and setter functions. The players know their location, their purse, and their address. 

### Testing for the `player_struct` module

The unit testing for the player struct module is very straightforward and simple. Here we run through each function in the module and ensure it works as expected. 

### The `roster` module 

The roster module is also quite simple. Originally, the roster module was going to be used somewhat as the `game` module is now used, however we thought it would be valuable to keep organizationally. Our roster holds players, can delete players, delete the roster, and decrement the roster. 

### Testing for the `roster` module

The unit testing for the roster module is very straightforward and simple. Here we run through each function in the module and ensure it works as expected.

### Compiling 

To compile simply ensure you are in the correct directory and run the following 

    make clean 
    make

### Using 

The usage of the common files are similar to that of the `support` module. Please see the `README.md` within the `support` directory for more information. 

### Testing 
All of the unit testing files are described above.

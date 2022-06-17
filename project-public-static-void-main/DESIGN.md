# CS50 Nuggets
## Design Spec
### Group Name: Public Static Void
#### Anna Martin, Jeanmarcos Perez, Lindsey Kim, Emilie Hopkinson

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server. However, as our group decremented to a group of 3, we have implemented only the server. 
Our design also includes player, spectator, and display modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Server
### User interface

The server's interface with the user is on the command-line; it must have either one or two arguments. There is no further interaction with the user.

	./server map.txt [seed]

Where the first argument is the pathname for a map and the second is an optional seed for the random-number generator; if provided, the seed must be a positive integer.


### Inputs and outputs

The map file is the main input into the server. The map can be varied and could contain multiple rooms, passage ways, and walls that separate each room. 

The server will print out instructions for what is required for gameplay. Additionally the server will log errors such as mutliple spectators, memory issues, or other errors regarding inputs or file readability. For example, if the map is not readable this will result in a log to stderr. 


### Functional decomposition into modules

We implemented the following modules or functions: 

1. `main` which will initialize other modules
2. `parseArgs` which will parse and validate command-line arguments
3. `initializeGame` which will set up data structures,initialize the network, and print port number.
3. `handleMessage` which will recieve messages from clients and relay to all users.
4. `updatePersonals` which will update a player's personal grid visibility.
5. `checkLocation` which will check a given (x, y) coordinate to see if there is an open spot (".") at that coordinate.
6. `findPlayer` which will return a player after finding it in the set of players. 
7. `find_player_helper` which will help the find player function by identifying the player's address.
8. `setGold` which will randomly choose the number of gold piles on the map and allocate the number of pieces of gold in each pile. It will also randomly place the gold piles on the map. 

The following functions directly correspond to the messages recieved on the server end.

9. `spectate` a function which handles this message from the client when a spectator starts
10. `play` a function which handles this message from the client when a player starts and finds parameter `real name` to initialize the player's name if not `QUIT`
11. `key` a function which handles a message from the client and will handle keystrokes typed by the client
12. `ok` a module handling server message which will initialize that a player has successfully entered the game and did not QUIT
13. `grid` a module handling server message which will find the size of the grid in parameters `nrows` and `ncols`. It will be sent immediately from the server to clients when a player joins 
14. `gold` a module handling server message which will find parameters to describe how many nuggets it has just collected, how many are in its purse, and how many are left to be found as parameters `n` `p` and `r`, respectively. It will be sent immediately from the server after the player has joined.
15. `display` a module handling server message which will find parameter `string` to describe the grid as known from this client
16. `quit` a module handling server message which will find parameter `explanation` to make the client exit and print an explanation followed by a newline

The following functions directly correspond to the keystokes as sent by the user. 

17. `h` which will handle the "h" keystroke from the player. This function moves the player character to the left one space. 
18. `l` which will handle the "l" keystroke from the player. This function moves the player character to the right one space.
19. `j` which will handle "j" keystroke from the player. This function moves the player character down one space. 
20. `k` which will handle "k" keystroke from the player. This function moves the player character up one space. 
21. `y` which will handle the "l" keystroke from the player. This function moves the player diagonally up and to the left one space. 
22. `u` which will handle the "u" keystroke from the player. This function moves the player diagonally up and to the right one space. 
23. `b` which will handle the "b" keystroke from the player. This function moves the player diagonally down and to the left one space. 
24. `n` which will handle the "n" keystroke from the player. This function moves the player digonally down and to the right one space. 


And some helper modules that provide data structures:
1. `player_struct` a module providing the data structure to represent the player and its functions
2. `roster` a module providing the data structure to represent a group of players and their functions
3. `grid` a module providing the data strucutre to represent a grid and handle the vision of each player / spectator
4. `game` a module that acts as a holder for the game and has the single global struct


### Pseudo code for logic/algorithmic flow

The server will run as follows:

	execute from a command line per the requirement spec
	parse the command line, validate parameters with `parseArgs`
	call `initializeGame`
	set up a loop, to await client's messages
	recieve and handle messages from clients and handle with `recieveMessage` 
	if needed, relay information to other players/spectators within each modularized functions  
	call `gameOver` to inform all clients the game has ended when gold nuggets is 0
	clean up

### Major data structures

Some major data structures that we use here are `hashtable` when finding the players and handling gold. Originally, we attempted to use sets to handle some of the gold functionality but found it to be easier to reference gold in a hastable with x, y coordinates as the key.

---

## Player Struct module

A module which will keep track of each player's role, grid, location, purse, name, and letter associated with the player on the map.

### Functional decomposition

1. `player_struct_new` create a new player that is a spectator or a client
2. `player_struct_addGold` set the purse of the player
3. `player_struct_getGold` get the purse of the player
4. `player_struct_getName` get the name of the player
5. `player_struct_getID` get the letter of the player
6. `player_struct_getX` get the x location of the player
7. `player_struct_getY` get the y location of the player
8. `player_struct_updatePosition` set the location of the player
9. `player_struct_delete` delete player

### Major data structures

The major data structure is our own data structure called `player_struct` which holds a player's role, location, purse, name, personal grid, address, and letter associated with the player of the map.

## Grid module

A module providing the data structure to represent a grid (used for representing and using the map)

### Functional decomposition

1. `grid_new` initializes a new grid structure in the form of a 2D array
2. `find_x_of_array` takes in a file and checks if it is valid then returns the number of columns (the x length)
3. `find_Y_of_array` takes in a file and checks if it is valid then returns the number of rows (the y length)
4. `put_chars_in_grid` populates the grid with the correct chars
5. `print_grid` loops through the 2D array of the grid and prints it out 
6. `delete_grid` will free all malloced space for the 2D array 
7. `get_char` will return the char at a given x, y coordinate 
8. `set_char` will set a given x, y location on the grid to a given char 
9. `set_SEEN_char` will set a char to the correct value (unless it is a wall)
10. `grid_getRows` will return the number of rows in the grid
11. `grid_getCols` will return the number of columns in the grid
12. `add_visible_to_player` will add characters to the visible aspects of the personal grid for each player
13. `grid_2_string` will turn the grid that is treated as a 2D array to a string that will be printed
14. `inPlayerSight` will return a boolean telling us whether or not a point is in the players sight


### Pseudo code for logic/algorithmic flow

The grid module will run as follows:

	Take in a map and convert the map to a 2D array of characters that acts as a "master grid"
	For each individual player, there will be a personal grid created that only displays parts of the grid in the players field of sight (or places visited before)
	When the grid is sent to a player it is transformed back to a string

This is the basic functionality of the grid, however there are additional aspects in the grid module (scuah as get rows and get columns) that are used by other modules. 


### Major data structures

The major data structure used to represent the `grid` is a 2D array. 

## Roster module

The roster module implements a new data structures using a `set` to keep track of all of the `players`. The key is the player name and the item is the Player item

### Functional decomposition

1. `roster_new` creates a new roster with a set
2. `roster_add` used to insert a new player into the roster
3. `roster_decrement` decrements the roster when a player quits
3. `roster_find` updates a player
4. `roster_delete` deletes the roster
5. `itemdelete` deletes a player item 

### Major data structures
The major data structure is a `set` that is used to create the `roster`

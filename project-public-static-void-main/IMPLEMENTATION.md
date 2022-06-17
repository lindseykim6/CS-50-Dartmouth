# CS50 Nuggets
## Implementation Spec
### public-static-static void - Anna Martin, Jeanmarcos Perez, Lindsey Kim, Emilie Hopkinson

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server. However, because we finished the term with three members, we only implemented the server. 
Our design also includes Grid, Player, and Roster modules.
We describe each program and modules separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.


## Module Structure flow in the Implementation Specification
-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Plan for division of labor
Lindsey and Anna - Server and Game logic, Design and Implementation
Emilie - Grid and Visibility, Design and Implementation
Jeanmarcos - Contributed to Server, Player Struct Module, Game Module, Roster Module


## Player Struct

The player is a data object itself and stores valuable information about a each user such as their location, their purse, their name, the letter that represents their location, and their address

### Data structures
We create the `player_stuct_t` data structure, which holds the aforementioned updateable variables


### Definition of function prototypes

```c
Typedef struct player player_t;
player_struct_t* player_struct_new(char* enteredName, int x, int y, addr_t address);
void player_struct_addGold(player_struct_t* player_struct, int newGold);
int player_struct_getGold(player_struct_t* player_struct);
char* player_struct_getName(player_struct_t* player_struct);
char* player_struct_getID(player_struct_t* player_struct);
int player_struct_getX(player_struct_t* player_struct);
int player_struct_getY(player_struct_t* player_struct);
void player_struct_updatePosition(player_struct_t* player_struct, int deltaX, int deltaY);
void player_struct_delete(player_struct_t* player_struct);

```

### Detailed pseudo code

#### player_struct_t *player_struct_new* function 
	Allocates space for new play and must be given a character string name
	Must also be given starting space coordinates x and y ints
	Set gold tally to 0 to start
	Create and set character ID

#### void *player_struct_addGold* function
	Given a player pointer and int of gold adding
	Increment the players gold

#### int *player_struct_getGold* function 
	Given a player, it will return the gold var in players inventory

#### char *player_struct_getName* function
	Given a player, it will return the players name

#### char *player_struct_getID* function 
Given a player, it will return the players ID

#### int *player_struct_getX* function
	Given a player, it will return the players X coordinate

#### int *player_struct_getY* function 
	Given a player, it will return the players Y coordinate

#### void *player_struct_updatePosition* function 
	Given player and x and y, it will set the player to an x and y position

#### void *player_struct_delete* function 
	Given a player it will free memory associated with the player
	As well as the player itself


### Testing plan

For this single module, we will test all the behavior independently with a simple c script

-  We test various border and strenuous cases for the player
-  Test the memory allocation and deletion to make sure there are no memory leaks (valgrind)
-  Test the player get and set functions work properly manually checking them


## Roster
The roster module is a simple module that helps us keep track of all of our players, mainly held in our server module. 


### Data structures
We use the `set_t` struct provided by libcs50 to store a sort of array of `player_struct_t` objects with their IDs as keys


### Definition of function prototypes

```c
roster_t* roster_new();
bool roster_add(roster_t* roster, const char* playerID, player_t* player);
void roster_decrement(roster_t* roster);
void* roster_find(roster_t* roster, const char* playerID);
void roster_delete(roster_t* roster);
void itemdelete(void* item);
```
### Detailed pseudo code

#### roster *roster_new* function
	When calling roster new we need no args
	Simply allocate space for the new roster
	And initialize the `set_t` set object within the roster struct

#### boolean *roster_add* function
	Given a roster and a player, insert the `player_struct_t` obj into the rosters set
    
#### void *roster_decrement* function 
    Given a roster, decrement the number of players

#### void *roster_find* function
	Given a roster and a character ID string, search for the players ID and 

#### void *roster_delete* function
	Given a roster, free the memory allocated for its set
	As well as itself
    
#### void *itemdelete* function
	Given an item, free the memory allocated for each item


### Testing plan

Testing is independent and will be a simple c script

-  initiate a roster
-  test insertion of player objects
-  testing finding certain players ID’s
-  and finally test deletion
-  valgrind for memory leaks


## Game struct
The game module is a simple holder module that keeps track of the roster, each player, the base grid, the master grid, and all gold. 


### Data structures
We use the `roster_t` struct  to store a sort of array of `player_struct_t` objects with their IDs as keys. 
We are also using two `grid_t` structs, one that holds the *base grid* with just the map, and a *master grid* with the players and gold.

### Definition of function prototypes

```c
game_t* game_new();
void game_delete(game_t* game);
```

### Detailed pseudo code

#### game *game_new* function
	game new mallocs space for a game object which has global static variables
    calls helper functions to initialize each aspect of the game
    returns a valid pointer to a game_t object

#### void *game_delete* function
	Given a a valid game object, free all of the memory associated with the game 



### Testing plan

Testing is independent and will be a simple c script

-  initiate the game and check memory malloced
-  test insertion of players, different grids, 
-  testing finding certain players ID’s
-  testing sending correct messenges to its clients
-  checking for collisions between two players
-  only updating grids as needed
-  checking if moves are valid and sending response
-  checking if it keeps the main server grid and the client grid up to day synchronously
-  and finally test deletion
-  valgrind for memory leaks


## Server

The server is a module that implements most of the functionality and logic of the game. It also sends and receives requests from up to 24 different players, and approves requests to move and collect gold, and is responsible for player player collisions. It also keeps track of when the game is won by a player and sorts player scores. 

### Data structures
-  `player_struct_t` to create players and keep track of their ids, scores, locations, maps, addresses
-  `grid_t` to create a master and base map, which it will also send to a spectator, and check for players collisions
-  `roster_t` to keep all the players joined in a set and easily accessible to find 

### Definition of function prototypes

```c
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[]);
static void initializeGame(char* map, const int argc, char* argv[]);
static bool handleMessage(void* arg, const addr_t from, const char* message);
void updatePersonals (void *arg, const char *key, void *item);
bool checkLocation(int x, int y, grid_t* grid);
player_struct_t* findPlayer(addr_t from);
void find_player_helper(void *arg, const char *key, void *item);
grid_t* setGold(grid_t* baseGrid, grid_t* masterGrid);
static void foundGold(player_struct_t* player);
void find_player_helper(void *arg, const char *key, void *item);
player_struct_t* findPlayer(const addr_t from);
grid_t* setGold(grid_t* baseGrid, grid_t* masterGrid);
static bool spectate(const addr_t from, const char* message);
static bool play(const addr_t from, const char* message);
static bool key(const addr_t from, const char* message);
static void ok(const addr_t from);
static void grid(const addr_t from);
static void gold(const addr_t from, int collected);
static void display(const addr_t from);
static void quit(const addr_t from);
static bool h(const addr_t from);
static bool l(const addr_t from);
static bool j(const addr_t from);
static bool k(const addr_t from);
static bool y(const addr_t from);
static bool u(const addr_t from);
static bool b(const addr_t from);
static bool n(const addr_t from);

static bool spectate(addr_t from, const char* message);
static bool play(addr_t from, const char* message);
static bool key(addr_t from, const char* message);
static void ok(addr_t from);
static void grid(addr_t from);
static void gold(addr_t from, int collected);
static void display(addr_t from);
static void quit(addr_t from);

static bool h(addr_t from);
static bool l(addr_t from);
static bool j(addr_t from);
static bool k(addr_t from);
static bool y(addr_t from);
static bool u(addr_t from);
static bool b(addr_t from);
static bool n(addr_t from);
```


### Detailed pseudo code for major functions


#### int *main* function
	Calls parse args
	Calls initialize game

	Sends out messages updating clients
	While game is going
		Receive and approve messages from client players
		Send out updates to all other servers about other players position
		Once all good is collected 
			Break

#### void *parseArgs* function
	Takes in the map (and possibly seed) and and verifies its a readable file

#### void *initializeGame* function
	Initialize the base grid and the master grid
    Set the gold on the master grid
    Initialize the roster 
	
#### bool *handleMessage* function
	Call helper functions to handle different messages recieved

#### bool *gameOver* function
    Once all of the gold is collected, finish the game
    
#### grid *setGold* function 
    Randomly decide how many piles of gold 
    Randomly allocate the pieces of gold between the piles
    Randomly place all of the gold throughout the map, verifying that it is within bounds 

#### bool *play* function
	If someone tries to join the game to play as a player add them to the roster
    handle different messaging functions that a player is allowed
    
The rest of the functions in the server module handle either messages or key strokes. Please see the `server.c` file for more information. 


### Testing plan

To test this individual, we will create a simple c script and send in messages to see if the server handles them properly. 

- adding clients to the server manually
- moving player requests
- viewing the master grid
- if two clients request position at the same time
- gold count with players
- valgrind for memory leaks
- end game function to clean
- test border and breaking cases for the `parseArgs` function


## Grid

The grid is a `grid_t` struct which parses in a text file and holds a 2d array full of key numbers and has a hashtable of key values of how to make the 2D array and print it back into string form.

### Data structures

Besides making the `grid_t` struct, we will also be using a `set_t` struct to hold the keys of values we are making.

### Definition of function prototypes

```c
grid_t* grid_new(char* mapDir);
int find_cols_of_array(char* mapDir);
int find_rows_of_array(char* mapDir);
void* put_chars_in_grid(char* mapDir, grid_t* emptyGrid);
void* print_grid(FILE* fp, grid_t* grid);
void* delete_grid(grid_t* grid);
char* get_char(int row, int column, grid_t* grid);
bool set_char(int row, int column, char character, grid_t* grid);
bool set_SEEN_char(int row, int column, char character, grid_t* grid);
char* grid_2_string(grid_t* grid_2_string);
int grid_getRows(grid_t* grid);
int grid_getCols(grid_t* grid);
void add_visible_to_player(int player_x, int player_y, grid_t* grid, grid_t* masterGrid);
bool inPlayerSight(int player_row, int player_col, int coord_row, int coord_col, grid_t* masterGrid);
```

### Detailed pseudo code
#### grid_new(char* mapDir)

    grid new takes in a valid char * map directory
    checks if the directory is valide
    
    if it is then we call find cols and find rows
    
    allocate space for the grid
    
    for every line in the map text tfile
        find the strlen of that line and allocate that amount of space in the arraay
        
    now we clean that allocated memory
    for every line in the arraay
        for every row in the array
            set the array of the grid to blank space
            
    return the valid pointer
    
    
#### int find_cols_of_array(char* mapDir)

    if the file given is a valid and readabld fil
    read the first lin
    get the size of that line
    and returns the int

#### int find_rows_of_array(char* mapDir)

    if the file is valid and readable
    continue to read the lines until null
    keeps a pointer of how many lines we have read
    
    return how many lines we have read
    
#### put_chars_in_grid(char* mapDir, grid_t* emptyGrid)
    
    given an char* map and an empty grid
    
    if the allocated space in the grid is the same as the rows and cols in the map dir
    
    we loop through every line in the valid mapDir file
        and set that array of the line numb to the line we have read
        
        
    returns nothing
    
    
#### print_grid(FILE* fp, grid_t* grid)

    if given a valid grid and valid output file
    will print grid out in correct formatting
    
    
#### delete_grid(grid_t* grid)

    given a valid grid
    
    delete grid will delete the space allocated for the array
    the space allocated for the grid struct
    
    and then free the pointer to the grid as well
    
    
#### get_char(int row, int column, grid_t* grid)


    given a valid row and col (within bounds)
    and given a valid grid struct
    
    will allocate space for a char* and retrive that char at row col in the array
    
    returns the char
    
#### set_char(int row, int column, char character, grid_t* grid)

    given a valid row and col (within bounds)
    and given a valid grid
    and a char character
    
    
    will set the specific spot in the grid array to be that character
    will prevent any setting of characters in walls 
        no |+_ and ' ' spaces can be set, they are rocks
    
    
#### set_SEEN_char(int row, int column, char character, grid_t* grid)
    given a valid row and col (within bounds)
    and given a valid grid
    and a char character
    
    will set that specific spot in the grid array to be that character
        but overrides the aboves rules, will set no matter what
    
#### grid_2_string(grid_t* grid_2_string)
    
    given a valid grid pointer
    will allocate space for the string given row and col

    
#### grid_getRows(grid_t* grid)

    if given a valid grid pointer
    it will return the number of rows in the grid map
    
#### grid_getCols(grid_t* grid)

    if given a valid grid pointer
    it will return the number of collumns in the grid map
    
#### add_visible_to_player(int player_x, int player_y, grid_t* grid, grid_t* masterGrid)

    if given a valid players x and ys coordinates and the players seen grid and the overal master grid
    
    it will loop through each point in the master grid
        if it is not a space, (we do not care about outside space)
        
            then we check if it is in the players sight
            
                if in sight then we set the char at the loop in the master grid in the seen grid


#### inPlayerSight(int player_row, int player_col, int coord_row, int coord_col, grid_t* masterGrid)


    in player sight takes in a single point to test if it is in vision with the player
    
    
    // if the player is on the same collum
        loop through all the rows on that column TOWARDS the player 
            if we hit an invalid char
                return false

        return true as we successfully looped through 
            
    // if the player is on the same row
        loop through all columns on that row TOWARDS the player
            if we hit an invalid char on that column
                return false
                
        return true as we have not hit any walls
        
        
    // else
    
    we need to calculate the diagnol (point slop equation)
    
    find which way is towards the player in the column direction
    find which way is towards the player in the row direction
    
    
    loop through all the ROWS between the point row we are testing and the players row
    
        with out formula above, calculate the column in which it the players line of sight intersects
        
        if the calculated column is exactly one point
            check that point
            
        if the calculated column is between one point
            check that rounded version as well as one step towards the players column
            
    loop throguh all the COLUMNS between the point col we are testing and the players cols
        
        with the equation we calculated above
        calculate the intersecting row(s) for that column interatiion
        
            if it intersects perfectly
                check if invalid
                
            if it intersects inbetween
                check the rounded down
                check the rounded down + the next row towards player
                
    return true if it passed all the checks
    

### Testing plan
Testing will be small and of this singular module for now, perhaps a small c script. 
We will test making new grids, update function, print out the cols and rows, delete the grid, and valgrind to check for memory leaks. 


## Overall Testing
### integration testing

### system testing

To test the client and the server together we will consider the following tests:
1. One player playing the game, collecting all of the gold
2. Two players playing the game, each one collecting gold
3. Two players playing the game, one spectator 
4. Two players playing the game, one other player joins late
5. Two players playing the game, one more joins then collects gold then quits
6. The max number of players join the game
7. More than the max number of players try to join the game

---

## Limitations

We are aware of many limitations within our implementation. The main source of these limitations was the time contraint, however we discuss in both `README.md` files many of the shortcomings of our game. 

One notable limitation of our choice of implementation was in visibility. We chose to implement our visibility with a boolean keeping track of places we have already visited and places we can see. This made it so we are unable to not see the gold in previous rooms we have visited. A more dynamic implementation choice would have been to store the places we have been before as something other than a boolean (such as an int or a char or an x, y location). 

Another limitation of our implementation is the fact that we chose to only have one grid. This is somewhat restrictive in the way that we can implement visibility also. 

Because we decided to keep our `roster` struct for organization purposes we ran into quite a few memory issues. This caused us to have difficulty testing the server combined with the client, and created many other memory issues. 

We believe the rest of our logic throughout the game was well implemented and did not cause many obvious limitations other than those previously mentioned. 

# CS50 Lab 3
## CS50 Spring 2021

### set

A set maintains an unordered collection of (key,item) pairs; any given key can only occur in the set once. It starts out empty and grows as the caller inserts new (key,item) pairs. The caller can retrieve items by asking for their key, but cannot remove or update pairs. Items are distinguished by their key.

### Usage

The *set* module, defined in `set.h` and implements a bag of `void*`, and exports the following functions:

```c
counters_t* counters_new(void);
set_t* set_new(void);
bool set_insert(set_t* set, const char* key, void* item);
void* set_find(set_t* set, const char* key);
void set_print(set_t* set, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item) );
void set_iterate(set_t* set, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) );
void set_delete(set_t* set, void (*itemdelete)(void* item) );
```

### Implementation

We implement set as a linked list with each node containing a key and an item.

The *set* itself is represented as a `struct set` containing a pointer to the head of the list; the head pointer is NULL when the bag is empty.

Each node in the list is a `struct setnode`, a type defined internally to the module.

To insert a new setnode in set we insert it at the head of the list. To extract, we return the item linked with the key. 

The `set_print` method prints using `itemprint` with formatting.

The `set_iterate` method calls the `itemfunc` function on each item by scanning the linked list.

The `set_delete` method calls the `itemdelete` function on each item by scanning the linked list, freeing counters as it proceeds.
It concludes by freeing the `struct set`.

### Assumptions

The item and key inserted can't be NULL.

### Files

* `Makefile` - compilation procedure
* `set.h` - the interface
* `set.c` - the implementation
* `settest.c` - unit test driver
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make set.o`.

### Testing

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

Try testing with `MEMTEST` by editing Makefile to turn on that flag and then `make test`.

To test with valgrind, `make valgrind`.

# CS50 Lab 3
## CS50 Spring 2021

### hashtable
A hashtable is a set of (key,item) pairs. It acts just like a set, but is far more efficient for large collections. It uses the Jenkins Hashing function.

### Usage

The *hashtable* module, defined in `hashtable.h` and implemented in `hashtable.c`, implements a hashtable of char* key and void* item sets, and exports the following functions:

```c
hashtable_t* hashtable_new(const int num_slots);
bool hashtable_insert(hashtable_t* ht, const char* key, void* item);
void* hashtable_find(hashtable_t* ht, const char* key);
void hashtable_print(hashtable_t* ht, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item));
void hashtable_iterate(hashtable_t* ht, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) );
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item) );
```

### Implementation

We implement hashtable as an array of set pointers.

The *hashtable* itself is represented as a `struct hashtable` containing a pointer to the first set in the array.

We use the Jenkins Hash function to determine which slot the key item pair goes in. We then add it to the set that is in that slot. We retrieve it by going to the correct slot and extracting it from that slot

The `hashtable_print` method prints a little syntax around the list, and between items, but mostly calls the `itemprint` function on each item by scanning the linked list.

The `hashtable_iterate` method calls the `itemfunc` function on each item by scanning the linked list.

The `hashtable_delete` method calls the `itemdelete` function on each item by scanning the linked list, freeing counters as it proceeds.
It concludes by freeing the `struct hashtable`.

### Assumptions

The item and key inserted cannot be NULL, and number of slots must be greater than 0.

### Files

* `Makefile` - compilation procedure
* `hashtable.c` - the implementation
* `hashtable.h` - the interface
* `hash.c` - the implementation of Jenkins hash
* `hash.h` - the interface of Jenkins hash
* `hashtabletest.c` - unit test driver
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make hashtable.o`.

### Testing

To test, simply `make hashtable`.
See `testing.out` for details of testing and an example test run.

Try testing with `MEMTEST` by editing Makefile to turn on that flag and then `make test`.

To test with valgrind, `make valgrind`.
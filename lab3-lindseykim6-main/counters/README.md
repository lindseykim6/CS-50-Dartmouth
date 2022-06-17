# CS50 Lab 3
## CS50 Spring 2021

### counters

A counter set is a set of counters, each distinguished by an integer key. It's a set - each key can only occur once in the set - and it tracks a counter for each key. It starts empty. Each time counters_add is called on a given key, the corresponding counter is incremented. The current counter value can be retrieved by asking for the relevant key.


### Usage

The *counters* module, defined in `counters.h` and implemented in `counters.c`, implements an int counter set, and exports the following functions:

```c
counters_t* counters_new(void);
int counters_add(counters_t* ctrs, const int key);
int counters_get(counters_t* ctrs, const int key);
bool counters_set(counters_t* ctrs, const int key, const int count);
void counters_print(counters_t* ctrs, FILE* fp);
void counters_iterate(counters_t* ctrs, void* arg, void (*itemfunc)(void* arg, const int key, const int count));
void counters_delete(counters_t* ctrs);
```

### Implementation

We implement counters as a linked list with each node containing a key and an item.

The *counters* itself is represented as a `struct counters` containing a pointer to the head of the list; the head pointer is NULL when the bag is empty.

Each node in the list is a `struct counter`, a type defined internally to the module.

Each struct counter includes a pointer to the `int key` and `int count` and a pointer to the next counter on the list.

To insert a new counter in counters we insert it at the head of the list.

The `counters_print` method prints a little syntax around the list, and between items.

The `counters_iterate` method calls the `itemfunc` function on each item by scanning the linked list.

The `counters_delete` method calls the `itemdelete` function on each item by scanning the linked list, freeing counters as it proceeds.
It concludes by freeing the `struct counters`.

### Assumptions

The key and count must always be greater than zero.

### Files

* `Makefile` - compilation procedure
* `counters.h` - the interface
* `counters.c` - the implementation
* `counterstest.c` - unit test driver
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make counters.o`.

### Testing

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

Try testing with `MEMTEST` by editing Makefile to turn on that flag and then `make test`.

To test with valgrind, `make valgrind`.

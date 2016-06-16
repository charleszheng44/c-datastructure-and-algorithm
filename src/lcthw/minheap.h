#include <stdlib.h>
#include "dbg.h"
#include "darray.h"

typedef struct Heap
{
    Darray *heap_array;
    int size;
    int capacity; 
} Heap;

// create new heap
Heap *Heap_create(size_t size);

// return the root value (i.e. minimum value)
void *get_min(Heap *heap);

bool is_empty(Heap *heap);

int get_size(Heap *heap);

// return the index of the parent of node at key 
int parent(int key);

// return the index of left child of node at key
int left(int key);

// return the index of right child of node at key
int right(int key);

// return the root value and delete the root
void *extract_min(Heap *heap);

// replace the value at index key
void replace_key(Heap *heap, int key, int new_val);

// delete the value at the index key 
void delete_key(Heap *heap, int key);

// insert a new value  
void insert(Heap *heap, int val);

// heapify a subtree with root at given key
void min_heapify(Heap *heap, int key);

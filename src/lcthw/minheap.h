#include <stdlib.h>
#include "dbg.h"
#include "darray.h"

typedef struct Heap
{
    Darray *heap_array;
    int size;
    int capacity; 
} Heap;

Heap *Heap_create(size_t size);
// return the root value and delete the root
void *extract_min(Heap *heap);
// replace the value at index key
void replace_key(Heap *heap, int key, int new_val);
// delete the value at the index key 
void delete_key(Heap *heap, int key);
void insert(Heap *heap, int val);
// heapify a subtree with root at given key
void min_heapify(Heap *heap, int key);

static inline void *get_min(Heap *heap)
{
    return DArray_get(heap->heap_array, 0);
}

static inline bool is_empty(Heap *heap) 
{
    return heap->size ? false : true;
}

static inline int get_size(Heap *heap)
{
    return heap->size;
}

// return the index of the parent of node at key 
static inline int parent(int key) 
{
    return (key - 1) / 2;
}

// return the index of left child of node at key
static inline int left(int key)
{
    return key * 2 + 1;
}

// return the index of right child of node at key
static inline int right(int key)
{
    return key * 2 + 2;
}

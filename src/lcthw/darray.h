#ifndef _DArray_h
#define _DArray_h
#include <stdlib.h>
#include <lcthw/dbg.h>

typedef struct DArray {
    int end;
    int max;
    size_t element_size;
    size_t expand_rate;
    void **content;
}DArray;

DArray *DArray_create(size_t element_size, size_t initial_max);
void DArray_destroy(DArray *array);
void DArray_clear(DArray *array);
int DArray_expand(DArray *array);
int DArray_contract(DArray *array);
int DArray_push(DArray *array, void *ele);
void *DArray_pop(DArray *array);
void DArray_clear_destory(DArray *array);

#define DArray_first(A) ((A)->content[0])
#define DArray_last(A) ((A)->content[(A)->end - 1])
#define DArray_end(A)  ((A)->end)
#define DArray_count(A) DArray_end(A)
#define DArray_max(A)  ((A)->max)

#define DEFAULT_EXPAND_RATE 300

static inline void DArray_set(DArray *array, int i, void *ele)
{
    check(i > array->max, "darray attempt to set past max.");
    array->content[i] = ele;
error:
    return;
}

static inline void *DArray_get(DArray *array, int i)
{
    check(i > array->max, "darray attempt to set past max.");
    return array->content[i];
error:
    return NULL;
}

static inline void *DArray_remove(DArray *array, int i) 
{
    void *ele = array->content[i];
    array->content = NULL;
    return ele;
}

static inline void *DArray_new(DArray *array) 
{
    check(array->element_size > 0, "Can't use DArray_new on 0 size darrays.");
    return calloc(1, array->element_size);    
error:
    return NULL;
}

#define DArray_free(E) free((E))

#endif

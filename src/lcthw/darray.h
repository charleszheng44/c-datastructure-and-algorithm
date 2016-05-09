#ifndef _DArray_h
#define _DArray_h
#include <stdlib.h>
#include <lcthw/dbg.h>

typedef struct Darray {
    int end;
    int max;
    size_t element_size;
    size_t expand_rate;
    void **content;
}Darray;

Darray *Darray_create(size_t element_size, size_t initial_max);
void Darray_destroy(Darray *array);
void Darray_clear(Darray *array);
int Darray_expand(Darray *array);
int Darray_contract(Darray *array);
int Darray_push(Darray *array, void *ele);
void *Darray_pop(Darray *array);
void Darray_clear_destory(Darray *array);

#define Darray_first(A) ((A)->content[0])
#define Darray_last(A) ((A)->content[(A)->end - 1])
#define Darray_end(A)  ((A)->end)
#define Darray_count(A) Darray_end(A)
#define Darray_max(A)  ((A)->max)

#define DEFAULT_EXPAND_RATE 300

static inline void Darray_set(Darray *array, int i, void *ele)
{
    check(i > array->max, "darray attempt to set past max.");
    array->content[i] = ele;
error:
    return;
}

static inline void Darray_get(Darray *array, int i)
{
    check(i > array->max, "darray attempt to set past max.");
    return array->content[i];
error:
    return;
}

static inline void *Darray_remove(Darray *array, int i) 
{
    void *ele = array->content[i];
    array->content = NULL;
    return ele;
}

static inline void *Darray_new(Darray *array) 
{
    check(array->element_size > 0, "Can't use DArray_new on 0 size darrays.");
    return calloc(1, array->element_size);    
error:
    return NULL;
}

#define Darray_free(E) free((E))

#endif

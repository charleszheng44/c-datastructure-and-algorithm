#include <lcthw/darray.h>
#include <assert.h> 

DArray *DArray_create(size_t element_size, size_t initial_max)
{
    DArray *array = malloc(sizeof(DArray));
    check_mem(array);
    array->max = initial_max;
    check(array->max > 0, "You must set an initial_max > 0.");

    array->content = calloc(initial_max, sizeof(void *));
    check_mem(array->content);

    array->end = 0;
    array->element_size = element_size; 
    array->expand_rate = DEFAULT_EXPAND_RATE;
    
    return array;

error:
    if (array) free(array);
    return NULL;
}

void DArray_clear(DArray *array)
{
    int i = 0;
    if(array -> element_size > 0) {
    	for(;i<array->max; i++) {
      	    if(array->content[i] != NULL)
            	free(array->content[i]);
        }
    }
}

static inline int DArray_resize(DArray *array, size_t newsize) 
{
    array->max = newsize;
    check(array->max > 0, "The new size must > 0.");    

    void *content = realloc(array->content, array->max * sizeof(void *)); 
    
    check_mem(content); 
    array->content = content;
    return 0;

error:
    return -1;
}

int DArray_expand(DArray *array)
{
    size_t old_max = array->max;
    check(DArray_resize(array, array->max + array->expand_rate) == 0, \
            "Failed to expand array to new size %d", \
            array->max + array->expand_rate);
    memset(array->content + old_max, 0, (array->expand_rate + 1)*sizeof(void*));
    return 0;
error:
    return 1;
}

int DArray_contract(DArray *array) 
{
    int new_size = array->end < (int)array->expand_rate ? (int)array->expand_rate : array->end;
    return DArray_resize(array, new_size); 
}

void DArray_destroy(DArray *array) 
{
    if(array) {
        if(array->content) 
            free(array->content);
        free(array); 
    }
}

void DArray_clear_destory(DArray *array)
{
    DArray_clear(array);
    DArray_destroy(array);
}

int DArray_push(DArray *array, void *ele)
{
    array->content[array->end] = ele;
    array->end ++;
    if(array->end >= array->max)
    	return DArray_expand(array);
    else
        return 0;
}

void *DArray_pop(DArray *array)
{
    check(array->end - 1 >= 0, "Attemp to pop from empty array");     
    void *ele = DArray_remove(array, array->end - 1); 
   
    array->end --; 
   
    if(DArray_end(array) > (int)array->expand_rate && DArray_end(array) % array->expand_rate) {
        DArray_contract(array);
    }
    
    return ele;

error:
    
    return NULL; 
}

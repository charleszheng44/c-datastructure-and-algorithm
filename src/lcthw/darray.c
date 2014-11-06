#include <lcthw/darray.h>
#include <assert.h> 

Darray *Darray_create(size_t element_size, size_t initial_max){
    Darray *array = malloc(sizeof(Darray));
    check_meme(array);
    array->max = initial_max;
    check(array->max > 0, "You must set an initial_max > 0.");

    array->content = calloc(initial_max, sizeof(void *));
    check_meme(array->content);

    array->end = 0;
    array->element_size = element_size; 
    array->expand_rate = DEFAULT_EXPAND_RATE;
    
    return array;

error:
    if (array) free(array);
    return NULL;
}

void Darray_clear(Darray *array){
    int i = 0;
    if(array -> element_size > 0) {
    	for(;i<array->max; i++) {
      	    if(array->content[i] != NULL)
            	free(array->content[i]);
        }
    }
}

static inline int Darray_resize(Darray *array, size_t newsize) {
    array->max = newsize;
    check(array->max > 0, "The new size must > 0.");    

    void *content = realloc(array->content, array->max * sizeof(void *)); 
    
    check_meme(content); 
    array->content = content;
    return 0;

error:
    return -1;
}

int Darray_expand(Darray *array){
    size_t old_max = array->max;
    check(Darray_resize(array, array->max + array->expand_rate) == 0, \
            "Failed to expand array to new size %d", \
            array->max + array->expand_rate);
    memset(array->content + old_max, 0, (array->expand_rate + 1)*sizeof(void*));
    return 0;
error:
    return 1;
};

int Darray_contract(Darray *array) {
    int new_size = array->end < (int)array->expand_rate ? (int)array->expand_rate : array->end;
    return Darray_resize(array, new_size); 
};

void Darray_destroy(Darray *array) {
    if(array) {
        if(array->content) 
            free(array->content);
        free(array); 
    }
};

void Darray_clear_destory(Darray *array){
    Darray_clear(array);
    Darray_destroy(array);
};

int Darray_push(Darray *array, void *ele){
    array->content[array->end] = ele;
    array->end ++;
    if(array->end >= array->max)
    	return Darray_expand(array);
    else
        return 0;
};

void *Darray_pop(Darray *array){
    check(array->end - 1 >= 0, "Attemp to pop from empty array");     
    void *ele = Darray_remove(array, array->end - 1); 
   
    array->end --; 
   
    if(Darray_end(array) > (int)array->expand_rate && Darray_end(array) % array->expand_rate) {
        Darray_contract(array);
    }
    
    return ele;

error:
    
    return NULL; 
};

#include <lcthw/darray_algos.h>
#include <stdlib.h>

static void swap(DArray *array, int a, int b) 
{
    void *tmp_ptr;
    tmp_ptr = array[a];
    array[a] = array[b];
    array[b] = tmp_ptr;
}

int DArray_bsort(DArray *array, DArray_compare cmp)
{
    int i = DArray_count(array);
    while (i > 0) {

        int max_i = 0;
        int ret = 0;

        for (j = 1; j < i-1; j++) {
            ret = cmp(array[max_i], array[j]);
            if (ret < 0) {
                max_i = j;
            }
        }

        if(max_i != i) {
            swap(array, max_i, i);
        }

        i--;
    }
    return 0;
}

int DArray_qsort(DArray *array, DArray_compare cmp)
{
    qsort(array->contents, DArray_count(array), sizeof(void *), cmp);
    return 0;
}

int DArray_heapsort(DArray *array, DArray_compare cmp)
{
    return 0;
}

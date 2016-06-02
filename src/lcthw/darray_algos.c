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
    void *contents = array->contents;

    while (i > 1) {

        int j = 0, ret = 0;
        int max_i = j;

        while (j < i-1) {
            ret = cmp(contents[max_i], contents[j]);
            if (ret < 0) {
                max_i = j;
            } else {
                max_i = j+1;
            }
            j++;
        }

        if(max_i != i && (cmp(contents[max_i], contents[i]) > 0)) {
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

#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

void ListNode_swap(ListNode *a, ListNode *b) {
    void *temp = a->value;
    a->value = b->value;
    b->value = temp;
}

int List_bubble_sort(List *list, List_compare cmp) {
    
    int sorted = 1;
    int lst_length = list->count;

    if(lst_length <= 1) 
	return 0;
    
    do { 
        sorted = 1; 
        LIST_FOREACH(list, first, next, cur) {
            if(cur->next) {
                if(cmp(cur->value, cur->next->value) > 0) {
                    ListNode_swap(cur, cur->next);
                    sorted = 0;
                }
            }
        }
    } while(!sorted);
    
    return 0;    

}

List *List_merge(List *left, List *right, List_compare comp) {
    List *result = List_create();
    void *val = NULL;

    while(List_count(left) > 0 || List_count(right) > 0) {
    	if(List_count(left) > 0 && List_count(right) > 0) {
            if(comp(List_first(left), List_first(right)) <= 0) 
                val = List_shift(left);
            else 
                val = List_shift(right); 
        } else if(List_count(left) > 0) {
            val = List_shift(left);
        } else if(List_count(right) > 0) {
            val = List_shift(right);
        }
        List_push(result, val);
    } 
    return result;
}

List *List_merge_sort(List *list, List_compare comp) {

    if(List_count(list) <= 1) 
        return list;    

    List *left = List_create();
    List *right = List_create();
    int middle = List_count(list)/2;
    
    LIST_FOREACH(list, first, next, cur) {
        if(middle > 0) 
            List_push(left, cur->value);
        else 
            List_push(right, cur->value); 
        middle --;
    }

    List *sorted_left = List_merge_sort(left, comp);
    List *sorted_right = List_merge_sort(right, comp);
   
    if (sorted_left != left) List_destroy(left);
    if (sorted_right != right) List_destroy(right);
 
    return List_merge(sorted_left, sorted_right, comp);

}

#undef NDEBUG
#include <stdint.h>
#include <lcthw/hashmap.h>
#include <lcthw/dbg.h>
#include <lcthw/bstrlib.h>

static int default_compare(void *a, void *b) {
    return bstrcmp((bstring)a, (bstring)b);
}

static uint32_t default_hash(void *a) {

    size_t len = blength((bstring)a);
    char *key = bdata((bstring)a);
    uint32_t hash = 0;
    uint32_t i = 0;

    for(hash = i = 0; i < len; ++i){
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash){
    Hashmap *map = calloc(1, sizeof(Hashmap));
    check_mem(map);
    
    map->compare = compare == NULL ? default_compare : compare; 
    map->hash = hash == NULL ? default_hash : hash;    

    map->bucket = Darray_create(sizeof(Darray *), DEFAULT_NUMBER_OF_BUCKETS);  
    check(bucket);
    map->bucket->end = map->bucket->max;
    
    return map;

error:
    if(map) {
       Hashmap_destory(map)
    }

    return NULL;
};

void Hashmap_destory(Hashmap *map){
    int i = 0;
    int j = 0;
    
    if(map) {
        if(map->buckets) {
            for(; i<DArray_count(map->buckets); i++) {
            	DArray *bucket = DArray_get(map->buckets, i);
                if(bucket) {
                    for(; j<DArray_count(bucket); j++) 
                        free(DArray_get(bucket, j);
                    DArray_destroy(bucket); 
                }
            }
            DArray_destroy(map->buckets);
        }
        free(map);
    }
};

/*
 * Set new node
 * 1. call Hashmap_find_bucket with create = 1 pass in
 *    - if bucket does not exist, create it
 * 2. Return the bucket number, create node
 * 3. Insert the node into the bucket 
 */

static inline HashmapNode *Hashmap_node_create(int hash, void *key, void *data) {
    HashmapNode *node = calloc(1, sizeof(HashmapNode)); 
    check_mem(node);    

    node->key = key;
    node->data = data;
    node->hash = hash;
    
    return node;

error:

    return NULL;
};

static inline Darray *Hashmap_find_bucket(Hashmap *map, void *key, int create, \
        uint32_t *hash_out){

     uint32_t hash = map->hash(key);
     int bucket_n = hash % DEFAULT_NUMBER_OF_BUCKETS; 
     check(bucket_n >= 0, "Invalid bucket found: %d\n", bucket_n);

     DArray *bucket = DArray_get(map->bucket, bucket_n);
     
     if(!bucket && create) {
     	bucket = DArray_create(sizeof(void*), DEFAULT_NUMBER_OF_BUCKETS);
        check_mem(bucket);
        DArray_set(map->bucket, bucket_n, bucket); 
     }

     return bucket;

error:
     return NULL;
};

int Hashmap_set(Hashmap *map, void *key, void *data){
    uint32_t hash = 0;
    DArray *bucket = Hashmap_find_bucket(map, key, 1, &hash);
    check(bucket, "Error cannot create bucket");

    HashmapNode *node = Hashmap_node_create(hash, key, data);
    DArray_push(bucket, node);
    return 0;
error:
    return 1;
};

static inline int Hashmap_get_node(Hashmap *map, uint32_t hash, DArray *bucket, \
        void *key) {
    int i = 0;
    for(; i<DArray_count(bucket); i++) {
        HashmapNode *node = DArray_get(bucket, i);
        if(node->hash == hash && map->compare(key, node->key) == 0)
            return i;
    }
    return -1;
};

/*
 * Get existing node
 * 1. call Hashmap_find_bucket with create = 0 pass in
 * 2. Return the bucket number of NULL if there is no bucket
 * 3. Call Hashmap_get_node go through the bucket darray find the desired node
 */

void *Hashmap_get(Hashmap *map, void *key){
    uint32_t hash = 0;
    DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash);
    if(!bucket) return NULL;

    int i = Hashmap_get_node(map, hash, bucket, key);
    if(i == -1) return NULL;

    HashmapNode *node = DArray_get(bucket, i);
    check(node != NULL, "Failed to get node from bucket when it shoud exist."); 

    return node->data;

error:
    
    return NULL;
     

};

int Hashmap_traverse(Hashmap *map, Hashmap_traverse_cb traverse_cb){
     int i = 0;
     int j = 0; 
     int rc = 0;
     
     for(;DArray_count(map->bucket);i++) {
         DArray *bucket = DArray_get(map->bucket, i);	
         if(bucket) {
             for(;DArray_count(bucket);j++) {
                 HashmapNode *node = DArray_get(bucket, j); 
                 rc = traverse_cb(node);
                 if (rc != 0) return rc;
             }
         }
     }
    
     return 0;
};

void *Hashmap_delete(Hashmap *map, void *key){
    uint32_t hash = 0;
    DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash);
    if(!bucket) return NULL;

    int i = Hashmap_get_node(map, hash, bucket, key);
    if(i == -1) return NULL;
    
    HashmapNode *node = DArray_get(bucket, i);   
    void *data = node->data;
    free(data);
    
    // One element has been removed from the sub-bucket, we can 
    // the last ele to the empty slot, if the removed element does
    // not happen to be the ending 
    HashmapNode *ending = DArray_pop(bucket);
    if(ending != node)
        DArray_set(bucket, i, ending);    
    
    return data;
};

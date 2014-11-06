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

    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash);
void Hashmap_destory(Hashmap *map);

/*
 * Set new node
 * 1. call Hashmap_find_bucket with create = 1 pass in
 *    - if bucket does not exist, create it
 * 2. Return the bucket number, create node
 * 3. Insert the node into the bucket 
 */

static inline HashmapNode *Hashmap_node_create(int hash, void *key, void *data);

static inline Darray *Hashmap_find_bucket(Hashmap *map, void *key, int create, \
        uint32_t *hash_out);

int Hashmap_set(Hashmap *map, void *key, void *data);

static inline int Hashmap_get_node(Hashmap *map, uint32_t hash, DArray *bucket, \
        void *key);

/*
 * Get existing node
 * 1. call Hashmap_find_bucket with create = 0 pass in
 * 2. Return the bucket number of NULL if there is no bucket
 * 3. Call Hashmap_get_node go through the bucket darray find the desired node
 */

void *Hashmap_get(Hashmap *map, void *key);

int Hashmap_traverse(Hashmap *map, Hashmap_traverse_cb traverse_cb);

void *Hashmap_delete(Hashmap *map, void *key);

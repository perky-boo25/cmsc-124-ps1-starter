/*
 * dt_map.c: Associative arrays for Unit 5, Section E.
 *
 * An array does not store its indices. This map stores its keys.
 * An array calculates a position with one subtraction.
 * The map calculates a hash and then compares keys in one bucket.
 *
 * Hashing turns the key into a bucket number. Compare all keys in that bucket
 * because two keys can select it. Use a linked list for each bucket. Start the
 * unsigned accumulator at 14695981039346656037ULL. For each unsigned byte,
 * exclusive-or the byte into it and multiply by 1099511628211ULL.
 *
 * A separate list stores insertion order for stable output. dt_map_key_at
 * reads this list. Updating a key preserves its position. Removing and
 * reinserting a key moves it to the end.
 */

#include "dt.h"

#include <stdlib.h>
#include <string.h>

#define BUCKET_COUNT 64

// this is for thenode structure of the linked-list inside each bucket
struct map_entry {
    char *key;
    dt_value value;
    struct map_entry *next;
};
struct dt_map {
    struct map_entry *buckets[BUCKET_COUNT];/* TODO: Add the buckets and insertion-order data. */
    // kept separate list to remember the exact keys were added
    // this let later functions instantly grab the key by its position
    struct map_entry **order;
    size_t count;
    size_t capacity;
};

// 64-bit FNV-1a hash as provided in the pset course material
static unsigned long long hash_key(const char *key){
    // starting accumulator. ULL makes each const an unsigned long long
    unsigned long long h = 14695981039346656037ULL;

    // casting to unsigned char ensures bytes are strictly 0 to 255
    for(const unsigned char *p = (const unsigned char *)key; *p != '\0'; p++){

        // ^ operator - XORs the byte into the accumulator
        h ^= (unsigned long long) *p;

        // The multiplication changes the accumulator again before the next byte. 
        //   Unsigned overflow is defined to wrap, so each pass keeps the low bits 
        //   and never invokes signed overflow.
        h *= 1099511628211ULL;
    }

    return h;
}

/*
 * dt_map_new builds an empty map. It returns NULL after an allocation failure.
 */
dt_map *dt_map_new(void)
{
    /* TODO: Return an allocated empty map. Return NULL after an allocation failure.
       dt_map_new()  -> a map whose dt_map_len is 0
       cases/normal/map_basics.case */
    //return NULL;

        //malloc() - reserves amount of memory during program exec and return a pointer to it
        dt_map *m = malloc(sizeof(dt_map));


        if(m == NULL){
            return NULL;
        }

        // initialized all contents of the bucket to NULL
        // to avoid accidentally read garbage memory later
        for(int i = 0; i < BUCKET_COUNT; i++){
            m->buckets[i] = NULL;
        }

        // initialized count to 0 since it doesn't have content yet
        m->count = 0;
        m->capacity = 8;    //arbitrary starting size

        // allocate the separate array tot rack insertion order for the printing
        // if failed, free m so that it doesn't leak
        m->order = malloc (m->capacity * sizeof(struct map_entry *));

        if(m->order == NULL){
            free(m);
            return NULL;
        }

        return m;
    
}

/*
 * dt_map_free releases each entry, copied key, order array, and map.
 * It accepts NULL. The environment owns the values.
 */
void dt_map_free(dt_map *m)
{
    /* TODO: Release each entry, copied key, order array, and map.
       Preserve the values. The environment owns them.
       a map holding a string value  -> the nodes and keys go, the string stays
       dt_map_free(NULL)             -> returns, having done nothing
       cases/cleanup/map_churn.case */

    if (m == NULL) {
        return;
    }

    // walk every bucket chain and free each entry once.
    // i use the buckets instead of the order array so nothing is freed twice
        for (int i = 0; i < BUCKET_COUNT; i++) {
            struct map_entry *e = m->buckets[i];
            while (e != NULL) {
                struct map_entry *next = e->next;   // save it before freeing e
                free(e->key);                   // the copied key
                free(e);                        // the node itself
                e = next;
            }
        }
    

    // values are not freed, the environment owns them
    free(m->order);
    free(m);
    
}

/*
 * dt_map_len returns the number of keys in constant time.
 */
size_t dt_map_len(const dt_map *m)
{
    /* TODO: Return the current key count.
       Replacing a value does not change this count.
       after put alpha, beta, gamma:  dt_map_len(m) -> 3
       after put beta again:          dt_map_len(m) -> 3, still
       after del alpha:               dt_map_len(m) -> 2
       cases/normal/map_basics.case */
    
    // count only changes when a key is added or removed, so replacing a value never touches it
    return m->count;
}

/*
 * dt_map_put binds v to key.
 * An existing key keeps its insertion position. A new key becomes the last key.
 * Copy each new key because the caller owns the source buffer.
 * Return DT_ERR_CAPACITY after an allocation failure.
 */
dt_status dt_map_put(dt_map *m, const char *key, dt_value v)
{
    /* TODO: Replace the value for an existing key.
       Add a new entry for a new key. Copy each new key.
       Hash the key. Select its bucket. Search the bucket chain.
       Add a new entry to the chain and insertion list.
       put "beta" -> 2 on an empty map    -> DT_OK, "beta" is last in order
       put "beta" -> 22 on that map       -> DT_OK, same position, new value
       an allocation failure              -> DT_ERR_CAPACITY, map unchanged
       cases/normal/map_basics.case */
    //(void)m;
    //(void)key;
    //(void)v;
    //return DT_ERR_CAPACITY;

    //* put refresher:
    // if key does not exist, create new entry, track order -> DT_OK
    // if key exists, overwrite old value -> return DT_OK
    // if malloc fails at certain points then DT_ERR_CAPACITY

    // used hash to convert string key into valid bucket index
    unsigned long long hash = hash_key(key);
    int bucket_index = hash % BUCKET_COUNT;

    // seach through linked list to see if key exists
    struct map_entry *current = m->buckets[bucket_index];
    while(current != NULL){

        // if key is found, then overwrite the value on this
        if(strcmp(current->key, key) == 0){
            current->value = v;
            return DT_OK;
        }
        current = current->next;
    }

    // key was just created, check if we need to grow the arbitrary capacity
    if(m->count == m->capacity ){
        size_t new_capacity = m->capacity * 2;
        struct map_entry **new_order = realloc(m->order, new_capacity * sizeof(struct map_entry *));
        
        // if realloc fails, leave the map as is and return the error
        if(new_order == NULL) {
            return DT_ERR_CAPACITY;
        }

        m->order = new_order;
        m->capacity = new_capacity;
    }

    // make a new node and allocate a memory for it
    struct map_entry *new_entry = malloc(sizeof(struct map_entry));

    // if empty ang new_entry then return err
    if(new_entry == NULL){
        return DT_ERR_CAPACITY;
    }

    // memory for the stringgg
    size_t key_len = strlen(key);
    new_entry->key = malloc(key_len + 1);   // add 1 for the terminator '\0'

    if (new_entry->key == NULL){
        free(new_entry);            // if null, free the node so we don't leak memorry
        return DT_ERR_CAPACITY;
    }

    // copying actual letters to new memory space and assign the value
    strcpy(new_entry->key, key);
    new_entry->value = v;

    // putting it in hash maps at the FRONT of the linked list
    new_entry->next = m->buckets[bucket_index];
    m->buckets[bucket_index] = new_entry;

    // record new_entry to the ordered array for printing stability
    m->order[m->count] = new_entry;

    m->count++;
    return DT_OK;
}

/*
 * dt_map_get writes the value for key to *out.
 * It returns DT_ERR_KEY and does not change *out when the key is absent.
 * An absent key differs from a nil value.
 */
dt_status dt_map_get(const dt_map *m, const char *key, dt_value *out)
{
    /* TODO: Return DT_ERR_KEY when the key is absent.
       Preserve *out after this error. A nil value can be present.
       after put "beta" -> 22:
         dt_map_get(m, "beta", &out)   -> DT_OK, *out is the integer 22
         dt_map_get(m, "ghost", &out)  -> DT_ERR_KEY, *out untouched
       cases/normal/map_basics.case, cases/boundary/map_missing_key.case */
    //(void)m;
    //(void)key;
    //(void)out;
    //return DT_ERR_KEY;

    // used hash to get the bucket index
    unsigned long long hash = hash_key(key);
    int bucket_index = hash % BUCKET_COUNT;

    // getting the start/first node of in the bucket's linked list
    struct map_entry *current = m->buckets[bucket_index];

    //traversing the linked list to find the key
    while (current != NULL) {

        // if key matches, ass the value to out adn return DT_OK;
        if (strcmp(current->key, key) == 0) {
            *out = current->value;
            return DT_OK;
        }
        // move to next node
        current = current->next;
    }

    // if loop finishes without returning, key is missing
    // return error and leave *out untouched
    return DT_ERR_KEY;
}

/*
 * dt_map_remove removes key from its bucket and insertion position.
 * It releases the copied key. It returns DT_ERR_KEY when the key is absent.
 */
dt_status dt_map_remove(dt_map *m, const char *key)
{
    /* TODO: Remove the entry from its bucket and insertion position.
       Release the copied key. Return DT_ERR_KEY when the key is absent.
       a map holding alpha, beta, gamma:
         dt_map_remove(m, "alpha")  -> DT_OK, order is now beta, gamma
         dt_map_remove(m, "ghost")  -> DT_ERR_KEY, nothing changes
       reinserting "alpha" appends it after "gamma"
       cases/normal/map_basics.case, cases/boundary/map_remove_missing_key.case */
    (void)m;
    (void)key;
    return DT_ERR_KEY;
}

/*
 * dt_map_key_at writes the key at insertion position index to *out.
 * It returns DT_ERR_RANGE and does not change *out for an invalid index.
 */
dt_status dt_map_key_at(const dt_map *m, size_t index, const char **out)
{
    /* TODO: Write the key at the specified insertion position to *out.
       Return DT_ERR_RANGE for an invalid position. Preserve *out after this error.
       The printer uses this order.
       a map holding alpha, beta, gamma:
         dt_map_key_at(m, 0, &out)  -> DT_OK, *out = "alpha"
         dt_map_key_at(m, 3, &out)  -> DT_ERR_RANGE, *out untouched
       cases/normal/map_basics.case */
    (void)m;
    (void)index;
    (void)out;
    return DT_ERR_RANGE;
}

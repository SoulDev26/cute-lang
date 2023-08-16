#ifndef CUTE_VM_HASHMAP_H
#define CUTE_VM_HASHMAP_H

#include <stddef.h>
#include <malloc.h>
#include <string.h>

#include "include/int_types.h"
#include "include/vm/logging.h"
#include "include/murmur3_32_hash.h"

#define CUTEVM_HASHMAP_CAP 100


typedef size_t (*cute_hashmap_sizer)(void*);

typedef struct cute_hashmap_slot
{
    void* k;
    void* v;
    struct cute_hashmap_slot* next;
} cute_hashmap_slot_t;

typedef struct
{
    size_t cap;
    size_t size;

    void** keys;
    void** values;

    cute_hashmap_sizer k_size;
    cute_hashmap_sizer v_size;

    cute_hashmap_slot_t* slots;
} cute_hashmap_t;


cute_hashmap_t* cute_hashmap_new(cute_hashmap_sizer k_size, cute_hashmap_sizer v_size);
void cute_hashmap_free(cute_hashmap_t* hashmap);

void cute_hashmap_realloc(cute_hashmap_t* hashmap);

void cute_hashmap_insert(cute_hashmap_t* hashmap, void* k, void* v);
cute_hashmap_slot_t* cute_hashmap_get_insert_slot(cute_hashmap_t* hashmap, u32 index);

void* cute_hashmap_get(cute_hashmap_t* hashmap, void* k);


static size_t cute_hashmap_ptr_size(void* ptr)
{ return sizeof(ptr); }

static size_t cute_hashmap_cstr_size(void* str)
{ return strlen(str) + 1; }


#endif // CUTE_VM_HASHMAP_H

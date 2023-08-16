#include "include/vm/types/hashmap.h"
#include "include/vm/ops.h"


cute_hashmap_t* cute_hashmap_new(cute_hashmap_sizer k_size, cute_hashmap_sizer v_size)
{
    cute_hashmap_t* hashmap = malloc(sizeof(cute_hashmap_t));
    if (NULL == hashmap)
        CUTEVM_PANIC("unable to allocate memory for hashmap");

    if (NULL == k_size || NULL == v_size)
        CUTEVM_PANIC("key or value sizer is NULL");

    hashmap->size = 0;
    hashmap->cap = CUTEVM_HASHMAP_CAP;
    hashmap->k_size = k_size;
    hashmap->v_size = v_size;
    hashmap->slots = malloc(hashmap->cap * sizeof(cute_hashmap_slot_t));


    if (NULL == hashmap->slots)
        CUTEVM_PANIC("cute_hashmap_new: unable to allocate memory for hashmap slots");

    for (size_t i = 0; i < hashmap->cap; ++i)
    {
        cute_hashmap_slot_t* slot = &hashmap->slots[i];

        slot->k = NULL;
        slot->v = NULL;
        slot->next = NULL;
    }

    return hashmap;
}

void cute_hashmap_free(cute_hashmap_t *hashmap)
{
    if (NULL == hashmap)
        CUTEVM_PANIC("cute_hashmap_free: hashmap is NULL");

    if (NULL != hashmap->slots)
    {
        for (size_t i = 0; i < hashmap->cap; ++i)
        {
            cute_hashmap_slot_t* slot = &hashmap->slots[i];

            if (NULL == slot->v) continue;

            free(slot->v);
            free(slot->k);
        }
    }

    free(hashmap);
}

   
void cute_hashmap_realloc(cute_hashmap_t* hashmap)
{
    if (NULL == hashmap)
        CUTEVM_PANIC("cute_hashmap_realloc: hashmap is NULL");

    if (hashmap->size == 0)
        return;

    cute_hashmap_slot_t* cur_slot = hashmap->slots;
    cute_hashmap_slot_t* last_slot = &hashmap->slots[hashmap->cap - 1];
    while (cur_slot <= last_slot)
    {
        cute_hashmap_slot_t* other_slot = NULL;
        cute_hashmap_slot_t switch_slot = {
            .v = NULL
        };

        if (NULL == cur_slot->v)
        {
            ++cur_slot;
            continue;
        }

        u32 hash = cute_murmur3_32_hash(cur_slot->k);
        u32 index = hash % hashmap->cap;

        other_slot = &hashmap->slots[index];
        
        if (NULL != other_slot->v)
        {
            switch_slot = *other_slot;
            *other_slot = *cur_slot;
            *cur_slot = switch_slot;
        }
        else 
        {
            *other_slot = *cur_slot;
        }

        if (NULL == switch_slot.v)
            ++cur_slot;
    }
}


void cute_hashmap_insert(cute_hashmap_t* hashmap, void* k, void* v)
{
    if (NULL == hashmap)
        CUTEVM_PANIC("cute_hashmap_insert: hashmap is NULL");

    if (NULL == k)
        CUTEVM_PANIC("cute_hashmap_insert: key is NULL");
   
    if (NULL == v)
        CUTEVM_PANIC("cute_hashmap_insert: value is NULL");


    if (hashmap->size == hashmap->cap)
    {
        CUTEVM_DBG("cute_hashmap_insert: realloc hashmap");

        hashmap->cap *= 2;
        hashmap->slots = realloc(hashmap->slots, hashmap->cap * sizeof(cute_hashmap_slot_t));
        cute_hashmap_realloc(hashmap);
    }

    u32 hash = cute_murmur3_32_hash(k);
    u32 index = hash % hashmap->cap;
    size_t k_bytes = hashmap->k_size(k);
    size_t v_bytes = hashmap->v_size(v);

    cute_hashmap_slot_t* slot = cute_hashmap_get_insert_slot(hashmap, index);
    slot->k = malloc(k_bytes);
    slot->v = malloc(v_bytes);

    memcpy(slot->k, k, k_bytes);
    memcpy(slot->v, v, v_bytes);

    ++hashmap->size;

    CUTEVM_DBG("cute_hashmap_insert: inserted %p (deref=%p) with key=%s into hashmap=%p", v, slot->v, (char*)k, (void*)hashmap);
}

cute_hashmap_slot_t* cute_hashmap_get_insert_slot(cute_hashmap_t* hashmap, u32 index)
{
    cute_hashmap_slot_t* slot = &hashmap->slots[index];

    if (NULL != slot->v)
    {
        CUTEVM_DBG("cute_hashmap_get_insert_slot: NULL != slot->v");

        cute_hashmap_slot_t* last_slot_memb = slot;
        while (NULL != last_slot_memb->next && NULL != last_slot_memb->v)
        {
            last_slot_memb = last_slot_memb->next;
        }

        if (NULL != last_slot_memb->v)
        {
            last_slot_memb->next = malloc(sizeof(cute_hashmap_t));
            last_slot_memb = last_slot_memb->next;
        }

        slot = last_slot_memb;
    }
    
    return slot;
}

void* cute_hashmap_get(cute_hashmap_t* hashmap, void* k)
{
    if (NULL == hashmap)
        CUTEVM_PANIC("cute_hashmap_get: hashmap is NULL");
   
    if (NULL == k)
        CUTEVM_PANIC("cute_hashmap_get: key is NULL");
   
    u32 hash = cute_murmur3_32_hash(k);
    u32 index = hash % hashmap->cap;
    cute_hashmap_slot_t* slot = &hashmap->slots[index];

    // TODO: разобраться, почему постоянно вылетает эта ошибка
    if (NULL == slot->v)
        CUTEVM_PANIC("cute_hashmap_get: value by key \"%s\" doesn't exists", (char*)k);

    size_t slot_k_size = hashmap->k_size(slot->k);
    size_t k_size = hashmap->k_size(k);

    while (NULL != slot && NULL != slot->v && slot_k_size == k_size && memcmp(slot->k, k, k_size))
    {
        slot_k_size = hashmap->k_size(slot->k);
        k_size = hashmap->k_size(k);
        slot = slot->next;
    }

    if (NULL == slot || NULL == slot->v)
        CUTEVM_PANIC("cute_hashmap_get: ! value by key \"%s\" doesn't exists", (char*)k);

    return slot->v;
}

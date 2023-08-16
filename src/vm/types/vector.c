#include "include/vm/types/vector.h"


cute_vector_t* cute_vector_new(size_t elem_size)
{
    if (elem_size == 0)
        CUTEVM_PANIC("element size must be > then 0");

    cute_vector_t* vec = malloc(sizeof(cute_vector_t));
    if (NULL == vec)
        CUTEVM_PANIC("Unable to allocate vector");

    vec->elem_size = elem_size;
    vec->vec = NULL;
    vec->cap = 0;
    vec->size = 0;

    return vec;
}

void cute_vector_free(cute_vector_t* vec)
{
    if (NULL == vec)
        CUTEVM_PANIC("vec is NULL");

    if (NULL != vec->vec)
        free(vec->vec);

    vec->elem_size = 0;
    vec->size = 0;
    vec->cap = 0;

    free(vec);
}

void cute_vector_push(cute_vector_t* vec, void* v)
{
    if (NULL == vec)
        CUTEVM_PANIC("vec is NULL");

    if (NULL == v)
        CUTEVM_PANIC("value is NULL");

    if (vec->size == 0)
    {
        vec->cap = 1;
        vec->vec = malloc(vec->cap * vec->elem_size);
    }
    
    if (vec->size == vec->cap)
    {
        vec->cap *= 2;
        vec->vec = realloc(vec->vec, vec->cap * vec->elem_size);
    }

    memcpy(&vec->vec[vec->size * vec->elem_size], v, vec->elem_size);
    ++vec->size;
}

void* cute_vector_pop(cute_vector_t* vec)
{
    if (NULL == vec)
        CUTEVM_PANIC("vec is NULL");
    
    if (vec->size == 0)
        CUTEVM_PANIC("vector is empty");

    return &vec->vec[--vec->size * vec->elem_size];
}

void* cute_vector_get(cute_vector_t* vec, size_t index)
{
    if (NULL == vec)
        CUTEVM_PANIC("vec is NULL");
   
    if (index >= vec->size)
        CUTEVM_PANIC("invalid index '%zu'", index);

    return &vec->vec[(vec->size - 1) * vec->elem_size];
}

void cute_vector_reserve(cute_vector_t* vec, size_t elem_count)
{
    if (NULL == vec)
        CUTEVM_PANIC("vec is NULL");
  
    if (elem_count == 0)
        CUTEVM_PANIC("requested elements count is 0");

    if (NULL == vec->vec)
    {
        vec->cap = elem_count;
        vec->vec = malloc(vec->cap * vec->elem_size);
        return;
    }

    vec->cap += elem_count;
    vec->vec = realloc(vec->vec, vec->cap * vec->elem_size);
}

void cute_vector_shrink(cute_vector_t* vec)
{
    if (NULL == vec)
        CUTEVM_PANIC("vec is NULL");
    
    if (NULL == vec->vec || vec->size == vec->cap)
        return;

    vec->cap = vec->size;
    vec->vec = realloc(vec->vec, vec->cap * vec->elem_size);
}

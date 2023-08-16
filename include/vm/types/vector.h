#ifndef CUTE_VM_TYPES_VECTOR_H
#define CUTE_VM_TYPES_VECTOR_H

#include <stddef.h>
#include <malloc.h>
#include <string.h>

#include "include/vm/object.h"
#include "include/vm/logging.h"


typedef struct 
{
    void* vec;
    size_t elem_size;
    size_t cap;
    size_t size;
} cute_vector_t;

// Allocates the new vector with specific element size
cute_vector_t* cute_vector_new(size_t elem_size);
// Frees the vector
void cute_vector_free(cute_vector_t* vec);

// Pushes the 'v' to the top of the vector
void cute_vector_push(cute_vector_t* vec, void* v);
// Removes the last element and returns it
void* cute_vector_pop(cute_vector_t* vec);
// Retunrs an element by a specific index
void* cute_vector_get(cute_vector_t* vec, size_t index);

// Reserves capacity for a specific elements count
void cute_vector_reserve(cute_vector_t* vec, size_t elem_count);
// Frees the unused capacity
void cute_vector_shrink(cute_vector_t* vec);


#endif // CUTE_VM_TYPES_VECTOR_H

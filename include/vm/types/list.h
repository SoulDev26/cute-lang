#ifndef CUTE_VM_TYPES_LIST_H
#define CUTE_VM_TYPES_LIST_H

#include <malloc.h>
#include <string.h>
#include <stddef.h>

#include "include/vm/logging.h"


typedef struct
{
    size_t elem_size;
    size_t size;
    void* list;
} cute_list_t;

cute_list_t* cute_list_new(size_t elem_size, size_t size);
void cute_list_free(cute_list_t* list);

void* cute_list_get(cute_list_t* list, size_t index);
void cute_list_set(cute_list_t* list, size_t index, void* v);


#endif // CUTE_VM_TYPES_LIST_H

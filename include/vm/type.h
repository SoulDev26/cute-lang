#ifndef CUTE_VM_TYPE_H
#define CUTE_VM_TYPE_H

#include <stddef.h>

#include "trait.h"
#include "object.h"
#include "logging.h"
#include "types/list.h"

typedef struct
{
    char* name;
    cute_list_t* fields; // cute_type_field_t[]
    cute_list_t* traits; // cute_trait_t[]
} cute_type_t;

typedef struct
{
    char* name;
    cute_object_type_t type;
} cute_type_field_t;


#endif // CUTE_VM_TYPE_H

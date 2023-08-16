#ifndef CUTE_VM_TRAIT_H
#define CUTE_VM_TRAIT_H

#include <stddef.h>

#include "types/list.h"


typedef struct
{
    char* name;
    cute_list_t* signatures; // cute_fn_signature_t[]
} cute_trait_t;


#endif // CUTE_VM_TRAIT_H

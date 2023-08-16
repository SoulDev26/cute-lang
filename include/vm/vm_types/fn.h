#ifndef CUTE_VM_TYPES_FN_H
#define CUTE_VM_TYPES_FN_H

#include <string.h>
#include <stdio.h>

#include "include/vm/vm.h"
#include "include/vm/logging.h"
#include "include/vm/vm_types/unknown.h"
#include "include/int_types.h"

#define CUTEVM_FN_STR_SIZE 1 + 8 + 1 + 18 + 1 + 4 + 1 + 20 + 1 + 1 // <function 'fn_ptr' size='size'> 


typedef struct
{
    cute_object_type_t *arg_types;
    size_t arg_types_count;
    cute_object_type_t ret_type;
} cute_fn_signature_t;

typedef struct 
{
    u32 size;
    byte* bytecode;
    cute_fn_signature_t signature;
} cute_fn_t;

void cute_parse_fn_type(cute_fn_t* fn, cute_vm_t *vm);
void cute_call_fn(cute_vm_t* vm, cute_fn_t* fn);

char* cute_fn_to_cstr(cute_fn_t* fn);

cute_fn_t* cute_take_fn(cute_object_t* obj);


#endif // CUTE_VM_TYPES_FN_H

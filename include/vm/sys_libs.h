#ifndef CUTE_VM_EXTRA_LIB_LOADER_H
#define CUTE_VM_EXTRA_LIB_LOADER_H

#include <dlfcn.h>

#include "vm.h"
#include "include/vm/ops.h"
#include "include/vm/types/hashmap.h"


typedef struct 
{
    char* real_name;
    char* vm_name;
} cute_sys_lib_fn_name_t;

typedef cute_sys_lib_fn_name_t* (*cute_get_sys_lib_fn_names_t)();

void cute_load_sys_lib(cute_vm_t* vm, const char* path);


#endif // CUTE_VM_EXTRA_LIB_LOADER_H

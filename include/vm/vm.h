#ifndef CUTE_VM_VM_H
#define CUTE_VM_VM_H

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

#include "logging.h"
#include "object.h"
#include "type.h"

#define CUTEVM_SYS_FUNCS_CAP 256
#define CUTEVM_STACK_SIZE 1024


typedef struct
{
    byte* bytecode;
    size_t bytecode_size;

    cute_object_t** static_field;
    u32 static_field_size;

    cute_object_t** stack;
    size_t stack_size;

    u64 ip;
    u64 sp;

    struct 
    {
        bool equal;
        bool less;
        bool greater;
    } flags;

    cute_hashmap_t* vtables; // hashmap contains vtables (aka hashmap :-) )

    void** extra_lib_handlers;
    size_t extra_lib_count;
    size_t extra_lib_cap;

    cute_hashmap_t* sys_funcs_map;
} cute_vm_t;

cute_vm_t* cute_spawn_vm(byte* bytecode, size_t bytecode_size);
void cute_reset_vm(cute_vm_t* vm);
void cute_destroy_vm(cute_vm_t* vm);

void cute_run_vm(cute_vm_t* vm);

void cute_read_code_bytes(cute_vm_t* vm, byte* dest, size_t size);
char* cute_read_code_string(cute_vm_t* vm);
char* cute_check_code_string(cute_vm_t* vm);
byte* cute_check_code_bytes(cute_vm_t* vm, size_t size);

void cute_stack_push(cute_vm_t* vm, cute_object_t* obj);
cute_object_t* cute_stack_pop(cute_vm_t* vm);

void cute_static_field_set(cute_vm_t* vm, u32 index, cute_object_t* obj);
cute_object_t* cute_static_field_get(cute_vm_t* vm, u32 index);


#endif // CUTE_VM_VM_H

#include "include/vm/vm.h"
#include "include/vm/ops.h"
#include "include/vm/sys_libs.h"
#include "include/vm/vm_types/numeric.h"


cute_vm_t* cute_spawn_vm(byte* bytecode, size_t bytecode_size)
{
    CUTEVM_INFO("Spawning new VM");

    if (NULL == bytecode)
    {
        CUTEVM_PANIC("cute_spawn_vm: bytecode pointer is NULL");
    }

    cute_vm_t* vm = malloc(sizeof(cute_vm_t));
    if (NULL == vm)
    {
        CUTEVM_PANIC("cute_spawn_vm: unable to allocate memory for vm");
    }

    vm->bytecode = bytecode;
    vm->bytecode_size = bytecode_size;
   
    vm->ip = sizeof(u32); // skips the static_field_size
    vm->sp = 0;

    // Stack
    vm->stack_size = CUTEVM_STACK_SIZE;
    vm->stack = calloc(vm->stack_size, sizeof(cute_object_t*));
    if (NULL == vm->stack)
    {
        CUTEVM_PANIC("cute_spawn_vm: unable to allocate memory for stack");
    }

    // Static field
    vm->static_field_size = *(u32*)bytecode;
    vm->static_field = calloc(vm->static_field_size, sizeof(cute_object_t*));
    if (NULL == vm->static_field)
    {
        CUTEVM_PANIC("cute_spawn_vm: unable to allocate memory for static field");
    }


    // VTables hashmap
    vm->vtables = cute_hashmap_new(cute_hashmap_cstr_size, cute_hashmap_ptr_size);

    // Extra libs and sys function map
    vm->extra_lib_count = 0;
    vm->extra_lib_cap = 0;
    vm->extra_lib_handlers = NULL;

    vm->sys_funcs_map = cute_hashmap_new(cute_hashmap_cstr_size, cute_hashmap_ptr_size);
    cute_load_sys_lib(vm, "./libstd.so");

    return vm;
}

void cute_destroy_vm(cute_vm_t* vm)
{
    if (NULL == vm)
        CUTEVM_PANIC("cute_destroy_vm: vm is NULL");

    free(vm->bytecode);
    cute_hashmap_free(vm->sys_funcs_map);

    for (size_t i = 0; i <= vm->sp; ++i)
        cute_dec_object_ref_count(vm->stack[i]);
    free(vm->stack);

    for (size_t i = 0; i <= vm->static_field_size; ++i)
    {
        if (NULL == vm->static_field[i]) continue;

        cute_dec_object_ref_count(vm->static_field[i]);
    }
    free(vm->static_field);

    free(vm);
}


void cute_run_vm(cute_vm_t* vm)
{
    CUTEVM_INFO("Execution start");

    while (vm->ip < vm->bytecode_size)
    {
        byte opcode = vm->bytecode[vm->ip];

        if (opcode > CUTE_EXIT_OP)
        {
            CUTEVM_PANIC("CuteVM: unknown operation %us", opcode);
        }

        CUTEVM_DBG("exec %s | ip=%lu", cute_Op_names[opcode], vm->ip);

        switch (opcode)
        {
        case CUTE_NOP_OP:
            vm->ip++;
            break;

        case CUTE_EXIT_OP:
            CUTEVM_INFO("Exit");
            return;
        
        default:
            vm->ip++;
            cute_Ops[opcode](vm);

            break;
        }
    }
}

void cute_read_code_bytes(cute_vm_t* vm, byte* dest, size_t size)
{
    if (NULL == dest)
    {
        CUTEVM_PANIC("cute_read_code_bytes: dest pointer is NULL");
    }
    if (vm->ip + size > vm->bytecode_size)
    {
        CUTEVM_PANIC("cute_read_code_bytes: not enough bytes left to read");
    }

    memcpy(dest, &vm->bytecode[vm->ip], size);
    vm->ip += size;
}

char* cute_read_code_string(cute_vm_t* vm)
{
    if (vm->ip + 1 == vm->bytecode_size)
    {
        CUTEVM_PANIC("cute_read_code_string: not enough bytes left to read");
    }

    size_t len = strlen((char*)&vm->bytecode[vm->ip]) + 1;
    char* str = malloc(len);
    if (NULL == str)
    {
        CUTEVM_PANIC("cute_read_code_string: unable to allocate memory for string");
    }

    strncpy(str, (char*)&vm->bytecode[vm->ip], len - 1);
    vm->ip += len;

    return str;
}

char* cute_check_code_string(cute_vm_t* vm)
{
    if (vm->ip + 1 == vm->bytecode_size)
    {
        CUTEVM_PANIC("cute_check_code_string: not enough bytes left");
    }

    char* str = (char*)&vm->bytecode[vm->ip];
    size_t len = strlen(str);

    vm->ip += len + 1;
    return str;
}

byte* cute_check_code_bytes(cute_vm_t* vm, size_t size)
{
    if (vm->ip + size > vm->bytecode_size)
        CUTEVM_PANIC("not enough bytes left");

    byte* data = &vm->bytecode[vm->ip];
    vm->ip += size;

    return data;
}

void cute_stack_push(cute_vm_t* vm, cute_object_t* obj)
{
    if (vm->sp + 1 == vm->stack_size)
    {
        CUTEVM_WARN("cute_stack_push: reallocate stack");

        vm->stack_size *= 2;
        vm->stack = realloc(vm->stack, vm->stack_size);
    }

    vm->stack[vm->sp++] = obj;
    CUTEVM_DBG("Stack pushed type `%s`", cute_Type_names[obj->type]);
    //CUTEVM_DBG("Stack pushed %s [%p]", cute_stringify_object(obj), (void*)obj);
}

cute_object_t* cute_stack_pop(cute_vm_t* vm)
{
    if (vm->sp == 0)
    {
        CUTEVM_PANIC("cute_stack_pop: stack is empty");
    }

    return vm->stack[--vm->sp];
}

void cute_static_field_set(cute_vm_t* vm, u32 index, cute_object_t* obj)
{
    if (NULL == obj)
    {
        CUTEVM_PANIC("cute_static_field_set: object is NULL");
    }

    if (index >= vm->static_field_size)
    {
        CUTEVM_PANIC("cute_static_field_set: invalid index %u", index);
    }

    obj->ref_count++;
    vm->static_field[index] = obj;
}

cute_object_t* cute_static_field_get(cute_vm_t* vm, u32 index)
{
    if (index >= vm->static_field_size)
    {
        CUTEVM_PANIC("cute_static_field_get: invalid index %u", index);
    }
    
    return vm->static_field[index];
}

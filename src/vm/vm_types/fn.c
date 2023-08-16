#include "include/vm/vm_types/fn.h"


void cute_parse_fn_type(cute_fn_t* fn, cute_vm_t *vm)
{
    cute_read_code_bytes(vm, (byte*)&fn->signature.ret_type, sizeof(cute_object_type_t));
    cute_read_code_bytes(vm, (byte*)&fn->signature.arg_types_count, sizeof(size_t));
    fn->signature.arg_types = (cute_object_type_t*)cute_check_code_bytes(vm, fn->signature.arg_types_count);

    cute_read_code_bytes(vm, (byte*)&fn->size, sizeof(u32));
    if (vm->ip + fn->size > vm->bytecode_size)
    {
        CUTEVM_PANIC("cute_parse_fn_type: invalid fn size");
    }

    fn->bytecode = &vm->bytecode[vm->ip];
    vm->ip += fn->size;
}

void cute_call_fn(cute_vm_t* vm, cute_fn_t* fn)
{
    if (NULL == fn)
        CUTEVM_PANIC("cute_call_fn: function is NULL");
    
    if (NULL == fn->bytecode)
        CUTEVM_PANIC("cute_call_fn: function bytecode is NULL");


    u32 ip = vm->ip;
    byte* bytecode = vm->bytecode;
    size_t bytecode_size = vm->bytecode_size;
    
    vm->ip = 0;
    vm->bytecode = fn->bytecode;
    vm->bytecode_size = fn->size;

    cute_run_vm(vm);

    vm->ip = ip;
    vm->bytecode = bytecode;
    vm->bytecode_size = bytecode_size;
}

char* cute_fn_to_cstr(cute_fn_t* fn)
{
    char* str = malloc(CUTEVM_FN_STR_SIZE);

    sprintf(str, "<function %p size=%u>", (void*)fn, fn->size);

    return str;
}

cute_fn_t* cute_take_fn(cute_object_t* obj)
{
    return (cute_fn_t*)cute_take_unknown(obj);
}

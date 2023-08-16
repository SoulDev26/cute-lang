#include "include/vm/ops.h"
#include "include/vm/object.h"
#include "include/vm/vm_types/fn.h"
#include <stdio.h>


char cute_Op_names[CUTE_EXIT_OP + 1][CUTE_OP_NAME_SIZE] = {
    [CUTE_NOP_OP] = "nop",
    [CUTE_EXIT_OP] = "exit",

    [CUTE_NEW_OP] = "new",
    [CUTE_PUSH_OP] = "push",
    [CUTE_POP_OP] = "pop",
    [CUTE_CALL_OP] = "call",
    [CUTE_SCALL_OP] = "scall",
    [CUTE_SYSCALL_OP] = "syscall",
    [CUTE_DROP_OP] = "drop",
    [CUTE_SDROP_OP] = "sdrop",

    //[CUTE_ADD_OP] = "add",
    //[CUTE_SUB_OP] = "sub",
    //[CUTE_MUL_OP] = "mul",
    //[CUTE_DIV_OP] = "div",
    //[CUTE_MOD_OP] = "mod",

    [CUTE_CMP_OP] = "cmp",
    [CUTE_JE_OP] = "je",
    [CUTE_JNE_OP] = "jne",
    [CUTE_JL_OP] = "jl",
    [CUTE_JLE_OP] = "jle",
    [CUTE_JG_OP] = "jg",
    [CUTE_JGE_OP] = "jge",
    [CUTE_JMP_OP] = "jmp",

    [CUTE_AND_OP] = "and",
    [CUTE_OR_OP] = "or",
    [CUTE_XOR_OP] = "xor",
    [CUTE_NOT_OP] = "not",
    [CUTE_LSHIFT_OP] = "lshift",
    [CUTE_RSHIFT_OP] = "rshift",

    [CUTE_CALL_METHOD_OP] = "call_method",
    [CUTE_CREATE_VTABLE_OP] = "create_vtable",
    [CUTE_LINK_VTABLE_OP] = "link_vtable",
    [CUTE_ADD_METHOD_OP] = "add_method"
};

void (*cute_Ops[CUTE_EXIT_OP + 1])(cute_vm_t*) = {
    [CUTE_NOP_OP] = NULL,
    [CUTE_EXIT_OP] = NULL,

    [CUTE_NEW_OP] = cute_new_op,
    [CUTE_PUSH_OP] = cute_push_op,
    [CUTE_POP_OP] = cute_pop_op,
    [CUTE_CALL_OP] = cute_call_op,
    [CUTE_SCALL_OP] = cute_scall_op,
    [CUTE_SYSCALL_OP] = cute_syscall_op,
    [CUTE_DROP_OP] = cute_drop_op,
    [CUTE_SDROP_OP] = cute_sdrop_op,

    //[CUTE_ADD_OP] = cute_add_op,
    //[CUTE_SUB_OP] = cute_sub_op,
    //[CUTE_MUL_OP] = cute_mul_op,
    //[CUTE_DIV_OP] = cute_div_op,
    //[CUTE_MOD_OP] = cute_mod_op

    [CUTE_CMP_OP] = cute_cmp_op,
    [CUTE_JE_OP] = cute_je_op,
    [CUTE_JNE_OP] = cute_jne_op,
    [CUTE_JL_OP] = cute_jl_op,
    [CUTE_JLE_OP] = cute_jle_op,
    [CUTE_JG_OP] = cute_jg_op,
    [CUTE_JGE_OP] = cute_jge_op,
    [CUTE_JMP_OP] = cute_jmp_op,

    [CUTE_AND_OP] = cute_and_op,
    [CUTE_OR_OP] = cute_or_op,
    [CUTE_XOR_OP] = cute_xor_op,
    [CUTE_NOT_OP] = cute_not_op,
    [CUTE_LSHIFT_OP] = cute_lshift_op,
    [CUTE_RSHIFT_OP] = cute_rshift_op,

    [CUTE_CALL_METHOD_OP] = cute_call_method_op,
    [CUTE_CREATE_VTABLE_OP] = cute_create_vtable_op,
    [CUTE_LINK_VTABLE_OP] = cute_link_vtable_op,
    [CUTE_ADD_METHOD_OP] = cute_add_method_op,

};

char cute_Math_method_names[CUTE_MATH_POW + 1][CUTE_MATH_METHOD_NAME_SIZE] = {
    [CUTE_MATH_ADD] = "@add",
    [CUTE_MATH_SUB] = "@sub",
    [CUTE_MATH_MUL] = "@mul",
    [CUTE_MATH_DIV] = "@div",
    [CUTE_MATH_MOD] = "@mod",
    [CUTE_MATH_POW] = "@pow"
};


void cute_new_op(cute_vm_t* vm)
{
    cute_object_t *obj;
    char* obj_name;
    cute_object_type_t obj_type;

    obj_name = cute_check_code_string(vm);
    cute_read_code_bytes(vm, (byte*)&obj_type, 2);

    obj = cute_alloc_object(obj_type, obj_name);

    CUTEVM_DBG("calling `%s` parser", cute_Type_names[obj_type]);
    
    byte* data = (byte*)obj + sizeof(cute_object_t);

    cute_Type_parsers[obj_type](data, vm);

    cute_stack_push(vm, obj);
}

void cute_push_op(cute_vm_t* vm)
{
    u32 index;
    cute_read_code_bytes(vm, (byte*)&index, sizeof(u32));

    cute_object_t* obj = cute_static_field_get(vm, index);

    cute_stack_push(vm, obj);
}

void cute_pop_op(cute_vm_t* vm)
{
    u32 index;
    cute_read_code_bytes(vm, (byte*)&index, sizeof(u32));
    
    cute_object_t* obj = cute_stack_pop(vm);
    
    cute_static_field_set(vm, index, obj);
}

void cute_call_op(cute_vm_t* vm)
{
    cute_object_t* obj = cute_stack_pop(vm);
    if (obj->type != CUTE_FN_TYPE)
    {
        CUTEVM_PANIC("cute_call_op: object[%p] is not a function", (void*)obj);
    }

    cute_fn_t* fn = (cute_fn_t*)((byte*)obj + sizeof(cute_object_t));
    cute_call_fn(vm, fn);
}

void cute_scall_op(cute_vm_t* vm)
{
    u32 index;
    cute_read_code_bytes(vm, (byte*)&index, sizeof(u32));

    cute_object_t* obj = cute_static_field_get(vm, index);
    if (obj->type != CUTE_FN_TYPE)
    {
        CUTEVM_PANIC("cute_scall_op: object[%p] is not a function", (void*)obj);
    }

    cute_fn_t* fn = (cute_fn_t*)((byte*)obj + sizeof(cute_object_t));
    cute_call_fn(vm, fn);
}

void cute_syscall_op(cute_vm_t* vm)
{
    char* sys_fn_name = cute_check_code_string(vm);
    cute_sys_func_t sys_func = *(cute_sys_func_t*)cute_hashmap_get(vm->sys_funcs_map, sys_fn_name);

    CUTEVM_DBG("cute_syscall_op: calling sys func '%s'", sys_fn_name);

    sys_func(vm);
}


void cute_drop_op(cute_vm_t* vm)
{
    cute_object_t* obj = cute_stack_pop(vm);
    cute_dec_object_ref_count(obj);
}

void cute_sdrop_op(cute_vm_t* vm)
{
    u32 index;
    cute_read_code_bytes(vm, (byte*)&index, sizeof(u32));

    cute_object_t* obj = cute_static_field_get(vm, index);
    cute_dec_object_ref_count(obj);
}

//void cute_add_op(cute_vm_t* vm)
//{
//    cute_object_t* obj1 = cute_stack_pop(vm);
//    cute_object_t* obj2 = cute_stack_pop(vm);
//    cute_object_t* res_obj = cute_alloc_object(obj1->type, NULL);
//
//    void* n1 = cute_take_unknown(obj1);
//    void* n2 = cute_take_unknown(obj2);
//    void* res = cute_take_unknown(res_obj);
//
//    if (obj1->type != obj2->type)
//    {
//        CUTEVM_PANIC("cute_add_op: object types are not the same")
//    }
//
//    if (cute_check_object_type(obj1, CUTE_INT_TYPE | CUTE_UINT_TYPE))
//    {
//        *(u64*)res = *(u64*)n1 + *(u64*)n2;
//    }
//    else if (obj1->type == CUTE_FLOAT_TYPE)
//    {
//        *(double*)res = *(double*)n1 + *(double*)n2;
//    }
//    else
//    {
//
//    }
//
//    cute_dec_object_ref_count(obj1);
//    cute_dec_object_ref_count(obj2);
//    cute_stack_push(vm, res_obj);
//}
//
//void cute_sub_op(cute_vm_t* vm)
//{
//    cute_object_t* obj1 = cute_stack_pop(vm);
//    cute_object_t* obj2 = cute_stack_pop(vm);
//    cute_object_t* res_obj = cute_alloc_object(obj1->type, NULL);
//
//    void* n1 = cute_take_unknown(obj1);
//    void* n2 = cute_take_unknown(obj2);
//    void* res = cute_take_unknown(res_obj);
//
//    if (obj1->type != obj2->type)
//    {
//        CUTEVM_PANIC("cute_sub_op: object types are not the same")
//    }
//
//    if (cute_check_object_type(obj1, CUTE_INT_TYPE | CUTE_UINT_TYPE))
//        *(u64*)res = *(u64*)n2 - *(u64*)n1;
//    else if (obj1->type == CUTE_FLOAT_TYPE)
//        *(double*)res = *(double*)n2 - *(double*)n1;
//    else 
//    {
//        CUTEVM_PANIC("cute_sub_op: object types are not numeric");
//    }
//
//    cute_stack_push(vm, obj2);
//    cute_dec_object_ref_count(obj1);
//}
//
//void cute_mul_op(cute_vm_t* vm)
//{
//    cute_object_t* obj1 = cute_stack_pop(vm);
//    cute_object_t* obj2 = cute_stack_pop(vm);
//    cute_object_t* res_obj = cute_alloc_object(obj1->type, NULL);
//
//    void* n1 = cute_take_unknown(obj1);
//    void* n2 = cute_take_unknown(obj2);
//    void* res = cute_take_unknown(res_obj);
//
//    if (obj1->type != obj2->type)
//    {
//        CUTEVM_PANIC("cute_mul_op: object types are not the same")
//    }
//
//    if (cute_check_object_type(obj1, CUTE_INT_TYPE | CUTE_UINT_TYPE))
//        *(u64*)res = *(u64*)n2 * *(u64*)n1;
//    else if (obj1->type == CUTE_FLOAT_TYPE)
//        *(double*)res = *(u64*)n2 * *(u64*)n1;
//    else 
//        CUTEVM_PANIC("cute_mul_op: object types are not numeric");
//
//    cute_stack_push(vm, obj2);
//    cute_dec_object_ref_count(obj1);
//}
//
//void cute_div_op(cute_vm_t* vm)
//{
//    cute_object_t* obj1 = cute_stack_pop(vm);
//    cute_object_t* obj2 = cute_stack_pop(vm);
//    cute_object_t* res_obj = cute_alloc_object(obj1->type, NULL);
//
//    void* n1 = cute_take_unknown(obj1);
//    void* n2 = cute_take_unknown(obj2);
//    void* res = cute_take_unknown(res_obj);
//
//    if (obj1->type != obj2->type)
//    {
//        CUTEVM_PANIC("cute_div_op: object types are not the same")
//    }
//
//    if (cute_check_object_type(obj1, CUTE_INT_TYPE | CUTE_UINT_TYPE))
//        *(u64*)res = *(u64*)n2 / *(u64*)n1;
//    else if (obj1->type == CUTE_FLOAT_TYPE)
//        *(double*)res = *(double*)n2 / *(double*)n1;
//    else 
//        CUTEVM_PANIC("cute_div_op: object types are not numeric");
//
//    cute_stack_push(vm, obj2);
//    cute_dec_object_ref_count(obj1);
//}
//
//void cute_mod_op(cute_vm_t* vm)
//{
//    cute_object_t* obj1 = cute_stack_pop(vm);
//    cute_object_t* obj2 = cute_stack_pop(vm);
//    cute_object_t* res_obj = cute_alloc_object(obj1->type, NULL);
//
//    void* n1 = cute_take_unknown(obj1);
//    void* n2 = cute_take_unknown(obj2);
//    void* res = cute_take_unknown(res_obj);
//
//    if (obj1->type != obj2->type)
//    {
//        CUTEVM_PANIC("cute_mod_op: object types are not the same")
//    }
//
//    if (cute_check_object_type(obj1, CUTE_INT_TYPE | CUTE_UINT_TYPE))
//        *(u64*)res = *(u64*)n2 % *(u64*)n1;
//    else if (obj1->type == CUTE_FLOAT_TYPE)
//        *(double*)res = fmod(*(double*)n2, *(double*)n1);
//    else 
//        CUTEVM_PANIC("cute_mod_op: object types are not numeric");
//
//    cute_stack_push(vm, obj2);
//    cute_dec_object_ref_count(obj1);
//}
//
//void cute_pow_op(cute_vm_t* vm)
//{
//
//}

//void cute_do_math(cute_vm_t* vm, cute_math_op_type_t math_type)
//{
//    cute_object_t* n1_obj = cute_stack_pop(vm);
//    cute_object_t* n2_obj = cute_stack_pop(vm);
//    cute_object_t* res_obj = cute_alloc_object(n2_obj->type, NULL);
//    void* res = cute_take_uint(res_obj);
//    void* n1 = cute_take_uint(n1_obj);
//    void* n2 = cute_take_uint(n2_obj);
//
//    if (cute_check_object_type(n1_obj, CUTE_INT_TYPE | CUTE_UINT_TYPE)
//        && cute_check_object_type(n2_obj, CUTE_INT_TYPE | CUTE_UINT_TYPE))
//    {
//        switch (math_type)
//        {
//        case CUTE_MATH_ADD:
//            *(u64*)res = *(u64*)n2 + *(u64*)n1;
//            break;
//        case CUTE_MATH_SUB:
//            *(u64*)res = *(u64*)n2 - *(u64*)n1;
//            break;
//        case CUTE_MATH_MUL:
//            *(u64*)res = *(u64*)n2 * *(u64*)n1;
//            break;
//        case CUTE_MATH_DIV:
//            *(u64*)res = *(u64*)n2 / *(u64*)n1;
//            break;
//        case CUTE_MATH_MOD:
//            *(u64*)res = *(u64*)n2 % *(u64*)n1;
//            break;
//        case CUTE_MATH_POW:
//            *(u64*)res = pow(*(u64*)n2, *(u64*)n1);
//            break;
//        }
//    }
//    else if (n1_obj->type == CUTE_FLOAT_TYPE && n2_obj->type == CUTE_FLOAT_TYPE)
//    {
//        switch (math_type)
//        {
//        case CUTE_MATH_ADD:
//            *(double*)res = *(double*)n2 + *(double*)n1;
//            break;
//        case CUTE_MATH_SUB:
//            *(double*)res = *(double*)n2 - *(double*)n1;
//            break;
//        case CUTE_MATH_MUL:
//            *(double*)res = *(double*)n2 * *(double*)n1;
//            break;
//        case CUTE_MATH_DIV:
//            *(double*)res = *(double*)n2 / *(double*)n1;
//            break;
//        case CUTE_MATH_MOD:
//            *(double*)res = fmod(*(double*)n2, *(double*)n1);
//            break;
//        case CUTE_MATH_POW:
//            *(double*)res = pow(*(double*)n2, *(double*)n1);
//            break;
//        }
//    }
//    else
//    {
//        cute_fn_t *fn;
//        
//        HANDLE_PANIC(fn = cute_hashmap_get(n1_obj->vtable, cute_Math_method_names[math_type]););
//        if (IS_PANICED)
//            CUTEVM_PANIC("object %p has not a numeric type or method '%s'", n2_obj, cute_Math_method_names[math_type]);
//
//        cute_stack_push(vm, n2_obj);
//        cute_stack_push(vm, n1_obj);
//        cute_call_fn(vm, fn);
//    }
//}


void cute_cmp_op(cute_vm_t* vm)
{
    cute_object_t* obj2 = cute_stack_pop(vm);
    cute_object_t* obj1 = cute_stack_pop(vm);



    cute_fn_t* equal;
    cute_fn_t* less;
    cute_fn_t* greater;

    HANDLE_PANIC(equal = cute_hashmap_get(obj1->vtable, "@equal"););
    if (IS_PANICED) CUTEVM_PANIC("object '%s' [%p] does not have '@equal' method", obj1->name, obj1);
   
    HANDLE_PANIC(less = cute_hashmap_get(obj1->vtable, "@less"););
    if (IS_PANICED) CUTEVM_PANIC("object '%s' [%p] does not have '@less' method", obj1->name, obj1);
   
    HANDLE_PANIC(greater = cute_hashmap_get(obj1->vtable, "@greater"););
    if (IS_PANICED) CUTEVM_PANIC("object '%s' [%p] does not have '@greater' method", obj1->name, obj1);

    cute_object_t* equal_res_obj;
    cute_object_t* less_res_obj;
    cute_object_t* greater_res_obj;

    // compare methods must not push objects back onto the stack

    obj1->ref_count++;
    obj2->ref_count++;
    cute_stack_push(vm, obj1);
    cute_stack_push(vm, obj2);
    cute_call_fn(vm, equal);
    equal_res_obj = cute_stack_pop(vm);

    obj1->ref_count++;
    obj2->ref_count++;
    cute_stack_push(vm, obj1);
    cute_stack_push(vm, obj2);
    cute_call_fn(vm, less);
    less_res_obj = cute_stack_pop(vm);
    
    obj1->ref_count++;
    obj2->ref_count++;
    cute_stack_push(vm, obj1);
    cute_stack_push(vm, obj2);
    cute_call_fn(vm, greater);
    greater_res_obj = cute_stack_pop(vm);

    if (equal_res_obj->type != CUTE_BOOL_TYPE
            || less_res_obj->type != CUTE_BOOL_TYPE
            || greater_res_obj->type != CUTE_BOOL_TYPE)
        CUTEVM_PANIC("compare methods must return `bool` type");

    vm->flags.equal = *cute_take_bool(equal_res_obj);
    vm->flags.less = *cute_take_bool(less_res_obj);
    vm->flags.greater = *cute_take_bool(greater_res_obj);

    cute_dec_object_ref_count(obj1);
    cute_dec_object_ref_count(obj2);
}

void cute_je_op(cute_vm_t* vm)
{
    if (!vm->flags.equal) return;

    size_t addr;
    cute_read_code_bytes(vm, (byte*)&addr, sizeof(size_t));

    if (addr >= vm->bytecode_size)
        CUTEVM_PANIC("addres(%zu) must be less than bytecode size", addr);

    vm->ip = addr;
}

void cute_jne_op(cute_vm_t* vm)
{
    if (vm->flags.equal) return;

    size_t addr;
    cute_read_code_bytes(vm, (byte*)&addr, sizeof(size_t));

    if (addr >= vm->bytecode_size)
        CUTEVM_PANIC("addres(%zu) must be less than bytecode size", addr);

    vm->ip = addr;
}

void cute_jl_op(cute_vm_t* vm)
{
    if (!vm->flags.less) return;

    size_t addr;
    cute_read_code_bytes(vm, (byte*)&addr, sizeof(size_t));

    if (addr >= vm->bytecode_size)
        CUTEVM_PANIC("addres(%zu) must be less than bytecode size", addr);

    vm->ip = addr;
}

void cute_jle_op(cute_vm_t* vm)
{
    if (vm->flags.less) return;

    size_t addr;
    cute_read_code_bytes(vm, (byte*)&addr, sizeof(size_t));

    if (addr >= vm->bytecode_size)
        CUTEVM_PANIC("addres(%zu) must be less than bytecode size", addr);

    vm->ip = addr;
}

void cute_jg_op(cute_vm_t* vm)
{
    if (!vm->flags.greater) return;

    size_t addr;
    cute_read_code_bytes(vm, (byte*)&addr, sizeof(size_t));

    if (addr >= vm->bytecode_size)
        CUTEVM_PANIC("addres(%zu) must be less than bytecode size", addr);

    vm->ip = addr;
}

void cute_jge_op(cute_vm_t* vm)
{
    if (vm->flags.greater) return;

    size_t addr;
    cute_read_code_bytes(vm, (byte*)&addr, sizeof(size_t));

    if (addr >= vm->bytecode_size)
        CUTEVM_PANIC("addres(%zu) must be less than bytecode size", addr);

    vm->ip = addr;
}

void cute_jmp_op(cute_vm_t* vm)
{
    size_t addr;
    cute_read_code_bytes(vm, (byte*)&addr, sizeof(size_t));

    if (addr >= vm->bytecode_size)
        CUTEVM_PANIC("addres(%zu) must be less than bytecode size", addr);

    vm->ip = addr;
}

void cute_and_op(cute_vm_t* vm)
{

}

void cute_or_op(cute_vm_t* vm)
{

}

void cute_xor_op(cute_vm_t* vm)
{

}

void cute_not_op(cute_vm_t* vm)
{

}

void cute_lshift_op(cute_vm_t* vm)
{

}

void cute_rshift_op(cute_vm_t* vm)
{
    
}


void cute_call_method_op(cute_vm_t* vm)
{
    cute_object_t* obj = cute_stack_pop(vm);
    char* method_name = cute_check_code_string(vm);

    cute_fn_t *method;
    HANDLE_PANIC(method = cute_hashmap_get(obj->vtable, method_name););
    if (IS_PANICED)
        CUTEVM_PANIC("object '%s' [%p] does not have '%s' method", obj->name, obj, method_name);

    cute_stack_push(vm, obj);

    cute_call_fn(vm, method);
}

void cute_create_vtable_op(cute_vm_t* vm)
{
    char* name = cute_check_code_string(vm);

    cute_vtable_t* vtable = cute_hashmap_new(cute_hashmap_cstr_size, cute_hashmap_ptr_size);
    cute_hashmap_insert(vm->vtables, name, vtable);
}

void cute_link_vtable_op(cute_vm_t* vm)
{
    char* vtable_name = cute_check_code_string(vm);
    cute_object_t* obj = cute_stack_pop(vm);

    cute_vtable_t* vtable;
    HANDLE_PANIC(vtable = cute_hashmap_get(vm->vtables, vtable_name););
    if (IS_PANICED)
        CUTEVM_PANIC("vtable '%s' does not exists", vtable_name);

    obj->vtable = vtable;

    cute_stack_push(vm, obj);
}

void cute_add_method_op(cute_vm_t* vm)
{
    char* vtable_name = cute_check_code_string(vm);
    cute_object_t* method_obj = cute_stack_pop(vm);

    if (method_obj->type != CUTE_FN_TYPE)
        CUTEVM_PANIC("object '%s' [%p] does not have `fn` type", method_obj->name, method_obj);

    cute_vtable_t* vtable;
    HANDLE_PANIC(vtable = cute_hashmap_get(vm->vtables, method_obj););
    if (IS_PANICED)
        CUTEVM_PANIC("vtable '%s' does not exists", vtable_name);

    cute_hashmap_insert(vtable, method_obj->name, cute_take_fn(method_obj));
}

#ifndef CUTE_VM_OPS_H
#define CUTE_VM_OPS_H

#include <math.h>

#include "vm.h"
#include "vm_types/numeric.h"
#include "vm_types/unknown.h"
#include "vm_types/bool.h"
#include "types/hashmap.h"

#define CUTE_OP_NAME_SIZE 21
#define CUTE_MATH_METHOD_NAME_SIZE 11


typedef enum 
{
    CUTE_NOP_OP = 0x0,

    CUTE_NEW_OP = 0x1,
    CUTE_PUSH_OP = 0x2,
    CUTE_POP_OP = 0x3,
    CUTE_CALL_OP = 0x4,
    CUTE_SCALL_OP = 0x5,
    CUTE_SYSCALL_OP = 0x6,
    CUTE_DROP_OP = 0x7,
    CUTE_SDROP_OP = 0x8,

    //CUTE_ADD_OP = 0x9,
    //CUTE_SUB_OP = 0xA,
    //CUTE_MUL_OP = 0xB,
    //CUTE_DIV_OP = 0xC,
    //CUTE_MOD_OP = 0xD,

    CUTE_CMP_OP = 0xE,
    CUTE_JE_OP = 0xF,
    CUTE_JNE_OP = 0x10,
    CUTE_JL_OP = 0x11,
    CUTE_JLE_OP = 0x12,
    CUTE_JG_OP = 0x13,
    CUTE_JGE_OP = 0x14,
    CUTE_JMP_OP = 0x15,

    CUTE_AND_OP = 0x16,
    CUTE_OR_OP = 0x17,
    CUTE_XOR_OP = 0x18,
    CUTE_NOT_OP = 0x19,
    CUTE_LSHIFT_OP = 0x1A,
    CUTE_RSHIFT_OP = 0x1B,

    CUTE_CALL_METHOD_OP = 0x1C,
    CUTE_CREATE_VTABLE_OP = 0x1D,
    CUTE_LINK_VTABLE_OP = 0x1E,
    CUTE_ADD_METHOD_OP = 0x1F,

    CUTE_EXIT_OP
} cute_op_t;

typedef enum
{
    CUTE_MATH_ADD,
    CUTE_MATH_SUB,
    CUTE_MATH_MUL,
    CUTE_MATH_DIV,
    CUTE_MATH_MOD,
    CUTE_MATH_POW
} cute_math_op_type_t;

typedef void (*cute_sys_func_t)(cute_vm_t*);


void cute_new_op(cute_vm_t* vm);
void cute_push_op(cute_vm_t* vm);
void cute_pop_op(cute_vm_t* vm);
void cute_call_op(cute_vm_t* vm);
void cute_scall_op(cute_vm_t* vm);
void cute_syscall_op(cute_vm_t* vm);
void cute_drop_op(cute_vm_t* vm);
void cute_sdrop_op(cute_vm_t* vm);

//void cute_add_op(cute_vm_t* vm);
//void cute_sub_op(cute_vm_t* vm);
//void cute_mul_op(cute_vm_t* vm);
//void cute_div_op(cute_vm_t* vm);
//void cute_mod_op(cute_vm_t* vm);
//void cute_pow_op(cute_vm_t* vm);
//void cute_do_math(cute_vm_t* vm, cute_math_op_type_t math_type);

void cute_cmp_op(cute_vm_t* vm);
void cute_je_op(cute_vm_t* vm);
void cute_jne_op(cute_vm_t* vm);
void cute_jl_op(cute_vm_t* vm);
void cute_jle_op(cute_vm_t* vm);
void cute_jg_op(cute_vm_t* vm);
void cute_jge_op(cute_vm_t* vm);
void cute_jmp_op(cute_vm_t* vm);

void cute_and_op(cute_vm_t* vm);
void cute_or_op(cute_vm_t* vm);
void cute_xor_op(cute_vm_t* vm);
void cute_not_op(cute_vm_t* vm);
void cute_lshift_op(cute_vm_t* vm);
void cute_rshift_op(cute_vm_t* vm);

void cute_call_method_op(cute_vm_t* vm);
void cute_create_vtable_op(cute_vm_t* vm);
void cute_link_vtable_op(cute_vm_t* vm);
void cute_add_method_op(cute_vm_t* vm);


extern char cute_Op_names[CUTE_EXIT_OP + 1][CUTE_OP_NAME_SIZE];

extern void (*cute_Ops[CUTE_EXIT_OP + 1])(cute_vm_t*);

extern char cute_Math_method_names[CUTE_MATH_POW + 1][CUTE_MATH_METHOD_NAME_SIZE];


#endif // CUTE_VM_OPS_H

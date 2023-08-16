#include "include/vm_std/math.h"


void cute_std_pow(cute_vm_t *vm)
{
    cute_object_t* exp_obj = cute_stack_pop(vm);
    cute_object_t* base_obj = cute_stack_pop(vm);
    cute_object_t* res_obj = cute_alloc_object(CUTE_FLOAT_TYPE, NULL);

    if (base_obj->type != CUTE_FLOAT_TYPE || exp_obj->type != CUTE_FLOAT_TYPE)
    {
        CUTEVM_PANIC("cute_pow_sys_func: base and exponent must have the float type");
    }

    double* base = cute_take_float(base_obj);
    double* exp = cute_take_float(exp_obj);
    double* res = cute_take_float(res_obj);

    *res = pow(*base, *exp);

    cute_dec_object_ref_count(base_obj);
    cute_dec_object_ref_count(exp_obj);
    cute_stack_push(vm, base_obj);
}

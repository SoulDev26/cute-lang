#include "include/vm_std/nums.h"


void cute_std_to_float(cute_vm_t* vm)
{
    cute_object_t* obj = cute_stack_pop(vm);

    cute_object_t* float_obj = cute_alloc_object(CUTE_FLOAT_TYPE, NULL);

    double* f = cute_take_float(float_obj);
    *f = cute_floatify_object(cute_take_unknown(obj));

    cute_dec_object_ref_count(obj);
    cute_stack_push(vm, float_obj);
}

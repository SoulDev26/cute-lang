#include "include/vm_std/str.h"


void cute_std_to_str(cute_vm_t* vm)
{
    cute_object_t* obj = cute_stack_pop(vm);
    
    char* cstr = cute_stringify_object(cute_take_unknown(obj));

    cute_object_t* str_obj = cute_alloc_object(CUTE_STRING_TYPE, NULL);

    cute_string_t* str = cute_take_string(str_obj);
    str->str_size = strlen(cstr);
    str->str = cstr;

    cute_dec_object_ref_count(obj);
    cute_stack_push(vm, str_obj);
}


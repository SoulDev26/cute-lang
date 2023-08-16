#ifndef CUTE_VM_VM_STD_STD_H
#define CUTE_VM_VM_STD_STD_H

#define CUTEVM_STD_FN_NAME_SIZE 31

#include "include/vm/sys_libs.h"

cute_sys_lib_fn_name_t cute_std_fn_names[] = {
    {"cute_std_print", "std.io.print"},
    {"cute_std_input", "std.io.input"},
    
    {"cute_std_pow", "std.math.pow"},
   
    {"cute_std_to_str", "std.str.to_str"},
    {"cute_std_to_float", "std.nums.to_float"},

    {NULL, NULL}
};

cute_sys_lib_fn_name_t* get_cute_fn_names();


#endif // CUTE_VM_VM_STD_STD_H

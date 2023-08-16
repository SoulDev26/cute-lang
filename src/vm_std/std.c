#include "include/vm_std/std.h"

extern cute_sys_lib_fn_name_t cute_std_fn_names[];

cute_sys_lib_fn_name_t* get_cute_fn_names()
{
    return cute_std_fn_names;
}

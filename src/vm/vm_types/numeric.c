#include "include/vm/vm_types/numeric.h"


void cute_parse_numeric_types(u64* num, cute_vm_t* vm)
{
    cute_read_code_bytes(vm, (byte*)num, sizeof(u64));
    CUTEVM_DBG("parsed %f value", *(double*)num);
}


i64* cute_take_int(cute_object_t* obj)
{
    if (obj->type != CUTE_INT_TYPE)
        CUTEVM_PANIC("cute_take_int: object must have the int type");

    return (i64*)cute_take_unknown(obj);
}

u64* cute_take_uint(cute_object_t* obj)
{
    if (obj->type != CUTE_UINT_TYPE)
        CUTEVM_PANIC("cute_take_uint: object must have the uint type");

    return (u64*)cute_take_unknown(obj);
}

double* cute_take_float(cute_object_t* obj)
{
    if (obj->type != CUTE_FLOAT_TYPE)
        CUTEVM_PANIC("cute_take_float: object must have the float type");

    return (double*)cute_take_unknown(obj);
}


double cute_string_to_float(cute_string_t* str)
{
    return atof(str->str);
}

double cute_cstr_to_float(const char* cstr)
{
    return atof(cstr);
}
double cute_int_to_float(i64* num)
{
    return *num;
}

double cute_uint_to_float(u64* num)
{
    return *num;
}

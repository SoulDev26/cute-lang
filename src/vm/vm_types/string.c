#include "include/vm/vm_types/string.h"


void cute_parse_string_type(cute_string_t* str, cute_vm_t* vm)
{
    char* code_str = cute_check_code_string(vm);
    str->str_size = strlen(code_str);
    
    str->str = malloc(str->str_size + 1);
    strcpy(str->str, code_str);
}

cute_string_t* cute_take_string(cute_object_t* obj)
{
    return (cute_string_t*)cute_take_unknown(obj);
}

char* cute_int_to_cstr(i64* num)
{
    char* str = malloc(CUTEVM_INT_STR_SIZE);
    sprintf(str, "%ld", *num);

    return str;
}

char* cute_uint_to_cstr(u64* num)
{
    char* str = malloc(CUTEVM_UINT_STR_SIZE);
    sprintf(str, "%lu", *num);

    return str;
}

char* cute_float_to_cstr(double* num)
{
    char* str = malloc(CUTEVM_FLOAT_STR_SIZE);
    sprintf(str, "%f", *num);

    return str;
}

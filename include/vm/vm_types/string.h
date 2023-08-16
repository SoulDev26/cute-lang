#ifndef CUTE_VM_TYPES_STRING_H
#define CUTE_VM_TYPES_STRING_H

#include <stddef.h>

#include "include/vm/vm.h"
#include "include/vm/object.h"
#include "include/vm/vm_types/unknown.h"

#define CUTEVM_INT_STR_SIZE 20
#define CUTEVM_UINT_STR_SIZE 20
#define CUTEVM_FLOAT_STR_SIZE 200


typedef struct 
{
    char* str;
    size_t str_size;
} cute_string_t;

extern char* (*cute_Type_stringifiers[CUTE_UNDEFINED_TYPE + 1])(void*);

void cute_parse_string_type(cute_string_t* str, cute_vm_t* vm);

cute_string_t* cute_take_string(cute_object_t* obj);

char* cute_stringify_object(cute_object_t* obj);

char* cute_int_to_cstr(i64* num);
char* cute_uint_to_cstr(u64* num);
char* cute_float_to_cstr(double* num);


#endif // CUTE_VM_TYPES_STRING_H

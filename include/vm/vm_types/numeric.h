#ifndef CUTE_VM_TYPES_NUMERIC_H
#define CUTE_VM_TYPES_NUMERIC_H

#include <stdlib.h>

#include "include/vm/vm.h"
#include "include/vm/logging.h"
#include "include/vm/vm_types/unknown.h"
#include "include/int_types.h"
#include "include/vm/vm_types/string.h"


extern double (*cute_Type_floatifiers[CUTE_UNDEFINED_TYPE + 1])(void*);

void cute_parse_numeric_types(u64* num, cute_vm_t* vm);

i64* cute_take_int(cute_object_t* obj);
u64* cute_take_uint(cute_object_t* obj);
double* cute_take_float(cute_object_t* obj);

double cute_floatify_object(cute_object_t* obj);

double cute_string_to_float(cute_string_t* str);
double cute_cstr_to_float(const char* cstr);
double cute_int_to_float(i64* num);
double cute_uint_to_float(u64* num);


#endif

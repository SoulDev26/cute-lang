#ifndef CUTE_VM_OBJECT_H
#define CUTE_VM_OBJECT_H

#include <stdbool.h>
#include <malloc.h>
#include <math.h>

#include "types/hashmap.h"

#include "include/int_types.h"
#include "logging.h"

#define CUTE_OBJECT_TYPE_NAME_SIZE 21


typedef enum
{
    CUTE_FN_TYPE = 1,
    
    CUTE_INT_TYPE = 2,
    CUTE_UINT_TYPE = 4,
    CUTE_FLOAT_TYPE = 8,
    CUTE_BOOL_TYPE = 16,
   
    CUTE_STRING_TYPE = 32,
    CUTE_VECTOR_TYPE = 64,
    CUTE_LIST_TYPE = 128,
    
    CUTE_UNKNOWN_TYPE = 256,
    CUTE_UNDEFINED_TYPE = 512
} cute_object_type_t;

typedef void (*cute_type_parser_t)(void*,void*);
typedef void (*cute_type_destructor_t)(void*);
//typedef char* (*cute_type_stringifier_t)(void*);
//
//typedef double (*cute_type_floatifier_t)(void*);

typedef cute_hashmap_t cute_vtable_t;

typedef struct 
{
    cute_object_type_t type;
    char* name;
    u32 ref_count;
    cute_vtable_t* vtable;
} cute_object_t;


extern u32 cute_Type_sizes[CUTE_UNDEFINED_TYPE + 1];
extern char cute_Type_names[CUTE_UNDEFINED_TYPE + 1][CUTE_OBJECT_TYPE_NAME_SIZE];

extern void (*cute_Type_parsers[CUTE_UNDEFINED_TYPE + 1])(void*, void*);
extern void (*cute_Type_destructors[CUTE_UNDEFINED_TYPE + 1])(void*);


bool cute_is_type(cute_object_type_t type);
cute_object_t* cute_alloc_object(cute_object_type_t type, const char* name);
void cute_free_object(cute_object_t* obj);

bool cute_check_object_type(cute_object_t* obj, cute_object_type_t type);
void cute_dec_object_ref_count(cute_object_t *obj);


#endif // CUTE_VM_OBJECT_H

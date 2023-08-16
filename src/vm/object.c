#include "include/vm/object.h"

#include "include/vm/vm_types/fn.h"
#include "include/vm/vm_types/numeric.h"
#include "include/vm/vm_types/string.h"
#include "include/vm/vm_types/vector.h"


u32 cute_Type_sizes[CUTE_UNDEFINED_TYPE + 1] = {
    [CUTE_FN_TYPE] = sizeof(cute_fn_t),

    [CUTE_INT_TYPE] = sizeof(i64),
    [CUTE_UINT_TYPE] = sizeof(u64),
    [CUTE_FLOAT_TYPE] = sizeof(double),
    [CUTE_BOOL_TYPE] = 1,

    [CUTE_STRING_TYPE] = sizeof(cute_string_t),
    [CUTE_VECTOR_TYPE] = sizeof(cute_vector_t),

    [CUTE_UNKNOWN_TYPE] = 0,
    [CUTE_UNDEFINED_TYPE] = 0
};

char cute_Type_names[CUTE_UNDEFINED_TYPE + 1][CUTE_OBJECT_TYPE_NAME_SIZE] = {
    [CUTE_FN_TYPE] = "fn",
    
    [CUTE_INT_TYPE] = "int",
    [CUTE_UINT_TYPE] = "uint",
    [CUTE_FLOAT_TYPE] = "float",
    [CUTE_BOOL_TYPE] = "bool",

    [CUTE_STRING_TYPE] = "string",
    [CUTE_VECTOR_TYPE] = "vector",
    [CUTE_LIST_TYPE] = "list",

    [CUTE_UNKNOWN_TYPE] = "unknown",
    [CUTE_UNDEFINED_TYPE] = "undefined"
};

void (*cute_Type_parsers[CUTE_UNDEFINED_TYPE + 1])(void*, void*) = {
    [CUTE_FN_TYPE] = (cute_type_parser_t)cute_parse_fn_type,

    [CUTE_INT_TYPE] = (cute_type_parser_t)cute_parse_numeric_types,
    [CUTE_UINT_TYPE] = (cute_type_parser_t)cute_parse_numeric_types,
    [CUTE_FLOAT_TYPE] = (cute_type_parser_t)cute_parse_numeric_types,
    [CUTE_BOOL_TYPE] = NULL,

    [CUTE_STRING_TYPE] = (cute_type_parser_t)cute_parse_string_type,
    [CUTE_VECTOR_TYPE] = NULL,
    [CUTE_LIST_TYPE] = NULL,

    [CUTE_UNKNOWN_TYPE] = NULL,
    [CUTE_UNDEFINED_TYPE] = NULL
};

void (*cute_Type_destructors[CUTE_UNDEFINED_TYPE + 1])(void*) = {
    [CUTE_FN_TYPE] = NULL,

    [CUTE_INT_TYPE] = NULL,
    [CUTE_UINT_TYPE] = NULL,
    [CUTE_FLOAT_TYPE] = NULL,

    [CUTE_STRING_TYPE] = NULL,
    [CUTE_VECTOR_TYPE] = (cute_type_destructor_t)cute_free_vector_content,
    [CUTE_LIST_TYPE] = NULL,

    [CUTE_UNKNOWN_TYPE] = NULL,
    [CUTE_UNDEFINED_TYPE] = NULL
};

//char* (*cute_Type_stringifiers[CUTE_UNDEFINED_TYPE + 1])(void*) = {
//    [CUTE_FN_TYPE] = (cute_type_stringifier_t)cute_fn_to_cstr,
//
//    [CUTE_INT_TYPE] = (cute_type_stringifier_t)cute_int_to_cstr,
//    [CUTE_UINT_TYPE] = (cute_type_stringifier_t)cute_uint_to_cstr,
//    [CUTE_FLOAT_TYPE] = (cute_type_stringifier_t)cute_float_to_cstr,
//    [CUTE_BOOL_TYPE] = NULL,
//
//    [CUTE_STRING_TYPE] = NULL,
//    [CUTE_VECTOR_TYPE] = NULL,
//    [CUTE_LIST_TYPE] = NULL,
//
//    [CUTE_UNKNOWN_TYPE] = NULL,
//    [CUTE_UNDEFINED_TYPE] = NULL
//};
//
//double (*cute_Type_floatifiers[CUTE_UNDEFINED_TYPE + 1])(void*) = {
//    [CUTE_FN_TYPE] = NULL,
//
//    [CUTE_INT_TYPE] = (cute_type_floatifier_t)cute_int_to_float,
//    [CUTE_UINT_TYPE] = (cute_type_floatifier_t)cute_uint_to_float,
//    [CUTE_FLOAT_TYPE] = NULL,
//    [CUTE_BOOL_TYPE] = NULL,
//
//    [CUTE_STRING_TYPE] = (cute_type_floatifier_t)cute_string_to_float,
//    [CUTE_VECTOR_TYPE] = NULL,
//    [CUTE_LIST_TYPE] = NULL,
//
//    [CUTE_UNKNOWN_TYPE] = NULL,
//    [CUTE_UNDEFINED_TYPE] = NULL
//};


// TODO: remake this
/*
char* cute_stringify_object(cute_object_t* obj)
{
    if (NULL == obj)
        CUTEVM_PANIC("cute_stringify_object: object is NULL");

    if (obj->type == CUTE_STRING_TYPE)
    {
        cute_string_t* string = cute_take_string(obj);

        char* str = malloc(string->str_size + 1);
        strcpy(str, string->str);

        return str;
    }

    cute_type_stringifier_t stringify = cute_Type_stringifiers[obj->type];
    if (NULL == stringify)
        CUTEVM_PANIC("Impossible to stringify type `%s`", cute_Type_names[obj->type]);

    return stringify(cute_take_unknown(obj));
}
*/

bool cute_is_type(cute_object_type_t type)
{
    // Check if type >= CUTE_FN_TYPE 
    // and type is power of 2 ( type & -type == type )
    // and type <= CUTE_UNDEFINED_TYPE (512)
    if (type >= 1 && (type & -type) == type && type <= CUTE_UNDEFINED_TYPE)
    {
        return true;
    }

    return false;
}

cute_object_t* cute_alloc_object(cute_object_type_t type, const char* name)
{
    if (!cute_is_type(type))
    {
        CUTEVM_PANIC("cute_alloc_object: unknown %d type", type);
    }

    cute_object_t* obj = malloc(sizeof(cute_object_t) + cute_Type_sizes[type]);
    if (NULL == obj)
    {
        CUTEVM_PANIC("cute_alloc_object: unable to allocate memory for object with type %d", type);
    }

    obj->name = malloc(strlen(name) + 1);
    obj->type = type;
    obj->ref_count = 1;
    obj->vtable = cute_hashmap_new(cute_hashmap_cstr_size, cute_hashmap_ptr_size);

    if (NULL == name)
        *obj->name = '\0';
    else
        strcpy(obj->name, name);

    CUTEVM_DBG("Created new object '%s' with type `%s` %p", obj->name, cute_Type_names[type], (void*)obj);

    return obj;
}

void cute_free_object(cute_object_t * obj)
{
    if (NULL == obj)
    {
        CUTEVM_PANIC("cute_free_object: obj is NULL");
    }

    if (NULL != cute_Type_destructors[obj->type])
    {
        cute_Type_destructors[obj->type]((byte*)obj + sizeof(cute_object_t));
    }

    free(obj);
}


bool cute_check_object_type(cute_object_t* obj, cute_object_type_t types)
{
    if (NULL == obj)
    {
        CUTEVM_PANIC("cute_check_object_type: obj is NULL");
    }

    return (types & obj->type) == obj->type;
}


void cute_dec_object_ref_count(cute_object_t *obj)
{
    if (NULL == obj)
    {
        CUTEVM_PANIC("cute_dec_object_ref_count: obj is NULL");
    }

    if (obj->ref_count == 1)
    {
        cute_free_object(obj);
    }

    obj->ref_count--;
}

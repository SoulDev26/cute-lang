#include "include/vm/vm_types/vector.h"


void cute_free_vector_content(cute_vector_t* vec)
{
    if (NULL == vec)
        CUTEVM_PANIC("cute_free_vector_content: vector is NULL");

    for (size_t i = 0; i < vec->size; ++i)
        cute_dec_object_ref_count((cute_object_t*)cute_vector_get(vec, i));

    free(vec->vec);
}

cute_vector_t* cute_take_vector(cute_object_t* obj)
{
    return (cute_vector_t*)cute_take_unknown(obj);
}

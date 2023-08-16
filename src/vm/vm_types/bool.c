#include "include/vm/vm_types/bool.h"


bool* cute_take_bool(cute_object_t* obj)
{
    if (obj->type != CUTE_BOOL_TYPE)
        CUTEVM_PANIC("object '%s' [%p] must have a `bool` type", obj->name, obj);

    return (bool*)cute_take_unknown(obj);
}

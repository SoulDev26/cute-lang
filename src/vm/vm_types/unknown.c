#include "include/vm/vm_types/unknown.h"


void* cute_take_unknown(cute_object_t* obj)
{
    return (void*)(obj + 1);
}

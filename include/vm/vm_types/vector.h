#ifndef CUTE_VM_VM_TYPES_VECTOR_H
#define CUTE_VM_VM_TYPES_VECTOR_H

#include <stddef.h>

#include "include/vm/types/vector.h"
#include "include/vm/vm.h"
#include "include/vm/object.h"
#include "include/vm/logging.h"
#include "include/vm/vm_types/unknown.h"


void cute_free_vector_content(cute_vector_t* vec);

cute_vector_t* cute_take_vector(cute_object_t* obj);


#endif // CUTE_VM_VM_TYPES_VECTOR_H

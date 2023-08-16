#ifndef CUTE_VM_VM_STD_IO_H
#define CUTE_VM_VM_STD_IO_H

#include "include/vm/vm.h"
#include "include/vm/vm_types/string.h"

#define CUTE_STD_IO_INPUT_BUF_SIZE 8096


void cute_std_print(cute_vm_t* vm);
void cute_std_input(cute_vm_t* vm);


#endif // CUTE_VM_VM_STD_IO_H

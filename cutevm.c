#include <stdio.h>
#include <string.h>

#include "include/int_types.h"
#include "include/vm/types/hashmap.h"
#include "include/vm/object.h"
#include "include/vm/ops.h"
#include "include/vm/vm_types/numeric.h"

#define CUTEVM_INTERACTIVE_MODE_BYTECODE_SIZE 1024


void cute_print_object(cute_object_t* obj)
{
    CUTEVM_INFO("object = %p", (void*)obj);
    CUTEVM_INFO("name = %s", obj->name);
    CUTEVM_INFO("type = %s", cute_Type_names[obj->type]);
    CUTEVM_INFO("ref_count = %u", obj->ref_count);
   
    void* data = obj + 1;

    switch (obj->type)
    {
    case CUTE_FN_TYPE:
        break;

    case CUTE_INT_TYPE:
        CUTEVM_INFO("value = %ld", *(i64*)data);
        break;
    case CUTE_UINT_TYPE:
        CUTEVM_INFO("vaue = %lu", *(u64*)data);
        break;
    case CUTE_FLOAT_TYPE:
        CUTEVM_INFO("value = %f", *(double*)data);
        break;
    default:
        break;
    }
}

void cute_interactive_mode(byte* bytecode, size_t bytecode_size)
{
    cute_vm_t* vm = cute_spawn_vm(bytecode, bytecode_size);
}

void cute_normal_mode(byte* bytecode, size_t bytecode_size)
{
    cute_vm_t* vm = cute_spawn_vm(bytecode, bytecode_size);
    
    cute_run_vm(vm);
}

int main(int argc, char* argv[]) 
{
    byte* bytecode = NULL;
    size_t bytecode_size = 0;
    FILE* bytecode_fp;

    if (argc < 2)
    {
        bytecode_size = CUTEVM_INTERACTIVE_MODE_BYTECODE_SIZE;
        bytecode = malloc(bytecode_size);

        if (NULL == bytecode)
            CUTEVM_PANIC("unable to allocate memory for bytecode in interactive mode");
        memset(bytecode, CUTE_EXIT_OP, bytecode_size);

        *(u32*)bytecode = 100; // set static_field size

        cute_interactive_mode(bytecode, bytecode_size);
    }
    else 
    {
        bytecode_fp = fopen(argv[1], "rb");
        if (NULL == bytecode_fp)
            CUTEVM_PANIC("unable to open file '%s'", argv[1]);

        fseek(bytecode_fp, 0, SEEK_END);
        bytecode_size = ftell(bytecode_fp);
        fseek(bytecode_fp, 0, SEEK_SET);

        bytecode = malloc(bytecode_size);
        if (NULL == bytecode)
            CUTEVM_PANIC("unable to allocate memory for bytecode");
        memset(bytecode, 0, bytecode_size);

        fread(bytecode, 1, bytecode_size, bytecode_fp);

        cute_normal_mode(bytecode, bytecode_size);
    }

    CUTEVM_WARN("TEST PTR=%p", (void*)UINT64_MAX);

    //byte bytecode[] = {
    //    0,0,0,0,
    //    CUTE_NEW_OP, 'n','1','\0', 8,0, 0,0,0,0,0,0,16,64,
    //    CUTE_NEW_OP, 'n','2','\0', 8,0, 0,0,0,0,0,0,0,64,
    //    CUTE_SYSCALL_OP, 'p','o','w','\0',
    //};

}

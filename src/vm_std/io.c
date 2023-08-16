#include "include/vm_std/io.h"


void cute_std_print(cute_vm_t* vm)
{
    cute_object_t* str_obj = cute_stack_pop(vm);

    if (str_obj->type != CUTE_STRING_TYPE)
        CUTEVM_PANIC("cute_print_sys_func: object type is not `string`");

    cute_string_t* str = cute_take_string(str_obj);
    
    printf("%s", str->str);

    cute_dec_object_ref_count(str_obj);
}

void cute_std_input(cute_vm_t* vm)
{
    static char input_buf[CUTE_STD_IO_INPUT_BUF_SIZE];

    fgets(input_buf, CUTE_STD_IO_INPUT_BUF_SIZE, stdin);

    cute_object_t* obj = cute_alloc_object(CUTE_STRING_TYPE, NULL);

    cute_string_t* str = cute_take_string(obj);
    str->str_size = strlen(input_buf);
    str->str = malloc(str->str_size + 1);
    strcpy(str->str, input_buf);

    // remove '\n' symbol
    str->str[str->str_size - 1] = '\0';

    for (size_t i = 0; i < str->str_size; ++i)
    {
        printf("%c", str->str[i]);
    }

    cute_stack_push(vm, obj);
}

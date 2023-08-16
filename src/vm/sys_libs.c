#include "include/vm/sys_libs.h"


void cute_load_sys_lib(cute_vm_t* vm, const char* path)
{
    if (NULL == vm)
        CUTEVM_PANIC("cute_load_sys_lib: VM is NULL");

    if (NULL == path)
        CUTEVM_PANIC("cute_load_sys_lib: path is NULL");

    if (vm->extra_lib_cap == 0)
    {
        CUTEVM_DBG("cute_load_sys_lib: allocated memory for handlers array");
        vm->extra_lib_handlers = malloc(sizeof(void*));
        vm->extra_lib_cap = 1;
    }

    if (vm->extra_lib_count == vm->extra_lib_cap)
    {
        vm->extra_lib_cap *= 2;
        vm->extra_lib_handlers = realloc(vm->extra_lib_handlers, sizeof(void*) * vm->extra_lib_cap);
    }


    void* handler = dlopen(path, RTLD_LAZY);
    if (NULL == handler)
        CUTEVM_PANIC("cute_load_sys_lib: %s", dlerror());

    vm->extra_lib_handlers[vm->extra_lib_count++] = handler;

    cute_get_sys_lib_fn_names_t get_fn_names = (cute_get_sys_lib_fn_names_t)dlsym(handler, "get_cute_fn_names");
    if (NULL == get_fn_names)
        CUTEVM_PANIC("cute_load_sys_lib: can't get 'get_cute_fn_names' function from library");

    cute_sys_lib_fn_name_t* fn_names = get_fn_names();

    if (NULL == fn_names[0].real_name)
        CUTEVM_WARN("cute_load_sys_lib: library '%s' hasn't any function", path);

    for (size_t i = 0; fn_names[i].real_name != NULL; ++i)
    {
        cute_sys_func_t fn = (cute_sys_func_t)dlsym(handler, fn_names[i].real_name);
        CUTEVM_DBG("fn = %p", (void*)fn);

        if (NULL == fn)
            CUTEVM_PANIC("cute_load_sys_lib: can't get '%s' function from '%s' library", fn_names[i].real_name, path);

        cute_hashmap_insert(vm->sys_funcs_map, fn_names[i].vm_name, &fn);
    }

    CUTEVM_INFO("Loaded '%s' extra library", path);
}

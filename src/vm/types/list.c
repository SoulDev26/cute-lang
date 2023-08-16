#include "include/vm/types/list.h"


cute_list_t* cute_list_new(size_t elem_size, size_t size)
{
    if (elem_size == 0)
        CUTEVM_PANIC("element size must be > then 0");

    cute_list_t* list = malloc(sizeof(cute_list_t));
    if (NULL == list)
        CUTEVM_PANIC("unable to allocate list");

    list->elem_size = elem_size;
    list->size = size;

    list->list = malloc(size * elem_size);
    if (NULL == list->list)
        CUTEVM_PANIC("unable to allocate memory for list");

    return list;
}

void cute_list_free(cute_list_t* list)
{
    if (NULL == list)
        CUTEVM_PANIC("list is NULL");

    if (NULL != list->list)
        free(list->list);

    list->size = 0;
    list->elem_size = 0;

    free(list);
}

void* cute_list_get(cute_list_t* list, size_t index)
{
    if (NULL == list)
        CUTEVM_PANIC("list is NULL");

    if (index >= list->size)
        CUTEVM_PANIC("invalid index %zu", index);

    return list->list + (index * list->elem_size);
}

void cute_list_set(cute_list_t* list, size_t index, void* v)
{
    if (NULL == list)
        CUTEVM_PANIC("list is NULL");

    if (NULL == v)
        CUTEVM_PANIC("value is NULL");

    if (index >= list->size)
        CUTEVM_PANIC("invalid index %zu", index);

    void* place = list->list + (index * list->elem_size);
    memcpy(place, v, list->elem_size);
}

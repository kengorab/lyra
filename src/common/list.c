#include <stdlib.h>
#include <stdbool.h>

#include "list.h"

#define LIST_INIT_CAPACITY 8

List* newList() {
    List* l = malloc(sizeof(List));
    l->count = 0;
    l->capacity = LIST_INIT_CAPACITY;
    l->values = NULL;
    return l;
}

void listAdd(List* list, void* item) {
    if (list->capacity < list->count + 1) {
        list->capacity = list->capacity * 2;
        list->values = realloc(list->values, (size_t) list->capacity * 2);
    }

    list->values[list->count++] = item;
}

void listFree(List* list) {
    int idx = 0;
    while (true) {
        void* item = list->values[idx];
        if (item == NULL)
            break;
        free(item);
        idx++;
    }
    free(list);
}
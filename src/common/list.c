#include <stdlib.h>
#include <stdbool.h>
#include <lexer.h>

#include "list.h"

List* newList() {
    List* l = malloc(sizeof(List));
    l->count = 0;
    l->capacity = LIST_INIT_CAPACITY;
    l->values = malloc(LIST_INIT_CAPACITY * sizeof(void*));
    return l;
}

void listAdd(List* list, void** item) {
    if (list->capacity < list->count + 1) {
        list->capacity = list->capacity * 2;
        list->values = realloc(list->values, (size_t) list->capacity * sizeof(void*) * 2);
    }

    list->values[list->count++] = *item;
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

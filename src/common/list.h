#ifndef CLYRA_LIST_H
#define CLYRA_LIST_H

typedef struct {
    int count;
    int capacity;
    void** values;
} List;

List* newList();

void listAdd(List* list, void* item);

void listFree(List* list);

#endif //CLYRA_LIST_H

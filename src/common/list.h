#ifndef CLYRA_LIST_H
#define CLYRA_LIST_H

#include "lexer.h"

#define LIST_INIT_CAPACITY 8


typedef struct {
    int count;
    int capacity;
    void** values;
} List;

/**
 * Create empty list with initial size LIST_INIT_CAPACITY, whose type is void*.
 * Items inserted into the list should be cast to (void*), and items exiting
 * the list should be cast to their target type.
 */
List* newList();

/**
 * Inserts an item into the list. Note that, in an effort to achieve quasi-generics,
 * the item argument will need to be cast to a (void**).
 */
void listAdd(List* list, void** item);

/**
 * Frees the list, including all members within that list.
 * // TODO: Pass in opt function reference to facilitate cleanup of deeply-nested objects
 */
void listFree(List* list);

#endif //CLYRA_LIST_H

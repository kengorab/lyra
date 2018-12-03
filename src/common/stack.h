#ifndef CLYRA_STACK_H
#define CLYRA_STACK_H

#include <stdbool.h>

#define STACK_FULL -2
#define STACK_EMPTY -1
#define STACK_OK 0
#define STACK_MAX_SIZE 16

typedef struct {
    int top;
    int size;
    void** items;
} lyra_stack_t;

lyra_stack_t* stack_new();

bool stack_is_empty(lyra_stack_t* stack);

int stack_push(lyra_stack_t* stack, void** item);

int stack_pop(lyra_stack_t* stack, void** outItem);

int stack_peek_n(lyra_stack_t* stack, void** outItem, int depth);

int stack_peek(lyra_stack_t* stack, void** outItem);

#endif //CLYRA_STACK_H

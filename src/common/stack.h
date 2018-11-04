#ifndef CLYRA_STACK_H
#define CLYRA_STACK_H

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

int stack_push(lyra_stack_t* stack, void** item);

int stack_pop(lyra_stack_t* stack, void** outItem);

#endif //CLYRA_STACK_H

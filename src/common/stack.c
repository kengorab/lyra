#include <stdlib.h>

#include "stack.h"

lyra_stack_t* stack_new() {
    lyra_stack_t* s = malloc(sizeof(lyra_stack_t));
    s->items = malloc(sizeof(void*) * STACK_MAX_SIZE); // TODO: calloc
    s->top = -1;
    s->size = 0;

    return s;
}

bool stack_is_empty(lyra_stack_t* stack) {
    return stack->size == 0;
}

int stack_push(lyra_stack_t* stack, void** item) {
    if (stack->size + 1 > STACK_MAX_SIZE) {
        return STACK_FULL; // TODO: Should I dynamically increase the size of the stack? Is it worth the time?
    }
    stack->items[++stack->top] = *item;
    stack->size++;
    return STACK_OK;
}

int stack_pop(lyra_stack_t* stack, void** outItem) {
    if (stack->size == 0) return STACK_EMPTY;
    *outItem = stack->items[stack->top--];
    stack->size--;
    return STACK_OK;
}

int stack_peek_n(lyra_stack_t* stack, void** outItem, int depth) {
    if (stack->size == depth) return STACK_EMPTY;
    *outItem = stack->items[stack->top - depth];
    return STACK_OK;
}

int stack_peek(lyra_stack_t* stack, void** outItem) {
    return stack_peek_n(stack, outItem, 0);
}

void stack_free(lyra_stack_t* stack) {
    free(stack->items);
    free(stack);
}

#ifndef CLYRA_VM_H
#define CLYRA_VM_H

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value* stackTop;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM(VM* vm);

void freeVM();

InterpretResult interpret(VM* vm, Chunk* chunk);

void push(VM* vm, Value value);

Value pop(VM* vm);

#endif //CLYRA_VM_H

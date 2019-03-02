#ifndef CLYRA_COMMON_H
#define CLYRA_COMMON_H

#define DEBUG_TRACE_EXECUTION

typedef enum {
    OP_CONSTANT,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_NEGATE,
    OP_RETURN
} Opcode;

// Ignore warning; initialized statically
static const char* opcodes[] = {
    "OP_CONSTANT",
    "OP_ADD",
    "OP_SUB",
    "OP_MUL",
    "OP_DIV",
    "OP_NEGATE",
    "OP_RETURN"
};

#endif //CLYRA_COMMON_H

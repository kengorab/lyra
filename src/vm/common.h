#ifndef CLYRA_COMMON_H
#define CLYRA_COMMON_H

#include "common/enums.h"

#define DEBUG_TRACE_EXECUTION

MAKE_ENUM(
    Opcode,
    opcodes,

    OP_CONSTANT,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_NEGATE,
    OP_RETURN
)

#endif //CLYRA_COMMON_H

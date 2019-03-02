#include <stdio.h>

#include "debug.h"
#include "common.h"

static int simpleInstruction(Opcode opcode, int offset);

static int constantInstruction(Opcode opcode, Chunk* chunk, int offset);

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

int disassembleInstruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);

    bool isStartOfLine;
    int lineNum = getLineForOffset(&chunk->lines, offset, &isStartOfLine);
    if (isStartOfLine) {
        printf("%4d ", lineNum);
    } else {
        printf("   | ");
    }

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_CONSTANT: {
            return constantInstruction(instruction, chunk, offset);
        }
        case OP_NEGATE:
        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
        case OP_DIV:
        case OP_RETURN: {
            return simpleInstruction(instruction, offset);
        }
        default: {
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
        }
    }
}

static int simpleInstruction(Opcode opcode, int offset) {
    const char* name = opcodes[opcode];
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(Opcode opcode, Chunk* chunk, int offset) {
    const char* name = opcodes[opcode];
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

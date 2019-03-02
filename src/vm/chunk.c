#include "chunk.h"

#include <stdlib.h>

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;

    initLines(&chunk->lines);
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk, u_int8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = oldCapacity < 8 ? 8 : (oldCapacity * 2);
        chunk->code = realloc(chunk->code, (size_t) chunk->capacity * sizeof(uint8_t));
    }

    addLine(&chunk->lines, line);
    chunk->code[chunk->count++] = byte;
}

void freeChunk(Chunk* chunk) {
    free(chunk->code);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

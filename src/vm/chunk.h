#ifndef CLYRA_CHUNK_H
#define CLYRA_CHUNK_H

#include <stdint.h>

#include "value.h"
#include "lines.h"

typedef struct {
    int count;
    int capacity;
    Lines lines;
    ValueArray constants;
    uint8_t* code;
} Chunk;

void initChunk(Chunk* chunk);

void writeChunk(Chunk* chunk, u_int8_t byte, int line);

void freeChunk(Chunk* chunk);

int addConstant(Chunk* chunk, Value value);

#endif //CLYRA_CHUNK_H

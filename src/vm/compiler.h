#ifndef CLYRA_COMPILER_H
#define CLYRA_COMPILER_H

#include "typechecker/typechecker.h"
#include "chunk.h"

typedef struct {
    Typechecker* typechecker;
    Chunk* currentChunk;
} Compiler;


typedef void (* CompilerFn)(Compiler*, Node*);

typedef struct {
    AstNodeType nodeType;
    CompilerFn fn;
} CompilerRule;

Chunk compile(Typechecker* tc);

#endif //CLYRA_COMPILER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "common/list.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "debugging/printing_visitor.h"
#include "debugging/to_json_visitor.h"
#include "typechecker/typechecker.h"
#include "vm/chunk.h"
#include "vm/common.h"
#include "vm/debug.h"
#include "vm/vm.h"

static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");

    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\"\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t fileSize = (size_t) ftell(file);
    rewind(file);

    char* buffer = (char*) malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\"\n", path);
        exit(74);
    }
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\"\n", path);
        exit(74);
    }
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}

int main(int argc, char** argv) {
//    char* astOutput = NULL;
//    char* source;
//    if (argc >= 2) {
//        source = readFile(argv[1]);
//    } else {
//        printf("Usage: clyra [file]");
//        exit(1);
//    }
//    if (argc == 4) {
//        if (strcmp("--ast", argv[2]) == 0) {
//           astOutput = argv[3];
//        } else {
//            printf("Unknown option %s\n", argv[2]);
//            printf("Usage: clyra [file] [--ast ./path/to/ast-output.json]");
//            exit(1);
//        }
//    }
//
//    Lexer l = newLexer(source);
//
//    List* tokenList = newList();
//    while (true) {
//        Token* t = nextToken(&l);
//        listAdd(tokenList, (void**) &t);
//
//        if (t->type == TOKEN_EOF)
//            break;
//    }
//
//    Parser p = newParser((Token**) tokenList->values);
//    List* parseErrors = newList();
//    List* nodes = parse(&p, &parseErrors);
//    if (parseErrors->count != 0) {
//        fprintf(stderr, "Errors:\n");
//        for (int i = 0; i < parseErrors->count; ++i) {
//            const char* msg = parseErrorGetMessage(source, parseErrors->values[i]);
//            fprintf(stderr, "%s\n", msg);
//        }
//        return 1;
//    }
//
//    printing_visit(nodes);
//
//    Typechecker* tc = newTypechecker(nodes);
//    int numTypecheckErrors = typecheck(tc);
//    // TODO: Handle errors
//
//    if (astOutput != NULL) {
//        printf("Outputting ast file: %s\n", astOutput);
//        toJson_visit(astOutput, tc);
//    }

    VM vm;
    initVM(&vm);

    // -((1.2 + 3.4) / 5.6)
    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 0);
    writeChunk(&chunk, constant, 0);

    constant = addConstant(&chunk, 3.4);
    writeChunk(&chunk, OP_CONSTANT, 0);
    writeChunk(&chunk, constant, 0);

    writeChunk(&chunk, OP_ADD, 0);

    constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 0);
    writeChunk(&chunk, constant, 0);

    writeChunk(&chunk, OP_DIV, 0);

    writeChunk(&chunk, OP_NEGATE, 0);
    writeChunk(&chunk, OP_RETURN, 1);

    interpret(&vm, &chunk);
    freeVM();
    freeChunk(&chunk);

    return 0;
}
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <typechecker/typechecker.h>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "debugging/printing_visitor.h"
#include "common/list.h"

int main(int argc, char** argv) {
    char* source = "func add(a: Int, b: Int): Int = a + b\n"
                   "val a = 1\n"
                   "val b = 3 + 5\n"
                   "if (a == 1) add(a, b)\n"
                   "else add(add(b, 1), a)";
    if (argc == 2) {
        source = argv[1];
    }

    Lexer l = newLexer(source);

    List* tokenList = newList();
    while (true) {
        Token* t = nextToken(&l);
        listAdd(tokenList, (void**) &t);

        if (t->type == TOKEN_EOF)
            break;
    }

    Parser p = newParser((Token**) tokenList->values);
    List* parseErrors = newList();
    List* nodes = parse(&p, &parseErrors);
    if (parseErrors->count != 0) {
        fprintf(stderr, "Errors:\n");
        for (int i = 0; i < parseErrors->count; ++i) {
            const char* msg = parseErrorGetMessage(source, parseErrors->values[i]);
            fprintf(stderr, "%s\n", msg);
        }
        return 1;
    }

    printing_visit(nodes);
    typecheck(nodes);
    return 0;
}
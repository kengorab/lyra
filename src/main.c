#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "printing_visitor.h"
#include "common/list.h"

int main() {
    Lexer l = newLexer("val a = 1\nval b = 2");

    List* tokenList = newList();
    while (true) {
        Token* t = nextToken(&l);
        listAdd(tokenList, (void**) &t);

        if (t->type == TOKEN_EOF)
            break;
    }

    Parser p = newParser((Token**) tokenList->values);
    List* nodes = parse(&p);

    printing_visit(nodes);

    return 0;
}
#include <stdio.h>
#include <stdbool.h>

#include "lexer.h"
#include "parser.h"
#include "printing_visitor.h"

int main() {
    Lexer l = newLexer("val a = 1");

    int idx = 0;
    Token* tokens[4];
    while (true) {
        Token* t = nextToken(&l);
        if (t->type == TOKEN_EOF)
            break;
        PRINT_TOKEN(t);
        tokens[idx++] = t;
    }

    Parser p = newParser(tokens);
    Node* n = parse(&p);

    printing_visit(n);

    return 0;
}
#include <stdio.h>
#include <stdbool.h>

#include "lexer.h"
#include "parser.h"
#include "printing_visitor.h"

int main() {
    Lexer l = newLexer("1 \"2\" 3.0 true false");

    int idx = 0;
    Token* tokens[5];
    while (true) {
        Token* t = nextToken(&l);
        if (t->type == TOKEN_EOF)
            break;
        PRINT_TOKEN(t);
        tokens[idx++] = t;
    }

    Parser p = newParser(tokens + 1);
    Node* n = parse(&p);

    printing_visit(n);

    return 0;
}
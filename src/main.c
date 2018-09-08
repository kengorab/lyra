#include <stdio.h>

#include "lexer.h"

int main() {
    Lexer l = newLexer("1 + 1");

    Token t = nextToken(&l);
    while (t.type != TOKEN_EOF) {
        PRINT_TOKEN(t);
        printf("\n");

        t = nextToken(&l);
    }

    return 0;
}
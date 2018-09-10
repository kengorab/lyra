#include <stdio.h>
#include <stdbool.h>

#include "lexer.h"

int main() {
    Lexer l = newLexer("12.34 + 43.8 - 1 * 5.4 / 0.1");

    int idx = 0;
    Token* tokens[8];
    while (true) {
        Token* t = nextToken(&l);
        if (t->type == TOKEN_EOF) break;

        PRINT_TOKEN(t);
        tokens[idx++] = t;
    }

    return 0;
}
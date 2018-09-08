#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

/* Functions/macros for moving the cursor around */
#define PEEK(lexer) *lexer->current
#define IS_AT_END(lexer) (PEEK(lexer) == 0)
#define PEEK_NEXT(lexer) (IS_AT_END(lexer) ? 0 : lexer->current[1])

static char advance(Lexer* lexer) {
    lexer->current++;
    lexer->col++;
    return lexer->current[-1];
}

#define IS_DIGIT(ch) ('0' <= ch && ch <= '9')

Lexer newLexer(char* source) {
    Lexer l = {
        .current = source,
        .start = source,
        .col = 1,
        .line = 1
    };
    return l;
}


static void skipWhitespace(Lexer* lexer) {
    for (;;) {
        char c = PEEK(lexer);
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance(lexer);
                break;
            case '\n':
                lexer->line++;
                lexer->col = 1;
                advance(lexer);
                break;
            case '/':
                if (PEEK_NEXT(lexer) == '/') {
                    while (PEEK(lexer) != '\n' && !IS_AT_END(lexer))
                        advance(lexer);
                } else {
                    return;
                }
            default:
                return;
        }

        return;
    }
}

static Token newToken(TokenType type, const Lexer* lexer) {
    Token t = {
        .type = type,
        .start = lexer->start,
        .length = (int) (lexer->current - lexer->start),
        .line = lexer->line,
        .col = lexer->col
    };
    return t;
}

Token nextToken(Lexer* lexer) {
    // Skip the cursor (lexer->current) ahead, then reposition lexer->start
    skipWhitespace(lexer);
    lexer->start = lexer->current;

    if (IS_AT_END(lexer)) return newToken(TOKEN_EOF, lexer);

    char c = advance(lexer);

    if (IS_DIGIT(c)) {
        while (IS_DIGIT(PEEK(lexer)))
            advance(lexer);
        if (PEEK(lexer) == '.' && IS_DIGIT(PEEK_NEXT(lexer))) {
            advance(lexer); // Consume the '.'

            while (IS_DIGIT(PEEK(lexer)))
                advance(lexer);
        }
        return newToken(TOKEN_NUMBER, lexer);
    }

    return newToken(TOKEN_EOF, lexer);
}

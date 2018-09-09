#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/* Helpers */
#define IS_DIGIT(ch) ('0' <= ch && ch <= '9')

const char* tokenTypes[] = { TOKEN_TYPES };

Lexer newLexer(char* source) {
    Lexer l = {
        .current = source,
        .start = source,
        .col = 0, // Initialize col to 0, it'll be incremented at first advance()
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
            case '\t': {
                advance(lexer);
                break;
            }
            case '\n': {
                lexer->line++;
                lexer->col = -1; // Reset col to -1, it'll be incremented twice (next line, and next advance())
                advance(lexer);
                break;
            }
            case '/': {
                if (PEEK_NEXT(lexer) == '/') {
                    while (PEEK(lexer) != '\n' && !IS_AT_END(lexer))
                        advance(lexer);
                } else {
                    return;
                }
            }
            default: {
                return;
            }
        }

        return;
    }
}

static Token newToken(TokenType type, const Lexer* lexer, int col) {
    Token t = {
        .type = type,
        .start = lexer->start,
        .length = (int) (lexer->current - lexer->start),
        .line = lexer->line,
        .col = col
    };
    return t;
}

static Token newSingleCharToken(TokenType type, const Lexer* lexer) {
    return newToken(type, lexer, lexer->col);
}


static Token errorToken(const char* message, const Lexer* lexer) {
    Token t = {
        .type = TOKEN_ERROR,
        .start = message,
        .length = (int) strlen(message),
        .line = lexer->line,
        .col = lexer->col
    };
    return t;
}


Token nextToken(Lexer* lexer) {
    // Skip the cursor (lexer->current) ahead, then reposition lexer->start
    skipWhitespace(lexer);
    lexer->start = lexer->current;

    if (IS_AT_END(lexer)) return newSingleCharToken(TOKEN_EOF, lexer);

    char c = advance(lexer);

    if (IS_DIGIT(c)) {
        int col = lexer->col;
        while (IS_DIGIT(PEEK(lexer)))
            advance(lexer);
        if (PEEK(lexer) == '.' && IS_DIGIT(PEEK_NEXT(lexer))) {
            advance(lexer); // Consume the '.'

            while (IS_DIGIT(PEEK(lexer)))
                advance(lexer);
        }
        return newToken(TOKEN_NUMBER, lexer, col);
    }

    switch (c) {
        case '+': return newSingleCharToken(TOKEN_PLUS, lexer);
        case '-': return newSingleCharToken(TOKEN_MINUS, lexer);
        case '*': return newSingleCharToken(TOKEN_STAR, lexer);
        case '/': return newSingleCharToken(TOKEN_SLASH, lexer);

        default: {
            char msg[24];
            sprintf(msg, "Unexpected character: %c", c);
            return errorToken(msg, lexer);
        }
    }
}
